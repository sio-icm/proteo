/*
 * Proteo - Sistema de desoxigenación de agua mediante nitrógeno
 * Controlador principal para inyección controlada de N₂
 *
 * Organización: SIO-ICM
 * Hardware:
 *   - Arduino UNO R4 WiFi
 *   - Sensor PreSens OXYBase-wr-RS232 (vía convertidor MAX3232)
 *   - Válvula solenoide TDF_330132
 */

#include <Arduino.h>

// ==================== CONFIGURACIÓN DE PINES ====================

// Válvula solenoide para nitrógeno
const int VALVE_CONTROL = 9;        // Pin digital para control de válvula

// Comunicación con sensor PreSens (TTL directo, 5V)
// El sensor ya tiene señales TTL, NO necesita convertidor MAX3232
// Usamos Serial1 para el sensor (pines TX1/RX1)
// Serial0 (USB) queda libre para debug/comandos

// LED de estado
const int LED_STATUS = 13;

// ==================== VARIABLES DE CONTROL ====================

// Sistema
bool systemActive = false;

// Válvula
bool valveOpen = false;

// Sensor PreSens
float currentO2_mgL = 0.0;      // Oxígeno disuelto en mg/L
float currentO2_percent = 0.0;   // Oxígeno en %
float currentTemp = 0.0;         // Temperatura del agua

// Límites de seguridad
const float TARGET_O2 = 0.5;     // Objetivo: 0.5 mg/L (casi anóxico)
const float MAX_TEMP = 35.0;     // Temperatura máxima del agua
const float MIN_O2_READING = 0.02; // Límite de detección del sensor

// Tiempos
unsigned long lastSensorRead = 0;
const unsigned long SENSOR_READ_INTERVAL = 2000; // Leer sensor cada 2 segundos

// ==================== FUNCIONES DE COMUNICACIÓN RS232 ====================

/**
 * Inicializa la comunicación con el sensor PreSens
 * El protocolo exacto depende del manual del fabricante
 *
 * CONEXIÓN DIRECTA TTL:
 *   PreSens VCC → Arduino 5V
 *   PreSens GND → Arduino GND
 *   PreSens TX  → Arduino RX1 (pin 0)
 *   PreSens RX  → Arduino TX1 (pin 1)
 */
void initPreSensSensor() {
  // Serial1 para comunicación con sensor PreSens (TTL directo, 5V)
  Serial1.begin(9600); // Verificar baudrate en manual PreSens

  Serial.println("Inicializando sensor PreSens OXYBase-wr-RS232...");
  Serial.println("Conexión: TTL directo (sin MAX3232)");
  delay(1000); // Esperar estabilización

  // TODO: Enviar comandos de inicialización según protocolo PreSens
  // Ejemplo (ajustar según documentación):
  // Serial1.println("MODE 1");  // Modo de lectura continua
  // Serial1.println("UNIT 1");  // Unidades en mg/L

  Serial.println("Sensor PreSens inicializado");
}

/**
 * Lee datos del sensor PreSens
 * NOTA: Esta función es un placeholder
 * Debe implementarse según el protocolo propietario de PreSens
 */
bool readPreSensSensor() {
  // TODO: Implementar parser del protocolo PreSens
  // El protocolo exacto requiere documentación del fabricante

  if (Serial1.available() > 0) {
    String response = Serial1.readStringUntil('\n');

    // Placeholder: parsear respuesta
    // Formato esperado (ejemplo): "O2:5.23;TEMP:22.5;"
    // Esto debe ajustarse al protocolo real

    Serial.print("Sensor raw: ");
    Serial.println(response);

    // TODO: Parsear valores reales
    // Por ahora, valores simulados para testing
    currentO2_mgL = 8.0;  // Placeholder
    currentO2_percent = 100.0 * currentO2_mgL / 9.0; // Aproximado
    currentTemp = 22.0;    // Placeholder

    return true;
  }

  return false;
}

/**
 * Solicita una lectura al sensor PreSens
 */
void requestSensorReading() {
  // TODO: Implementar según protocolo PreSens
  // Ejemplo (ajustar según manual):
  // Serial1.println("READ");

  Serial1.println("?"); // Placeholder - ajustar al comando real
}

// ==================== CONTROL DE VÁLVULA ====================

/**
 * Abre la válvula de nitrógeno
 */
void openValve() {
  digitalWrite(VALVE_CONTROL, HIGH);
  valveOpen = true;
  Serial.println("Válvula ABIERTA - Inyectando N₂");
}

/**
 * Cierra la válvula de nitrógeno
 */
void closeValve() {
  digitalWrite(VALVE_CONTROL, LOW);
  valveOpen = false;
  Serial.println("Válvula CERRADA");
}

// ==================== CONTROL DEL SISTEMA ====================

/**
 * Control principal del sistema de desoxigenación
 * Usa lógica simple on/off (mejorar con PID)
 */
void controlDeoxygenation() {
  // Lógica simple: si O₂ > objetivo, abrir válvula
  if (currentO2_mgL > TARGET_O2 + 0.2) { // Histéresis de 0.2 mg/L
    if (!valveOpen) {
      openValve();
    }
  }
  // Si O₂ <= objetivo, cerrar válvula
  else if (currentO2_mgL <= TARGET_O2) {
    if (valveOpen) {
      closeValve();
    }
  }

  // TODO: Implementar control PID para mejor precisión
}

/**
 * Verifica condiciones de seguridad
 */
void checkSafety() {
  // Verificar temperatura
  if (currentTemp > MAX_TEMP) {
    Serial.println("⚠️ ALERTA: Temperatura alta - deteniendo sistema");
    stopSystem();
    return;
  }

  // Verificar que el sensor esté respondiendo
  // (implementar timeout de comunicación)
}

// ==================== COMANDOS DEL SISTEMA ====================

void startSystem() {
  systemActive = true;
  digitalWrite(LED_STATUS, HIGH);
  Serial.println("✓ Sistema activado");
  Serial.println("Objetivo de O₂: " + String(TARGET_O2) + " mg/L");
}

void stopSystem() {
  systemActive = false;
  closeValve(); // Seguridad: cerrar válvula
  digitalWrite(LED_STATUS, LOW);
  Serial.println("✓ Sistema detenido");
}

void printStatus() {
  Serial.println("\n=== ESTADO DEL SISTEMA PROTEO ===");
  Serial.print("Estado: ");
  Serial.println(systemActive ? "ACTIVO" : "DETENIDO");
  Serial.print("Válvula N₂: ");
  Serial.println(valveOpen ? "ABIERTA" : "CERRADA");
  Serial.println("\n--- Sensor PreSens OXYBase-wr ---");
  Serial.print("O₂ disuelto: ");
  Serial.print(currentO2_mgL);
  Serial.println(" mg/L");
  Serial.print("O₂ %: ");
  Serial.print(currentO2_percent);
  Serial.println(" %");
  Serial.print("Temperatura: ");
  Serial.print(currentTemp);
  Serial.println(" °C");
  Serial.print("Objetivo O₂: ");
  Serial.print(TARGET_O2);
  Serial.println(" mg/L");
  Serial.println("================================\n");
}

/**
 * Procesa comandos recibidos por Serial (USB)
 */
void processCommand(String cmd) {
  cmd.trim();
  cmd.toUpperCase();

  if (cmd == "START") {
    startSystem();
  }
  else if (cmd == "STOP") {
    stopSystem();
  }
  else if (cmd == "STATUS") {
    printStatus();
  }
  else if (cmd == "VALVE OPEN") {
    openValve();
  }
  else if (cmd == "VALVE CLOSE") {
    closeValve();
  }
  else if (cmd == "READ") {
    requestSensorReading();
  }
  else if (cmd == "HELP") {
    Serial.println("\n=== COMANDOS DISPONIBLES ===");
    Serial.println("START       - Iniciar sistema automático");
    Serial.println("STOP        - Detener sistema");
    Serial.println("STATUS      - Mostrar estado actual");
    Serial.println("VALVE OPEN  - Abrir válvula manualmente");
    Serial.println("VALVE CLOSE - Cerrar válvula manualmente");
    Serial.println("READ        - Solicitar lectura del sensor");
    Serial.println("HELP        - Mostrar esta ayuda");
    Serial.println("============================\n");
  }
  else {
    Serial.println("Comando desconocido. Escribe HELP para ver comandos.");
  }
}

// ==================== SETUP Y LOOP ====================

void setup() {
  // Comunicación USB para comandos y debug
  Serial.begin(9600);
  while (!Serial && millis() < 3000); // Esperar conexión USB (max 3s)

  Serial.println("\n================================");
  Serial.println("   PROTEO - Sistema de");
  Serial.println("   Desoxigenación con N₂");
  Serial.println("================================");
  Serial.println("Organización: SIO-ICM");
  Serial.println("Hardware: Arduino UNO R4 WiFi");
  Serial.println("================================\n");

  // Configurar pines
  pinMode(VALVE_CONTROL, OUTPUT);
  pinMode(LED_STATUS, OUTPUT);

  // Inicializar en estado seguro
  closeValve();
  digitalWrite(LED_STATUS, LOW);

  // Inicializar sensor PreSens
  initPreSensSensor();

  Serial.println("\nSistema listo.");
  Serial.println("Escribe HELP para ver comandos disponibles.\n");
}

void loop() {
  // Leer comandos del usuario (USB Serial)
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    processCommand(command);
  }

  // Leer sensor periódicamente
  if (millis() - lastSensorRead >= SENSOR_READ_INTERVAL) {
    requestSensorReading();
    delay(100); // Esperar respuesta
    readPreSensSensor();
    lastSensorRead = millis();
  }

  // Si el sistema está activo, controlar desoxigenación
  if (systemActive) {
    controlDeoxygenation();
    checkSafety();
  }

  delay(100);
}
