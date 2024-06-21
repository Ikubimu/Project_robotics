
#define PIN_BUTTON 5
EventButton button1(PIN_BUTTON);
bool funcionando=true;

void onbutton1Pulsado(EventButton& eb)
{
  funcionando=!funcionando; //cambiamos la seta de emergencia si estaba en seguridad a volver a funcionar o al reves
  Serial.print("boton pulsado");
  if(funcionando==true){
    restaurar_funcionamiento();
  }
  else{
     parar_todo();
  }
}

void on_setup() {
    //Inicializamos LOS PINES DE LOS LED
    Serial.print("SETA INICIALIZADA\n");
    pinMode(PIN_BUTTON, INPUT_PULLUP);
    cambiar_bottonpulsado(true);
    button1.setClickHandler(onbutton1Pulsado);
}


