# Guía de Conexión del Hardware - Proteo

## Resumen del Sistema

El sistema Proteo requiere conectar:
1. Arduino UNO R4 WiFi (controlador)
2. Sensor PreSens OXYBase-wr-RS232 (oxígeno disuelto)
3. Válvula solenoide TDF_330132 (control de nitrógeno)

---

## 1. Conexión del Sensor PreSens OXYBase-wr-RS232

### Problema: Incompatibilidad de voltajes
- **Sensor PreSens**: Usa comunicación RS232 (voltajes ±12V)
- **Arduino UNO R4**: Usa comunicación TTL (voltajes 0-5V)
- **Solución**: Se requiere un **convertidor RS232 a TTL**

### Hardware necesario
- **Módulo convertidor MAX3232** (o similar: SP3232, ICL3232)
  - Disponible en tiendas de electrónica
  - Precio aproximado: 2-5€
  - Incluye el chip MAX3232 y los capacitores necesarios

### Esquema de conexión

```
┌─────────────────────┐
│  PreSens OXYBase-wr │
│     (RS232)         │
└──────────┬──────────┘
           │ Cable RS232
           │ (DB9 o similar)
           │
     ┌─────▼─────────┐
     │  MAX3232      │
     │  Convertidor  │
     │  RS232 → TTL  │
     └─────┬─────────┘
           │
    ┌──────▼─────────────┐
    │  Arduino UNO R4    │
    │      WiFi          │
    └────────────────────┘
```

### Conexiones detalladas

#### Del PreSens al MAX3232:
```
PreSens RS232 → MAX3232
TX (pin 3)    → RIN  (RS232 IN)
RX (pin 2)    → ROUT (RS232 OUT)
GND (pin 5)   → GND
```

#### Del MAX3232 al Arduino:
```
MAX3232   → Arduino UNO R4 WiFi
TX (TTL)  → RX1 (pin 0 o Serial1 RX)
RX (TTL)  → TX1 (pin 1 o Serial1 TX)
VCC       → 5V
GND       → GND
```

### Notas importantes:
1. **NO conectar directamente** el sensor RS232 al Arduino sin convertidor (puede dañar ambos)
2. El Arduino UNO R4 WiFi tiene múltiples puertos seriales:
   - `Serial` (USB): Para comandos y debug
   - `Serial1` (TX1/RX1): Para comunicación con sensor PreSens
3. Alimentar el sensor PreSens con su fuente de 5V independiente
4. Verificar el pinout exacto del conector RS232 del sensor (consultar manual)

---

## 2. Conexión de la Válvula Solenoide TDF_330132

### Advertencia de seguridad
⚠️ **Las válvulas solenoides suelen requerir más corriente de la que un Arduino puede proporcionar directamente**

### Hardware necesario
- **Módulo relé** (5V o 12V según válvula)
  - Permite al Arduino controlar cargas de alta corriente de forma segura
  - Aislamiento óptico recomendado
- **Diodo flyback** (1N4007 o similar)
  - Protección contra picos de voltaje al apagar la válvula
- **Fuente de alimentación** para la válvula (según especificaciones)

### Esquema de conexión

```
Arduino pin 9 → Módulo Relé (IN)
              → 5V (VCC)
              → GND

Módulo Relé COM → Fuente alimentación (+)
Módulo Relé NO  → Válvula TDF_330132 (+)
Válvula (-)     → Fuente alimentación (-)
```

### Conexiones detalladas:

```
Arduino UNO R4        Módulo Relé         Válvula TDF_330132
pin 9 (VALVE_CONTROL) → IN
5V                    → VCC
GND                   → GND
                        COM → V+ fuente
                        NO  → Válvula (+)
                                Válvula (-) → GND fuente
```

### Notas importantes:
1. Verificar el voltaje de operación de la válvula (consultar especificaciones TDF_330132)
2. Usar un relé con capacidad suficiente (típicamente 10A es más que suficiente)
3. El diodo flyback protege contra la fuerza contraelectromotriz de la bobina
4. Considerar una válvula **normalmente cerrada (NC)** para seguridad (cierra si falla el sistema)

---

## 3. Alimentación del Sistema

### Opciones de alimentación:

#### Opción 1: USB + Fuentes externas (recomendado para desarrollo)
- Arduino: USB-C desde PC
- Sensor PreSens: Fuente 5V (1A mínimo)
- Válvula: Fuente según especificaciones

#### Opción 2: Fuente externa única (recomendado para producción)
- Fuente de alimentación 12V / 2A (o según válvula)
- Regulador step-down 12V → 5V para Arduino y sensor
- Módulo de relé alimentado desde la misma fuente

---

## 4. Diagrama de Conexión Completo

```
                    ┌──────────────────────┐
                    │  Fuente 12V          │
                    │  (botella N₂ con     │
                    │   regulador presión) │
                    └──────────┬───────────┘
                               │
        ┌──────────────────────┴────────────────────┐
        │                                            │
┌───────▼────────┐                    ┌──────────────▼─────────┐
│ Sensor PreSens │                    │  Válvula TDF_330132    │
│  OXYBase-wr    │                    │  (Solenoide N₂)        │
│   (Sumergido)  │                    └──────────┬─────────────┘
└───────┬────────┘                               │
        │ RS232                           ┌──────▼──────┐
        │                                 │   Relé      │
   ┌────▼──────┐                         └──────┬──────┘
   │ MAX3232   │                                │
   │ Conversor │                                │
   └────┬──────┘                                │
        │ TTL                                   │
        │                                       │
     ┌──▼───────────────────────────────────────▼──┐
     │                                              │
     │        Arduino UNO R4 WiFi                   │
     │                                              │
     │  Serial1 (RX1/TX1) ← Sensor                 │
     │  Pin 9              → Válvula (vía relé)    │
     │  Pin 13             → LED status            │
     │                                              │
     └──────────────────┬───────────────────────────┘
                        │
                   USB-C (PC)
```

---

## 5. Lista de Materiales Necesarios

### Componentes principales:
- [x] Arduino UNO R4 WiFi
- [x] Sensor PreSens OXYBase-wr-RS232
- [x] Válvula solenoide TDF_330132

### Componentes adicionales:
- [ ] Módulo convertidor MAX3232 (RS232 a TTL)
- [ ] Módulo relé 5V (1 canal, con optoacoplador)
- [ ] Cables jumper macho-macho y macho-hembra
- [ ] Diodo 1N4007 (flyback para válvula)
- [ ] Protoboard o PCB para montaje
- [ ] Fuente de alimentación 5V/12V (según válvula)
- [ ] Cable RS232 (para conectar sensor PreSens)
- [ ] Resistencias pull-up/pull-down si es necesario

### Herramientas:
- [ ] Multímetro
- [ ] Soldador y estaño (para conexiones permanentes)
- [ ] Pelacables
- [ ] Destornilladores

---

## 6. Pasos de Montaje

### Paso 1: Preparar el entorno
1. Instalar PlatformIO en VS Code
2. Clonar el repositorio del proyecto
3. Verificar que el Arduino se detecta en el sistema

### Paso 2: Montar el convertidor RS232
1. Conectar MAX3232 al Arduino (TX1, RX1, 5V, GND)
2. Conectar cable RS232 al sensor PreSens
3. Verificar continuidad con multímetro

### Paso 3: Montar el control de válvula
1. Conectar módulo relé al Arduino (pin 9, 5V, GND)
2. Conectar válvula al relé (COM, NO)
3. Instalar diodo flyback en paralelo a la bobina de la válvula

### Paso 4: Alimentación
1. Conectar todas las tierras (GND común)
2. Alimentar Arduino por USB-C o fuente externa
3. Alimentar sensor y válvula según especificaciones

### Paso 5: Programar y probar
1. Cargar el código con PlatformIO
2. Abrir monitor serial (9600 baud)
3. Probar comandos: `HELP`, `STATUS`, `READ`

---

## 7. Protocolo de Comunicación PreSens

⚠️ **IMPORTANTE**: El sensor PreSens usa un **protocolo propietario**

### Información necesaria del fabricante:
- Baudrate exacto (probablemente 9600, 19200 o 38400)
- Formato de comandos (ASCII, binario, etc.)
- Comandos de inicialización
- Formato de respuestas
- Checksum o validación

### Acciones requeridas:
1. Consultar el **manual del usuario** del OXYBase-wr-RS232
2. Contactar con soporte técnico de PreSens si es necesario
3. Implementar el parser específico en el código

### Recursos:
- [PreSens - Soporte Técnico](https://www.presens.de/support)
- [PreSens - Descargas](https://www.presens.de/downloads)

---

## 8. Verificación y Pruebas

### Lista de verificación antes de encender:

- [ ] Todas las conexiones de GND están conectadas
- [ ] Voltajes de alimentación son correctos
- [ ] No hay cortocircuitos (verificar con multímetro)
- [ ] Polaridad de alimentación correcta
- [ ] Válvula conectada con diodo de protección
- [ ] Sensor sumergido en agua (si aplica)

### Pruebas graduales:

1. **Prueba 1: Arduino solo**
   - Cargar programa
   - Verificar LED status
   - Probar comandos básicos

2. **Prueba 2: Sensor PreSens**
   - Verificar comunicación Serial1
   - Leer datos raw del sensor
   - Implementar parser de protocolo

3. **Prueba 3: Válvula**
   - Probar apertura/cierre manual
   - Verificar que el relé conmuta correctamente
   - Escuchar el click del relé y válvula

4. **Prueba 4: Sistema completo**
   - Iniciar modo automático
   - Verificar control de O₂
   - Monitorizar estabilidad

---

## Contacto y Soporte

Para preguntas sobre este proyecto:
- Organización: [SIO-ICM](https://github.com/sio-icm)
- Repositorio: [github.com/sio-icm/proteo](https://github.com/sio-icm/proteo)

---

*Última actualización: 2025-10-16*
