#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include <LiquidCrystal.h>
const int RS = D2, EN = D3, d4 = D5, d5 = D6, d6 = D7, d7 = D8;   
LiquidCrystal lcd(RS, EN, d4, d5, d6, d7);

#define ONE_WIRE_BUS_1 14 //2
#define ONE_WIRE_BUS_2 5 //4

const char* ssid = "<WIFI SSID>";
const char* password = "<WIFI PASSWORD>";

const char* mqtt_server = "<MQTT SERVER IP ADDRESS>";
const char* mqtt_username = "<MQTT_BROKER_USERNAME>";
const char* mqtt_password = "<MQTT_BROKER_PASSWORD>";

  WiFiClient espClient;
  PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {


  char *payload_string = (char *) payload;
  payload_string[length] = NULL;
  
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
//  for (int i=0;i<length;i++) {
//    Serial.print((char)payload[i]);
  Serial.print(payload_string);
//  }
  Serial.println();
  
//  Serial.print(topic[length]);

  if(strcmp(topic,"<MQTT TOPIC FOR AIRTEMP>") == 0)
  {
      lcd.setCursor(0, 1);
      lcd.print("A: ");
      lcd.print(payload_string);
  }
  if(strcmp(topic,"<MQTT TOPIC FOR WATERTEMP>") == 0)
  {
      lcd.setCursor(9, 1);
      lcd.print("W: ");
      lcd.print(payload_string);  
  }
    
    Serial.println();

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("MQTTView")) {
      Serial.println("connected");

      client.subscribe("<MQTT TOPIC FOR AIRTEMP>");
      client.subscribe("<MQTT TOPIC FOR WATERTEMP>");
         
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
 
void setup() {

  Serial.begin(115200);
  
  Serial.println("MQTT DATA LCD Viewer");
  Serial.println("====================");
                           // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
                      // Print a message to the LCD.
  lcd.setCursor(2, 0);
  lcd.print("MQTTLCD Data");
  lcd.setCursor(0, 1);
  lcd.print("A: ---   W: --- ");

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
 
}
 
void loop() {

  float airtemp, watertemp;
  char airtemp_string[10], watertemp_string[10];

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

}
 
