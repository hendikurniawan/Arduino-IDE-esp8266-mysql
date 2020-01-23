//--------library esp8266--------
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

//---------library DHT22---------
#include <dht.h>
#define dataPin D6 // Defines pin number to which the sensor is connected
dht DHT; // Creats a DHT object

//---------input ssid and pass wif to conect---------
const char *ssid = "@WiFi.UBL";
const char *password = "ublkecee1";

//---------power setup------------
void setup() {
  
  delay(1000);
  Serial.begin(115200);
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");

  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
}

//----------Main Program Loop--------
void loop() {

  HTTPClient http;    //Declare object of class HTTPClient

  String sensor, suhu, kelembaban, postData;

    int readData = DHT.read22(dataPin); // Reads the data from the sensor
    float t = DHT.temperature;          // Gets the values of the temperature
    float h = DHT.humidity;             // Gets the values of the humidity
      
  sensor = "DHT22";
  suhu = String(t); //String to integer conversion
  kelembaban = String(h); //String to integer conversion

  //Post Data
  postData = "sensor=" + sensor + "&suhu=" + suhu + "&kelembaban=" + kelembaban;
  
  http.begin("http://172.32.1.91/postData/postdemo.php");                //Specify request destination or api service
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");    //Specify content-type header

  int httpCode = http.POST(postData);   //Send the request
  String payload = http.getString();    //Get the response payload

  Serial.println(httpCode);   //Print HTTP return code
  Serial.println(payload);    //Print request response payload
  
  //suhu print into serial monitor
  Serial.println(t);

  http.end();  //Close connection
  
  delay(5000);  //Post Data at every 5 seconds
}
