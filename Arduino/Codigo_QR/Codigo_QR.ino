#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

#include <Arduino.h>
#include <EventButton.h>
#include "src/quirc/quirc.h"
#include "esp_camera.h"
#include "Config.h"
#define BUFSIZE 10

typedef struct Buffer {
	int datos[BUFSIZE];
	int bufIN=0;
  int bufOUT=0;
  int contador=0;
  portMUX_TYPE taskMux = portMUX_INITIALIZER_UNLOCKED;
} Buffer_Circ;
// creating a task handle
TaskHandle_t QRCodeReader_Task; 
TaskHandle_t Consumidor_Task; 
/*  Variables declaration */
volatile bool botton_emergencia=false;
bool caja_urgente;
portMUX_TYPE botton = portMUX_INITIALIZER_UNLOCKED; // critical section mutex
portMUX_TYPE urgencia = portMUX_INITIALIZER_UNLOCKED; // critical section mutex
struct QRCodeData
{
  bool valid;
  int dataType;
  uint8_t payload[1024];
  int payloadLen;
};

struct quirc *q = NULL;
uint8_t *image = NULL;  
camera_fb_t * fb = NULL;
struct quirc_code code;
struct quirc_data data;
quirc_decode_error_t err;
struct QRCodeData qrCodeData;  
String QRCodeResult = "";

static Buffer_Circ Mi_buffer;
/* VOID SETTUP() */
void setup_QR() {
  // Disable brownout detector.
  //WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

  /* Init serial communication speed (baud rate). */
 Serial.begin(115200);
  /* Camera configuration. */
  Serial.println("Start configuring and initializing the camera...");
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 10000000;
  config.pixel_format = PIXFORMAT_GRAYSCALE;
  config.frame_size = FRAMESIZE_QVGA;
  config.jpeg_quality = 15;
  config.fb_count = 1;
  
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    ESP.restart();
  }


  
  sensor_t * s = esp_camera_sensor_get();
  s->set_framesize(s, FRAMESIZE_QVGA);
  
  Serial.println("Configure and initialize the camera successfully.");
  Serial.println();
  
  /* Create "QRCodeReade
  r_Task" using the xTaskCreatePinnedToCore() function */
  xTaskCreatePinnedToCore(
             QRCodeReader,          /* Task function. */
             "QRCodeReader_Task",   /* name of task. */
             10000,                 /* Stack size of task */
             (void *)&Mi_buffer,  /* parameter of the task */
             1,                     /* priority of the task */
             &QRCodeReader_Task,    /* Task handle to keep track of created task */
             0);                    /* task to core 0 */

xTaskCreatePinnedToCore(
             Consumidor,          /* Task function. */
             "Consumidor_Task",   /* name of task. */
             10000,                 /* Stack size of task */
             (void*)&Mi_buffer,  /* parameter of the task */
             1,                     /* priority of the task */
             &Consumidor_Task,    /* Task handle to keep track of created task */
             1);                    /* pin task to core 0 */
}
/* The function to be executed by "QRCodeReader_Task" */
// This function is to instruct the camera to take or capture a QR Code image,
// then it is processed and translated into text.
void QRCodeReader( void * pvParameters ){
  Buffer_Circ *buff_prod=(Buffer_Circ*) pvParameters;
  Serial.println("QRCodeReader is ready.");
  Serial.print("QRCodeReader running on core ");
  Serial.println(xPortGetCoreID());
  Serial.println();
  bool comprobarQR;
  int valor_a_enviar;
  /* Loop to read QR Code in real time. */
  while(1){
    comprobarQR=get_botton();//ahora lo cambio
    if(comprobarQR==true){
        q = quirc_new();
        if (q == NULL){
          //enviarMensajePorTopic(CONTROL,"can't create quirc object\r\n" );
          Serial.print("can't create quirc object\r\n");  
          continue;
        }
        fb = esp_camera_fb_get();
        if (!fb)
        {
          Serial.println("Camera capture failed");
          enviarMensajePorTopic(CONTROL,"Camera capture failed" );
          continue;
        }   
        
        quirc_resize(q, fb->width, fb->height);
        image = quirc_begin(q, NULL, NULL);
        memcpy(image, fb->buf, fb->len);
        quirc_end(q);
        
        int count = quirc_count(q);
        if (count > 0) {
          quirc_extract(q, 0, &code);
          err = quirc_decode(&code, &data);
      
          if (err){
            enviarMensajePorTopic(CONTROL,"Decoding FAILED");
            Serial.println("Decoding FAILED");
            QRCodeResult = "Decoding FAILED";
          } else {
            enviarMensajePorTopic(CONTROL,"Decoding successful:\n");
            Serial.printf("Decoding successful:\n");
            valor_a_enviar=dumpData_bis(&data);
             if(!isFull(buff_prod)){
                  put_item(valor_a_enviar,buff_prod);
              }
          } 
          Serial.println();
        } 
        
        esp_camera_fb_return(fb);
        fb = NULL;
        image = NULL;  
        quirc_destroy(q);
       vTaskDelay(100/portTICK_RATE_MS);
    }

    else
    {
      Serial.print("SETA ACTIVADA - MODO SLEEP\n");
       vTaskDelay(1000/portTICK_RATE_MS);
    }
    
   

  }
}

/* Function to display the results of reading the QR Code on the serial monitor. */
int dumpData_bis(const struct quirc_data *data)
{
  //MQTT
  QRCodeResult = (const char *)data->payload;
  portENTER_CRITICAL (&urgencia);
  bool caja_urg=caja_urgente;
  portEXIT_CRITICAL (&urgencia);
  int producto_enviar;
  if(QRCodeResult=="Caja_Pequeña"){
      producto_enviar=1;
      Serial.printf("Recibida caja pequeña\n");
    }
   else if(QRCodeResult=="Caja_Media"){
      producto_enviar=2;
        Serial.printf("Recibida caja media\n");
    }
    else if (QRCodeResult=="Caja_Grande"){
      producto_enviar=3;
      Serial.printf("Recibida caja grande\n");
    }

    if(caja_urg==true){
      return producto_enviar+10;
    }
    return producto_enviar;
}
void cambiar_urgencia(bool valor){
  portENTER_CRITICAL (&urgencia);
  caja_urgente=valor;
  portEXIT_CRITICAL (&urgencia); 
}
void cambiar_bottonpulsado(bool valor){
  portENTER_CRITICAL (&botton);
  botton_emergencia=valor;
  portEXIT_CRITICAL (&botton); 
}
bool get_botton(){
  portENTER_CRITICAL (&botton);
  bool valor=botton_emergencia;
  portEXIT_CRITICAL (&botton); 
  return valor;
}