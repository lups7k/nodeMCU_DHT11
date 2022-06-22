#include<ESP8266WiFi.h> 
#include<ESP8266WebServer.h> 
#include <DHT.h> 

const char* ssid = "xxxxxxx";  
const char* password = "xxxxx"; 

ESP8266WebServer server(80); // server port

#define DHTPIN 4 
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

float temp, humid;

void setup() {
  Serial.begin(115200); // serial port
  delay(50);
  dht.begin();

  Serial.println("Connecting to network: ");
  Serial.println(ssid); 

  WiFi.begin(ssid, password);

  // checking connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print("."); 
  }
  Serial.println(" \n wifi connected\nIP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound); 
  server.begin();
  Serial.println("HTTP Server initialized");

}

void loop() {
  server.handleClient();
}

void handle_OnConnect() {
  temp = dht.readTemperature(); 
  humid = dht.readHumidity();
  Serial.print("Temperature: "); // being printed on serial monitor
  Serial.print(temp); 
  Serial.println(" ºC");
  Serial.print("Humidity: ");
  Serial.print(humid); 
  Serial.println(" %");
  server.send(200, "text/html", sendHTML(temp, humid));

}

void handle_NotFound() { 
  server.send(404, "text/plain", "Not found"); 

}

string sendHTML(float tempstat, float humidstat) { 
  string ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<meta http-equiv='refresh' content='2'>"; 
  ptr += "<link href=\"https://fonts.googleapis.com/css?family=Open+Sans:300,400,600\" rel=\"stylesheet\">\n";
  ptr += "<title>Temperature and Humidity monitor</title>\n";
  
  ptr += "<style>html { font-family: 'Open Sans', sans-serif; display: block; margin: 0px auto; text-align: center;color: #000000;}\n";
  ptr += "body{margin-top: 50px;}\n";
  ptr += "h1 {margin: 50px auto 30px;}\n";
  ptr += "h2 {margin: 40px auto 20px;}\n";
  ptr += "p {font-size: 24px;color: #000000;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<div id=\"webpage\">\n";
  ptr += "<h1>Temperature and humidity</h1>\n";
  ptr += "<h2>NODEMCU ESP8266 Web Server</h2>\n";

  ptr += "<p><b>Temperature: </b>";
  ptr += (float)tempstat;
  ptr += " Graus Celsius</p>";
  ptr += "<p><b>Humidity: </b>";
  ptr += (float)humidstat;
  ptr += " %</p>";

  ptr += "</div>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;

}
