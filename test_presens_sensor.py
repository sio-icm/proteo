#!/usr/bin/env python3
"""
Script de prueba para sensor PreSens OXYBase-wr-RS232
Conectado mediante conversor RS232-USB

Uso:
    python test_presens_sensor.py [puerto]

Ejemplo:
    python test_presens_sensor.py /dev/ttyUSB0
    python test_presens_sensor.py COM3  (Windows)
"""

import serial
import time
import sys

def find_serial_ports():
    """Lista puertos seriales disponibles"""
    import serial.tools.list_ports
    ports = serial.tools.list_ports.comports()
    return [port.device for port in ports]

def init_sensor(ser):
    """Inicializa el sensor con comando mode0001"""
    print("📡 Enviando comando de inicialización: 'mode0001'")
    ser.write(b'mode0001\r')
    time.sleep(0.5)

    # Leer respuesta si hay
    if ser.in_waiting > 0:
        response = ser.read(ser.in_waiting).decode('ascii', errors='ignore')
        print(f"   Respuesta init: {repr(response)}")
    else:
        print("   (Sin respuesta de init)")

def request_data(ser):
    """Solicita datos al sensor con comando 'data'"""
    ser.write(b'data\r')
    ser.flush()

def parse_response(raw_string):
    """
    Parsea la respuesta del sensor
    Formato esperado: campo1;amplitud;fase;temperatura;oxigeno;error
    Los valores vienen multiplicados por 100
    """
    parts = raw_string.strip().split(';')

    if len(parts) < 6:
        return None

    try:
        data = {
            'raw': raw_string,
            'campo1': parts[0],
            'amplitud': int(parts[1]),
            'fase': int(parts[2]) / 100.0,      # Dividir por 100
            'temperatura': int(parts[3]) / 100.0, # Dividir por 100
            'oxigeno': int(parts[4]) / 100.0,    # Dividir por 100
            'error': int(parts[5])
        }
        return data
    except (ValueError, IndexError) as e:
        print(f"⚠️  Error parseando: {e}")
        return None

def main():
    # Detectar puerto serial
    if len(sys.argv) > 1:
        port = sys.argv[1]
    else:
        available_ports = find_serial_ports()
        if not available_ports:
            print("❌ No se encontraron puertos seriales")
            print("💡 Conecta el conversor RS232-USB y vuelve a intentar")
            return

        print("🔍 Puertos seriales disponibles:")
        for i, p in enumerate(available_ports):
            print(f"   {i+1}. {p}")

        port = available_ports[0]
        print(f"\n📌 Usando: {port}")
        print(f"   (Para usar otro: python {sys.argv[0]} <puerto>)\n")

    try:
        # Abrir puerto serial (19200 baud según el código Campbell)
        print(f"🔌 Conectando a {port} a 19200 baud...")
        ser = serial.Serial(
            port=port,
            baudrate=19200,
            bytesize=serial.EIGHTBITS,
            parity=serial.PARITY_NONE,
            stopbits=serial.STOPBITS_ONE,
            timeout=1
        )

        print("✅ Puerto serial abierto\n")

        # Limpiar buffer
        ser.reset_input_buffer()
        ser.reset_output_buffer()

        # Inicializar sensor
        init_sensor(ser)
        time.sleep(1)

        print("\n" + "="*60)
        print("🔬 LEYENDO DATOS DEL SENSOR PRESENS")
        print("="*60)
        print("Presiona Ctrl+C para detener\n")

        counter = 0
        while True:
            counter += 1
            print(f"\n📊 Lectura #{counter} - {time.strftime('%H:%M:%S')}")
            print("-" * 50)

            # Solicitar datos
            request_data(ser)

            # Esperar respuesta (timeout 1 segundo)
            time.sleep(0.3)

            if ser.in_waiting > 0:
                # Leer hasta CR (carácter 13)
                raw_data = ser.read_until(b'\r').decode('ascii', errors='ignore')

                print(f"📥 Raw: {repr(raw_data)}")

                # Parsear datos
                data = parse_response(raw_data)

                if data:
                    print(f"✅ Datos parseados:")
                    print(f"   🔢 Amplitud:     {data['amplitud']}")
                    print(f"   📐 Fase:         {data['fase']:.2f}°")
                    print(f"   🌡️  Temperatura:  {data['temperatura']:.2f}°C")
                    print(f"   💧 Oxígeno:      {data['oxigeno']:.2f} % (o mg/L)")
                    print(f"   ⚠️  Error code:   {data['error']}")
                else:
                    print("❌ No se pudo parsear la respuesta")
            else:
                print("❌ Sin respuesta del sensor")
                print("💡 Verifica:")
                print("   - Conexión física del cable RS232")
                print("   - Alimentación del sensor (5V)")
                print("   - Puerto serial correcto")

            # Esperar 5 segundos antes de la próxima lectura
            time.sleep(5)

    except serial.SerialException as e:
        print(f"\n❌ Error al abrir puerto serial: {e}")
        print(f"💡 Verifica que {port} sea el puerto correcto")
        print("   En Linux: ls /dev/ttyUSB* o /dev/ttyACM*")
        print("   En Windows: Device Manager → Ports (COM & LPT)")
    except KeyboardInterrupt:
        print("\n\n👋 Detenido por el usuario")
    finally:
        if 'ser' in locals() and ser.is_open:
            ser.close()
            print("🔌 Puerto serial cerrado")

if __name__ == "__main__":
    main()
