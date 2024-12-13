
  #include <Arduino.h>
  #include <ESP8266WiFi.h>
  #include "HTTPSRedirect.h"
 
  // Enter network credentials:
 
  const char* ssid = "Redmi 13C";
  const char* password = "12345679";   
 
  // Enter Google Script Deployment ID:
 
  const char *GScriptId = "AKfycbyS3y7XV4kiKWC-5B50mCvvFe7EkRFqNILm1TV6z-4hduoEH8A6F6n247gIprZiRB6f";
 
  // Enter command (insert_row or append_row) and your Google Sheets sheet name (default is Sheet1):
 
  String payload_base = "{\"command\": \"insert_row\", \"sheet_name\": \"Sheet1\", \"values\": ";
  String payload = "";
 
  // Google Sheets setup (do not edit)
 
  const char* host = "script.google.com";
  const int httpsPort = 443;
  const char* fingerprint = "";
  String url = String("/macros/s/") + GScriptId + "/exec";
  HTTPSRedirect* client = nullptr;
 
  // Declare variables that will be published to Google Sheets
 
  int esteira0 = 0;
  int esteira1 = 0;
  int esteira2 = 0;
 
void setup() {
  Serial.begin(9600);
  delay(10);
  Serial.println('\n');
 
  // Connect to WiFi
 
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.print(ssid);
  Serial.println(" ...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println('\n');
  Serial.println("Connection established!");
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());
 
  // Use HTTPSRedirect class to create a new TLS connection client = new HTTPSRedirect(httpsPort);
  client = new HTTPSRedirect(httpsPort);
  client->setInsecure();
  client->setPrintResponseBody (true);
  client->setContentTypeHeader("application/json");
 
  Serial.print("Connecting to ");
  Serial.println(host);
 
  // Try to connect for a maximum of 5 times
 
  bool flag = false;
  for (int i=0; i<5; i++){
    int retval = client->connect (host, httpsPort);
    if (retval == 1){
      flag = true;
      Serial.println("Connected");
      break;
  }
  else
    Serial.println("Connection failed. Retrying...");
  }
  if (!flag) {
    Serial.print("Could not connect to server: ");
    Serial.println(host); return;
  }
  delete client; // delete HTTPSRedirect object
  client = nullptr; // delete HTTPSRedirect object
  }
 
void loop() {
 
  // create some fake data to publish
 
  esteira0 ++;
  esteira1 = random (0,1000);
  esteira2 = random(0,100000);
 
  static bool flag = false;
  if (!flag){
    client = new HTTPSRedirect (httpsPort);
    client->setInsecure();
    flag = true;
    client->setPrintResponseBody (true);
    client->setContentTypeHeader("application/json");
}
  if (client != nullptr) {
    if (!client->connected()){
      client->connect (host, httpsPort);
      client->connect(host, httpsPort);
    }
  }
  else{
  Serial.println("Error creating client object!");
  }
 
  // Create json object string to send to Google Sheets
 
  payload = payload_base + "\"" + esteira0 + "," + esteira1 + "," + esteira2 + "\"}";
 
  // Publish data to Google Sheets
  Serial.println("Publishing data...");
  Serial.println(payload);
  if(client->POST(url, host, payload)) {
 
  // do stuff here if publish was successful
 
  }
  else{
 
  // do stuff here if publish was not successful
 
  Serial.println("Error while connecting");
  }
 
  // a delay of several seconds is required before publishing again
 
  delay(5000);
  }
 