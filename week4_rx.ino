#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN
const byte writeaddress[6] = "00002"; // Address for device 2
const byte readaddress[6] = "00001"; // Address for device 1
const char SynAckResponse = 10; // ACK response character
bool RF24Linked = false; // Track if linked with the transmitter

void setup() {
    Serial.begin(9600);
    radio.begin();
    radio.openWritingPipe(writeaddress);
    radio.openReadingPipe(1, readaddress);
    radio.setPALevel(RF24_PA_HIGH);
    radio.setDataRate(RF24_250KBPS);
    radio.setChannel(80);
    radio.startListening();
    Serial.println("Receiver ready and listening...");
}

void loop() {
    // Check for incoming messages
    if (radio.available()) {
        char receivedText[32] = "";
        radio.read(receivedText, sizeof(receivedText));
        Serial.print("Received: ");
        Serial.println(receivedText);

        // Check for SYN message
        if (receivedText[0] == 10) { // Check for SYN signal
            Serial.println("SYN received, sending ACK...");
            sendAck();
        }
    }

    // Only check for outgoing messages if linked
    if (RF24Linked && Serial.available() > 0) {
        String inputString = Serial.readStringUntil('\n');
        sendMessage(inputString.c_str());
    }
}

// Function to send an ACK response
void sendAck() {
    radio.stopListening(); // Stop listening to send
    radio.write(&SynAckResponse, sizeof(SynAckResponse)); // Send ACK response
    Serial.println("ACK sent.");
    radio.startListening(); // Start listening again
    RF24Linked = true; // Mark as linked after sending ACK
}

// Function to send a message over RF24
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

