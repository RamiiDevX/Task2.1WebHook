#include <SPI.h>
#include <WiFiNINA.h>
#include "DHT.h"
#include "ThingSpeak.h"

// WiFi Credentials
char ssid[] = "iphone";   
char pass[] = "Rameesha";   

// ThingSpeak Info
#define ChannelID 2889192          
#define WriteAPI_KEY "YEM1A5X4Q0I7TAZ9"  

WiFiClient client;

#define DHTPIN 2        // Digital pin 
#define DHTTYPE DHT11   // Define DHT11 sensor

DHT dht(DHTPIN, DHTTYPE);

void setup() {
    Serial.begin(115200);
    dht.begin();
    
    // Connect to WiFi
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print("?");
        WiFi.begin(ssid, pass);
        delay(5000);
    }
    Serial.println("\nWiFi is Connected successfully !!");
    
    // Initialize ThingSpeak
    ThingSpeak.begin(client);
}

void loop() {
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    // Check whether reading fail
    if (isnan(humidity) || isnan(temperature)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }

    // Print Temperature and Humidity
    Serial.print("Temperature is: ");
    Serial.print(temperature);
    Serial.print(" Celcius, Humidity: ");
    Serial.print(humidity);
    Serial.println("%");

    // Send data to ThingSpeak Web
    ThingSpeak.setField(1, temperature);
    ThingSpeak.setField(2, humidity);
    ThingSpeak.writeFields(ChannelID, WriteAPI_KEY);
    
    delay(20000);  // Wait 20 seconds until sends the next data.
}
