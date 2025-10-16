/*
 * Proteo - Sistema de extracción de oxígeno
 * Controlador principal para electrólisis de agua salada
 * 
 * Organización: SIO-ICM
 */

// Configuración de pines
const int ELECTRODE_POSITIVE = 9;  // Pin PWM para electrodo positivo
const int ELECTRODE_NEGATIVE = 10; // Pin PWM para electrodo negativo
const int SENSOR_CURRENT = A0;     // Sensor de corriente
const int SENSOR_TEMP = A1;        // Sensor de temperatura
const int LED_STATUS = 13;         // LED de estado

// Variables de control
float targetCurrent = 2.0;  // Corriente objetivo en Amperios
float maxTemp = 60.0;       // Temperatura máxima en Celsius
bool systemActive = false;

void setup() {
  Serial.begin(9600);
  
  // Configurar pines
  pinMode(ELECTRODE_POSITIVE, OUTPUT);
  pinMode(ELECTRODE_NEGATIVE, OUTPUT);
  pinMode(LED_STATUS, OUTPUT);
  
  // Inicializar sistema apagado
  digitalWrite(ELECTRODE_POSITIVE, LOW);
  digitalWrite(ELECTRODE_NEGATIVE, LOW);
  digitalWrite(LED_STATUS, LOW);
  
  Serial.println("Proteo - Sistema iniciado");
  Serial.println("Comandos: START, STOP, STATUS");
}

void loop() {
  // Leer comandos seriales
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    processCommand(command);
  }
  
  // Si el sistema está activo, controlar electrólisis
  if (systemActive) {
    controlElectrolysis();
    checkSafety();
  }
  
  delay(100);
}

void processCommand(String cmd) {
  cmd.trim();
  
  if (cmd == "START") {
    startSystem();
  } else if (cmd == "STOP") {
    stopSystem();
  } else if (cmd == "STATUS") {
    printStatus();
  }
}

void startSystem() {
  systemActive = true;
  digitalWrite(LED_STATUS, HIGH);
  Serial.println("Sistema activado");
}

void stopSystem() {
  systemActive = false;
  digitalWrite(ELECTRODE_POSITIVE, LOW);
  digitalWrite(ELECTRODE_NEGATIVE, LOW);
  digitalWrite(LED_STATUS, LOW);
  Serial.println("Sistema detenido");
}

void controlElectrolysis() {
  // Control PWM básico - implementar control PID más adelante
  int pwmValue = 128; // Valor inicial 50%
  
  analogWrite(ELECTRODE_POSITIVE, pwmValue);
  analogWrite(ELECTRODE_NEGATIVE, 0);
}

void checkSafety() {
  float temperature = readTemperature();
  
  if (temperature > maxTemp) {
    Serial.println("ALERTA: Temperatura alta - deteniendo sistema");
    stopSystem();
  }
}

float readTemperature() {
  // Placeholder - implementar lectura real del sensor
  int rawValue = analogRead(SENSOR_TEMP);
  float temperature = (rawValue * 5.0 / 1023.0) * 100.0; // Conversión básica
  return temperature;
}

float readCurrent() {
  // Placeholder - implementar lectura real del sensor
  int rawValue = analogRead(SENSOR_CURRENT);
  float current = (rawValue * 5.0 / 1023.0); // Conversión básica
  return current;
}

void printStatus() {
  Serial.println("=== ESTADO DEL SISTEMA ===");
  Serial.print("Activo: ");
  Serial.println(systemActive ? "SI" : "NO");
  Serial.print("Temperatura: ");
  Serial.print(readTemperature());
  Serial.println(" C");
  Serial.print("Corriente: ");
  Serial.print(readCurrent());
  Serial.println(" A");
}