
TaskHandle_t membrana_matriz_Task; 
void on_setup() {
    //Inicializamos LOS PINES DE LOS LED
    pinMode(PEQ, OUTPUT);
    pinMode(MED, OUTPUT);
    pinMode(GRAN, OUTPUT);
    pinMode(URGENTE, OUTPUT);
    ditalWrite(PEQ,LOW);
    digitalWrite(MED,LOW);
    digitalWrite(GRAN,LOW);
    digitalWrite(URGENTE,LOW);
//Creo una tarea que se ira ejecutando para los botones de la membrana
 xTaskCreatePinnedToCore(
             membrana_matriz,          /* Task function. */
             "membrana_matriz",   /* name of task. */
             5000,                 /* Stack size of task */
             NULL,                  /* parameter of the task */
             1,                     /* priority of the task */
             &membrana_matriz_Task,    /* Task handle to keep track of created task */
             0);                    /* pin task to core 0 */
}



