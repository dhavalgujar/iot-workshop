// Include the necessary libraries
#include <Arduino.h>
// Include the necessary library for controlling the WS2812 LED
#include "Freenove_WS2812_Lib_for_ESP32.h"

// Define constants for the LED configuration
#define CHANNEL     0       // The channel number for the RMT peripheral
#define NUM_OF_LEDS 1       // The number of LEDs in the strip
#define ESP32_C3_LED_GPIO 8 // The GPIO pin connected to the LED

// Create an instance of the Freenove_ESP32_WS2812 class to control the LED
Freenove_ESP32_WS2812 strip = Freenove_ESP32_WS2812(NUM_OF_LEDS, ESP32_C3_LED_GPIO, CHANNEL);

// The setup function runs once when the Arduino board is powered on or reset
void setup()
{
    // Initialize the serial communication with a baud rate of 115200
    // This sets up a communication channel between the Arduino and your computer
    Serial.begin(115200);

    // Initialize the LED strip
    strip.begin();
}

// Declare variables for the LED color (range should be 0 - 255)
int red = 255, green = 0, blue = 0;

// Declare a variable to store the delay time in milliseconds
// In this case, 1 second (1000 milliseconds) is stored in the variable
int delayMilliSeconds = 1 * 1000;

// The loop function runs continuously after the setup function has finished executing
void loop()
{
    // Print a message to the serial monitor
    Serial.println("Turning LED ON");

    // Set the color of the first (0th) LED in the strip
    strip.setLedColor(0, red, green, blue);

    // Pause the program for the duration specified by delayMilliSeconds
    delay(delayMilliSeconds);

    // Print a message to the serial monitor
    Serial.println("Turning LED OFF");

    // Turn off the first (0th) LED in the strip
    strip.setLedColor(0, 0, 0, 0);

    // Pause the program for the duration specified by delayMilliSeconds
    // In this case, the program will wait for 1 second (1000 milliseconds) before continuing
    delay(delayMilliSeconds);
}
