// COMM BAUDS
#define BAUDS 115200

#define LOGGER_ENABLED            // Comentar para deshabilitar el logger por consola serie

#define LOG_LEVEL TRACE           // nivells en c_logger: TRACE, DEBUG, INFO, WARN, ERROR, FATAL, NONE

// DEVICE
//#define DEVICE_ESP_ID             "54CE0361421"   // ESP32 ID
#define DEVICE_GIIROB_PR2_ID      "19" //"giirobpr2_00"

// WIFI

#define NET_SSID              "Galaxy" //   "UPV-PSK"
#define NET_PASSWD             "12345678"   //"giirob-pr2-2023"


// MQTT
#define MQTT_SERVER_IP            "broker.hivemq.com"
#define MQTT_SERVER_PORT          1883
#define MQTT_USERNAME             "giirob"    // Descomentar esta línea (y la siguiente) para que se conecte al broker MQTT usando usuario y contraseña
#define MQTT_PASSWORD             "UPV2024"
//Conexión MQTT
#define ROBODK                   "giirob/pr2/PackRobotic_station/id/ESP32S3/id/tipo_caja"
#define ESP32S3                    "giirob/pr2/packrobotics/Esp32S3/id/Urgencia"
#define PAGINA_WEB                 "giirob/pr2/packrobotics/web_mqtt/id/Compra_productos"
//Entradas y salidas pines
#define CONTROL              "giirob/pr2/esp32s3/camara"
/* GPIO of CAMERA_MODEL_ESP32S3_EYE */
#define PWDN_GPIO_NUM     -1
  #define RESET_GPIO_NUM    -1
  #define XCLK_GPIO_NUM      15
  #define SIOD_GPIO_NUM     4
  #define SIOC_GPIO_NUM     5
  
  #define Y9_GPIO_NUM       16
  #define Y8_GPIO_NUM       17
  #define Y7_GPIO_NUM       18
  #define Y6_GPIO_NUM       12
  #define Y5_GPIO_NUM       10
  #define Y4_GPIO_NUM       8
  #define Y3_GPIO_NUM       9
  #define Y2_GPIO_NUM       11
  #define VSYNC_GPIO_NUM    6
  #define HREF_GPIO_NUM     7
  #define PCLK_GPIO_NUM     13

//Botton_parar_proceso
#define BUTTON_EMERGENCIA 5