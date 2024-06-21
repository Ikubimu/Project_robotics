#include "Config.h"
void suscribirseATopics() {
  mqtt_subscribe(ESP32S3);
}
void alRecibirMensajePorTopic(char* topic, String incomingMessage) {
 if (strcmp(topic, ESP32S3) == 0 ) {
    if (!err) {
      Serial.print(incomingMessage);
      if(incomingMessage[0]=='1'){
        cambiar_urgencia(true);
      }
      else{
        cambiar_urgencia(false);
      }
      Serial.printf("Mensaje urgente recibido");
    }
  }
}
void enviarMensajePorTopic(const char* topic, String outgoingMessage) {
  mqtt_publish(topic, outgoingMessage.c_str());
}





