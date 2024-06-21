void parar_todo(){
  cambiar_bottonpulsado(false);
  enviarMensajePorTopic(ROBODK,"PARAR_TODO");
}
void restaurar_funcionamiento(){
  cambiar_bottonpulsado(true);
  enviarMensajePorTopic(ROBODK,"VOLVER_NORMALIDAD");
}