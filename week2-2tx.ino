#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

char string1[32];
RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";

void setup() {
    Serial.begin(9600);

    // Check if radio is initialized properly
    if (radio.begin()) {
        Serial.println("RF24 initialized successfully");
    } else {
        Serial.println("RF24 initialization failed");
    }
    radio.openWritingPipe(address);
    radio.setPALevel(RF24_PA_HIGH);
    radio.setDataRate(RF24_250KBPS);
    radio.setChannel(80);
    radio.stopListening();
    delay(1000);
}

void loop() {
    static String inputString = ""; // Create a string to hold incoming data

    while (Serial.available() > 0) { // Check if data is available
        char c = Serial.read(); // Read a character from the serial buffer
        if (c == '\n') { // Check for newline character
            inputString.toCharArray(string1, sizeof(string1)); // Copy to char array
            string1[sizeof(string1) - 1] = '\0'; // Ensure null termination
            
            Serial.println("sent the data:");
            Serial.println(string1); // Print the string to the serial monitor
            
            // Check the length of the data being sent
            Serial.print("Sending data of size: ");
            Serial.println(strlen(string1) + 1); // +1 for the null terminator
            
            // Send the string data using RF24
            if (radio.write(string1, strlen(string1) + 1)) {
                Serial.println("Data sent successfully");
            } else {
                Serial.println("Data send failed");
            }

            inputString = ""; // Clear the input string for the next input
        } else {
            inputString += c; // Add the character to the input string
            if (inputString.length() >= sizeof(string1) - 1) {
                inputString.remove(0, 1); // Remove the oldest character if buffer is full
            }
        }
    }
}
