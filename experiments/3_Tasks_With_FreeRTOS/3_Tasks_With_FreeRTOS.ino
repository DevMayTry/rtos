#include <Arduino_FreeRTOS.h>
#include <task.h>

#define LED_PIN 12

void task_serial_reader(void *pvParameters);
void task_led(void *pvParameters);
void task_disturb(void *pvParameters);

TaskHandle_t TaskDisturb_Handler;

void setup(void) {
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    Serial.begin(19200);

    while (!Serial) { ; }

    Serial.println("Demonstração de tasks com FreeRTOS");

    Serial.print("Valor de um tick em milissegundos: ");
    Serial.print(portTICK_PERIOD_MS);
    Serial.println(" ms");

    Serial.println("3 tarefas criadas:");
    Serial.println("* LED Blink com 480ms (30 ticks) de período e prioridade 10");
    Serial.println("* Disturb task com prioridade 5");
    Serial.println("* Serial Reader com prioridade 1");

    xTaskCreate(
        task_led,
        (const char*)"LED",
        128,
        (void*)480,
        10,
        NULL
    );

    xTaskCreate(
        task_disturb,
        (const char*)"disturb",
        128,
        NULL,
        5,
        &TaskDisturb_Handler
    );

    xTaskCreate(
        task_serial_reader,
        (const char*)"comm_reader",
        128,
        NULL,
        1,
        NULL
    );

    vTaskSuspend(TaskDisturb_Handler); // Inicialmente suspende a task_disturb
}

void loop(void) { }

void task_led(void *pvParameters) {
    unsigned int param = (unsigned int)pvParameters;

    TickType_t xLastWakeTime;
    const TickType_t xFrequency = param / portTICK_PERIOD_MS;

    xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        Serial.print("LED Current Ticks = ");
        Serial.println(xTaskGetTickCount());
        if (digitalRead(LED_PIN)) {
            digitalWrite(LED_PIN, LOW);
        } else {
            digitalWrite(LED_PIN, HIGH);
        }

        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

void task_serial_reader(void *pvParameters) {
    (void)pvParameters;
    char receivedChar;

    for (;;) {
        if (Serial.available() > 0) {
            receivedChar = Serial.read();
            if (receivedChar == 's') {
                vTaskResume(TaskDisturb_Handler);
                Serial.println("Disturb task resumed");
            } else if (receivedChar == 'p') {
                vTaskSuspend(TaskDisturb_Handler);
                Serial.println("Disturb task suspended");
            }
        }
        vTaskDelay(20 / portTICK_PERIOD_MS); // Aguarda por 20ms antes de verificar novamente
    }
}

void task_disturb(void *pvParameters) {
    (void)pvParameters;
    int k;
    float number = 0;
    for (;;) {
        // Simula um distúrbio, por exemplo, executando uma carga de processamento sem sentido
		    Serial.print("DISTURB process init running... | Current Ticks = ");
		    Serial.println(xTaskGetTickCount());
        for (volatile uint32_t i = 0; i < 100000; ++i) {
            // Loop de processamento para simular carga
        }
        Serial.print("DISTURB process end... | Current Ticks = ");
        Serial.println(xTaskGetTickCount());
        vTaskDelay(100 / portTICK_PERIOD_MS); // Aguarda 100ms antes de causar mais distúrbios
    }
}
