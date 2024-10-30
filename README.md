# Arduino-data-reporting
This project is to use an Arduino with sensors to communicate over 2.4ghz to an arduino to track data over time. DISCLAIMER: this is for a school project and the code is not that good.
<h1>Week 1</h1>
This week we worked to allow our 2.4 ghz sensors to communicate, via arduino. One was programmed to send a signal, and the other was programmed to recieve the signal. We did this by first connecting the 2.4 ghz sensors to seperate arduino boards using the pins. We then coded the sensors, making one of them able to send a signal and the other to be able to recieve the signal. The codes we used were https://github.com/MrTibbz2/Arduino-data-reporting/edit/main/Rx.ino 
and https://github.com/MrTibbz2/Arduino-data-reporting/edit/main/tx.ino 
we also used these diagrams to figure out the wiring for the nrf24l01 modules at: https://github.com/MrTibbz2/Arduino-data-reporting/edit/main/diagram.jpg 
next week to-do list:
- make a system for sending data from serial to another arduino as proof of concept
<h1>Week 2</h1>
This week we were able to setup and complete a serial based communications system between two NRF24s.
the week2.2tx and week2.2rx use buffer controlling to ensure data sending stability. 
this allows us to send data between two arduinos easily.
next week to-do list:
- maybe integrate a display? (could be done later)
- try and integrate a dht11 sensor
- maybe make a system so that two way communication is possible

<H1>week 3</H1>
Had some issues with the nrf24.
End of week 3: ended up just starting to work miraculously? in week3-tx and week3-rx we've got two way communication working pretty well, the response packets arent very functional but they arent really nneeded yet.

<h1>week 4</h1>
currently working on a system for a syn ack connect style authentication on startup. Not sure how well it'd work but wanted to write it from scratch so its a little simpler. In week4tx and week4rx there is now a working system for transmitting and receiving from two linked arduinos. I have code for a dht11 sensor as well to tell what humidity and temp one arduino is in but well.... I mightve fried it.
<h1>how does the code work</h1>
the tx.ino script does a few things. 

```cpp

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

```

these lines of code add the needed libraries for the nrf24l01 to function.
 ```cpp

RF24 radio(9, 10); // CE, CSN
const byte writeaddress[6] = "00001"; // Address for device 1
const byte readaddress[6] = "00002"; // Address for device 2
const char SynSend = 10; // SYN signal
const char SynAckResponse = 10; // ACK response character
bool RF24Linking = false;
bool RF24Linked = false;
bool AckListen = false;

```

these lines setup all of the needed variables, as well as creating an instance of the nrf24 driver.

```cpp

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

```
void setup starts serial as well as the needed pipes/parameters for the nrf24.

```cpp

void loop() {
    if (!RF24Linking) {
        Serial.println("Attempting SYN connection...");
        updateSynAckState();
    }

```
the first part of the loop allows the arduino to check if it is already trying a connection, and if not, begin the UpdateSynAckState. Syn ack is a modern wifi standard, but can also be used as an abbreviation for "synchronise" and "acknowledge". This allows the two arduinos to make sure theyre both able to send and receive successfully.

```cpp

   if (AckListen) {
        if (radio.available()) {
            char AckResponse;
            radio.read(&AckResponse, sizeof(AckResponse));
            if (AckResponse == SynAckResponse) {
                Serial.println("ACK received.");
                RF24Linked = true;
                AckListen = false; // Stop listening for ACK
            }
        }
    }

```

the second part of the loop checks if the arduino should be listening for an acknowledge, and if so, waits for another arduino to send this.

```cpp

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

```
the last part of the loop checks for both serial data and nrf24 data. it either receives data over serial to be send over the arduino, or prints data received over nrf24. if data needs to be transmitted, it calls the sendMessage function, allowing for cleaner code.

```cpp

void updateSynAckState() {
    radio.stopListening();
    radio.write(&SynSend, sizeof(SynSend)); // Send SYN signal
    Serial.println("SYN sent, waiting for ACK...");
    radio.startListening();
    AckListen = true; // Start listening for ACK
    RF24Linking = true; // Update state
}

```
the UpdateSynAck function tries to connect the arduino to another through a series of checks.

```cpp

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

```

finally, the sendMessage function takes the data from the loop and transmits it. (also includes debug).


<h3>receiver?</h3>
The receiver code looks almost identical, but instead of sending syn, it listens for it and sends ack.



