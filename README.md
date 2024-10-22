# Arduino-data-reporting
This project is to use an Arduino with sensors to communicate over 2.4ghz to an arduino to track data over time.
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
so uh.. currently the rf24 is being really weird an not even working... gonna troubleshoot. 
After some careful good work with handling com ports like a dumbass I managed to cause a windows kernel error and a BSOD.... nice!
