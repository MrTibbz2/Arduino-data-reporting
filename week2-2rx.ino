
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";

void setup() {
    Serial.begin(9600);
    radio.begin();
    radio.openReadingPipe(1, address); // Open the reading pipe
    radio.setPALevel(RF24_PA_HIGH); // Set power level
    radio.setDataRate(RF24_250KBPS); // Set data rate
    radio.setChannel(80); // Set channel to match transmitter
    radio.startListening(); // Start listening for incoming data

    Serial.println("Receiver ready and listening...");
}

void loop() {
    if (radio.available()) { // Check if there is data available to read
        char text[32] = ""; // Buffer for incoming data
        radio.read(text, sizeof(text)); // Read the incoming data

        // Print the received data only if it's not empty
        if (text[0] != '\0') { // Check if the first character is not null
            Serial.print("Received: ");
            Serial.println(text); // Print the received data
        }
    }
}
