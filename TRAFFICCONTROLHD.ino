#include <WiFiNINA.h>

char ssid[] = "Roji";     // your network SSID (name)
char pass[] = "12345678"; // your network password
int status = WL_IDLE_STATUS;

WiFiServer server(80);

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Attempt to connect to WiFi network
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(5000); // Wait 5 seconds for connection
  }

  // If successfully connected to WiFi, start the server
  server.begin();
  Serial.println("Server started");

  // Set pin modes for LEDs
  pinMode(LED_BUILTIN, OUTPUT); // Onboard LED
  pinMode(10, OUTPUT);           // Red LED
  pinMode(9, OUTPUT);           // Green LED
  pinMode(8, OUTPUT);           // Blue LED
}

void loop() {
  // Check for client connections
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New client");
    String currentLine = "";

    // Wait for client to send request
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        currentLine += c;

        // Check if end of HTTP request is reached
        if (c == '\n') {
          // If request is GET /red/on, turn on red LED
          if (currentLine.indexOf("/red/on") != -1) {
            digitalWrite(10, HIGH);
          }
          // If request is GET /red/off, turn off red LED
          else if (currentLine.indexOf("/red/off") != -1) {
            digitalWrite(10, LOW);
          }
 // Similar handling for green and blue LEDs
          else if (currentLine.indexOf("/green/on") != -1) {
            digitalWrite(9, HIGH);
          }
          else if (currentLine.indexOf("/green/off") != -1) {
            digitalWrite(9, LOW);
          }
          else if (currentLine.indexOf("/blue/on") != -1) {
            digitalWrite(8, HIGH);
          }
          else if (currentLine.indexOf("/blue/off") != -1) {
            digitalWrite(8, LOW);
          }
          // Respond to the client with a 200 OK status
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println();
          break; // Exit the loop
        }
      }
    }
    // Close the connection
    delay(1);
    client.stop();
    Serial.println("Client disconnected");
  }
}


