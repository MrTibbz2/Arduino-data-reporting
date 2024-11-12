# Arduino-data-reporting
This project is to use an Arduino with sensors to communicate over 2.4ghz to an arduino to track data over time. DISCLAIMER: this is for a school project and the code is not that good.
<h1>Week 1</h1>
This week we worked to allow our 2.4 ghz transceivers to communicate, via arduino. One was programmed to send a signal, and the other was programmed to recieve the signal. We did this by first connecting the 2.4 ghz sensors to seperate arduino boards using the pins. We then coded the sensors, making one of them able to send a signal and the other to be able to recieve the signal. The codes we used were https://github.com/MrTibbz2/Arduino-data-reporting/edit/main/Rx.ino 
and https://github.com/MrTibbz2/Arduino-data-reporting/edit/main/tx.ino 
we also used these diagrams to figure out the wiring for the nrf24l01 modules at: https://github.com/MrTibbz2/Arduino-data-reporting/edit/main/diagram.jpg 
next week to-do list:
- make a system for sending data from serial to another arduino as proof of concept
<h1>Week 2</h1>
This week we were able to setup and complete a serial data based communications system between two NRF24s.
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
currently working on a system for a syn ack connect style authentication on startup. Not sure how well it'd work but wanted to write it from scratch so its a little simpler. In week4tx and week4rx there is now a working system for transmitting and receiving from two linked arduinos. I have code for a dht11 sensor as well to tell what humidity and temp one arduino is in but.... I mightve fried it.
<h1>how does the code work?</h1>
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


<h1>How could this be used in a real world application?</h1>

Long ranged radio devices exist so that we are able to transmit and receive data without the need of human interaction. not only does it allow us to automate different parts of our lives, it allows for data to be collected when it wouldnt be possible due to safety reasons. High grade technology can be used in very hot, cold and extreme conditions where it wouldn't be safe for a human to do so. This project is simply a proof of concept of how these devices that power our daily lives work. There are countless examples where this can be used, such as bushfires, remote living areas where internet isn't available, underwater, and many many more. 

<h3>Bushfires?</h3>

Bushfires are just one example of where this could be used. Microcontrollers could be dropped into high risk areas to safely collect, transmit, and analyse temperature/humidity data. This can also be made cheap using easy to find components such as a raspberry pi pico, DHT11 sensors, and nrf24l01 transceivers.

<h1>how do the electronics work?</h1>

the nrf24 is wired as the following:


![8151-2a82-1600x0_interfejs-nrf24l01-z-arduino-02-nrf24l01-receiver](https://github.com/user-attachments/assets/c148031f-df02-4da0-bd7c-fcb65bdfca19)

# nRF24L01 Wireless Module Overview

The **nRF24L01** is a low-power, 2.4 GHz wireless module commonly used in short-range communication for IoT devices, wireless sensors, and remote controls. It offers a simple and cost-effective solution for wireless communication.

## Key Features

- **Frequency**: 2.4 GHz ISM band
- **Data rates**: 250 Kbps, 1 Mbps, or 2 Mbps
- **Range**: Up to 100 meters (in open space)
- **Power consumption**: Very low, with sleep modes
- **Channels**: 125 available channels

## how is it configured

### 1. Pipes and Addresses

The nRF24L01 uses **pipes** to manage communication channels. Each pipe has a unique **address** for sending and receiving data. There are **6 pipes** (0-5) available.

- **Address**: Each device has a unique address (up to 5 bytes). Communication happens between devices with matching addresses.
- **Address Width**: The address can be 3, 4, or 5 bytes long, with 5-byte addresses providing better uniqueness.

### 2. Communication Modes

- **TX (Transmit) Mode**: The device sends data.
- **RX (Receive) Mode**: The device listens for incoming data.

Data is transmitted via a pipe, and the receiving device listens on the corresponding pipe.

### 3. Pipe Configuration

- **Pipe 0** is the primary receiving pipe.
- **Pipes 1-5** can be configured to listen for data from other devices.

This allows a device to listen on multiple pipes and communicate with several devices simultaneously.

### 4. Packet Structure

Each transmission packet consists of:
- **Receiver address**
- **Payload** (data to send, up to 32 bytes)
- **CRC** (Cyclic Redundancy Check for error detection)
- **Control bits** (for retransmissions, flags, etc.)

Larger payloads require multiple packets.

### 5. Frequency and Channel

The nRF24L01 operates on **125 channels** in the 2.4 GHz band. Channels correspond to different frequencies, and the channel selection can help avoid interference.

### 6. Power and Modes

The module offers several power modes:
- **Power-down**: Lowest power mode, used when idle.
- **Standby**: The module is on but not transmitting or receiving.
- **Active**: The module is actively transmitting or receiving.

Transmission power can be adjusted from **-18 dBm to 0 dBm**, balancing range and power consumption.

### 7. Auto-Acknowledgment and Retransmission

The nRF24L01 supports **auto-acknowledgment** and **retransmission**. If a transmission isn’t acknowledged, the module will automatically retry, improving reliability in noisy environments.

### 8. Dynamic Payload Length

The module supports **dynamic payload length**, which means the size of the data can vary per packet. This allows for more efficient communication when sending data of different sizes.

## Example Setup

### Receiver Setup

1. Set **Pipe 0** to listen for data at address `0xE7E7E7E7E7`.
2. Enable **auto-acknowledgment** and **CRC**.
3. The receiver waits for data on Pipe 0 and sends an acknowledgment when received.

### Transmitter Setup

1. Send data to address `0xE7E7E7E7E7` via **Pipe 0**.
2. If the receiver doesn’t acknowledge, the transmitter retries automatically.

## Conclusion

The **nRF24L01** is a versatile, low-power wireless module ideal for short-range communication. With **pipes** and **addresses**, it can communicate with multiple devices. Features like **auto-acknowledgment**, **retransmission**, and **dynamic payload** length make it a reliable and flexible choice for wireless applications in IoT, sensor networks, and remote control systems.
