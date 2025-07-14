#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include <time.h>

// Configuración WiFi
const char* ssid = "DvEstudios";
const char* password = "02569833";

// Configuración del servidor web
WebServer server(80);

// Configuración de pines
const int RELAY_PIN = 2;  // Pin para controlar el relé del timbre
const int LED_PIN = 23;   // LED indicador (opcional)

// Almacenamiento persistente
Preferences preferences;

// Estructura para los horarios
struct Schedule {
  int id;
  String name;
  int hour;
  int minute;
  int duration;
  String type;  // "continuo" o "intermitente"
  bool enabled;
};

// Variables globales
Schedule schedules[10];  // Máximo 10 horarios
int scheduleCount = 0;
unsigned long lastTimeCheck = 0;
bool timbreActive = false;
unsigned long timbreStartTime = 0;
int currentTimbreDuration = 0;
String currentTimbreType = "continuo";
bool intermittentState = false;  // Para timbre intermitente: false=primera parte, true=pausa/segunda parte
unsigned long intermittentStateTime = 0;

// Configuración de zona horaria (Colombia UTC-5)
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -5 * 3600;  // UTC-5 para Colombia
const int daylightOffset_sec = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("\n=== SISTEMA DE CONTROL DE TIMBRE ===");
  Serial.println("Iniciando ESP32...");
  
  // Configurar pines
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
  
  // Inicializar almacenamiento
  preferences.begin("timbre", false);
  
  // Conectar a WiFi
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(1000);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi conectado!");
    Serial.print("Dirección IP: ");
    Serial.println(WiFi.localIP());
    
    // Configurar tiempo por NTP
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    
    // Esperar sincronización de tiempo
    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
      Serial.println("Tiempo sincronizado con NTP");
    }
  } else {
    Serial.println("\nNo se pudo conectar a WiFi. Continuando sin conexión...");
  }
  
  // Cargar horarios guardados
  loadSchedules();
  
  // Configurar rutas del servidor web
  setupWebServer();
  
  // Iniciar servidor
  server.begin();
  Serial.println("Servidor web iniciado");
  Serial.println("=================================\n");
  
  // Indicar que está listo
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(200);
    digitalWrite(LED_PIN, LOW);
    delay(200);
  }
}

void loop() {
  // Manejar cliente web
  server.handleClient();
  
  // Verificar horarios cada segundo
  unsigned long currentTime = millis();
  if (currentTime - lastTimeCheck >= 1000) {
    lastTimeCheck = currentTime;
    checkSchedules();
  }
  
  // Controlar duración del timbre
  if (timbreActive) {
    if (currentTimbreType == "continuo") {
      // Timbre continuo - como antes
      if (currentTime - timbreStartTime >= currentTimbreDuration * 1000) {
        stopBell();
      }
    } else if (currentTimbreType == "intermitente") {
      // Timbre intermitente - manejo especial
      handleIntermittentBell(currentTime);
    }
  }
  
  delay(100);
}

void setupWebServer() {
  // Permitir CORS para la app web
  server.onNotFound([]() {
    if (server.method() == HTTP_OPTIONS) {
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.sendHeader("Access-Control-Allow-Methods", "GET,POST,PUT,DELETE,OPTIONS");
      server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
      server.send(200);
    } else {
      server.send(404, "text/plain", "Endpoint no encontrado");
    }
  });
  
  // Estado del sistema
  server.on("/status", HTTP_GET, []() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    
    DynamicJsonDocument doc(1024);
    doc["status"] = "ok";
    doc["wifi_connected"] = WiFi.status() == WL_CONNECTED;
    doc["ip"] = WiFi.localIP().toString();
    doc["schedule_count"] = scheduleCount;
    doc["timbre_active"] = timbreActive;
    
    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
      char timeStr[64];
      strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &timeinfo);
      doc["current_time"] = timeStr;
    }
    
    String response;
    serializeJson(doc, response);
    server.send(200, "application/json", response);
  });
  
  // Obtener horarios
  server.on("/schedules", HTTP_GET, []() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    
    DynamicJsonDocument doc(2048);
    JsonArray schedulesArray = doc.createNestedArray("schedules");
    
    for (int i = 0; i < scheduleCount; i++) {
      JsonObject scheduleObj = schedulesArray.createNestedObject();
      scheduleObj["id"] = schedules[i].id;
      scheduleObj["name"] = schedules[i].name;
      
      char timeStr[6];
      sprintf(timeStr, "%02d:%02d", schedules[i].hour, schedules[i].minute);
      scheduleObj["time"] = timeStr;
      
      scheduleObj["duration"] = schedules[i].duration;
      scheduleObj["type"] = schedules[i].type;
      scheduleObj["enabled"] = schedules[i].enabled;
    }
    
    String response;
    serializeJson(doc, response);
    server.send(200, "application/json", response);
  });
  
  // Recibir horarios
  server.on("/schedules", HTTP_POST, []() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    
    if (server.hasArg("plain")) {
      DynamicJsonDocument doc(2048);
      deserializeJson(doc, server.arg("plain"));
      
      if (doc.containsKey("schedules")) {
        scheduleCount = 0;
        JsonArray schedulesArray = doc["schedules"];
        
        for (JsonObject scheduleObj : schedulesArray) {
          if (scheduleCount < 10) {
            schedules[scheduleCount].id = scheduleObj["id"];
            schedules[scheduleCount].name = scheduleObj["name"];
            
            // Parsear hora
            String timeStr = scheduleObj["time"];
            int colonPos = timeStr.indexOf(':');
            schedules[scheduleCount].hour = timeStr.substring(0, colonPos).toInt();
            schedules[scheduleCount].minute = timeStr.substring(colonPos + 1).toInt();
            
            schedules[scheduleCount].duration = scheduleObj["duration"];
            schedules[scheduleCount].type = scheduleObj.containsKey("type") ? scheduleObj["type"].as<String>() : "continuo";
            schedules[scheduleCount].enabled = scheduleObj["enabled"];
            scheduleCount++;
          }
        }
        
        saveSchedules();
        Serial.printf("Horarios actualizados: %d configurados\n", scheduleCount);
        
        server.send(200, "application/json", "{\"status\":\"ok\",\"message\":\"Horarios guardados\"}");
      } else {
        server.send(400, "application/json", "{\"status\":\"error\",\"message\":\"Formato inválido\"}");
      }
    } else {
      server.send(400, "application/json", "{\"status\":\"error\",\"message\":\"No hay datos\"}");
    }
  });
  
  // Probar timbre
  server.on("/test-bell", HTTP_POST, []() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    
    String type = "continuo";
    int duration = 3;
    
    if (server.hasArg("plain")) {
      DynamicJsonDocument doc(512);
      deserializeJson(doc, server.arg("plain"));
      
      if (doc.containsKey("type")) {
        type = doc["type"].as<String>();
      }
      if (doc.containsKey("duration")) {
        duration = doc["duration"];
      }
    }
    
    Serial.printf("Prueba de timbre %s activada desde la app\n", type.c_str());
    activateBell(duration, type);
    
    server.send(200, "application/json", "{\"status\":\"ok\",\"message\":\"Timbre activado\"}");
  });
  
  // Sincronizar tiempo
  server.on("/sync-time", HTTP_POST, []() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    
    if (server.hasArg("plain")) {
      DynamicJsonDocument doc(512);
      deserializeJson(doc, server.arg("plain"));
      
      if (doc.containsKey("year") && doc.containsKey("month") && doc.containsKey("day") &&
          doc.containsKey("hour") && doc.containsKey("minute") && doc.containsKey("second")) {
        
        struct tm timeinfo;
        timeinfo.tm_year = doc["year"] - 1900;
        timeinfo.tm_mon = doc["month"] - 1;
        timeinfo.tm_mday = doc["day"];
        timeinfo.tm_hour = doc["hour"];
        timeinfo.tm_min = doc["minute"];
        timeinfo.tm_sec = doc["second"];
        
        time_t t = mktime(&timeinfo);
        struct timeval tv = { .tv_sec = t };
        settimeofday(&tv, NULL);
        
        Serial.println("Tiempo sincronizado manualmente");
        server.send(200, "application/json", "{\"status\":\"ok\",\"message\":\"Tiempo sincronizado\"}");
      } else {
        server.send(400, "application/json", "{\"status\":\"error\",\"message\":\"Datos de tiempo incompletos\"}");
      }
    } else {
      server.send(400, "application/json", "{\"status\":\"error\",\"message\":\"No hay datos\"}");
    }
  });
}

void checkSchedules() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return;  // No hay tiempo válido
  }
  
  int currentHour = timeinfo.tm_hour;
  int currentMinute = timeinfo.tm_min;
  int currentSecond = timeinfo.tm_sec;
  
  // Solo verificar en el segundo 0 para evitar activaciones múltiples
  if (currentSecond != 0) {
    return;
  }
  
  for (int i = 0; i < scheduleCount; i++) {
    if (schedules[i].enabled && 
        schedules[i].hour == currentHour && 
        schedules[i].minute == currentMinute) {
      
      Serial.printf("Activando timbre %s: %s (%02d:%02d)\n", 
                    schedules[i].type.c_str(), schedules[i].name.c_str(), currentHour, currentMinute);
      
      activateBell(schedules[i].duration, schedules[i].type);
      break;  // Solo uno a la vez
    }
  }
}

void activateBell(int duration, String type = "continuo") {
  if (timbreActive) {
    return;  // Ya está activo
  }
  
  timbreActive = true;
  currentTimbreDuration = duration;
  currentTimbreType = type;
  timbreStartTime = millis();
  
  if (type == "intermitente") {
    // Para timbre intermitente, inicializamos el estado
    intermittentState = false;  // Empezamos con la primera parte
    intermittentStateTime = millis();
  }
  
  digitalWrite(RELAY_PIN, HIGH);  // Activar relé
  digitalWrite(LED_PIN, HIGH);    // Indicador LED
  
  Serial.printf("Timbre %s activado por %d segundos\n", type.c_str(), duration);
}

void handleIntermittentBell(unsigned long currentTime) {
  unsigned long elapsed = currentTime - intermittentStateTime;
  int halfDuration = (currentTimbreDuration * 1000) / 2;  // Duración de cada "piii"
  int pauseDuration = 300;  // Pausa de 300ms entre los dos "piii"
  
  if (!intermittentState) {
    // Primera parte del timbre
    if (elapsed >= halfDuration) {
      // Termina primera parte, comienza pausa
      digitalWrite(RELAY_PIN, LOW);
      digitalWrite(LED_PIN, LOW);
      intermittentState = true;
      intermittentStateTime = currentTime;
    }
  } else {
    // En pausa o segunda parte
    if (elapsed >= pauseDuration && elapsed < (pauseDuration + halfDuration)) {
      // Durante la segunda parte - activar relé
      digitalWrite(RELAY_PIN, HIGH);
      digitalWrite(LED_PIN, HIGH);
    } else if (elapsed >= (pauseDuration + halfDuration)) {
      // Terminar completamente
      stopBell();
    }
  }
}

void stopBell() {
  timbreActive = false;
  intermittentState = false;
  digitalWrite(RELAY_PIN, LOW);   // Desactivar relé
  digitalWrite(LED_PIN, LOW);     // Apagar LED
  
  Serial.println("Timbre desactivado");
}

void saveSchedules() {
  preferences.putInt("count", scheduleCount);
  
  for (int i = 0; i < scheduleCount; i++) {
    String prefix = "sch_" + String(i) + "_";
    preferences.putInt((prefix + "id").c_str(), schedules[i].id);
    preferences.putString((prefix + "name").c_str(), schedules[i].name);
    preferences.putInt((prefix + "hour").c_str(), schedules[i].hour);
    preferences.putInt((prefix + "min").c_str(), schedules[i].minute);
    preferences.putInt((prefix + "dur").c_str(), schedules[i].duration);
    preferences.putString((prefix + "type").c_str(), schedules[i].type);
    preferences.putBool((prefix + "en").c_str(), schedules[i].enabled);
  }
  
  Serial.println("Horarios guardados en memoria persistente");
}

void loadSchedules() {
  scheduleCount = preferences.getInt("count", 0);
  
  for (int i = 0; i < scheduleCount && i < 10; i++) {
    String prefix = "sch_" + String(i) + "_";
    schedules[i].id = preferences.getInt((prefix + "id").c_str(), 0);
    schedules[i].name = preferences.getString((prefix + "name").c_str(), "Horario " + String(i+1));
    schedules[i].hour = preferences.getInt((prefix + "hour").c_str(), 8);
    schedules[i].minute = preferences.getInt((prefix + "min").c_str(), 0);
    schedules[i].duration = preferences.getInt((prefix + "dur").c_str(), 3);
    schedules[i].type = preferences.getString((prefix + "type").c_str(), "continuo");
    schedules[i].enabled = preferences.getBool((prefix + "en").c_str(), true);
  }
  
  Serial.printf("Horarios cargados: %d configurados\n", scheduleCount);
}
