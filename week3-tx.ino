#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
// transmitter code

RF24 radio(9, 10); // CE, CSN
const byte writeaddress[6] = "00001"; // Address for device 1
const byte readaddress[6] = "00002"; // Address for device 2
bool isTransmitting = false;

void setup() {
    Serial.begin(9600);
    radio.begin();
    radio.openWritingPipe(writeaddress);
    radio.openReadingPipe(1, readaddress);
    radio.setPALevel(RF24_PA_HIGH);
    radio.setDataRate(RF24_250KBPS);
    radio.setChannel(80);
    radio.startListening();
    Serial.println("Device ready and listening...");
}

void loop() {
    // Handle incoming messages
    if (radio.available()) {
        char receivedText[32] = "";
        radio.read(receivedText, sizeof(receivedText));
        Serial.print("Received: ");
        Serial.println(receivedText);
        
        // Optionally respond to the received message
        String response = String(receivedText) + " (Response)";
        sendMessage(response.c_str());
    }

    // Check for outgoing messages
    if (Serial.available() > 0) {
        String inputString = Serial.readStringUntil('\n');
        sendMessage(inputString.c_str());
    }
}

void sendMessage(const char* message) {
    radio.stopListening(); // Stop listening to send
    Serial.print("Sending: ");
    Serial.println(message);

    // Send the message
    if (radio.write(message, strlen(message) + 1)) {
        Serial.println("Data sent successfully");
    } else {
        Serial.println("Data send failed");
    }

    delay(100); // Wait for a moment
    radio.startListening(); // Start listening again
}
