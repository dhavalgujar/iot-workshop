// Include the necessary libraries
#include <Arduino.h>
// Include the necessary library for controlling the WS2812 LED strip
#include "Freenove_WS2812_Lib_for_ESP32.h"

// Define constants for the LED strip configuration
#define CHANNEL 0            // The channel number for the RMT peripheral
#define NUM_OF_LEDS 1        // The number of LEDs in the strip

// Define the GPIO pin numbers for the push button and the LED
#define ESP32_C3_PUSH_BTN_GPIO 9
#define ESP32_C3_LED_GPIO 8

// Create an instance of the Freenove_ESP32_WS2812 class to control the LED strip
Freenove_ESP32_WS2812 strip = Freenove_ESP32_WS2812(NUM_OF_LEDS, ESP32_C3_LED_GPIO, CHANNEL);

// The setup function runs once when the Arduino board is powered on or reset
void setup()
{
    // Initialize the serial communication with a baud rate of 115200
    Serial.begin(115200);

    // Configure the push button pin as an input
    pinMode(ESP32_C3_PUSH_BTN_GPIO, INPUT);

    // Initialize the LED strip
    strip.begin();

    // Turn off the first LED in the strip
    strip.setLedColor(0, 0, 0, 0);
}

// Declare a variable to store the toggle state of the LED
bool toggle_state = false;

// The loop function runs continuously after the setup function has finished executing
void loop()
{
    // Check if the push button is pressed (the pin reads LOW when pressed)
    if (digitalRead(ESP32_C3_PUSH_BTN_GPIO) == LOW) {
        // Key debounce handling
        delay(100);

        // Record the start time of the button press
        int startTime = millis();

        // Wait for the button to be released (the pin reads HIGH when released)
        while (digitalRead(ESP32_C3_PUSH_BTN_GPIO) == LOW) {
            delay(50);
        }

        // Record the end time of the button press
        int endTime = millis();

        // Calculate and print the duration of the button press in seconds
        Serial.printf("Button pressed for %d seconds\n", (endTime - startTime) / 1000);

        // Toggle the LED state
        toggle_state = !toggle_state;

        // If the toggle state is true, turn on the LED with red color
        if (toggle_state == true) {
          strip.setLedColor(0, 255, 0, 0);
        }
        // If the toggle state is false, turn off the LED
        else {
          strip.setLedColor(0, 0, 0, 0);
        }
    }

    // Wait for a short period (100 ms) before checking the button state again
    delay(100);
}
