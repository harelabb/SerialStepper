#include <SerialStepper.h>

#include <mcp23017stepper.h>

#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif

#include <Wire.h>          // I²C
#include <PubSubClient.h>  // Mqtt
#include <ArduinoOTA.h>    // Over The Air update

/// @file

/// Specify name of WiFi net.
/// Replace with your own.
constexpr const char* ssid     {"myNetname"};
/// Specify password for WiFi net.
/// Replace with your own.
constexpr const char* password {"myPassword"};

//  Mqtt variables.
/// Define WiFiClient fot Mqtt.
WiFiClient wifi_client;
/// Define Mqtt client
PubSubClient mqtt_client{wifi_client};
/// Specify Mqtt broker.
/// Replace with your own.
constexpr const char* mqtt_server {"myServer"};
/// Select Mqtt topic for input commands.
constexpr const char* mqtt_in {"Stepper/in"};
/// Select Mqtt topic for output echo of commands.
constexpr const char* mqtt_out {"Stepper/out"};
/// Specify Mqtt port.
constexpr       int   mqtt_port {1883};

/// Define the StepperControl.
Mcp23017StepperControl stepper_ctl {0x30};
/// Define number of steppers to use.
constexpr int nsteppers {4};
/// Define the steppers.
Stepper steppers[nsteppers];
/// Define maximum stepper speed.
constexpr float max_speed {15.0f}; // rpm

/// Set speed of a Stepper.
/// @param idx: Stepper number
/// @param speed: target speed
void runStepper(int idx,
                float speed) {
  if (0 <= idx && idx < nsteppers) {
    if (speed == 0) {
      steppers[idx].stop();
      return;
    }
    steppers[idx].direction(speed < 0 ?
                            Stepper::BACKWARD :
                            Stepper::FORWARD);
    const float rpm {std::abs(speed) * max_speed};
    steppers[idx].speed(rpm);
    steppers[idx].start();
  }
}

/// Connect to WiFi.
bool WifiConnect(const char* ssid,
                 const char* password) {
  WiFi.mode(WIFI_STA);
  Serial.print("connecting to ");
  Serial.println(ssid);
  delay(1000);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(500);
    ESP.restart();
  }
  Serial.println("connected.");
  return true;
}

/// Connect to Mqtt.
void mqttConnect() {
  if (!mqtt_client.connected()) {
    Serial.print("MQTT state: ");
    Serial.println(mqtt_client.state());
    static const String client_id {WiFi.macAddress()};
    if (mqtt_client.connect(client_id.c_str())) {
      mqtt_client.subscribe(mqtt_in);
      Serial.println("MQTT connected");
    }
    else {
      Serial.println("MQTT not connected");
      return;
    }
  }
  mqtt_client.loop();
  yield();
}

/// Extract stepper index and speed from payload string.
bool parsePayload(const char* pl,
                  int& idx,
                  float& speed) {
  return sscanf(pl, "{\"idx\":%d, \"speed\":%f}",
                &idx, &speed) == 2;
}

/// PubSubClient callback function.
void mqttCallback(char* /* topic */,
                  uint8_t* pl,
                  unsigned int len) {
  String payload;
  payload.reserve(len+1);
  for (int l = len; l--; payload += static_cast<char>(*pl++)) {}
  int idx;
  float speed;
  if (parsePayload(payload.c_str(), idx, speed)) {
    // Set the stepper speed
    runStepper(idx, speed);
    // Echo the payload back to MQTT
    mqtt_client.publish(mqtt_out, payload.c_str());
  }
}

/// Select Mqtt server and assign callback.
void mqttSetup() {
  mqtt_client.setServer(mqtt_server, mqtt_port);
  mqtt_client.setCallback(mqttCallback);
}


/// This is called once at start-up.
void setup() {
  /// Initialize libraries.
  Serial.begin(115200);
  Wire.begin(0, 2); // ESP-1
  //  Wire.begin();

  /// Initialize the stepper motor controller.
  stepper_ctl.begin();
  for (auto& stepper : steppers) {
    stepper_ctl.addStepper(stepper);
  }

  /// Initialize Wifi and Mqtt.
  WifiConnect(ssid, password);
  mqttSetup();
  /// Prepare for Over The Air sketch update.
  ArduinoOTA.begin(true);
}


/// This is called repeatedly.
void loop() {

  /// Update clock.
  loopClock::tick();

  /// Handle MQTT.
  mqttConnect();

  /// Move the steppers if due time.
  stepper_ctl.run();

  /// Check for OTA updates.
  ArduinoOTA.handle();
  yield();
}
