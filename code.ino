// Include the necessary libraries for the ESP RainMaker, Wi-Fi and Wi-Fi Provisioning
#include "RMaker.h"
#include "WiFi.h"
#include "WiFiProv.h"

// Define the GPIO pin number for the push button
#define ESP32_C3_PUSH_BTN_GPIO 9

// Define provisioning service name and proof of possession (pop) code
const char *service_name = "PROV_3448"; // Replace with a random 4-digit code
const char *pop = "abcd1234";

// Declare a variable to store the power state
bool power_state = true;

// The framework provides some standard device types like switch, lightbulb, fan, temperature sensor.
// But, you can also define custom devices using the 'Device' base class object, as shown here
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

// Callback function for handling changes in parameter values
void write_callback(Device *device, Param *param, const param_val_t val, void *priv_data, write_ctx_t *ctx)
{
    const char *device_name = device->getDeviceName();
    const char *param_name = param->getParamName();

    if(strcmp(param_name, "Power") == 0) {
        Serial.printf("Received value = %s for %s - %s\n", val.val.b? "true" : "false", device_name, param_name);
        power_state = val.val.b;
        param->updateAndReport(val);
    }
}

// The setup function runs once when the Arduino board is powered on or reset
void setup()
{
    // Initialize the serial communication with a baud rate of 115200
    Serial.begin(115200);
    pinMode(ESP32_C3_PUSH_BTN_GPIO, INPUT);

    // Create and initialize the RainMaker node
    Node my_node;
    my_node = RMaker.initNode("ESP RainMaker Node");

    // Create a custom switch device
    my_device = Device("Switch", "esp.device.lightbulb", &power_state);

    // Add a name parameter for the custom device
    my_device.addNameParam();

    // Create and add a custom power parameter
    Param power_param("Power", "esp.param.power", value(power_state), PROP_FLAG_READ | PROP_FLAG_WRITE);

    // Add UI Widget Type in Phone app
    power_param.addUIType(ESP_RMAKER_UI_TOGGLE);

    // Add parameter to the device
    my_device.addParam(power_param);

    // Assign the primary parameter for the device
    my_device.assignPrimaryParam(my_device.getParamByName("Power"));

    // Set the write callback function for the device
    my_device.addCb(write_callback);

    // Add the custom device to the node
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
        // Key debounce handling: wait for 100 ms to avoid false triggering due to button bouncing
        delay(100);

        // Wait until the button is released (the pin reads HIGH when released)
        while (digitalRead(ESP32_C3_PUSH_BTN_GPIO) == LOW) {
            delay(50);
        }

        // Toggle the power state variable (switch between true and false)
        power_state = !power_state;

        // Print the new power state to the serial monitor
        Serial.printf("Toggle State to %s.\n", power_state ? "true" : "false");

        // Update and report the new power state to the RainMaker server
        my_device.updateAndReportParam("Power", power_state);
    }

    // Wait for a short period (100 ms) before checking the button state again
    delay(100);
}
