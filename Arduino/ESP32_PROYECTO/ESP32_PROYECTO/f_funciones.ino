//membrana
#include <Keypad.h>
const byte ROWS = 4; // Cuatro filas
const byte COLS = 4; // Cuatro columnas
byte rowPins[ROWS] = {37,38 ,39 ,40 }; // Conectar a los pines de fila del teclado
byte colPins[COLS] = {41, 42, 2, 1}; // Conectar a los pines de columna del teclado 
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
uint16_t reloj,tablet,movil,airpods;
uint8_t objeto_comprado;
enum posicion_pagina_web{comenzar,elegir,comprar,acabar_compra,numero_cuenta};
void membrana_matriz(void *pvParameters)
{
  posicion_pagina_web situacion_actual;
  situacion_actual=comenzar;
  char customKey;
  uint16_t valor=0;
  while(1){
      customKey=customKeypad.getKey();
    
      if(customKey)
      {
        Serial.print(customKey);
        if(situacion_actual==comenzar && customKey=='0'){
          reiniciar_valores();
          situacion_actual=elegir;
          enviarMensajePorTopic(PAGINA_WEB,"0");
        }
        else if(situacion_actual==elegir){
          switch(customKey){
            case 'A': enviarMensajePorTopic(PAGINA_WEB,"A"); objeto_comprado=0; situacion_actual=comprar; break;
            case 'B': enviarMensajePorTopic(PAGINA_WEB,"B"); objeto_comprado=1; situacion_actual=comprar;break;
            case 'C':enviarMensajePorTopic(PAGINA_WEB,"C"); objeto_comprado=2; situacion_actual=comprar;break;
            case 'D':enviarMensajePorTopic(PAGINA_WEB,"D"); objeto_comprado=3; situacion_actual=comprar;break;
            case '*':enviarMensajePorTopic(PAGINA_WEB,"*"); situacion_actual=acabar_compra; break;
          }
          valor=0;
        }
        else if(situacion_actual==comprar){
            if(customKey>='0' && customKey<='9')
            {
              valor=valor*10+(customKey-'0');
            }
            else if(customKey=='*')
            {
              switch(objeto_comprado){
                case 0: movil+=valor; break;
                case 1: airpods+=valor; break;
                case 2: reloj+=valor; break;
                case 3: tablet+=valor; break;
              }
              enviarMensajePorTopic(PAGINA_WEB,String(valor)); 
              situacion_actual=elegir;
            }
        }
        else if(situacion_actual==acabar_compra)
        {
          switch(customKey){
            case '#': calculo_caja(1);  enviarMensajePorTopic(PAGINA_WEB,"#"); break;
            case '*': calculo_caja(0);    enviarMensajePorTopic(PAGINA_WEB,"*"); break;
          }
          situacion_actual=numero_cuenta;
        }
        else if(situacion_actual==numero_cuenta){
          
        switch(customKey){
            case '*':enviarMensajePorTopic(PAGINA_WEB,"*"); break;
          }
          
          situacion_actual=comenzar;
        }
      }
  }
}
void calculo_caja(uint8_t urgente){
  if(urgente){
    digitalWrite(URGENTE,HIGH);
    enviarMensajePorTopic(ESP32S3,"1");
  }
  else{
    enviarMensajePorTopic(ESP32S3,"0");
  }
  
  uint16_t total=0;
  total=reloj*0.2+airpods*0.1+tablet*1+movil*0.5;
  if(total<20)
  {
    encender_luz_caja(0);
  }
  else if(total<35)
  {
    encender_luz_caja(1);
  }
  else
  {
    encender_luz_caja(2);
  }
}
void encender_luz_caja(uint8_t led)
{
  switch (led)
  {
    case 0:  digitalWrite(PEQ,HIGH); digitalWrite(MED,LOW); digitalWrite(GRAN,LOW);  break;
    case 1:  digitalWrite(PEQ,LOW); digitalWrite(MED,HIGH); digitalWrite(GRAN,LOW); break;
    case 2:  digitalWrite(PEQ,LOW); digitalWrite(MED,LOW); digitalWrite(GRAN,HIGH);  break;
  }
}
void reiniciar_valores()
{
  reloj=0;
  tablet=0;
  movil=0;
  airpods=0;
  digitalWrite(PEQ,LOW); 
  digitalWrite(MED,LOW);
  digitalWrite(GRAN,LOW);
  digitalWrite(URGENTE,LOW);
}