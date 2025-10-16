# Contexto del Proyecto Proteo para Claude

## Descripción del Proyecto
Proteo es un sistema de desoxigenación de agua para peceras mediante inyección controlada de nitrógeno, controlado por Arduino. El sistema monitoriza el nivel de oxígeno disuelto mediante un sensor y controla una válvula que regula el paso de nitrógeno. Al inyectar nitrógeno, este desplaza el oxígeno del agua, permitiendo crear condiciones anóxicas controladas. Es un proyecto de hardware abierto de la organización SIO-ICM.

## Estructura del Repositorio
```
proteo/
├── arduino/          # Código Arduino (proteo.ino)
├── docs/            # Documentación del proyecto
├── schematics/      # Esquemáticos del circuito (pendiente)
└── README.md        # Documentación principal
```

## Estado Actual del Proyecto

### Implementado ✅
- Código Arduino básico con:
  - Control de válvula solenoide para nitrógeno (pines 9 y 10)
  - Lectura de sensor de oxígeno disuelto vía RS232 (placeholder en A0)
  - Sistema de seguridad por temperatura máxima (60°C)
  - Comandos seriales: START, STOP, STATUS
  - LED de estado (pin 13)

### Pendiente ⏳
- Integrar comunicación RS232 con sensor PreSens OXYBase-wr-RS232
- Definir válvula solenoide específica para nitrógeno
- Crear esquemáticos del circuito en /schematics
- Implementar control PID para regulación de oxígeno objetivo
- Calibrar sensor PreSens (dos puntos: ambiente libre O₂ y aire saturado)
- Añadir sistema de seguridad para control de flujo de nitrógeno
- Añadir documentación técnica completa
- Definir licencia del proyecto
- Sistema de logging/monitoreo
- Pruebas y validación en condiciones reales

## Decisiones de Diseño

### Hardware
- **Arduino UNO R4 WiFi** como controlador principal
  - Procesador: Renesas RA4M1 (ARM Cortex-M4, 48 MHz)
  - Memoria: 32 kB SRAM, 256 kB Flash, 8 kB EEPROM
  - WiFi/Bluetooth integrado (ESP32-S3) para monitoreo remoto
  - 14 pines digitales, 6 entradas analógicas
  - Voltaje de operación: 5V
- **Sensor PreSens OXYBase-wr-RS232** para oxígeno disuelto
  - Sensor óptico con comunicación RS232 (protocolo propietario)
  - Rango: 0-50% O₂ (0-22.5 mg/L disuelto), precisión ±0.2%
  - Tiempo de respuesta: ~30 segundos
  - Sensor de temperatura integrado (NTC, ±1°C)
  - Diseño sumergible, resistencia hasta 3 bar
  - Alimentación: 5V, 1W máx.
- Válvula solenoide para regulación de flujo de nitrógeno
- Prioridad en seguridad: cierre automático de válvula ante condiciones anormales

### Software
- Arquitectura modular para fácil expansión
- Control por puerto serial (9600 baud) para interfaz usuario
- Comunicación RS232 con sensor PreSens (protocolo propietario)
- Sistema de comandos simple y extensible
- Pendiente: implementar parser de protocolo PreSens para lecturas reales

### Seguridad
- Temperatura máxima: 60°C
- Nivel de oxígeno objetivo: configurable
- Cierre automático de válvula ante condiciones anormales
- Prevención de sobrepresión y fugas de nitrógeno

## Convenciones del Proyecto
- Idioma: Español para documentación y comentarios
- Código: Comentarios descriptivos en español
- Commits: Mensajes claros en español
- Hardware abierto: Diseños y código compartibles

## Notas Importantes
- El proyecto está en fase inicial/prototipo
- Sensor PreSens usa RS232 con protocolo propietario (requiere implementación)
- Control de válvula es básico, requiere implementar PID
- Sistema pensado para uso en acuarios/peceras para experimentos de hipoxia/anoxia
- Requiere suministro de nitrógeno (botella de gas comprimido)
- Sensor PreSens incluye temperatura integrada (elimina necesidad de sensor adicional)
- Organización: SIO-ICM (https://github.com/sio-icm)

## Próximas Tareas Sugeridas
1. Implementar comunicación RS232 con sensor PreSens OXYBase-wr-RS232
2. Especificar válvula solenoide para nitrógeno
3. Diseñar esquemático del circuito completo para Arduino UNO R4 WiFi
4. Implementar control PID para mantener nivel de O₂ objetivo
5. Calibrar sensor PreSens (dos puntos: ambiente libre O₂ y aire saturado)
5. Crear documentación de construcción paso a paso
6. Implementar medidas de seguridad para manejo de gas nitrógeno

## Referencias Técnicas
- Desoxigenación por desplazamiento: N₂ desplaza O₂ disuelto en agua
- Sensor PreSens OXYBase-wr: medición óptica de O₂ disuelto (PSt3)
- Protocolo RS232 propietario PreSens (requiere documentación del fabricante)
- Control de válvulas solenoides con Arduino
- Seguridad en el manejo de gases inertes (nitrógeno)

---
*Última actualización: 2025-10-16*
*Este archivo ayuda a Claude a mantener contexto entre conversaciones*
