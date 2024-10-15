#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";
void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN); //set as: RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate(RF24_250KBPS); //set as: F24_250KBPS, F24_1MBPS, F24_2MBPS ==>250KBPS = longest range
  radio.setChannel(80); //sets channel from 2.4 to 2.524 GHz in 1 MHz increments 2.483.5 GHz is normal legal limit
  radio.startListening();
  pinMode(2, OUTPUT);
}
void loop() {
  if (radio.available()) {
    Serial.pringln("rf24 up")
    char text[32] = "";
    radio.read(&text, sizeof(text));
    String Data = String(text);
    Serial.println(text);
    Serial.println(text);
  }
}
