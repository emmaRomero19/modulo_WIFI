#include <ArduinoJson.h>

#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
//ingresamos nombre de la red y contaseña
const char* ssid = "LAPTOP-DI4IE39T 3857";
const char* password = "Ha656072";

// en host cambia la parte de la ciudad por la ciudad que desees
const String host =

//SOLEADO
"http://api.openweathermap.org/data/2.5/weather?q=Tulancingo,%20MX&units=metric&APPID=";

//NUBLADO
//"http://api.openweathermap.org/data/2.5/weather?q=Paris,%20FR&units=metric&APPID=";

//MAS DE 25°
//"http://api.openweathermap.org/data/2.5/weather?q=Jakarta,%20ID&units=metric&APPID=";

//API KEY
const String privateKey = "2a10a4c48fe1391efd05288b22e81c01";
const int delayTime = 10000; // sugerencia 1 minuto -> 60000


void setup() {
Serial.begin(115200);
delay(20);
//PINES PARA LED
pinMode(2,OUTPUT);
pinMode(4,OUTPUT);
pinMode(5,OUTPUT);

// Nos intentamos conectar a la red wifi
Serial.println();
Serial.print("Conectando a ");
Serial.println(ssid);
WiFi.mode(WIFI_STA); // modo wifi-client
WiFi.begin(ssid, password);

while (WiFi.status() != WL_CONNECTED) {
delay(250);
Serial.print(".");
}

Serial.println("");
Serial.println("Conectado a la red WiFi!!!");
Serial.println("");
} // EOF setup()


void loop() {
//APAGAMOS LOS LED
digitalWrite(2,LOW);
digitalWrite(4,LOW);
digitalWrite(5,LOW);

if ((WiFi.status() == WL_CONNECTED)) {
HTTPClient http;

http.begin(host + privateKey); //armamos la URL
int httpCode = http.GET(); //Realizamos la petición

if (httpCode > 0) { //código de retorno

String data = http.getString();

if(data){
Serial.println(httpCode); // esperamos que sea 200
Serial.println(data);
const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + 2*JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(8) + JSON_OBJECT_SIZE(12) + 420;

DynamicJsonBuffer jsonBuffer(capacity);

JsonObject& root = jsonBuffer.parseObject(data);

float coord_lon = root["coord"]["lon"]; // -0.13
float coord_lat = root["coord"]["lat"]; // 51.51

JsonArray& weather = root["weather"];

JsonObject& weather_0 = weather[0];
int weather_0_id = weather_0["id"]; // 301
//ID DEL CLIMA SI ES MENOR A 800 SE REFIERE A UN CLIMA SOLEADO
//SI ES MAYOR ES NUBLADO
 
Serial.println("ID del clima"+String(weather_0_id));

//TEMPERATURA EN GRADOS CELSIUS
JsonObject& main = root["main"];
float main_temp = main["temp"]; // 281.87
Serial.println("Weather "+String(main_temp));

if(main_temp>=25.0){
  //SI ES MAYOR A 25° ENCIENDE LED ROJO
  digitalWrite(5,HIGH);
}
else if(weather_0_id>=801){
  //CLIMA NUBLADO
  //SI ES MAYOR A 800 ENCIENDE LED AZUL
  digitalWrite(4,HIGH);
}
else if(weather_0_id<=800){
  //CLIMA SOLEADO
  //SI ES MENOR A 800 ENCIENDE LED AMARILLO
  digitalWrite(2,HIGH);
}

}
}

else {
Serial.println("Error en la petición HTTP");
}

http.end(); //cerramos conexión y liberamos recursos
}
delay(delayTime);
} // EOF loop()
