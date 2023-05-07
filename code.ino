// Include the necessary libraries
#include <Arduino.h>

// Define the GPIO pin number for the push button
#define ESP32_C3_PUSH_BTN_GPIO 9

// The setup function runs once when the Arduino board is powered on or reset
void setup()
{
    // Initialize the serial communication with a baud rate of 115200
    Serial.begin(115200);

    // Configure the push button pin as an input
    pinMode(ESP32_C3_PUSH_BTN_GPIO, INPUT);
}

// The loop function runs continuously after the setup function has finished executing
void loop()
{
    // Check if the push button is pressed (the pin reads LOW when pressed)
    if (digitalRead(ESP32_C3_PUSH_BTN_GPIO) == LOW) {
        // Key debounce handling: Debouncing is used to eliminate the effect of
        // mechanical noise or switch bounce that can cause a single button press
        // to be detected as multiple presses. By waiting for a short period (100 ms)
        // after detecting a button press, we can ensure that any bounce has settled
        // and the button press is registered only once.
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
    }

    // Wait for a short period (100 ms) before checking the button state again
    delay(100);
}
