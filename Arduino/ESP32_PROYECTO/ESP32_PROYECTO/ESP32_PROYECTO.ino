/*
 * Grado GIIROB
 * Asignatura PR2
 * Joan Fons (jjfons@dsic.upv.es)
 * Curso 2023/24
 *
 * Plantilla Dispositivo IoT
 *
 * Control de versiones
 * Version: 0.6   (2024/02/21) JSON Management
 * Version: 0.5.1 (2024/02/20) SSL fork
 * Version: 0.5   (2024/02/19) Mejora en comunicaciones y reconexiones
 * Version: 0.4   (2024/02/15) Reestructuración y simplificación
 * Version: 0.3   (2024/02/14) Añadido funciones de log
 * Version: 0.2   (2023/12/28) Dividido en sub-ficheros
 * Version: 0.1   (2023/11/29) Prototipo Inicial Funcional
*/
#include "Config.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <WiFi.h>
#ifdef SSL_ROOT_CA
  #include <WiFiClientSecure.h>
#endif
#include <PubSubClient.h>
#include <ArduinoJson.h>

// ID de Dispositivo : se proporcionan varias alternativas, a modo de ejemplo
String deviceID = String("giirobpr2-device-") + String(DEVICE_GIIROB_PR2_ID); 
  // Versión usando el ID asignado en la asignatura GIIROB-PR2
//String deviceID = String("device-") + String(WiFi.macAddress());            
  // Versión usando la dirección MAC del dispositivo
//String deviceID = String("device-esp32s3-") + String(DEVICE_ESP_ID);        
  // Versión usando el ID de ESP del dispositivo

void setup() {
#ifdef LOGGER_ENABLED
  // Inicializamos comunicaciones serial
  Serial.begin(BAUDS);
  delay(1000);
  Serial.println();
#endif
  // Nos conectamos a la wifi
  wifi_connect();
  // Nos conectamos al broker MQTT, indicando un 'client-id'
  mqtt_connect(deviceID);
  suscribirseATopics();
  on_setup();
}
void loop() {
  wifi_loop();
  mqtt_loop();
}


