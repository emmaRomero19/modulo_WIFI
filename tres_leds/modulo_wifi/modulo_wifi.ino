#include <SoftwareSerial.h>
SoftwareSerial esp8266(2,3); //Pin 2 & 3 of Arduino as RX and TX. Connect TX and RX of ESP8266 respectively.
#define DEBUG true
#define led_pin1 13 //LED is connected to Pin 13 of Arduino
#define led_pin2 12 //LED Pin 12
#define led_pin3 11 //LED Pin 11

void setup()
  {
  pinMode(led_pin1, OUTPUT);
  pinMode(led_pin2, OUTPUT);
  pinMode(led_pin3, OUTPUT);
    digitalWrite(led_pin1, LOW);
    digitalWrite(led_pin2, LOW);
    digitalWrite(led_pin3, LOW);
    Serial.begin(9600);
    esp8266.begin(115200); //Baud rate for communicating with ESP8266. Your's might be different.
    esp8266Serial("AT+RST\r\n", 5000, DEBUG); // Reset the ESP8266
    esp8266Serial("AT+CWMODE=1\r\n", 5000, DEBUG); //Set station mode Operation
    esp8266Serial("AT+CWJAP=\"LAPTOP-DI4IE39T 3857\",\"Ha656072\"\r\n", 5000, DEBUG);//Enter your WiFi network's SSID and Password.
                                   
    while(!esp8266.find("OK")) 
    {
      }
    esp8266Serial("AT+CIFSR\r\n", 5000, DEBUG);//You will get the IP Address of the ESP8266 from this command. 
    esp8266Serial("AT+CIPMUX=1\r\n", 5000, DEBUG);
    esp8266Serial("AT+CIPSERVER=1,80\r\n", 5000, DEBUG);
  }

void loop()
  {
    if (esp8266.available())
      {
        if (esp8266.find("+IPD,"))
          {
            String msg;
            esp8266.find("?");
            msg = esp8266.readStringUntil(' ');
            String command1 = msg.substring(0, 3);
            String command2 = msg.substring(4);
                        
            if (DEBUG) 
              {
                Serial.println(command1);//Must print "led"
                Serial.println(command2);//Must print "ON" or "OFF"
              }
            delay(100);

              if (command2 == "ON") 
                    {
                      digitalWrite(led_pin1, HIGH);
                      Serial.println("Turn ON");
                    }
              else if (command2 == "ON2") 
                    {
                      digitalWrite(led_pin2, HIGH);
                      Serial.println("Turn ON");
                    }
              else if (command2 == "ON3") 
                    {
                      digitalWrite(led_pin3, HIGH);
                      Serial.println("Turn ON");
                    }
              
                   else 
                     {
                       digitalWrite(led_pin1, LOW);
                       digitalWrite(led_pin2, LOW);
                       digitalWrite(led_pin3, LOW);
                       Serial.println("Turn OFF");
                     }
          }
      }
  }
   
String esp8266Serial(String command, const int timeout, boolean debug)
  {
    String response = "";
    esp8266.print(command);
    long int time = millis();
    while ( (time + timeout) > millis())
      {
        while (esp8266.available())
          {
            char c = esp8266.read();
            response += c;
          }
      }
    if (debug)
      {
        Serial.print(response);
      }
    return response;
  }
