#include <ESP8266WebServer.h>
#include <EEPROM.h>

ESP8266WebServer server(80);    

void handleRoot() {        
  Serial.println("Visitor");                 
  server.send(200, "text/html", "<form action='/submit' method='POST'>"
                               "SSID: <input type='text' name='ssid'><br>"
                               "Password: <input type='password' name='password'><br>"
                               "<input type='submit' value='Submit'></form>");
}

void handleSubmit() {                       
  String ssid = server.arg("ssid");
  String password = server.arg("password");
  ssid.toCharArray(ssidCLI, sizeof(ssidCLI));
  password.toCharArray(passwordCLI, sizeof(passwordCLI));
  EEPROM.put(0, ssidCLI);
  EEPROM.put(EEPROM_SIZE/2, passwordCLI);
  EEPROM.commit();

  WIFI_init(false);
  ap_started=false;
  blink_interval=FAST_BLINK;
  current_state=STATE_FAST_DELAY;
  blink_delay = true;
  Serial.println("WiFi initialized in CLI mode.");
}

void handleNotFound(){
  server.send(404, "text/plain", "404: Not found"); 
}

void server_init() {
  server.on("/", HTTP_GET, handleRoot);     
  server.on("/submit", HTTP_POST, handleSubmit);  
  server.onNotFound(handleNotFound);        

  server.begin();                          
  Serial.println("HTTP server started");    
}