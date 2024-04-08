#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <queue.h>

#include "pico/stdlib.h"
#include <stdio.h>

#include "data.h"
QueueHandle_t xQueueData;


void data_task(void *p) {
    vTaskDelay(pdMS_TO_TICKS(400));

    int data_len = sizeof(sine_wave_four_cycles) / sizeof(sine_wave_four_cycles[0]);
    for (int i = 0; i < data_len; i++) {
        xQueueSend(xQueueData, &sine_wave_four_cycles[i], 1000000);
    }

    while (true) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void process_task(void *p) {
    int data = 0;
    int movel[5];
    while (true) {

        if (xQueueReceive(xQueueData, &data, 100)) {
            movel[0] = movel[1];
            movel[1] = movel[2];
            movel[2] = movel[3];
            movel[3] = movel[4];
            movel[4] = data;


            int mediaMovel = (movel[0] + movel[1] + movel[2] + movel[3] + movel[4]) / 5; 

            printf("%d \n",mediaMovel);

            vTaskDelay(pdMS_TO_TICKS(50));
        }
    }
}

int main() {
    stdio_init_all();

    xQueueData = xQueueCreate(64, sizeof(int));

    xTaskCreate(data_task, "task dado", 4096, NULL, 1, NULL);
    xTaskCreate(process_task, "task processo", 4096, NULL, 1, NULL);

    vTaskStartScheduler();

    while (true)
        ;
}