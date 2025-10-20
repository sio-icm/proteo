/*
 * Proteo - Sistema de Desoxigenación con Sensor PreSens
 * Control de oxígeno disuelto mediante inyección de nitrógeno
 *
 * Hardware:
 * - Arduino UNO R4 WiFi
 * - Sensor PreSens OXYBase-wr-RS232 (Serial1, 19200 baud)
 * - Válvula solenoide TDF_330132 (pines 9 y 10)
 */

#include <Arduino.h>

// ========== CONFIGURACIÓN DE PINES ==========
const int PIN_VALVULA_1 = 9;      // Control válvula solenoide
const int PIN_VALVULA_2 = 10;     // Control válvula solenoide (backup/doble bobina)
const int LED_STATUS = 13;        // LED de estado

// ========== CONFIGURACIÓN DEL SISTEMA ==========
const float TEMP_MAX = 60.0;           // Temperatura máxima de seguridad (°C)
const float OXYGEN_TARGET = 50.0;      // Nivel objetivo de oxígeno (%) - configurable
const unsigned long SENSOR_INTERVAL = 5000;  // Intervalo de lectura del sensor (ms)

// ========== ESTRUCTURA DE DATOS DEL SENSOR ==========
struct SensorData {
  bool valid;              // ¿Lectura válida?
  unsigned long timestamp; // Tiempo de la lectura (ms)
  long amplitud;          // Amplitud de la señal
  float fase;             // Fase (grados)
  float temperatura;      // Temperatura (°C)
  float oxigeno;          // Oxígeno disuelto (%)
  long error;             // Código de error del sensor
};

// ========== VARIABLES GLOBALES ==========
SensorData lastReading;           // Última lectura válida del sensor
bool sistemaActivo = false;       // Estado del sistema
bool valvulaAbierta = false;      // Estado de la válvula
unsigned long lastSensorRead = 0; // Última lectura del sensor
String serialBuffer = "";         // Buffer para datos del sensor

// ========== DECLARACIÓN DE FUNCIONES ==========
void initSensor();
void requestSensorData();
bool parseSensorResponse(String response, SensorData &data);
void readSensor();
void controlValvula(bool abrir);
void checkSafety();
void printSensorData(const SensorData &data);
void handleSerialCommands();

// ========== SETUP ==========
void setup() {
  // Inicializar Serial para debug/control
  Serial.begin(9600);
  while (!Serial && millis() < 3000);

  Serial.println("\n========================================");
  Serial.println("   PROTEO - Sistema de Desoxigenación");
  Serial.println("========================================\n");

  // Configurar pines
  pinMode(LED_STATUS, OUTPUT);
  pinMode(PIN_VALVULA_1, OUTPUT);
  pinMode(PIN_VALVULA_2, OUTPUT);

  // Asegurar válvula cerrada al inicio
  controlValvula(false);

  // Inicializar sensor PreSens
  Serial.println("🔧 Inicializando sensor PreSens...");
  initSensor();

  Serial.println("✅ Sistema inicializado");
  Serial.println("\nComandos disponibles:");
  Serial.println("  START  - Iniciar sistema de desoxigenación");
  Serial.println("  STOP   - Detener sistema");
  Serial.println("  STATUS - Ver estado actual");
  Serial.println("  READ   - Leer sensor manualmente");
  Serial.println("  TARGET <valor> - Establecer objetivo de O2 (%)");
  Serial.println("========================================\n");

  lastReading.valid = false;
}

// ========== LOOP PRINCIPAL ==========
void loop() {
  // Leer comandos seriales del usuario
  handleSerialCommands();

  // Leer sensor periódicamente
  if (millis() - lastSensorRead >= SENSOR_INTERVAL) {
    readSensor();
    lastSensorRead = millis();
  }

  // Verificar condiciones de seguridad
  if (sistemaActivo) {
    checkSafety();
  }

  // Parpadear LED si el sistema está activo
  if (sistemaActivo) {
    static unsigned long lastBlink = 0;
    if (millis() - lastBlink >= 500) {
      digitalWrite(LED_STATUS, !digitalRead(LED_STATUS));
      lastBlink = millis();
    }
  } else {
    digitalWrite(LED_STATUS, LOW);
  }

  delay(10);
}

// ========== INICIALIZACIÓN DEL SENSOR ==========
void initSensor() {
  // Inicializar puerto serial del sensor (Serial1 en Arduino R4)
  Serial1.begin(19200);
  delay(500);

  // Limpiar buffer
  while (Serial1.available()) {
    Serial1.read();
  }

  // Enviar comando de modo (mode0001)
  Serial.println("   Enviando comando 'mode0001'...");
  Serial1.print("mode0001\r");
  Serial1.flush();
  delay(500);

  // Limpiar posible respuesta
  while (Serial1.available()) {
    Serial1.read();
  }

  Serial.println("   ✅ Sensor configurado");
}

// ========== SOLICITAR DATOS AL SENSOR ==========
void requestSensorData() {
  // Limpiar buffer antes de solicitar
  serialBuffer = "";
  while (Serial1.available()) {
    Serial1.read();
  }

  // Enviar comando 'data'
  Serial1.print("data\r");
  Serial1.flush();
}

// ========== PARSEAR RESPUESTA DEL SENSOR ==========
bool parseSensorResponse(String response, SensorData &data) {
  // Formato esperado: N01;A0119201;P2543;T2932;O017227;E00000000
  // Separado por ';' con prefijos A, P, T, O, E

  response.trim();

  // Dividir por ';'
  int idx1 = response.indexOf(';');
  int idx2 = response.indexOf(';', idx1 + 1);
  int idx3 = response.indexOf(';', idx2 + 1);
  int idx4 = response.indexOf(';', idx3 + 1);
  int idx5 = response.indexOf(';', idx4 + 1);

  if (idx1 == -1 || idx2 == -1 || idx3 == -1 || idx4 == -1 || idx5 == -1) {
    return false;  // Formato inválido
  }

  // Extraer partes
  String amp_str = response.substring(idx1 + 1, idx2);    // A0119201
  String phase_str = response.substring(idx2 + 1, idx3);  // P2543
  String temp_str = response.substring(idx3 + 1, idx4);   // T2932
  String oxy_str = response.substring(idx4 + 1, idx5);    // O017227
  String err_str = response.substring(idx5 + 1);          // E00000000

  // Verificar prefijos y extraer valores
  if (amp_str.charAt(0) != 'A' || phase_str.charAt(0) != 'P' ||
      temp_str.charAt(0) != 'T' || oxy_str.charAt(0) != 'O' ||
      err_str.charAt(0) != 'E') {
    return false;
  }

  // Parsear valores (quitando el primer carácter que es el prefijo)
  data.amplitud = amp_str.substring(1).toInt();
  data.fase = phase_str.substring(1).toInt() / 100.0;
  data.temperatura = temp_str.substring(1).toInt() / 100.0;
  data.oxigeno = oxy_str.substring(1).toInt() / 100.0;
  data.error = err_str.substring(1).toInt();
  data.timestamp = millis();
  data.valid = true;

  return true;
}

// ========== LEER SENSOR ==========
void readSensor() {
  // Solicitar datos
  requestSensorData();

  // Esperar respuesta (hasta 1 segundo)
  unsigned long start = millis();
  serialBuffer = "";

  while (millis() - start < 1000) {
    if (Serial1.available()) {
      char c = Serial1.read();

      // El sensor termina con \n\r
      if (c == '\r' || c == '\n') {
        if (serialBuffer.length() > 0) {
          break;  // Fin de la respuesta
        }
      } else {
        serialBuffer += c;
      }
    }
    delay(10);
  }

  // Parsear respuesta
  if (serialBuffer.length() > 0) {
    SensorData newData;
    if (parseSensorResponse(serialBuffer, newData)) {
      lastReading = newData;

      // Mostrar datos si el sistema está activo o en modo debug
      if (sistemaActivo) {
        printSensorData(lastReading);
      }
    } else {
      Serial.println("⚠️  Error: Respuesta del sensor inválida");
      Serial.print("   Raw: ");
      Serial.println(serialBuffer);
    }
  } else {
    Serial.println("⚠️  Error: Sin respuesta del sensor");
  }
}

// ========== CONTROL DE VÁLVULA ==========
void controlValvula(bool abrir) {
  valvulaAbierta = abrir;

  if (abrir) {
    digitalWrite(PIN_VALVULA_1, HIGH);
    digitalWrite(PIN_VALVULA_2, HIGH);
    Serial.println("🔓 Válvula ABIERTA - Inyectando N₂");
  } else {
    digitalWrite(PIN_VALVULA_1, LOW);
    digitalWrite(PIN_VALVULA_2, LOW);
    Serial.println("🔒 Válvula CERRADA");
  }
}

// ========== VERIFICACIÓN DE SEGURIDAD ==========
void checkSafety() {
  if (!lastReading.valid) {
    return;  // No hay lectura válida aún
  }

  // Verificar temperatura máxima
  if (lastReading.temperatura > TEMP_MAX) {
    Serial.println("\n⚠️  ¡ALERTA! Temperatura excedida");
    Serial.print("   Temperatura: ");
    Serial.print(lastReading.temperatura);
    Serial.println("°C");

    // Cerrar válvula y detener sistema
    controlValvula(false);
    sistemaActivo = false;

    Serial.println("❌ Sistema detenido por seguridad");
    return;
  }

  // Verificar código de error del sensor
  if (lastReading.error != 0) {
    Serial.println("\n⚠️  ¡ALERTA! Error del sensor");
    Serial.print("   Código de error: ");
    Serial.println(lastReading.error);

    // Cerrar válvula por seguridad
    controlValvula(false);
    Serial.println("🔒 Válvula cerrada por error del sensor");
  }

  // Control simple: abrir válvula si O2 > objetivo
  // TODO: Implementar control PID para mayor precisión
  if (lastReading.oxigeno > OXYGEN_TARGET) {
    if (!valvulaAbierta) {
      controlValvula(true);
    }
  } else {
    if (valvulaAbierta) {
      controlValvula(false);
    }
  }
}

// ========== IMPRIMIR DATOS DEL SENSOR ==========
void printSensorData(const SensorData &data) {
  Serial.println("\n📊 Lectura del sensor:");
  Serial.print("   🌡️  Temperatura: ");
  Serial.print(data.temperatura, 2);
  Serial.println(" °C");

  Serial.print("   💧 Oxígeno:      ");
  Serial.print(data.oxigeno, 2);
  Serial.println(" %");

  Serial.print("   📐 Fase:         ");
  Serial.print(data.fase, 2);
  Serial.println(" °");

  Serial.print("   🔢 Amplitud:     ");
  Serial.println(data.amplitud);

  Serial.print("   ⚠️  Error:        ");
  Serial.println(data.error);

  Serial.print("   ⏱️  Timestamp:    ");
  Serial.print(data.timestamp / 1000);
  Serial.println(" s");
}

// ========== MANEJAR COMANDOS SERIALES ==========
void handleSerialCommands() {
  static String commandBuffer = "";

  while (Serial.available()) {
    char c = Serial.read();

    if (c == '\n' || c == '\r') {
      if (commandBuffer.length() > 0) {
        commandBuffer.toUpperCase();
        commandBuffer.trim();

        // Procesar comando
        if (commandBuffer == "START") {
          Serial.println("\n🚀 Iniciando sistema...");
          sistemaActivo = true;
          Serial.println("✅ Sistema ACTIVO");

        } else if (commandBuffer == "STOP") {
          Serial.println("\n⏹️  Deteniendo sistema...");
          sistemaActivo = false;
          controlValvula(false);
          Serial.println("✅ Sistema DETENIDO");

        } else if (commandBuffer == "STATUS") {
          Serial.println("\n📋 Estado del sistema:");
          Serial.print("   Sistema: ");
          Serial.println(sistemaActivo ? "ACTIVO" : "DETENIDO");
          Serial.print("   Válvula: ");
          Serial.println(valvulaAbierta ? "ABIERTA" : "CERRADA");
          Serial.print("   Objetivo O₂: ");
          Serial.print(OXYGEN_TARGET);
          Serial.println(" %");

          if (lastReading.valid) {
            printSensorData(lastReading);
          } else {
            Serial.println("   ⚠️  Sin lecturas del sensor");
          }

        } else if (commandBuffer == "READ") {
          Serial.println("\n📖 Leyendo sensor...");
          readSensor();
          if (lastReading.valid) {
            printSensorData(lastReading);
          }

        } else {
          Serial.println("\n❌ Comando desconocido");
          Serial.println("Comandos: START, STOP, STATUS, READ");
        }

        commandBuffer = "";
      }
    } else {
      commandBuffer += c;
    }
  }
}
