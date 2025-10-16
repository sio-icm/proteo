/*
 * Proteo - DEBUG COMPLETO del sensor PreSens
 * Diagnóstico exhaustivo de comunicación
 */

#include <Arduino.h>

const int LED_STATUS = 13;
unsigned long lastTest = 0;
int testCounter = 0;

// Declaración de función
void checkResponse(String command);

void setup() {
  Serial.begin(9600);
  while (!Serial && millis() < 3000);

  pinMode(LED_STATUS, OUTPUT);

  Serial.println("\n========================================");
  Serial.println("   DIAGNÓSTICO SENSOR PRESENS");
  Serial.println("========================================\n");

  // Test 1: Verificar que Serial1 existe
  Serial.println("Test 1: Inicializando Serial1...");
  Serial1.begin(19200);
  Serial.println("✅ Serial1 inicializado a 19200 baud");

  delay(1000);

  Serial.println("\n🔍 MODO DEBUG ACTIVADO");
  Serial.println("Cada 3 segundos:");
  Serial.println("  1. Enviaré comandos al sensor");
  Serial.println("  2. Mostraré CADA byte recibido");
  Serial.println("  3. Probaré diferentes comandos");
  Serial.println("\n⏳ Observa si aparece CUALQUIER dato...\n");
}

void loop() {
  // Cada 3 segundos hacer test
  if (millis() - lastTest >= 3000) {
    testCounter++;

    Serial.println("==========================================");
    Serial.print("TEST #");
    Serial.println(testCounter);
    Serial.println("==========================================");

    // Limpiar buffer antes de enviar
    while (Serial1.available()) {
      Serial1.read();
    }

    // Test diferentes comandos
    Serial.println("\n1️⃣ Enviando: 'mode0001' + CR");
    Serial1.print("mode0001\r");
    Serial1.flush();
    digitalWrite(LED_STATUS, HIGH);
    delay(200);
    digitalWrite(LED_STATUS, LOW);
    checkResponse("mode0001");

    Serial.println("\n2️⃣ Enviando: 'data' + CR");
    Serial1.print("data\r");
    Serial1.flush();
    digitalWrite(LED_STATUS, HIGH);
    delay(200);
    digitalWrite(LED_STATUS, LOW);
    checkResponse("data");

    Serial.println("\n3️⃣ Enviando: '?' + CR");
    Serial1.print("?\r");
    Serial1.flush();
    digitalWrite(LED_STATUS, HIGH);
    delay(200);
    digitalWrite(LED_STATUS, LOW);
    checkResponse("?");

    Serial.println("\n4️⃣ Enviando: 'READ' + CR");
    Serial1.print("READ\r");
    Serial1.flush();
    digitalWrite(LED_STATUS, HIGH);
    delay(200);
    digitalWrite(LED_STATUS, LOW);
    checkResponse("READ");

    Serial.println("\n5️⃣ Enviando: solo CR");
    Serial1.print("\r");
    Serial1.flush();
    delay(200);
    checkResponse("CR");

    // Estado del puerto
    Serial.println("\n📊 Estado del puerto Serial1:");
    Serial.print("   Bytes disponibles: ");
    Serial.println(Serial1.available());

    Serial.println("\n==========================================\n");

    lastTest = millis();
  }

  // Leer continuamente y mostrar CUALQUIER byte que llegue
  if (Serial1.available() > 0) {
    Serial.println("\n🎉 ¡¡¡DATO RECIBIDO!!!");
    Serial.print("Bytes disponibles: ");
    Serial.println(Serial1.available());
    Serial.print("Datos: ");

    while (Serial1.available() > 0) {
      byte b = Serial1.read();

      // Mostrar en diferentes formatos
      Serial.print("[");
      Serial.print(b, HEX);
      Serial.print(":");
      if (b >= 32 && b <= 126) {
        Serial.print((char)b);
      } else if (b == 13) {
        Serial.print("CR");
      } else if (b == 10) {
        Serial.print("LF");
      } else {
        Serial.print("?");
      }
      Serial.print("] ");

      // Parpadear LED cuando lleguen datos
      digitalWrite(LED_STATUS, HIGH);
      delay(10);
      digitalWrite(LED_STATUS, LOW);
    }
    Serial.println("\n");
  }

  delay(10);
}

void checkResponse(String command) {
  Serial.print("   Esperando respuesta");

  int dots = 0;
  unsigned long start = millis();
  bool gotData = false;

  // Esperar hasta 800ms
  while (millis() - start < 800) {
    if (Serial1.available() > 0) {
      gotData = true;
      break;
    }

    // Mostrar puntos suspensivos
    if ((millis() - start) % 200 == 0 && dots < 3) {
      Serial.print(".");
      dots++;
    }
    delay(10);
  }

  if (gotData) {
    Serial.println(" ✅ ¡RESPUESTA!");
    Serial.print("   Bytes: ");
    Serial.println(Serial1.available());
    Serial.print("   Datos: ");

    String response = "";
    while (Serial1.available() > 0) {
      char c = Serial1.read();
      response += c;

      // Mostrar también byte por byte
      Serial.print("[");
      if (c >= 32 && c <= 126) {
        Serial.print(c);
      } else if (c == 13) {
        Serial.print("CR");
      } else if (c == 10) {
        Serial.print("LF");
      } else {
        Serial.print("0x");
        Serial.print((byte)c, HEX);
      }
      Serial.print("]");
    }
    Serial.println();
    Serial.print("   String completo: '");
    Serial.print(response);
    Serial.println("'");

  } else {
    Serial.println(" ❌ Sin respuesta");
  }
}
