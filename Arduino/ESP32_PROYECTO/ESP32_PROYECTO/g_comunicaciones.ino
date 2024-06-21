#include "Config.h"
void enviarMensajePorTopic(const char* topic, String outgoingMessage) {
  mqtt_publish(topic, outgoingMessage.c_str());
}





