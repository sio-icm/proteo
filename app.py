from flask import Flask, render_template, jsonify, send_file
from flask_cors import CORS
import threading
import time
import os
from datetime import datetime
from sensor import PresensSensor
from collections import deque

app = Flask(__name__)
CORS(app)

sensor = PresensSensor()
measurements = deque(maxlen=100)
measuring = False
measure_thread = None
lock = threading.Lock()
current_csv_session = None

def measurement_loop():
    """Loop de mediciones continuas"""
    global measuring, measurements
    
    while measuring:
        try:
            reading = sensor.read_measurement()
            if reading:
                with lock:
                    measurements.append(reading)
                print(f"Medición: O2={reading['oxygen']}%, T={reading['temperature']}°C")
        except Exception as e:
            print(f"Error en medición: {e}")
        
        time.sleep(5)

@app.route('/')
def index():
    """Página principal con el dashboard"""
    return render_template('index.html')

@app.route('/api/start', methods=['POST'])
def start_measuring():
    """Iniciar mediciones continuas"""
    global measuring, measure_thread, current_csv_session
    
    if measuring:
        return jsonify({'status': 'already_running', 'message': 'Measurements already in progress'})
    
    if not sensor.connect():
        return jsonify({'status': 'error', 'message': 'Could not connect to sensor'}), 500
    
    current_csv_session = sensor.start_new_csv_session()
    
    measuring = True
    measure_thread = threading.Thread(target=measurement_loop, daemon=True)
    measure_thread.start()
    
    return jsonify({'status': 'started', 'message': 'Measurements started', 'csv_file': current_csv_session})

@app.route('/api/stop', methods=['POST'])
def stop_measuring():
    """Detener mediciones continuas"""
    global measuring
    
    if not measuring:
        return jsonify({'status': 'not_running', 'message': 'No measurements in progress'})
    
    measuring = False
    return jsonify({'status': 'stopped', 'message': 'Measurements stopped'})

@app.route('/api/measure', methods=['POST'])
def single_measurement():
    """Realizar una sola medición"""
    if measuring:
        return jsonify({'status': 'error', 'message': 'Continuous measurements in progress. Stop first.'}), 400
    
    if not sensor.serial_connection or not sensor.serial_connection.is_open:
        if not sensor.connect():
            return jsonify({'status': 'error', 'message': 'Could not connect to sensor'}), 500
    
    reading = sensor.read_measurement()
    
    if reading:
        with lock:
            measurements.append(reading)
        return jsonify({'status': 'success', 'data': reading})
    else:
        return jsonify({'status': 'error', 'message': 'Could not read from sensor'}), 500

@app.route('/api/status', methods=['GET'])
def get_status():
    """Obtener estado actual y últimas mediciones"""
    with lock:
        recent_measurements = list(measurements)[-20:]
    
    return jsonify({
        'measuring': measuring,
        'connected': sensor.serial_connection is not None and sensor.serial_connection.is_open,
        'measurements': recent_measurements,
        'last_reading': sensor.last_reading
    })

@app.route('/api/clear', methods=['POST'])
def clear_measurements():
    """Limpiar historial de mediciones"""
    global measurements
    with lock:
        measurements.clear()
    return jsonify({'status': 'cleared', 'message': 'History cleared'})

@app.route('/api/list-csv', methods=['GET'])
def list_csv_files():
    """Listar todos los archivos CSV disponibles"""
    try:
        csv_files = []
        for filename in os.listdir('data'):
            if filename.endswith('.csv'):
                filepath = os.path.join('data', filename)
                size = os.path.getsize(filepath)
                modified = datetime.fromtimestamp(os.path.getmtime(filepath))
                csv_files.append({
                    'filename': filename,
                    'size': size,
                    'modified': modified.isoformat(),
                    'readable_date': modified.strftime('%d/%m/%Y %H:%M')
                })
        
        csv_files.sort(key=lambda x: x['modified'], reverse=True)
        return jsonify({'status': 'success', 'files': csv_files})
    except Exception as e:
        return jsonify({'status': 'error', 'message': str(e)}), 500

@app.route('/api/download-csv/<filename>', methods=['GET'])
def download_csv(filename):
    """Descargar un archivo CSV específico"""
    csv_path = os.path.join('data', filename)
    
    if not filename.endswith('.csv'):
        return jsonify({'status': 'error', 'message': 'Invalid file'}), 400
    
    if os.path.exists(csv_path):
        return send_file(
            csv_path,
            mimetype='text/csv',
            as_attachment=True,
            download_name=filename
        )
    else:
        return jsonify({'status': 'error', 'message': 'File not found'}), 404

if __name__ == '__main__':
    try:
        sensor.connect()
        app.run(host='0.0.0.0', port=5000, debug=False)
    except Exception as e:
        print(f"Error starting server: {e}")
        if sensor.serial_connection:
            sensor.disconnect()