#!/usr/bin/env python3
import serial
import sys
import threading
import time

def leer_serial(ser):
    """Lee continuamente del puerto serial"""
    while True:
        try:
            if ser.in_waiting > 0:
                line = ser.readline().decode('utf-8', errors='ignore').rstrip()
                if line:
                    print(f"{line}")
                    sys.stdout.flush()
        except Exception as e:
            print(f"Error leyendo: {e}")
            break
        time.sleep(0.01)

try:
    # Abrir puerto serial
    ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
    print("=" * 60)
    print("📡 Monitor Serial Interactivo - Proteo")
    print("=" * 60)
    print("Conectado a /dev/ttyACM0 @ 9600 baud")
    print("Escribe comandos y presiona Enter")
    print("Ctrl+C para salir")
    print("=" * 60)
    print()

    # Esperar reset del Arduino
    time.sleep(2)

    # Limpiar buffer
    ser.reset_input_buffer()

    # Iniciar hilo para leer del serial
    hilo_lectura = threading.Thread(target=leer_serial, args=(ser,), daemon=True)
    hilo_lectura.start()

    # Leer comandos del usuario
    while True:
        try:
            comando = input()
            if comando.strip():
                ser.write((comando + '\n').encode('utf-8'))
                ser.flush()
        except EOFError:
            break

except KeyboardInterrupt:
    print("\n\n✓ Desconectado")
except serial.SerialException as e:
    print(f"❌ Error: {e}")
    print("Asegúrate de que no hay otro programa usando el puerto")
except Exception as e:
    print(f"❌ Error: {e}")
finally:
    if 'ser' in locals():
        ser.close()
