    /* By Jeremy S. Cook 3/21/2019
    * 
    * Code Modified from: Arduino Wireless Communication Tutorial
    * Example 1 - Receiver Code
    * by Dejan Nedelkovski, www.HowToMechatronics.com
    * 
    * Library: TMRh20/RF24, https://github.com/nRF24/RF24
    */
    #include <SPI.h>
    #include <nRF24L01.h>
    #include <RF24.h>
    RF24 radio(7, 8); // CE, CSN
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
        char text[32] = "";
        radio.read(&text, sizeof(text));
        String Data = String(text);
        Serial.println(text);
        if(Data == "L") {
          Serial.println("is L - Light On");
          digitalWrite(2, HIGH);
          delay(500);
        }
      }
      else digitalWrite(2, LOW);
    }
