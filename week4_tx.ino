#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN
const byte writeaddress[6] = "00001"; // Address for device 1
const byte readaddress[6] = "00002"; // Address for device 2
const char SynSend = 10; // SYN signal
const char SynAckResponse = 10; // ACK response character
bool RF24Linking = false;
bool AckListen = false;

void setup() {
    Serial.begin(9600);
    radio.begin();
    radio.openWritingPipe(writeaddress);
    radio.openReadingPipe(1, readaddress);
    radio.setPALevel(RF24_PA_HIGH);
    radio.setDataRate(RF24_250KBPS);
    radio.setChannel(80);
    radio.startListening();
    Serial.println("Transmitter ready and listening...");
}

void loop() {
    if (!RF24Linking) {
        Serial.println("Attempting SYN connection...");
        updateSynAckState();
    }

    if (AckListen) {
        if (radio.available()) {
            char AckResponse;
            radio.read(&AckResponse, sizeof(AckResponse));
            if (AckResponse == SynAckResponse) {
                Serial.println("ACK received.");
                AckListen = false; // Stop listening for ACK
            }
        }
    }

    // Handle incoming messages
    if (radio.available()) {
        char receivedText[32] = "";
        radio.read(receivedText, sizeof(receivedText));
        Serial.print("Received: ");
        Serial.println(receivedText);
    }

    // Check for outgoing messages
    if (Serial.available() > 0) {
        String inputString = Serial.readStringUntil('\n');
        sendMessage(inputString.c_str());
    }
}

void updateSynAckState() {
    radio.stopListening();
    radio.write(&SynSend, sizeof(SynSend)); // Send SYN signal
    Serial.println("SYN sent, waiting for ACK...");
    radio.startListening();
    AckListen = true; // Start listening for ACK
    RF24Linking = true; // Update state
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

