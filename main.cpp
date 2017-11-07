#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <string>
#include <sstream>
#include "SparkFunLIS3DH.h"
#include "Wire.h"
#include "SPI.h" 

String deviceName = "CS244";

// WiFi settings
const char *ssid = "UCInet Mobile Access";
 
//Server details
const char* host = "169.234.6.224";
const int httpPort = 8888;
const char* streamId   = "CS244HW4/Assignment4.php";

const int SIZE_OF_DATA = 8;
float X[200]= {};
float Y[200]= {};
float Z[200]= {};

int i=0; 

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

String convertArrayToString(float Data[], int size = SIZE_OF_DATA) {
    
        String arrayString = "[";
    
        for(int i = 0; i < size - 1; i++) {
            arrayString += String(Data[i]) + ",";
        }
    
        arrayString += String(Data[size-1]) + "]";
    
        return arrayString;
    
    }

    // std::string Convert (float number){
    //     std::ostringstream buff;
    //     buff<<number;
    //     return buff.str();   
    // }
    
    void sendArrayToServer(float X[], float Y[], float Z[], int size = SIZE_OF_DATA) {
    
        //Try connecting to host
        WiFiClient client;
        if(!client.connect(host,httpPort)) {
            Serial.println("Cannot connect");
            return;
        } 
        //Create the URL
       String url = "/";
        url += streamId;
        url += "?X=" + convertArrayToString(X,SIZE_OF_DATA);
        url += "&Y=" + convertArrayToString(Y,SIZE_OF_DATA);
        url += "&Z=" + convertArrayToString(Z,SIZE_OF_DATA);
    //char *cstr = url.c_str();

        //Request URL
        client.print(String("GET ") + url.c_str() + " HTTP/1.1\r\n" +
                    "Host: " + host + "\r\n" +
                    "Connection: close\r\n\r\n");
    
        //Stop client
        client.stop(); 
    
    }

void sendString(String stringToSend) 
{

    Serial.print("Connecting to ");
    Serial.println(host);

    //Try connecting to host
    WiFiClient client;
    if(!client.connect(host,httpPort)) {
        Serial.println("Cannot connect");
        return;
    }
        

}
LIS3DH myIMU;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(1000); //relax...
  Serial.println("Processor came out of reset.\n");
  myIMU.settings.adcEnabled = 1;
  myIMU.settings.tempEnabled = 0;
  myIMU.settings.accelSampleRate = 50;  //Hz.  Can be: 0,1,10,25,50,100,200,400,1600,5000 Hz
  myIMU.settings.accelRange = 2;      //Max G force readable.  Can be: 2, 4, 8, 16
  myIMU.begin();


}
  
void loop()
{
  
    X[i] = myIMU.readFloatAccelX();
    Y[i]= myIMU.readFloatAccelY();
    Z[i]= myIMU.readFloatAccelZ();

  //Get all parameters
  Serial.print("\nAccelerometer:\n");
  Serial.print(" X = ");
  Serial.println(X[i], 4);
  Serial.print(" Y = ");
  Serial.println(Y[i], 4);
  Serial.print(" Z = ");
  Serial.println(Z[i], 4);
i++;
if (i == SIZE_OF_DATA)
{
    sendArrayToServer(X,Y,Z,SIZE_OF_DATA);
    i=0;
}
 
 
  delay(1000);
}
