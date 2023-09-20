#include <WiFi.h>
#include <ThingSpeak.h>
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;
WiFiClient  client;

void setup() {
  Serial.begin(115200);
  delay(1000);

  WiFi.mode(WIFI_STA);  // Set ESP32 to station mode

  Serial.println("Scanning Wi-Fi networks...");
  int numNetworks = WiFi.scanNetworks();  // Scan for available networks

  if (numNetworks == 0) {
    Serial.println("No networks found. Please check your Wi-Fi setup.");
    while (1) {
      delay(1000);
    }
  }

  Serial.println("Available networks:");
  for (int i = 0; i < numNetworks; i++) {
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.println(WiFi.SSID(i));  // Print the network name (SSID)
  }

  int selectedNetwork = -1;
  while (selectedNetwork < 0 || selectedNetwork >= numNetworks) {
    Serial.print("Enter the number of the network you want to connect to (1-");
    Serial.print(numNetworks);
    Serial.print("): ");
    while (!Serial.available()) {
      delay(100);
    }
    selectedNetwork = Serial.parseInt();
    Serial.println(selectedNetwork);

    if (selectedNetwork < 1 || selectedNetwork > numNetworks) {
      Serial.println("Invalid network number. Please try again.");
    }
  }

  Serial.print("Selected network: ");
  Serial.println(WiFi.SSID(selectedNetwork - 1));

  String password;
  while (password.length() == 0) {
    Serial.print("Enter the password for the selected network: ");
    while (!Serial.available()) {
      delay(100);
    }
    password = Serial.readString();
    password.trim();

    if (password.length() == 0) {
      Serial.println("Password cannot be empty. Please try again.");
    }
  }

  WiFi.begin(WiFi.SSID(selectedNetwork - 1).c_str(), password.c_str());  // Connect to the selected network

  Serial.print("Connecting to ");
  Serial.println(WiFi.SSID(selectedNetwork - 1));

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Wi-Fi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi connection lost. Reconnecting...");
    WiFi.reconnect();
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("Wi-Fi reconnected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }

  delay(2000);  // Check Wi-Fi connection every 2 seconds
}