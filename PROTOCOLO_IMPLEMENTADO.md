# ✅ Protocolo PreSens Implementado

Gracias al código Campbell que me proporcionaste, ahora el Arduino habla correctamente con el sensor.

## 🔑 Configuración Implementada

### Comunicación Serial
- **Baudrate**: `19200` (no 9600!)
- **Puerto**: `Serial1` (TX1/RX1, pines 0 y 1)
- **Terminador**: CR (`\r`, carriage return)

### Comandos
```cpp
// Inicialización
Serial1.print("mode0001\r");

// Solicitar datos
Serial1.print("data\r");
```

### Formato de Respuesta
```
campo1;Amplitud;Fase;Temperatura;Oxígeno;Error\r
```

Ejemplo:
```
0;1234567;3456;2234;823;0
```

Que se traduce a:
- Amplitud: 1234567
- Fase: 34.56° (dividir por 100)
- Temperatura: 22.34°C (dividir por 100)
- Oxígeno: 8.23 mg/L (dividir por 100)
- Error: 0

## 📝 Código Implementado

El código ahora:

1. ✅ Inicializa el sensor con `mode0001`
2. ✅ Solicita datos cada 5 segundos con `data`
3. ✅ Parsea la respuesta (separada por `;`)
4. ✅ Divide los valores por 100 (según protocolo)
5. ✅ Muestra datos procesados y raw
6. ✅ Controla la válvula según nivel de O₂
7. ✅ Acepta comandos: START, STOP, STATUS, READ, HELP

## 🔌 Verificar Conexiones

Asegúrate de que:
- ✅ VCC (sensor) → 5V (Arduino)
- ✅ GND (sensor) → GND (Arduino)
- ✅ TX (sensor) → RX1 / Pin 0 (Arduino)
- ✅ RX (sensor) → TX1 / Pin 1 (Arduino)

## 🧪 Probar

Ejecuta:
```bash
python3 /home/raul/code/proteo/ver_sensor.py
```

Deberías ver cada 5 segundos:
```
--- Leyendo sensor ---
📥 Datos raw: 0;1234567;3456;2234;823;0

✅ Datos procesados:
   Amplitud: 1234567
   Fase: 34.56 °
   Temperatura: 22.34 °C
   Oxígeno: 8.23 mg/L
   Error: 0
```

## ⚠️ Si No Funciona

1. **Verifica baudrate**: Debe ser 19200
2. **Verifica TX/RX**: Deben estar cruzados
3. **Sensor en agua**: El sensor puede necesitar estar sumergido
4. **Tiempo de warm-up**: Algunos sensores tardan 1-2 minutos en estabilizarse

## 📊 Comandos Disponibles

```
HELP         - Ver comandos
STATUS       - Ver estado completo
READ         - Leer sensor manualmente
START        - Activar control automático
STOP         - Detener sistema
VALVE OPEN   - Abrir válvula manualmente
VALVE CLOSE  - Cerrar válvula manualmente
```

## 🎯 Control Automático

Cuando escribes `START`:
- Lee el sensor cada 5 segundos
- Si O₂ > 0.7 mg/L → Abre válvula (inyecta N₂)
- Si O₂ ≤ 0.5 mg/L → Cierra válvula
- Objetivo: mantener ~0.5 mg/L (condiciones anóxicas)

---

**El código está listo. Solo falta verificar que el sensor responda.**

Si el sensor está bien conectado, deberías empezar a ver datos reales cada 5 segundos.
