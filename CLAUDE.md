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
  - Lectura de sensor de oxígeno disuelto (A0) y temperatura (A1)
  - Sistema de seguridad por temperatura máxima (60°C)
  - Comandos seriales: START, STOP, STATUS
  - LED de estado (pin 13)

### Pendiente ⏳
- Definir hardware específico (sensor de O₂ disuelto exacto, válvula solenoide)
- Crear esquemáticos del circuito en /schematics
- Implementar control PID para regulación de oxígeno objetivo
- Calibrar sensores de oxígeno disuelto y temperatura
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
- Sensor de oxígeno disuelto (analógico) para monitorización continua
- Sensor de temperatura para control de condiciones
- Válvula solenoide para regulación de flujo de nitrógeno
- Prioridad en seguridad: cierre automático de válvula ante condiciones anormales

### Software
- Arquitectura modular para fácil expansión
- Control por puerto serial (9600 baud)
- Sistema de comandos simple y extensible
- Placeholders para sensores (pendiente calibración real)

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
- Lecturas de sensores son placeholders (conversión básica)
- Control de válvula es básico, requiere implementar PID
- Sistema pensado para uso en acuarios/peceras para experimentos de hipoxia/anoxia
- Requiere suministro de nitrógeno (botella de gas comprimido)
- Organización: SIO-ICM (https://github.com/sio-icm)

## Próximas Tareas Sugeridas
1. Especificar sensores exactos (sensor O₂ disuelto, válvula solenoide, sensor temperatura)
2. Diseñar esquemático del circuito completo para Arduino UNO R4 WiFi
3. Implementar control PID para mantener nivel de O₂ objetivo
4. Calibrar y probar sensores de oxígeno disuelto y temperatura
5. Crear documentación de construcción paso a paso
6. Implementar medidas de seguridad para manejo de gas nitrógeno

## Referencias Técnicas
- Desoxigenación por desplazamiento: N₂ desplaza O₂ disuelto en agua
- Sensores de oxígeno disuelto: principios de medición
- Control de válvulas solenoides con Arduino
- Seguridad en el manejo de gases inertes (nitrógeno)

---
*Última actualización: 2025-10-16*
*Este archivo ayuda a Claude a mantener contexto entre conversaciones*
