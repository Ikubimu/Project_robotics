


//Función para saber si el Buffer está vacío
bool isEmpty(Buffer_Circ* buff){
	if(buff->contador == 0){
		return true;
	}else{
		return false;
	}	
}


//Función para saber si el Buffer está lleno
bool isFull(Buffer_Circ* buff){
	if(buff->contador == BUFSIZE){
		return true;
	}else{
		return false;
	}	
}

//Funcion para obtener elemento del Buffer
int get_item(int* X, Buffer_Circ* buff ){
	if(isEmpty(buff)){
		Serial.printf("Error: No hay datos en el vector\n"); 
		return -1;
	}
	else{
    portENTER_CRITICAL (&(buff->taskMux));
		*X = buff->datos[buff->bufOUT];
		buff->contador--;
		//Aritmetica en módulo del índice del vector
		buff->bufOUT = (buff->bufOUT+1) % BUFSIZE;
    portEXIT_CRITICAL (&(buff->taskMux)); 
    Serial.printf("Sacas dato %d de posicion %d\n", *X, buff->bufOUT);
		return 0;	
	}
}

//Función para introducir elemento en el Buffer
int put_item(int X , Buffer_Circ* buff ){
	if(isFull(buff)){
		Serial.printf("Error: El vector esta lleno\n"); 
		return -1;
	}
	else{
    portENTER_CRITICAL (&(buff->taskMux));
		buff->datos[buff->bufIN]=X;
		buff->contador++;
		//Aritmetica en módulo del índice del vector
		buff->bufIN = (buff->bufIN+1) % BUFSIZE;
    portEXIT_CRITICAL (&(buff->taskMux)); 
    Serial.printf("Meto dato %d en posicion %d\n", X, buff->bufIN);
		return 0;	
	}	
}

//Función para saber cuántos elementos tiene el Buffer
int number(Buffer_Circ* buff){
	return buff->contador;	
}

//Función para listar el contenido del Buffer
int listBuffer(Buffer_Circ* buff){
	Serial.printf("Tu buffer contiene: ");
	for(int i=0; i<BUFSIZE; i++){
		Serial.printf("\t%d", buff->datos[i]);
	}
	Serial.printf("\n");
}

void Consumidor(void *pvParameters)
{
  Buffer_Circ *buff_prod=(Buffer_Circ*) pvParameters;
  while(1){
      if(!isEmpty(buff_prod)){
        int valor_enviar;
        get_item(&valor_enviar,buff_prod);
        JsonDocument doc;
        if(valor_enviar>10){
          doc["urgencia"] =1;
          cambiar_urgencia(false);
        }
        else{
          doc["urgencia"] =0;
        }
        doc["tipo_caja"]=valor_enviar%10;
        String msg_json;
        serializeJson(doc, msg_json);
        enviarMensajePorTopic(ROBODK,msg_json);
        vTaskDelay(10000/portTICK_RATE_MS);//hacemos un delay para que no haya problemas en la simulación asi también permitimos que se llene el buffer
      }
       vTaskDelay(100/portTICK_RATE_MS);
  }
}


