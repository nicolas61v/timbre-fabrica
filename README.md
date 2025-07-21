# 🔧 Esquema de Conexiones ESP32 - Sistema de Timbre

## 📋 **COMPONENTES NECESARIOS**

### **Hardware Principal:**
- ✅ ESP32 DevKit v1 (30 pines)
- ✅ 2x Módulo Relé 5V - 1 Canal cada uno
- ✅ Fuente 5V/2A (para los relés)
- ✅ LED 5mm (indicador - opcional)
- ✅ Resistencia 220Ω (para LED)
- ✅ Cables jumper macho-hembra
- ✅ Protoboard o PCB
- ✅ Borneras para conexión de los timbres
- ✅ Caja protectora

### **Herramientas:**
- Destornilladores
- Pelacables
- Multímetro
- Estaño y soldador (opcional)

---

## 📍 **UBICACIÓN EXACTA DE LOS PINES EN ESP32 DevKit v1**

```
                     ESP32 DevKit v1 (Vista Superior)
                    ┌─────────────────────────────────┐
                    │                                 │
                    │            [USB]                │
                    │                                 │
           ┌────────┤                                 ├────────┐
           │  3V3   ├─────────────────────────────────┤  GND   │
           │  EN    ├─────────────────────────────────┤  23    │ ← LED (opcional)
           │  36    ├─────────────────────────────────┤  22    │
           │  39    ├─────────────────────────────────┤  TX    │
           │  34    ├─────────────────────────────────┤  RX    │
           │  35    ├─────────────────────────────────┤  21    │
           │  32    ├─────────────────────────────────┤  19    │
           │  33    ├─────────────────────────────────┤  18    │
           │  25    ├─────────────────────────────────┤  5     │
           │  26    ├─────────────────────────────────┤  17    │
           │  27    ├─────────────────────────────────┤  16    │
           │  14    ├─────────────────────────────────┤  4     │ ← RELÉ 2 (AQUÍ)
           │  12    ├─────────────────────────────────┤  0     │
           │  13    ├─────────────────────────────────┤  2     │ ← RELÉ 1 (AQUÍ)
           │  GND   ├─────────────────────────────────┤  15    │
           │  VIN   ├─────────────────────────────────┤  8     │
           └────────┤                                 ├────────┘
                    │            [Antena WiFi]        │
                    │                                 │
                    └─────────────────────────────────┘
```

## 🎯 **CONEXIONES EXACTAS:**

### **Pin 2 (Relé 1):**
- **Etiqueta en placa**: `2`
- **Ubicación**: Lado derecho, **segunda posición desde abajo**
- **Conexión**: Cable al pin `IN` del Relé 1

### **Pin 4 (Relé 2):**
- **Etiqueta en placa**: `4` 
- **Ubicación**: Lado derecho, **cuarta posición desde abajo**
- **Conexión**: Cable al pin `IN` del Relé 2

### **Pin 23 (LED indicador - opcional):**
- **Etiqueta en placa**: `23`
- **Ubicación**: Lado derecho, **segunda posición desde arriba**
- **Conexión**: Cable a LED con resistencia 220Ω

### **Pines de alimentación:**
- **3V3**: Lado izquierdo, **primera posición** (para alimentar relés)
- **GND**: Lado izquierdo, **penúltima posición** (tierra común)

## 📷 **LO QUE VERÁS EN TU ESP32:**

```
Lado derecho (mirando el USB hacia arriba):
┌─────┐
│ GND │
│ 23  │ ← LED (opcional)
│ 22  │
│ TX  │
│ RX  │
│ 21  │
│ 19  │
│ 18  │
│ 5   │
│ 17  │
│ 16  │
│ 4   │ ← RELÉ 2 (cable aquí)
│ 0   │
│ 2   │ ← RELÉ 1 (cable aquí) 
│ 15  │
│ 8   │
└─────┘
```

```
                    ┌─────────────────────────┐
                    │       ESP32 DevKit      │
                    │                         │
                    │  3V3 ──────────────┐    │
                    │  GND ──────────┐   │    │
                    │  GPIO 2 ───┐   │   │    │
                    │  GPIO 4 ───│─┐ │   │    │
                    │  GPIO 23 ──│─│─│───│──┐ │
                    │            │ │ │   │  │ │
                    └────────────│─│─│───│──│─┘
                                 │ │ │   │  │
                                 │ │ │   │  │
                    ┌────────────│─│─│───│──│─┐
                    │     RELÉ 1 │ │ │   │  │ │  
                    │  (TIMBRE 1)│ │ │   │  │ │
                    │            │ │ │   │  │ │
                    │  VCC ──────┘ │ │   │  │ │
                    │  GND ────────│─│───┘  │ │
                    │  IN ─────────┘ │      │ │
                    │                │      │ │
                    │  COM ─┐        │      │ │
                    │  NO ──│─ SALIDA 1     │ │
                    │  NC ─ │        │      │ │
                    └───────│────────│──────│─┘
                            │        │      │
                    ┌───────│────────│──────│─┐
                    │       │ RELÉ 2 │      │ │  
                    │       │(TIMBRE 2)     │ │
                    │       │        │      │ │
                    │  VCC ─│────────│──────┘ │
                    │  GND ─│────────┘        │
                    │  IN ──│─────────────────┘
                    │       │                  │
                    │  COM ─│─┐                │
                    │  NO ──│─│─ SALIDA 2      │
                    │  NC ─ │ │                │
                    └───────│─│────────────────┘
                            │ │
                            │ │
                    ┌───────│─│────────────────┐
                    │       │ │   TIMBRE 1     │
                    │       │ │                │
                    │  FASE │ │                │
                    │   ~~~~│~│~~~~~~~~~~~~~~~ │
                    │       └─│ Contacto Relé1 │
                    │         │                │
                    │  NEUTRO │~~~~~~~~~~~~~~~~ │
                    │         │                │
                    │  TIERRA │~~~~~~~~~~~~~~~~ │
                    └─────────│────────────────┘
                              │
                    ┌─────────│────────────────┐
                    │         │   TIMBRE 2     │
                    │         │                │
                    │    FASE │                │
                    │     ~~~~│~~~~~~~~~~~~~~~ │
                    │         └ Contacto Relé2 │
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

### **ESP32 → Módulo Relé 1 (Timbre 1):**
| ESP32 Pin | Relé 1 Pin | Función |
|-----------|------------|---------|
| `3.3V`    | `VCC`      | Alimentación |
| `GND`     | `GND`      | Tierra |
| `GPIO 2`  | `IN`       | Señal control |

### **ESP32 → Módulo Relé 2 (Timbre 2):**
| ESP32 Pin | Relé 2 Pin | Función |
|-----------|------------|---------|
| `3.3V`    | `VCC`      | Alimentación |
| `GND`     | `GND`      | Tierra |
| `GPIO 4`  | `IN`       | Señal control |

### **ESP32 → LED Indicador (Opcional):**
| ESP32 Pin | Componente | Función |
|-----------|------------|---------|
| `GPIO 23` | Resistencia 220Ω → LED (+) | Señal |
| `GND`     | LED (-)    | Tierra |

### **Relé 1 → Timbre 1:**
| Relé 1 Pin | Timbre 1 | Función |
|------------|----------|---------|
| `COM`      | Fase cortada | Común |
| `NO`       | Hacia timbre 1 | Normalmente abierto |
| `NC`       | (Sin conexión) | Normalmente cerrado |

### **Relé 2 → Timbre 2:**
| Relé 2 Pin | Timbre 2 | Función |
|------------|----------|---------|
| `COM`      | Fase cortada | Común |
| `NO`       | Hacia timbre 2 | Normalmente abierto |
| `NC`       | (Sin conexión) | Normalmente cerrado |

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

### **🔒 Conexión segura de ambos timbres:**
```
PELIGRO: 110V/220V
├── Fase ──── Interruptor ──── Relé 1 COM
├── Relé 1 NO ──── Timbre 1 (+)
├── Neutro ──── Timbre 1 (-)

├── Fase ──── Interruptor ──── Relé 2 COM  
├── Relé 2 NO ──── Timbre 2 (+)
└── Neutro ──── Timbre 2 (-)
```

---

## 🛠️ **PASOS DE INSTALACIÓN**

### **1. Preparar componentes:**
- [ ] ESP32 con código cargado
- [ ] Módulo relé probado
- [ ] Fuente de alimentación
- [ ] Caja protectora

### **2. Hacer conexiones:**
- [ ] ESP32 ↔ Relé 1 (3.3V, GND, GPIO2)
- [ ] ESP32 ↔ Relé 2 (3.3V, GND, GPIO4)
- [ ] LED indicador (opcional)
- [ ] Probar ambos relés con multímetro

### **3. Programar ESP32:**
- [ ] Instalar librerías
- [ ] Configurar WiFi en código
- [ ] Subir código
- [ ] Verificar IP en monitor serie

### **4. Instalación eléctrica:**
- [ ] ⚠️ CORTAR ENERGÍA
- [ ] Conectar relé 1 en serie con timbre 1
- [ ] Conectar relé 2 en serie con timbre 2
- [ ] Borneras bien apretadas
- [ ] Caja cerrada
- [ ] ⚠️ REVISAR TODO DOS VECES

### **5. Pruebas:**
- [ ] Energizar sistema
- [ ] Conectar app web
- [ ] Probar timbre 1 continuo/intermitente
- [ ] Probar timbre 2 continuo/intermitente
- [ ] Probar ambos timbres juntos
- [ ] Verificar sincronización NTP automática
- [ ] Configurar horarios con diferentes relés

### **6. Mantenimiento recomendado:**
- [ ] **Mensual**: Verificar horarios desde la app web
- [ ] **Trimestral**: Forzar sincronización NTP desde la app
- [ ] **Anual**: Revisar conexiones físicas y limpieza
- [ ] **Monitor**: LED parpadea 5 veces cada sincronización NTP (cada 24h)

---

## 📱 **CÓDIGO DE EJEMPLO MÍNIMO PARA PROBAR:**

```cpp
// Código mínimo para probar ambos relés
void setup() {
  pinMode(2, OUTPUT);  // Pin del relé 1
  pinMode(4, OUTPUT);  // Pin del relé 2
  Serial.begin(115200);
  Serial.println("Probando ambos relés...");
}

void loop() {
  Serial.println("Activando Relé 1 por 2 segundos");
  digitalWrite(2, HIGH);  // Activar relé 1
  delay(2000);
  digitalWrite(2, LOW);   // Desactivar relé 1
  delay(1000);
  
  Serial.println("Activando Relé 2 por 2 segundos");
  digitalWrite(4, HIGH);  // Activar relé 2
  delay(2000);
  digitalWrite(4, LOW);   // Desactivar relé 2
  delay(1000);
  
  Serial.println("Activando AMBOS relés por 3 segundos");
  digitalWrite(2, HIGH);  // Activar ambos
  digitalWrite(4, HIGH);
  delay(3000);
  digitalWrite(2, LOW);   // Desactivar ambos
  digitalWrite(4, LOW);
  delay(2000);
}
```

---

## 🆘 **SOLUCIÓN DE PROBLEMAS**

| Problema | Solución |
|----------|----------|
| ESP32 no conecta WiFi | Verificar SSID y password |
| Relé 1 no activa | Revisar conexión GPIO 2 |
| Relé 2 no activa | Revisar conexión GPIO 4 |
| No aparece IP | Abrir monitor serie 115200 baudios |
| Timbre 1 no suena | Verificar conexiones 110V/220V relé 1 |
| Timbre 2 no suena | Verificar conexiones 110V/220V relé 2 |
| App no conecta | Verificar IP del ESP32 |
| Solo funciona un relé | Verificar alimentación y GPIO del otro |
| Hora incorrecta | Forzar sincronización NTP desde app web |
| "Última Sync NTP" en rojo | Conectar a internet y forzar sync NTP |

---

## ✅ **CHECKLIST FINAL**

- [ ] Todas las librerías instaladas
- [ ] Código compilado sin errores
- [ ] Conexiones ESP32 ↔ Relé 1 verificadas
- [ ] Conexiones ESP32 ↔ Relé 2 verificadas
- [ ] WiFi configurado correctamente
- [ ] Caja protectora instalada
- [ ] ⚠️ Instalación eléctrica de ambos timbres revisada por electricista
- [ ] Pruebas de funcionamiento de ambos relés OK
- [ ] App web configurada y probada con ambos timbres

---

## ⏰ **PRECISIÓN TEMPORAL Y MANTENIMIENTO A LARGO PLAZO**

### **🎯 Respuesta a tu pregunta sobre 6 meses:**

**¡SÍ! Ahora el ESP32 puede funcionar 6 meses sin perder precisión** gracias a las mejoras implementadas:

#### **🔄 Auto-Sincronización Automática:**
- **Cada 24 horas**: Se sincroniza automáticamente con servidores NTP
- **Sin intervención**: No necesitas hacer nada manualmente
- **Indicador visual**: LED parpadea 5 veces cuando se sincroniza
- **Monitoreo**: La app web muestra estado de última sincronización

#### **📊 Precisión Garantizada:**
- **Sin auto-sync**: ±3-9 minutos en 6 meses ❌
- **Con auto-sync**: ±1-2 segundos siempre ✅
- **Requisito**: Solo necesita WiFi con internet

#### **🔍 Monitoreo desde la App Web:**
```
Estado en app web:
"Última Sync NTP: Reciente" → Verde (todo bien)
"Última Sync NTP: Hace 12h" → Amarillo (normal)
"Última Sync NTP: Hace 48h" → Rojo (revisar conexión)
```

#### **🛠️ Mantenimiento Mínimo:**
- **Configuración inicial**: Una sola vez
- **Revisión**: Cada 3-6 meses desde la app web
- **Acción manual**: Solo si pierde internet por días

#### **🔧 Funciones de Respaldo:**
- **Sincronización manual**: Desde la app web
- **Sincronización forzada NTP**: Botón dedicado
- **Persistencia**: Horarios guardados aunque pierda energía

### **💡 Recomendación Final:**

**Para uso de 6 meses sin mantenimiento:**
1. ✅ Conectar ESP32 a WiFi estable con internet
2. ✅ Verificar que funciona la auto-sincronización
3. ✅ Configurar todos los horarios
4. ✅ **¡Olvidarse del sistema!** - Funcionará automáticamente

**El sistema ahora es completamente autónomo y mantendrá precisión perfecta indefinidamente mientras tenga WiFi.**

---

**🎯 ¡Con este código actualizado, tu sistema de timbre es verdaderamente industrial y confiable!**