import serial
import time
import re
import csv
import os
from typing import Optional, Dict
from datetime import datetime

class PresensSensor:
    def __init__(self, port='/dev/ttyUSB0', baudrate=19200, timeout=2):
        self.port = port
        self.baudrate = baudrate
        self.timeout = timeout
        self.serial_connection = None
        self.last_reading = None
        self.current_csv_file = None
        
    def connect(self):
        """Conectar al sensor"""
        try:
            self.serial_connection = serial.Serial(
                port=self.port,
                baudrate=self.baudrate,
                timeout=self.timeout
            )
            time.sleep(1)
            self.serial_connection.reset_input_buffer()
            
            self.serial_connection.write(b'mode0001\r')
            time.sleep(0.1)
            self.serial_connection.reset_input_buffer()
            
            return True
        except Exception as e:
            print(f"Error connecting to sensor: {e}")
            return False
    
    def disconnect(self):
        """Desconectar del sensor"""
        if self.serial_connection and self.serial_connection.is_open:
            self.serial_connection.close()
    
    def parse_response(self, response: str) -> Optional[Dict]:
        """Parsear la respuesta del sensor"""
        pattern = r'N\d+;A(\d+);P(\d+);T(\d+);O(\d+);E(\d+)'
        match = re.match(pattern, response)
        
        if match:
            amplitude, phase, temp, oxygen, error = match.groups()
            return {
                'timestamp': datetime.now().isoformat(),
                'amplitude': int(amplitude),
                'phase': int(phase) / 100,
                'temperature': int(temp) / 100,
                'oxygen': int(oxygen) / 100,
                'error': int(error)
            }
        return None
    
    def start_new_csv_session(self):
        """Iniciar una nueva sesión CSV con timestamp"""
        timestamp = datetime.now().strftime('%Y%m%d_%H%M%S')
        self.current_csv_file = f'data/session_{timestamp}.csv'
        return self.current_csv_file
    
    def save_to_csv(self, measurement: Dict):
        """Guardar medición en archivo CSV de la sesión actual"""
        try:
            if not self.current_csv_file:
                self.start_new_csv_session()
            
            file_exists = os.path.exists(self.current_csv_file)
            
            with open(self.current_csv_file, 'a', newline='') as csvfile:
                fieldnames = ['timestamp', 'oxygen', 'temperature', 'phase', 'amplitude', 'error']
                writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
                
                if not file_exists:
                    writer.writeheader()
                
                writer.writerow(measurement)
                
        except Exception as e:
            print(f"Error saving to CSV: {e}")
    
    def read_measurement(self) -> Optional[Dict]:
        """Leer una medición del sensor"""
        if not self.serial_connection or not self.serial_connection.is_open:
            if not self.connect():
                return None
        
        try:
            self.serial_connection.reset_input_buffer()
            self.serial_connection.write(b'data\r')
            time.sleep(0.2)
            
            response = self.serial_connection.readline()
            
            if response:
                decoded = response.decode('utf-8', errors='ignore').strip()
                measurement = self.parse_response(decoded)
                if measurement:
                    self.last_reading = measurement
                    self.save_to_csv(measurement)
                    return measurement
                    
        except Exception as e:
            print(f"Error reading from sensor: {e}")
            
        return None
    
    def __enter__(self):
        """Permitir uso con context manager"""
        self.connect()
        return self
    
    def __exit__(self, exc_type, exc_val, exc_tb):
        """Cerrar conexión al salir del context manager"""
        self.disconnect()