// Include the necessary libraries for the ESP RainMaker, Wi-Fi and Wi-Fi Provisioning
#include "RMaker.h"
#include "WiFi.h"
#include "WiFiProv.h"
#include "Freenove_WS2812_Lib_for_ESP32.h"

#define CHANNEL 0
#define NUM_OF_LEDS 1

// Define GPIO pins for push button and LED
#define ESP32_C3_PUSH_BTN_GPIO 9
#define ESP32_C3_LED_GPIO 8

// Define provisioning service name and proof of possession (pop) code
const char *service_name = "PROV_3448"; // Replace with a random 4-digit code
const char *pop = "abcd1234";

// Define variables for power state and HSV color values
bool power_state = true;
int hue_val = 255;
int saturation_val = 25;
int brightness_val = 50;

// Initialize the LED strip
Freenove_ESP32_WS2812 strip = Freenove_ESP32_WS2812(NUM_OF_LEDS, ESP32_C3_LED_GPIO, CHANNEL);

// Define the custom device
static Device my_device;

// Event handler for system provisioning events
void sysProvEvent(arduino_event_t *sys_event)
{
    switch (sys_event->event_id) {
        case ARDUINO_EVENT_PROV_START:
            Serial.printf("\nProvisioning Started with name \"%s\" and PoP \"%s\" on BLE\n", service_name, pop);
            printQR(service_name, pop, "ble");
            break;
        default:;
    }
}

static void convertHSVtoRGB(int h, int s, int v, int *r, int *g, int *b)
{
    h %= 360; // h -> [0,360]
    uint32_t rgb_max = v * 2.55f;
    uint32_t rgb_min = rgb_max * (100 - s) / 100.0f;
    uint32_t i = h / 60;
    uint32_t diff = h % 60;
    // RGB adjustment amount by hue
    uint32_t rgb_adj = (rgb_max - rgb_min) * diff / 60;
    switch (i) {
    case 0:
        *r = rgb_max;
        *g = rgb_min + rgb_adj;
        *b = rgb_min;
        break;
    case 1:
        *r = rgb_max - rgb_adj;
        *g = rgb_max;
        *b = rgb_min;
        break;
    case 2:
        *r = rgb_min;
        *g = rgb_max;
        *b = rgb_min + rgb_adj;
        break;
    case 3:
        *r = rgb_min;
        *g = rgb_max - rgb_adj;
        *b = rgb_max;
        break;
    case 4:
        *r = rgb_min + rgb_adj;
        *g = rgb_min;
        *b = rgb_max;
        break;
    default:
        *r = rgb_max;
        *g = rgb_min;
        *b = rgb_max - rgb_adj;
        break;
    }
}

void mySetLedColor(int hue, int saturation, int brightness)
{
    int red, green, blue;
    convertHSVtoRGB(hue, saturation, brightness, &red, &green, &blue);
    strip.setLedColor(0, red, green, blue);
}

// Callback function to handle parameter updates from the RainMaker server
void write_callback(Device *device, Param *param, const param_val_t val, void *priv_data, write_ctx_t *ctx)
{
    const char *device_name = device->getDeviceName();
    const char *param_name = param->getParamName();

    // Update the power state and RGB color values based on the received parameter updates
    if (strcmp(param_name, "Power") == 0) {
        Serial.printf("Received value = %s for %s - %s\n", val.val.b? "true" : "false", device_name, param_name);
        power_state = val.val.b;
    } else if (strcmp(param_name, "Hue") == 0) {
        Serial.printf("Received value = %d for %s - %s\n", val.val.i, device_name, param_name);
        hue_val = val.val.i;
    } else if (strcmp(param_name, "Saturation") == 0) {
        Serial.printf("Received value = %d for %s - %s\n", val.val.i, device_name, param_name);
        saturation_val = val.val.i;
    } else if (strcmp(param_name, "Brightness") == 0) {
        Serial.printf("Received value = %d for %s - %s\n", val.val.i, device_name, param_name);
        brightness_val = val.val.i;
    } else {
        return;
    }
    if (power_state == false) {
        mySetLedColor(0, 0, 0);
    } else {
        mySetLedColor(hue_val, saturation_val, brightness_val);
    }
    param->updateAndReport(val);
}

// The setup function runs once when the Arduino board is powered on or reset
void setup()
{
    // Initialize the serial communication with a baud rate of 115200
    Serial.begin(115200);

    // Set the push button pin as an INPUT
    pinMode(ESP32_C3_PUSH_BTN_GPIO, INPUT);

    // Initialize the LED strip and set its initial color
    strip.begin();
    mySetLedColor(hue_val, saturation_val, brightness_val);

    // Initialize and configure the RainMaker node
    Node my_node;
    my_node = RMaker.initNode("ESP RainMaker Node");

    // Create a custom device with a lightbulb type and power state
    my_device = Device("LED Light", "esp.device.lightbulb", &power_state);

    // Add a name parameter for the custom device
    my_device.addNameParam();

    // Create, configure, and add a custom power parameter to the device
    Param power_param("Power", "esp.param.power", value(power_state), PROP_FLAG_READ | PROP_FLAG_WRITE);
    power_param.addUIType(ESP_RMAKER_UI_TOGGLE);
    my_device.addParam(power_param);

    // Assign the power parameter as the primary parameter
    my_device.assignPrimaryParam(my_device.getParamByName("Power"));

    // Create, configure, and add custom Hue, Saturation, and Brightness parameters to the device
    // These parameters will be used to control the color of the LED
    Param hue_param("Hue", "esp.param.hue", value(hue_val), PROP_FLAG_READ | PROP_FLAG_WRITE);
    hue_param.addBounds(value(0), value(360), value(1));
    hue_param.addUIType(ESP_RMAKER_UI_HUE_SLIDER);
    my_device.addParam(hue_param);

    Param saturation_param("Saturation", "esp.param.saturation", value(saturation_val), PROP_FLAG_READ | PROP_FLAG_WRITE);
    saturation_param.addBounds(value(0), value(100), value(1));
    saturation_param.addUIType(ESP_RMAKER_UI_SLIDER);
    my_device.addParam(saturation_param);

    Param brightness_param("Brightness", "esp.param.brightness", value(brightness_val), PROP_FLAG_READ | PROP_FLAG_WRITE);
    brightness_param.addBounds(value(0), value(100), value(1));
    brightness_param.addUIType(ESP_RMAKER_UI_SLIDER);
    my_device.addParam(brightness_param);

    // Add the write_callback function to handle parameter updates
    my_device.addCb(write_callback);

    // Add the custom device to the RainMaker node
    my_node.addDevice(my_device);

    // Start the RainMaker service
    RMaker.start();

    // Register the system provisioning event handler
    WiFi.onEvent(sysProvEvent);

    // Begin Wi-Fi provisioning using BLE
    WiFiProv.beginProvision(WIFI_PROV_SCHEME_BLE, WIFI_PROV_SCHEME_HANDLER_FREE_BTDM, WIFI_PROV_SECURITY_1, pop, service_name);
}

// The loop function runs continuously after the setup function has finished executing
void loop()
{
    // Check if the push button is pressed (the pin reads LOW when pressed)
    if (digitalRead(ESP32_C3_PUSH_BTN_GPIO) == LOW) {
        // Key debounce handling
        delay(100);

        // Wait until the button is released
        while(digitalRead(ESP32_C3_PUSH_BTN_GPIO) == LOW) delay(50);

        // Toggle the power state variable (switch between true and false)
        power_state = !power_state;
        Serial.printf("Toggle State to %s.\n", power_state ? "true" : "false");

        // Update the LED color based on the new power state and RGB color values
        if (power_state == true) {
            mySetLedColor(hue_val, saturation_val, brightness_val);
        } else {
            mySetLedColor(0, 0, 0);
        }

        // Report the updated power state to the RainMaker service
        my_device.updateAndReportParam("Power", power_state);
    }

    // Wait for a short period (100 ms) before checking the button state again
    delay(100);
}
