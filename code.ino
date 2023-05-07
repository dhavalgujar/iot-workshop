// Include the necessary libraries
#include <Arduino.h>

// The setup function runs once when the Arduino board is powered on or reset
void setup()
{
    // Initialize the serial communication with a baud rate of 115200
    // This sets up a communication channel between the Arduino and your computer
    Serial.begin(115200);

    // Print "Hello World" to the serial monitor
    // This serves as an initial message to verify that the program has started running
    Serial.println("Hello World");
}

// Declare a variable to store the delay time in milliseconds
// In this case, 1 second (1000 milliseconds) is stored in the variable
int delayMilliSeconds = 1 * 1000;

// The loop function runs continuously after the setup function has finished executing
void loop() {
    // Print a period (.) to the serial monitor
    // This will be printed every time the loop iterates
    Serial.println(".");

    // Pause the program for the duration specified by delayMilliSeconds
    // In this case, the program will wait for 1 second (1000 milliseconds) before continuing
    delay(delayMilliSeconds);
}
