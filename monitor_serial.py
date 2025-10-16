#!/usr/bin/env python3
import serial
import time
import sys

try:
    ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
    print("=" * 60)
    print("📡 Monitor Serial - Proteo")
    print("=" * 60)
    print("Puerto: /dev/ttyACM0 @ 9600 baud")
    print("Presiona Ctrl+C para salir")
    print("=" * 60)
    print()

    # Esperar un momento para que el Arduino se reinicie
    time.sleep(2)

    while True:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8', errors='ignore').strip()
            if line:
                print(line)
                sys.stdout.flush()
        time.sleep(0.01)

except KeyboardInterrupt:
    print("\n\n✓ Desconectado")
    ser.close()
except serial.SerialException as e:
    print(f"❌ Error: {e}")
except Exception as e:
    print(f"❌ Error: {e}")
