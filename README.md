# Proteo

Sistema de desoxigenación de agua mediante inyección controlada de nitrógeno para peceras, controlado por Arduino.

## Descripción

Proteo es un proyecto de hardware abierto que utiliza inyección de nitrógeno para desplazar el oxígeno del agua en peceras. El sistema está controlado por un Arduino que monitoriza el nivel de oxígeno mediante un sensor y controla una válvula que regula el paso de nitrógeno. Al inyectar nitrógeno, este desplaza el oxígeno disuelto en el agua, permitiendo crear condiciones anóxicas controladas. El sistema está diseñado para ser eficiente, seguro y preciso.

## Estructura del proyecto

```
proteo/
├── arduino/           # Código fuente para Arduino
├── schematics/        # Esquemáticos del circuito
├── docs/             # Documentación adicional
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
- Sensor de oxígeno disuelto (por definir modelo específico)
- Válvula solenoide para control de gas nitrógeno (por definir)
- Sensor de temperatura (por definir)

### Sistema de Gas
- Fuente de nitrógeno (botella de gas comprimido)
- Regulador de presión
- Mangueras y conectores apropiados

### Componentes Adicionales
- Fuente de alimentación (USB-C o externa)
- Protoboard o PCB para conexiones
- Resistencias, transistores y otros componentes electrónicos (lista por completar)

## Instalación

(Instrucciones por añadir)

## Uso

(Instrucciones por añadir)

## Licencia

(Por definir)

## Autor

Organización: [SIO-ICM](https://github.com/sio-icm)