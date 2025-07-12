# 🔧 Esquema de Conexiones ESP32 - Sistema de Timbre

## 📋 **COMPONENTES NECESARIOS**

### **Hardware Principal:**
- ✅ ESP32 DevKit v1 (30 pines)
- ✅ Módulo Relé 5V - 1 Canal 
- ✅ Fuente 5V/2A (para el relé)
- ✅ LED 5mm (indicador - opcional)
- ✅ Resistencia 220Ω (para LED)
- ✅ Cables jumper macho-hembra
- ✅ Protoboard o PCB
- ✅ Borneras para conexión del timbre
- ✅ Caja protectora

### **Herramientas:**
- Destornilladores
- Pelacables
- Multímetro
- Estaño y soldador (opcional)

---

## 🔌 **DIAGRAMA DE CONEXIONES**

```
                    ┌─────────────────────────┐
                    │       ESP32 DevKit      │
                    │                         │
                    │  3V3 ──────────────┐    │
                    │  GND ──────────┐   │    │
                    │  GPIO 2 ───┐   │   │    │
                    │  GPIO 23 ──│───│───│──┐ │
                    │            │   │   │  │ │
                    └────────────│───│───│──│─┘
                                 │   │   │  │
                                 │   │   │  │
                    ┌────────────│───│───│──│─┐
                    │            │   │   │  │ │  
                    │     MÓDULO RELÉ 5V     │
                    │                        │
                    │  VCC ──────────────────┘ │
                    │  GND ──────────────────┐ │
                    │  IN ───────────────────┘ │
                    │                          │
                    │  COM ─┐                  │
                    │  NO ──│─── SALIDA        │
                    │  NC ─ │                  │
                    └───────│──────────────────┘
                            │
                            │
                    ┌───────│──────────────────┐
                    │       │     TIMBRE       │
                    │       │                  │
                    │  FASE │                  │
                    │   ~~~~│~~~~~~~~~~~~~~~~~~ │
                    │       └─ Contacto Relé   │
                    │                          │
                    │  NEUTRO ~~~~~~~~~~~~~~~~~ │
                    │                          │
                    │  TIERRA ~~~~~~~~~~~~~~~~~ │
                    └──────────────────────────┘

              ┌────────────────────────────────┐
              │         LED INDICADOR          │
              │                                │
              │  GPIO 23 ──[220Ω]──[LED]──GND │
              └────────────────────────────────┘
```

---

## 🔗 **CONEXIONES DETALLADAS**

### **ESP32 → Módulo Relé:**
| ESP32 Pin | Relé Pin | Función |
|-----------|----------|---------|
| `3.3V`    | `VCC`    | Alimentación |
| `GND`     | `GND`    | Tierra |
| `GPIO 2`  | `IN`     | Señal control |

### **ESP32 → LED Indicador (Opcional):**
| ESP32 Pin | Componente | Función |
|-----------|------------|---------|
| `GPIO 23` | Resistencia 220Ω → LED (+) | Señal |
| `GND`     | LED (-)    | Tierra |

### **Relé → Timbre:**
| Relé Pin | Timbre | Función |
|----------|--------|---------|
| `COM`    | Fase cortada | Común |
| `NO`     | Hacia timbre | Normalmente abierto |
| `NC`     | (Sin conexión) | Normalmente cerrado |

---

## 📚 **LIBRERÍAS NECESARIAS EN ARDUINO IDE**

### **1. Configurar ESP32 en Arduino IDE:**
```
Archivo → Preferencias → URLs adicionales:
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json

Herramientas → Placa → Gestor de tarjetas → Buscar "ESP32" → Instalar
```

### **2. Librerías a instalar:**
```cpp
// YA INCLUIDAS con ESP32:
#include <WiFi.h>           ✅ Control WiFi
#include <WebServer.h>      ✅ Servidor web
#include <Preferences.h>    ✅ Almacenamiento persistente
#include <time.h>          ✅ Manejo de tiempo

// INSTALAR MANUALMENTE:
```

**📦 ArduinoJson (OBLIGATORIA):**
```
Sketch → Incluir Librería → Gestionar Librerías
Buscar: "ArduinoJson" por Benoit Blanchon
Instalar versión 6.x.x
```

### **3. Configuración de la placa:**
```
Herramientas → Placa: "ESP32 Dev Module"
Herramientas → Puerto: [Tu puerto COM]
Herramientas → Upload Speed: "921600"
Herramientas → Flash Size: "4MB"
```

---

## ⚡ **CONSIDERACIONES DE SEGURIDAD**

### **⚠️ IMPORTANTE - TRABAJO CON 110V/220V:**

1. **Cortar energía** antes de hacer conexiones
2. **Usar voltímetro** para verificar que NO hay voltaje
3. **Conexiones firmes** en borneras
4. **Caja protectora** para toda la electrónica
5. **Revisión por electricista** antes de energizar

### **🔒 Conexión segura del timbre:**
```
PELIGRO: 110V/220V
├── Fase ──── Interruptor ──── Relé COM
├── Relé NO ──── Timbre (+)
└── Neutro ──── Timbre (-)
```

---

## 🛠️ **PASOS DE INSTALACIÓN**

### **1. Preparar componentes:**
- [ ] ESP32 con código cargado
- [ ] Módulo relé probado
- [ ] Fuente de alimentación
- [ ] Caja protectora

### **2. Hacer conexiones:**
- [ ] ESP32 ↔ Relé (3.3V, GND, GPIO2)
- [ ] LED indicador (opcional)
- [ ] Probar con multímetro

### **3. Programar ESP32:**
- [ ] Instalar librerías
- [ ] Configurar WiFi en código
- [ ] Subir código
- [ ] Verificar IP en monitor serie

### **4. Instalación eléctrica:**
- [ ] ⚠️ CORTAR ENERGÍA
- [ ] Conectar relé en serie con timbre
- [ ] Borneras bien apretadas
- [ ] Caja cerrada
- [ ] ⚠️ REVISAR TODO DOS VECES

### **5. Pruebas:**
- [ ] Energizar sistema
- [ ] Conectar app web
- [ ] Probar timbre continuo
- [ ] Probar timbre intermitente
- [ ] Configurar horarios

---

## 📱 **CÓDIGO DE EJEMPLO MÍNIMO PARA PROBAR:**

```cpp
// Código mínimo para probar el relé
void setup() {
  pinMode(2, OUTPUT);  // Pin del relé
  Serial.begin(115200);
  Serial.println("Probando relé...");
}

void loop() {
  Serial.println("Activando relé 3 segundos");
  digitalWrite(2, HIGH);  // Activar
  delay(3000);
  
  Serial.println("Desactivando relé 2 segundos");
  digitalWrite(2, LOW);   // Desactivar
  delay(2000);
}
```

---

## 🆘 **SOLUCIÓN DE PROBLEMAS**

| Problema | Solución |
|----------|----------|
| ESP32 no conecta WiFi | Verificar SSID y password |
| Relé no activa | Revisar conexión GPIO 2 |
| No aparece IP | Abrir monitor serie 115200 baudios |
| Timbre no suena | Verificar conexiones 110V/220V |
| App no conecta | Verificar IP del ESP32 |

---

## ✅ **CHECKLIST FINAL**

- [ ] Todas las librerías instaladas
- [ ] Código compilado sin errores
- [ ] Conexiones ESP32 ↔ Relé verificadas
- [ ] WiFi configurado correctamente
- [ ] Caja protectora instalada
- [ ] ⚠️ Instalación eléctrica revisada por electricista
- [ ] Pruebas de funcionamiento OK
- [ ] App web configurada y probada

---

**🎯 ¡Con este esquema tienes todo lo necesario para implementar tu sistema de timbre inteligente de forma segura!**
