# Proteo

Sistema de desoxigenación de agua mediante inyección controlada de nitrógeno para peceras, controlado por Arduino.

## Descripción

Proteo es un proyecto de hardware abierto que utiliza inyección de nitrógeno para desplazar el oxígeno del agua en peceras. El sistema está controlado por un Arduino que monitoriza el nivel de oxígeno mediante un sensor y controla una válvula que regula el paso de nitrógeno. Al inyectar nitrógeno, este desplaza el oxígeno disuelto en el agua, permitiendo crear condiciones anóxicas controladas. El sistema está diseñado para ser eficiente, seguro y preciso.

## Estructura del proyecto

```
proteo/
├── src/               # Código fuente (PlatformIO)
│   └── main.cpp      # Programa principal
├── lib/              # Librerías locales
├── include/          # Headers
├── test/             # Tests unitarios
├── arduino/          # Código Arduino antiguo (deprecado)
├── schematics/       # Esquemáticos del circuito
├── docs/             # Documentación adicional
│   ├── INSTALACION_PLATFORMIO.md
│   └── CONEXION_HARDWARE.md
├── platformio.ini    # Configuración PlatformIO
└── README.md         # Este archivo
```

## Hardware requerido

### Controlador
- **Arduino UNO R4 WiFi**
  - Procesador: Renesas RA4M1 (ARM Cortex-M4, 48 MHz)
  - Memoria: 32 kB SRAM, 256 kB Flash, 8 kB EEPROM
  - Conectividad: WiFi y Bluetooth (ESP32-S3)
  - I/O: 14 pines digitales, 6 entradas analógicas
  - Voltaje de operación: 5V (tolerancia hasta 24V)
  - [Documentación oficial](https://docs.arduino.cc/hardware/uno-r4-wifi/)

### Sensores y Actuadores
- **Sensor de oxígeno disuelto: PreSens OXYBase-wr-RS232**
  - Principio de medición: Sensor óptico (PSt3)
  - Rango de medición: 0-50% O₂ (0-22.5 mg/L disuelto)
  - Precisión: ±0.2% a 20.9% O₂
  - Límite de detección: 0.03% O₂ (0.020 mg/L disuelto)
  - Tiempo de respuesta: ~30 segundos para O₂ disuelto
  - Interfaz: RS232 (protocolo propietario PreSens)
  - Alimentación: 5V, consumo máx. 1W
  - Sensor de temperatura integrado (NTC, ±1°C)
  - Diseño sumergible, carcasa acero inoxidable SUS 316L
  - Resistencia a presión: hasta 3 bar
  - Calibración: dos puntos (ambiente libre de O₂ y aire saturado)
  - [Especificaciones completas](https://www.presens.de/products/detail/oxybase-wr-rs232)
- **Válvula solenoide: TDF_330132**
  - Para control de flujo de gas nitrógeno
  - (Especificaciones por completar)

### Sistema de Gas
- Fuente de nitrógeno (botella de gas comprimido)
- Regulador de presión
- Mangueras y conectores apropiados

### Componentes Adicionales
- **Módulo convertidor MAX3232** (RS232 a TTL)
- **Módulo relé** (5V, 1 canal con optoacoplador)
- Fuente de alimentación (USB-C o externa)
- Protoboard o PCB para conexiones
- Diodo flyback 1N4007
- Cables jumper y conectores

📖 **[Ver guía completa de conexión hardware](docs/CONEXION_HARDWARE.md)**

## Instalación

### 1. Instalar PlatformIO

Este proyecto usa PlatformIO en lugar del Arduino IDE tradicional.

**Opción recomendada: VS Code + Extensión PlatformIO**

1. Instalar [VS Code](https://code.visualstudio.com/)
2. Instalar extensión "PlatformIO IDE"
3. Abrir este proyecto en VS Code

📖 **[Guía detallada de instalación de PlatformIO](docs/INSTALACION_PLATFORMIO.md)**

### 2. Compilar y cargar

```bash
# Compilar
pio run

# Cargar al Arduino UNO R4 WiFi
pio run --target upload

# Abrir monitor serial
pio device monitor
```

### 3. Conectar el hardware

Seguir la [Guía de Conexión del Hardware](docs/CONEXION_HARDWARE.md) para conectar:
- Sensor PreSens OXYBase-wr-RS232 (vía convertidor MAX3232)
- Válvula solenoide TDF_330132 (vía módulo relé)

## Uso

### Comandos disponibles

Una vez cargado el programa y abierto el monitor serial (9600 baud):

```
HELP         - Muestra todos los comandos disponibles
START        - Inicia el sistema de control automático
STOP         - Detiene el sistema
STATUS       - Muestra el estado actual del sistema
VALVE OPEN   - Abre la válvula de N₂ manualmente
VALVE CLOSE  - Cierra la válvula de N₂ manualmente
READ         - Solicita una lectura del sensor PreSens
```

### Modo automático

El sistema controla automáticamente el nivel de oxígeno:
1. Lee el sensor PreSens cada 2 segundos
2. Si O₂ > objetivo: abre válvula (inyecta N₂)
3. Si O₂ ≤ objetivo: cierra válvula
4. Monitorea temperatura y condiciones de seguridad

**Nota**: El objetivo de O₂ por defecto es 0.5 mg/L (condiciones casi anóxicas)

## Licencia

(Por definir)

## Autor

Organización: [SIO-ICM](https://github.com/sio-icm)