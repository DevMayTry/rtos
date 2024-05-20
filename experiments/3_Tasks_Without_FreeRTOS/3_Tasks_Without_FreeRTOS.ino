#define LED_PIN 12

unsigned long previousLedMillis = 0;
unsigned long ledInterval = 480/16; // Intervalo do LED em milissegundos

unsigned long previousSerialMillis = 0;
unsigned long serialInterval = 20/16; // Intervalo da serial reader em milissegundos

bool disturbRunning = false;

void setup() {
  Serial.begin(19200);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // Task LED
  unsigned long currentMillis = millis();
  if (currentMillis - previousLedMillis >= ledInterval) {
    toggleLED();
    previousLedMillis = currentMillis;
    Serial.print("LED Current Time: ");
    Serial.println(currentMillis);
  }

  // Task Serial Reader
  if (Serial.available() > 0) {
    char receivedChar = Serial.read();
    if (receivedChar == 's') {
      disturbRunning = true;
      Serial.println("Disturb task resumed");
    } else if (receivedChar == 'p') {
      disturbRunning = false;
      Serial.println("Disturb task suspended");
    }
  }

  // Task Disturb
  if (disturbRunning) {
    Serial.print("DISTURB process init running... | Current Time = ");
    Serial.println(currentMillis);    
    disturb();
    Serial.print("DISTURB process end... | Current Time = ");
    Serial.println(currentMillis);
  }
}

void toggleLED() {
  static bool ledState = false;
  ledState = !ledState;
  digitalWrite(LED_PIN, ledState ? HIGH : LOW);
}

void disturb() {
  // Simula uma carga de processamento pesada
  for (volatile uint32_t i = 0; i < 100000; ++i) {
    // Loop de processamento para simular carga
  }
}
