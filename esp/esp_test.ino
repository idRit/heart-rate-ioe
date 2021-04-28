#define USE_ARDUINO_INTERRUPTS false

#include "PulseSensorPlayground.h"
#include "WiFi.h"
#include "ArduinoJson.h"
#include "PubSubClient.h"     
#include "DHT.h"        

const char* mqtt_server = "lion.rmq.cloudamqp.com"; 
const char* mqtt_user = "ylhnwvqf:ylhnwvqf";
const char* mqtt_pass = "U0gpsudyXQBu8LiwAIOc7um4Av9ajPBd";
const char* clientSecret = "hetalq";

const char* ssid = "Nokia 6.1 Plus";
const char* password = "12345678";

const int PulseWire = 12;
int Threshold = 550;   
const int DHTwire = 4;

PulseSensorPlayground pulseSensor;
WiFiClient espClient;
PubSubClient client(espClient);
DHT dht = DHT(DHTwire, DHT11, 6);

void callback(char* topic, byte* payload, unsigned int length) {
    Serial.println("Message Recieved!");
    Serial.println(topic);
}

void reconnect() {
  // Loop until we're reconnected
  Serial.println("In reconnect...");
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(clientSecret, mqtt_user, mqtt_pass)) {
      client.subscribe("nodemcu_data_queue");
      client.setCallback(callback);
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {   

    Serial.begin(9600);

    pulseSensor.analogInput(PulseWire);   
    pulseSensor.setThreshold(Threshold);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println("Connecting to WiFi..");
    }
 
    Serial.println("Connected to the WiFi network");

    client.setServer(mqtt_server, 1883);   

    if (pulseSensor.begin()) {
        Serial.println("We created a pulseSensor Object !");  
    }
}



void loop() {
    client.loop();
    if (!client.connected()) {
        reconnect();
    }

    int myBPM = pulseSensor.getBeatsPerMinute();
    float humidity = dht.readHumidity(); 
    float temperature = dht.readTemperature();  

    if (pulseSensor.sawStartOfBeat()) {           
        char output[300];

        StaticJsonDocument<300> doc;
        doc["bpm"] = myBPM;
        doc["humidity"] = humidity;
        doc["temp"] = temperature;

        serializeJson(doc, output);
        Serial.println(output);

        client.publish(clientSecret,  output);
    }

    delay(20);                 
}
