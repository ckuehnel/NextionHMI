If you search the net for libraries for Arduino, supporting the Nextion HMIs, then you will find several solutions.

First, you will surely stick to the library of the manufacturer ITEAD. This library is very comprehensive, is currently being maintained and an extensive forum deals with questions surrounding the use of this library. The Nextion HMI is connected to the Arduino via a hardware UART. For Arduinos, like the Olimexino-328 or Arduino Uno used here, you must change the cabling after the program upload and make the hardware UART available to the Nextion HMI. Using an Arduino with multiple hardware UARTs will look different and the situation relaxes.

Here I am using the NeoNextion Library by Dan Nixon (https://github.com/DanNixon/NeoNextion), which is based on the manufacturer library and works for the connection of the Nextion HMI with a software UART. The Library can be installed in the Arduino IDE via the Library Manager.
