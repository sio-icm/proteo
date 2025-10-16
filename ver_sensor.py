#!/usr/bin/env python3
import serial
import time

ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
print("="*60)
print("📡 Leyendo Arduino - Solo mostrar datos")
print("="*60)
print("Presiona Ctrl+C para salir\n")

time.sleep(2)  # Esperar reset

try:
    while True:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8', errors='ignore').strip()
            if line:
                print(line)
        time.sleep(0.01)
except KeyboardInterrupt:
    print("\n\n✓ Cerrado")
    ser.close()
