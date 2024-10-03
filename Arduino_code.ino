#include <SPI.h>
#include <WiFiNINA.h>

char ssid[] = "";           // your network SSID (name)
char pass[] = "";  // your network password
int status = WL_IDLE_STATUS;
WiFiServer server(80);

// Define stepper motor control pins
const int stepPin = 3;  // Pin for DRV8825 Step
const int dirPin = 2;   // Pin for DRV8825 Direction

// Variables for stepper control
int stepDelay = 1000;          // Delay between steps (in microseconds)
int stepsPerRevolution = 180;  // Assuming a standard NEMA 17 with 1.8° per step and 1/16 microstepping
int currentPosition = 0;       // Current position in steps
int targetPosition = 0;        // Target position in steps

void setup() {
  Serial.begin(9600);

  // Set up the stepper motor pins
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

  // Check for the WiFi module
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true);
  }

  // Attempt to connect to WiFi
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);  // Connect to WiFi
    delay(10000);
  }

  server.begin();     // Start the web server
  printWifiStatus();  // Print WiFi status to Serial Monitor
}

void loop() {
  WiFiClient client = server.available();  // Listen for incoming clients

  if (client) {
    String currentLine = "";  // To store incoming data from the client
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '\n') {
          if (currentLine.length() == 0) {
            // Send an HTTP response with the web page
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // HTML + CSS + JS for the knob
            client.print("<html><head><title>Stepper Control</title>");
            client.print("<style>");
            client.print("body {font-family: Arial; text-align: center; margin-top: 50px;}");
            client.print("#knob {width: 200px; height: 200px; margin: 0 auto;}");
            client.print("</style>");
            client.print("<script src='https://cdn.jsdelivr.net/npm/jquery@3.6.0/dist/jquery.min.js'></script>");
            client.print("<script src='https://cdn.jsdelivr.net/npm/jquery-knob/js/jquery.knob.min.js'></script>");
            client.print("<script>");
            client.print("$(function() {");
            client.print("$('#knob').knob({");
            client.print("'min': 0,");
            client.print("'max': 180,");
            client.print("'angleArc': 180,");     // This creates the semicircular knob
            client.print("'angleOffset': -90,");  // Starts the knob from the left
            client.print("'change': function(value) {");
            client.print("var xhttp = new XMLHttpRequest();");
            client.print("xhttp.open('GET', '/setPosition?value=' + value, true);");
            client.print("xhttp.send();");
            client.print("document.getElementById('position').innerHTML = value;");  // Update the position in UI
            client.print("}");
            client.print("});");
            client.print("});");
            client.print("</script>");
            client.print("</head><body>");
            client.print("<h2>Control Stepper Motor</h2>");
            client.print("<input id='knob' type='text' value='");
            client.print(map(currentPosition, 0, stepsPerRevolution, 0, 180));  // Map steps to angle
            client.print("' data-width='200' data-height='200' data-fgColor='#ffec03' data-bgColor='#eeeeee' data-thickness='.3'>");
            client.print("<p>Current Position: <span id='position'>");
            client.print(map(currentPosition, 0, stepsPerRevolution, 0, 180));  // Display position in angle
            client.print("</span>°</p>");
            client.print("</body></html>");

            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }

        // Parse the request to extract the target position value
        if (currentLine.startsWith("GET /setPosition?value=")) {
          int posStart = currentLine.indexOf('=') + 1;
          int posEnd = currentLine.indexOf(' ', posStart);
          String posString = currentLine.substring(posStart, posEnd);
          int angle = posString.toInt();                               // Convert the string to an integer angle
          targetPosition = map(angle, 0, 180, 0, stepsPerRevolution);  // Map angle to steps
          
          if (targetPosition != currentPosition) {
            moveStepper();                                               // Move the stepper to the new position
          }

          Serial.print("Stepper position set to: ");
          Serial.println(targetPosition);
        }
      }
    }
   client.stop();  // Close the connection
  Serial.println("Client disconnected");
  }
  
}

// Function to move the stepper motor to the target position
void moveStepper() {
  int stepsToMove = targetPosition - currentPosition;  // Calculate the steps needed

  if (stepsToMove != 0) {
    digitalWrite(dirPin, stepsToMove > 0 ? HIGH : LOW);  // Set direction

    stepsToMove = abs(stepsToMove);
    for (int i = 0; i < stepsToMove; i++) {
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(stepDelay);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(stepDelay);
    }

    // Update the current position based on actual steps moved
    currentPosition += (stepsToMove * (digitalRead(dirPin) == HIGH ? 1 : -1));
    
    Serial.print("Current Position updated to: ");
    Serial.println(currentPosition);
  }
}

void printWifiStatus() {
  // Print the SSID of the network
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // Print the IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // Print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI): ");
  Serial.print(rssi);
  Serial.println(" dBm");

  // Print where to go in a browser
  Serial.print("To control the stepper, open a browser to http://");
  Serial.println(ip);
}
