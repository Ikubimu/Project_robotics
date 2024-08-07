
let state;
var bool_fin = false;
state = 0;

let point;
let arr = [0, 0, 0, 0];
let arr_cadena = ["Iphone", "ipods", "reloj", "tablet"];


var client = mqtt.connect("ws://broker.hivemq.com:8000/mqtt");
//Si lo usas en hosting para usar encriptado https
// var client = mqtt.connect("wss://test.mosquitto.org:8081/mqtts")

function select_imagen(imagen)
{
  if(imagen == "A")
    {
      document.getElementById("imagen_pop").src = "../css/images/iphone.png";
      point = 0;
    }
  else if(imagen == "B")
    {
      document.getElementById("imagen_pop").src = "../css/images/ipods.png";
      point = 1;
    }

    else if(imagen == "C")
      {
        document.getElementById("imagen_pop").src = "../css/images/reloj.png";
        point = 2;
      }
      else if(imagen == "D")
        {
          document.getElementById("imagen_pop").src = "../css/images/mac.png";
          point = 3;
        }
}

function EventoConectar() {
  console.log("Conectado a MQTT");
  client.subscribe("giirob/pr2/packrobotics/web_mqtt/id/Compra_productos", function (err) {
    if (!err) {
      client.publish("giirob/pr2/packrobotics/check", "CONECTED");
    }
  });
}

function handlerMQTT(mensaje, topic)
{
  if(topic == "giirob/pr2/packrobotics/web_mqtt/id/Compra_productos")
    {
      if(state == 0 && mensaje == "0")
      {
        document.getElementById("fuente").src = "../html/menu.html";
            // Ajustar el tamaño del iframe para que ocupe todo el espacio disponible en el contenedor padre
        document.getElementById("fuente").style.width = "100%";
        document.getElementById("fuente").style.height = "100%";
        state = 1;
      }
      else if(state == 1)
        {
          if (mensaje == "A" || mensaje == "B" || mensaje == "C" || mensaje == "D")
            {
              product = mensaje;
              console.log("dentro");
              var elemento = document.getElementById("ventana_pop");
              elemento.classList.replace("pop", "pop_active");
              select_imagen(mensaje);
              state = 2;
            }

          else if(mensaje == "*" && bool_fin == true)
            {
              var elemento = document.getElementById("pedido_urge");
              elemento.classList.replace("pop_urge", "pop_urge_active");
              state = 3;
            }

        } 
        
        else if(state == 2)
          {
            var cantidad = parseInt(mensaje);
            var elemento1 = document.getElementById("ventana_pop");
            elemento1.classList.replace("pop_active", "pop");
            var elemento2 = document.getElementById("mensaje_exit");
            elemento2.classList.replace("pop_fin", "pop_fin_active");
            var elemento3 = document.getElementById("lista_compra");
            elemento3.classList.replace("pop_lista", "pop_lista_active");
            var elemento4 = document.getElementById("posit_image");
            elemento4.classList.replace("pop_lista", "posit_image_active");
            bool_fin = true;
            let cadena = "";
            arr[point] += cantidad;
            for(let i = 0; i < arr.length; i++)
              {
                if(arr[i]!=0)
                  {
                    cadena += arr_cadena[i] + "  x"+arr[i]+"<br>";
                  }
                
              }
              elemento3.innerHTML = cadena;
            
            
            state = 1;
          }
          else if(state == 3)
          {
            var elemento1 = document.getElementById("pedido_urge");
            elemento1.classList.replace("pop_urge_active", "pop_urge");
            var elemento2 = document.getElementById("num_cuenta");
            elemento2.classList.replace("pop_cuenta", "pop_cuenta_active");
            
            state = 4;
          }
          else if(state == 4)
            {
              var elemento = document.getElementById("num_cuenta");
              elemento.classList.replace("pop_cuenta_active", "pop_cuenta");
              document.getElementById("fuente").src = "../html/final_screen.html";
              // Ajustar el tamaño del iframe para que ocupe todo el espacio disponible en el contenedor padre
              document.getElementById("fuente").style.width = "100%";
              document.getElementById("fuente").style.height = "100%";
              var elemento2 = document.getElementById("mensaje_exit");
              elemento2.classList.replace("pop_fin_active", "pop_fin");
              var elemento3 = document.getElementById("lista_compra");
              elemento3.classList.replace("pop_lista_active", "pop_lista");
              var elemento4 = document.getElementById("posit_image");
              elemento4.classList.replace("posit_image_active", "pop_lista");
              state = 5;
              setTimeout(function() {
                location.reload();
              }, 10000)
            }

        
    }

}

function EventoMensaje(topic, message) {
  
    console.log("La Temperatura es " + message.toString());
    handlerMQTT( message.toString(), topic);
  console.log(topic + " - " + message.toString());
  // client.end()
}





client.on("connect", EventoConectar);
client.on("message", EventoMensaje);

console.log("Empezando a conectar");
