// COMM BAUDS
#define BAUDS 115200

#define LOGGER_ENABLED            

#define LOG_LEVEL TRACE           // nivells en c_logger: TRACE, DEBUG, INFO, WARN, ERROR, FATAL, NONE

// DEVICE
//#define DEVICE_ESP_ID             "54CE0361421"   // ESP32 ID
#define DEVICE_GIIROB_PR2_ID      "18" //"giirobpr2_00"

// WIFI
#define NET_SSID                  "Galaxy" //"UPV-PSK"
#define NET_PASSWD                "12345678"//"giirob-pr2-2023"

// MQTT
#define MQTT_SERVER_IP            "broker.hivemq.com"
#define MQTT_SERVER_PORT          1883
#define MQTT_USERNAME             "giirob" 
#define MQTT_PASSWORD             "UPV2024"

#define ROBOT                    "giirob/pr2/PackRobotic_station/id/ESP32S3/id/tipo_caja"
#define ESP32S3                   "giirob/pr2/packrobotics/Esp32S3/id/Urgencia"
#define PAGINA_WEB                 "giirob/pr2/packrobotics/web_mqtt/id/Compra_productos"



//Entradas y salidas pines
#define PEQ 4
#define MED 5
#define GRAN 6
#define URGENTE 7

