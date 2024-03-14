#include <ThingSpeak.h> //library platform thingspeak
#include <ESP8266WiFi.h> //library Wifi NodeMCU ESP8266
#include <WiFiClient.h> //library client
#include <DHT.h> //library DHT
#include<ESP8266WebServer.h> //library webserver


#define DHTPIN D4 // mendefinisikan pin D4 sebagai pin DHT
#define DHTTYPE DHT11 //mendefinisikan type dari DHT
DHT dht(DHTPIN, DHTTYPE);


ESP8266WebServer server(80);
String webpage;  //mengatur pin DHT dan type DHT

unsigned int value;
const char *ssid = "ssid"; //setting nama wifi 
const char *password = "pass"; //password wifi 
WiFiClient client; //mengecek jika client sudah terhubung

unsigned long channel = ; //ID akun ThingSpeak
const char * myWriteAPIKey = "apikey"; //memasukan apikey
uint8_t temperature; //temperature dengan tipe unsigned 8-bit.

void setup() {
  Serial.begin(9600); //memulai komunikasi serial dengan baud rate 115200
  dht.begin(); //memulai sensor DHT.
  pinMode(D1, OUTPUT);
  // digitalWrite(D1, 1);
  ThingSpeak.begin(client);
  
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password); //menghubungkan dengan wifi router
  while (WiFi.status() != WL_CONNECTED){ //Cek koneksi Wifi.
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());
  ThingSpeak.begin(client);

  webpage = "<!DOCTYPE html> <html>\n";
  webpage +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  webpage +="<title>LED Control</title>\n";
  webpage +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  webpage +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  webpage +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  webpage +=".button-on {background-color: #1abc9c;}\n";
  webpage +=".button-on:active {background-color: #16a085;}\n";
  webpage +=".button-off {background-color: #34495e;}\n";
  webpage +=".button-off:active {background-color: #2c3e50;}\n";
  webpage +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  webpage +="</style>\n";
  webpage +="</head>\n";
  webpage +="<body>\n";
  webpage +="<h1>ESP8266 with ThingSpeak Server</h1>\n";
  webpage +="<h3>Using Station(STA) Mode</h3>\n";
  webpage +="<h4>Control status For D8</h4>\n";
  webpage +="<a class=\"button button-on\" href=\"API REQUEST\"\">ON</a>\n";
  webpage +="<a class=\"button button-off\" href=\"API REQUEST\"\">OFF</a>\n";
  webpage +="</body>\n";
  webpage +="</html>\n";

  server.on("/", []() {
    server.send(200, "text/html", webpage);
  });
  
  server.begin(); //melakukan inisialisasi ke thingspeak
}

void loop() {
  server.handleClient(); 
  temperature = dht.readTemperature(); //membaca suhu pada sensor DHT11
  Serial.print("Temperature Value is :");
  Serial.print(temperature);
  Serial.println("°C");
  
  ThingSpeak.writeField(channel, 2, temperature, myWriteAPIKey); // tulis isi field (Channel, field number, data input, apikey)

  int led=ThingSpeak.readFloatField(channel,1); // baca field (channel, field number)
  if(led == 0){
    digitalWrite(D1, 0);
    Serial.println("LED is OFF..!");
  }
  else if(led == 1){
    digitalWrite(D1, 1);
    Serial.println("LED is ON..!");
  }
  delay(2000);
}

