#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>

ESP8266WiFiMulti wifiMulti;    
WiFiClient wifiClient;
WiFiClient httpClient;

String ip = "(IP unset)"; 

String id(){
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);
  String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) + String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
  macID.toUpperCase();
  return macID;
}

bool StartAPMode() {
  IPAddress apIP(192, 168, 4, 1);
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP((ssidAP + " " + id()).c_str(), passwordAP.c_str());
  Serial.println("");
  Serial.println("WiFi up in AP mode with name: "+ ssidAP + " " + id());
  return true;
}

bool StartCLIMode() {
  Serial.println("Try WiFi in CLIENT mode ");
  WiFi.softAPdisconnect(true);
  wifiMulti.addAP(ssidCLI, passwordCLI);
  //it is possible to add more networks to connect
  long int timeerror=millis();
  while(wifiMulti.run() != WL_CONNECTED) {
    if (millis()>timeerror+10000){
      return false;
    }
  }
  Serial.println("WiFi in CLIENT mode ");
  return true;
}

void WIFI_init(bool mode_ap) {
    if (mode_ap) {
      StartAPMode();
      ip = WiFi.softAPIP().toString();
    } else {
      StartCLIMode();
      ip = WiFi.localIP().toString();
    }
    Serial.println("IP address: ");
    Serial.println(ip);  
}