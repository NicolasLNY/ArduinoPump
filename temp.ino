#include <dht.h>
#include <LiquidCrystal.h>
#include <SPI.h> // Libairie communicatin SPI avec le Shield éthernet
#include <Ethernet.h> // Librairie communication Ethernet sur un réseau local



dht DHT;


byte MAC[] = { 0x10, 0x11, 0x12, 0x13, 0x14, 0x15 }; // Adresse MAC du shield Ethernet
IPAddress IP(192,168,1, 11); // Adresse IP donné au Shield Ethernet
EthernetServer serveurHTTP(80); // Le Shield devient un serveur sur le port 80 (port HTTP)
// Initialise notre serveur
// Ce dernier écoutera sur le port 4200
EthernetServer serveur(4200);




#define DHT11_PIN 7
const int FANPinON =  9;
#define SensorPin A0 
float sensorValue = 0; 
const char* FanState = "OFF";

void setup(){
  Ethernet.begin(MAC, IP);
  serveurHTTP.begin();
  pinMode(FANPinON, OUTPUT);
}

void loop(){


   for (int i = 0; i <= 100; i++) 
  { 
    sensorValue = sensorValue + analogRead(SensorPin); 
    delay(1); 
  } 
  sensorValue = sensorValue/100.0; 
  Serial.println(sensorValue); 

   int chk = DHT.read11(DHT11_PIN);
  Serial.println("Temp: ");
  Serial.println(DHT.temperature);
  delay(2000);
  

  if (DHT.temperature >= 24) {
    // turn LED On:
    digitalWrite(FANPinON, HIGH);
  } else {
    // turn fan off:
    digitalWrite(FANPinON, LOW);
  }

  if (digitalRead(FANPinON) == HIGH) // Si FANPIN est BAS (0V)
{
  FanState = "ON";
}
else {         // Sinon (FANPIN est HAUT (5V)
FanState = "OFF";
}
 


   // Regarde si un client est connecté et attend une réponse
  EthernetClient client = serveur.available();
  if (client) {
    // Quelqu'un est connecté !
    Serial.print("On envoi !");
    // On fait notre en-tête
    // Tout d'abord le code de réponse 200 = réussite
    client.println("HTTP/1.1 200 OK");
    // Puis le type mime du contenu renvoyé, du json
    client.println("Content-Type: application/json");
    // Et c'est tout !
    // On envoie une ligne vide pour signaler la fin du header
    client.println();

    // Puis on commence notre JSON par une accolade ouvrante
    client.println("{");
    // On envoie l'état du ventilateue
    client.print("\t\"Etat Ventilateur\": ");
   client.println(FanState);
    //Une petite virgule pour séparer les deux clés
    client.println(",");
    // Et on envoie la seconde nommée "Temp"
    client.print("\t\"Temperature\": ");
    client.println(DHT.temperature);
    client.println(",");
    // Et on envoie la seconde nommée "Temp"
    client.print("\t\"Humidité\": ");
    client.println(sensorValue);
    // Et enfin on termine notre JSON par une accolade fermante
    client.println("}");
    // Donne le temps au client de prendre les données
    delay(10);
    // Ferme la connexion avec le client
    client.stop();
  }


 
 
}
