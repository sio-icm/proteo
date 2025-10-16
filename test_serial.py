#!/usr/bin/env python3
import serial
import time

try:
    # Abrir puerto serial
    ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
    print("✅ Conectado al Arduino en /dev/ttyACM0")
    print("📡 Esperando datos... (10 segundos)")
    print("-" * 50)

    start_time = time.time()
    while time.time() - start_time < 10:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8', errors='ignore').strip()
            if line:
                print(line)
        time.sleep(0.1)

    print("-" * 50)
    print("✅ Test completado")
    ser.close()

except serial.SerialException as e:
    print(f"❌ Error: {e}")
    print("\n💡 Solución:")
    print("sudo usermod -a -G dialout $USER")
    print("Luego cierra sesión y vuelve a entrar")
except Exception as e:
    print(f"❌ Error inesperado: {e}")
