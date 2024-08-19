#include "DHT.h"
#include <WiFiNINA.h>
#include "ThingSpeak.h"

// Pin and sensor type definitions
#define DHTPIN 4          // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22     // DHT 22 (AM2302), AM2321

// DHT sensor object
DHT dht(DHTPIN, DHTTYPE);

// WiFi credentials
char ssid[] = "Diyaa"; 
char pass[] = "1234567d";
WiFiClient client;

// ThingSpeak channel and API key
unsigned long myChannelNumber = 2625144;  // replace with your channel number
const char * myWriteAPIKey = "7F0SSF9IJZZYTKBT";  // replace with your channel write API Key

void setup() {
  Serial.begin(115200);
  dht.begin();  // Initialize the DHT sensor

  // Attempt to connect to WiFi
  while (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, pass);
    delay(10000);
  }

  // Initialize ThingSpeak
  ThingSpeak.begin(client);
}

void loop() {
  delay(2000);

  // Read humidity and temperature from the DHT22 sensor
  float humidity = dht.readHumidity();
  float temperatureC = dht.readTemperature();
  float temperatureF = dht.readTemperature(true);

  // Check if any reads failed
  if (isnan(humidity) || isnan(temperatureC) || isnan(temperatureF)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Print the sensor readings to the Serial Monitor
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%  Temperature: ");
  Serial.print(temperatureC);
  Serial.print("°C ~ ");
  Serial.print(temperatureF);
  Serial.println("°F");

  // Set the fields with the sensor readings
  ThingSpeak.setField(1, temperatureC);
  ThingSpeak.setField(2, humidity);

  // Write to ThingSpeak
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (x == 200) {
    Serial.println("Channel update successful.");
  } else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }

  // Wait 60 seconds before the next update
  delay(60000);
}
