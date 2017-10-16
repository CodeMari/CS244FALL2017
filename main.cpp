#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

String deviceName = "CS244";

// WiFi settings
const char *ssid = "UCInet Mobile Access";
 
//Server details
const char* host = "169.234.55.71";
const int httpPort = 8888;
const char* streamId   = "CS244HW1/CSVConn.php";

void printMacAddress()
{
    byte mac[6];
    WiFi.macAddress(mac);

    char MAC_char[18]="";
    for (int i = 0; i < sizeof(mac); ++i)
        sprintf(MAC_char, "%s%02x:", MAC_char, mac[i]);

    Serial.print("Mac address : ");
    Serial.print(MAC_char);

    WiFi.begin(ssid);
    while (WiFi.status() != WL_CONNECTED) delay(500);

    //Print the IP address
    Serial.println("\r\nWiFi connected");
    Serial.println(WiFi.localIP());
}

void sendString(String stringToSend) {

    Serial.print("Connecting to ");
    Serial.println(host);

    //Try connecting to host
    WiFiClient client;
    if(!client.connect(host,httpPort)) {
        Serial.println("Cannot connect");
        return;
    }

    //Create the URL
    String url = "/";
    url += streamId;
    url += "?Transmission="; //This is our parameter name for our PHP file
    url += stringToSend;

    //Request URL
    Serial.println("Requesting URL: " + url);
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                "Host: " + host + "\r\n" +
                "Connection: close\r\n\r\n");
    
    // Read reply from server
    // Helps in debugging e.g. a permissions error
    while(client.available()){
        String line = client.readStringUntil('\r');
        Serial.println(line);
    }


    //Stop client
    client.stop(); 
    Serial.println("");
    Serial.println("Closing connection");

    delay(500);

}

void setup()
{  
    Serial.begin(115200);
    
    printMacAddress();

    sendString("Twaha");
    sendString("Ibrahim");
    sendString("Amari");
    sendString("Lewis");
    sendString("Nora");
    sendString("Alzaid");
}

void loop()
{
  //Nothing needed here
}
