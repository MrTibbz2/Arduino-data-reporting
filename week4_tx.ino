#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// transmitter code
RF24 radio(9, 10); // CE, CSN
const byte writeaddress[6] = "00001"; // Address for device 1
const byte readaddress[6] = "00002"; // Address for device 2
bool RF24Linking = false; // Declare RF24Linking
bool SynAckReady = false;
bool RF24Link = false;
const char SynSend = 10;
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
    Serial.println("Device ready and listening...");
}

void loop() {
    if (RF24Linking == false) { // Fixed comparison
        Serial.println("Attempting Syn/Ack connection");
        updateSynAckState();
    }
    
    if (RF24Link == true) { // Handle incoming messages
        if (radio.available()) {
            char receivedText[32] = "";
            radio.read(receivedText, sizeof(receivedText));
            Serial.print("Received: ");
            Serial.println(receivedText);
        }
    }
    
    if (RF24Link == true) { // Check for outgoing messages
        if (Serial.available() > 0) {
            String inputString = Serial.readStringUntil('\n');
            sendMessage(inputString.c_str());
        }
    } 
    
    if (AckListen == true) { // Check for ACK responses
        char AckResponse[32]; // Declare as an array
        radio.startListening();
        Serial.println("Listening for ACK response...");
        radio.read(AckResponse, sizeof(AckResponse));
        if (AckResponse[0] == 10) { // Check the first character of the response
            RF24Link = true;
            AckListen = false;
            Serial.println("response received");
        }
    }
}

void updateSynAckState() {
    if (RF24Linking == false) { // Fixed comparison
        radio.stopListening();
        radio.write(&SynSend, sizeof(SynSend)); // Send single character
        Serial.println("Attempting RF24 connection link");
        radio.startListening();
        AckListen = true;
        RF24Linking = true; // Update RF24Linking to true
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
