# Conectar Sensor PreSens - Instrucciones

## ✅ Estado Actual

- **Arduino**: Conectado y funcionando
- **Código**: Cargado correctamente
- **Sistema**: Listo para probar el sensor

---

## 🔌 Conexión Física del Sensor

### Cable PreSens (5 hilos) → Arduino UNO R4 WiFi

```
┌─────────────────────┐
│  Sensor PreSens     │
│  OXYBase-wr-RS232   │
│  (Cable 5 hilos)    │
└──────────┬──────────┘
           │
           ▼
    Cable (identificar colores)

PreSens        Arduino UNO R4 WiFi
────────       ──────────────────────
VCC (rojo)  →  5V
GND (negro) →  GND
TX (verde?) →  Pin 0 (RX1) ⚠️
RX (blanco?)→  Pin 1 (TX1) ⚠️
Cable 5º    →  GND (o dejar sin conectar)
```

### ⚠️ MUY IMPORTANTE - Cruzar TX-RX

```
Sensor TX  →  Arduino RX1 (pin 0)
Sensor RX  →  Arduino TX1 (pin 1)
```

**CRUZADOS**: Lo que transmite el sensor entra por el receptor del Arduino.

---

## 🖥️ Abrir Monitor Serial

### Opción 1: Desde VS Code (recomendado)

1. Abrir VS Code en el proyecto
2. Clic en el botón **🔌** (monitor serial) en la barra inferior
3. O usar: View → Command Palette → "PlatformIO: Serial Monitor"

### Opción 2: Desde terminal

```bash
cd /home/raul/code/proteo
~/.platformio/penv/bin/pio device monitor --baud 9600
```

Para salir: `Ctrl+C`

---

## 📋 Qué deberías ver al abrir el monitor

```
================================
   PROTEO - Sistema de
   Desoxigenación con N₂
================================
Organización: SIO-ICM
Hardware: Arduino UNO R4 WiFi
================================

Inicializando sensor PreSens OXYBase-wr-RS232...
Conexión: TTL directo (sin MAX3232)
Sensor PreSens inicializado

Sistema listo.
Escribe HELP para ver comandos disponibles.
```

---

## 🧪 Comandos Disponibles

Una vez abierto el monitor serial, escribe:

### Comandos básicos:
```
HELP         - Muestra todos los comandos
STATUS       - Muestra estado actual
READ         - Solicita lectura al sensor
```

### Control del sistema:
```
START        - Inicia control automático
STOP         - Detiene el sistema
```

### Control manual de válvula:
```
VALVE OPEN   - Abre válvula manualmente
VALVE CLOSE  - Cierra válvula manualmente
```

---

## 🔍 Probar el Sensor

### Paso 1: Verificar conexión básica

1. **Abrir monitor serial**
2. **Escribir**: `STATUS`
3. **Deberías ver**:
   ```
   === ESTADO DEL SISTEMA PROTEO ===
   Estado: DETENIDO
   Válvula N₂: CERRADA

   --- Sensor PreSens OXYBase-wr ---
   O₂ disuelto: 0.00 mg/L
   O₂ %: 0.00 %
   Temperatura: 0.00 °C
   Objetivo O₂: 0.50 mg/L
   ```

### Paso 2: Solicitar lectura del sensor

1. **Escribir**: `READ`
2. **El sistema enviará** un comando al sensor
3. **Si el sensor responde**, verás algo como:
   ```
   Sensor raw: [lo que devuelva el sensor]
   ```

### Paso 3: Observar datos raw del sensor

Cuando escribas `READ`, aparecerá en el monitor serial **exactamente** lo que el sensor está enviando. Esto es crucial para:
- ✅ Verificar que el sensor está conectado
- ✅ Ver el formato de los datos
- ✅ Implementar el parser correcto

---

## 🐛 Qué hacer si no funciona

### No aparece nada al escribir `READ`

**Posibles causas**:
1. **Cables cruzados al revés**: Verificar TX→RX, RX→TX
2. **Baudrate incorrecto**: El sensor puede usar 19200 en lugar de 9600
3. **Sensor no alimentado**: Verificar VCC y GND
4. **Cable 5º necesario**: Probar conectándolo a GND

**Solución**: Verificar conexiones físicas

### Aparecen caracteres extraños

**Causa**: Baudrate incorrecto

**Solución**: Cambiar baudrate en el código:
```cpp
Serial1.begin(19200);  // Probar 19200, 38400, etc.
```

Recompilar y cargar:
```bash
~/.platformio/penv/bin/pio run --target upload
```

### El sensor responde pero datos incomprensibles

**Causa**: El protocolo es propietario de PreSens

**Solución**:
1. Anotar exactamente lo que aparece en "Sensor raw:"
2. Consultar manual del sensor o contactar PreSens
3. Implementar parser específico para ese formato

---

## 📝 Siguiente Paso

Una vez que veas datos del sensor (aunque sean incomprensibles):

1. **Capturar** varios ejemplos de "Sensor raw:"
2. **Anotar** el formato exacto
3. **Buscar** patrón o documentación
4. **Implementar** parser para extraer O₂ y temperatura

---

## 🆘 Información del Sensor

**Modelo**: PreSens OXYBase-wr-RS232
**Conexión**: TTL 5V (directo)
**Documentación**: https://www.presens.de/products/detail/oxybase-wr-rs232

**Contacto soporte PreSens**:
- Web: https://www.presens.de/support
- Tel: +49 941 942 72 100

---

## 💾 Código Actual

El código está configurado para:
- ✅ Leer Serial1 (pin 0/1) del sensor
- ✅ Mostrar datos raw del sensor
- ✅ Controlar válvula (pin 9)
- ✅ Sistema de seguridad
- ⏳ Parser del sensor (pendiente - depende del formato real)

---

¿Listo para probar? ¡Conecta el sensor y abre el monitor serial! 🚀
