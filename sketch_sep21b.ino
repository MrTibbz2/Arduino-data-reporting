#include <nRF24L01.h>
#include <RF24.h>
#include <DHT11.h>

#define SENSORTYPE DHT11
#define SENSORPIN 2

DHT11 dht11(SENSORPIN);



void setup() {
  Serial.begin(9600);
}

void loop() {
  int temperature = 0;
  int humidity = 0;

    int result = dht11.readTemperatureHumidity(temperature, humidity);

    if (result == 0) {
        Serial.print("Temperature: ");
        Serial.print(temperature);
        Serial.print(" °C\tHumidity: ");
        Serial.print(humidity);
        Serial.println(" %");
    } else {

        Serial.println(DHT11::getErrorString(result));
    }
}
