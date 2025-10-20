#!/usr/bin/env python3
"""
Script simple de prueba para sensor PreSens
"""

import serial
import time
import sys

print("=== TEST SENSOR PRESENS ===", flush=True)
print(f"Puerto: {sys.argv[1] if len(sys.argv) > 1 else '/dev/ttyUSB0'}", flush=True)

port = sys.argv[1] if len(sys.argv) > 1 else '/dev/ttyUSB0'

try:
    print(f"\n1. Abriendo puerto {port} a 19200 baud...", flush=True)
    ser = serial.Serial(
        port=port,
        baudrate=19200,
        timeout=1
    )
    print("   ✅ Puerto abierto", flush=True)

    # Limpiar
    ser.reset_input_buffer()
    ser.reset_output_buffer()

    print("\n2. Enviando 'mode0001\\r'...", flush=True)
    ser.write(b'mode0001\r')
    time.sleep(0.5)

    if ser.in_waiting > 0:
        resp = ser.read(ser.in_waiting)
        print(f"   Respuesta: {repr(resp)}", flush=True)
    else:
        print("   Sin respuesta", flush=True)

    print("\n3. Enviando 'data\\r' (3 intentos)...", flush=True)
    for i in range(3):
        print(f"\n   Intento {i+1}:", flush=True)
        ser.write(b'data\r')
        ser.flush()
        time.sleep(0.5)

        if ser.in_waiting > 0:
            data = ser.read_until(b'\r')
            print(f"   ✅ Recibido: {repr(data)}", flush=True)
            print(f"   Decodificado: {data.decode('ascii', errors='ignore').strip()}", flush=True)

            # Parsear
            parts = data.decode('ascii', errors='ignore').strip().split(';')
            if len(parts) >= 6:
                # Los valores tienen prefijos: A, P, T, O, E
                amp = int(parts[1][1:])  # Quitar 'A'
                phase = int(parts[2][1:]) / 100.0  # Quitar 'P'
                temp = int(parts[3][1:]) / 100.0   # Quitar 'T'
                oxy = int(parts[4][1:]) / 100.0    # Quitar 'O'
                error = int(parts[5][1:])          # Quitar 'E'

                print(f"   📊 Parseado:", flush=True)
                print(f"      Amplitud: {amp}", flush=True)
                print(f"      Fase: {phase:.2f}°", flush=True)
                print(f"      Temperatura: {temp:.2f}°C", flush=True)
                print(f"      Oxígeno: {oxy:.2f}%", flush=True)
                print(f"      Error: {error}", flush=True)
        else:
            print(f"   ❌ Sin respuesta", flush=True)

        time.sleep(2)

    ser.close()
    print("\n✅ Test completado", flush=True)

except Exception as e:
    print(f"\n❌ Error: {e}", flush=True)
    import traceback
    traceback.print_exc()
