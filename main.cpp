#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "MAX30105.h"

String deviceName = "CS244";

// WiFi settings
const char *ssid = "UCInet Mobile Access";
 
//Server details
const char* host = "169.234.54.225";
const int httpPort = 8888;
const char* streamId   = "CS244HW2/Assignment2.php";

//Sensor details
MAX30105 particleSensor;
const byte powerLevel = 0x1F; //0x02 = 0.4mA, 0x1F = 6.4mA (default), 0x7F = 25.4mA, 0xFF = 50mA
//const byte powerLevel1 = 0x02; //0.4mA 
//const byte powerLevel2 = 0x1F; //6.4mA
//const byte powerLevel3 = 0x7F; //25.4mA
//const byte powerLevel4 = 0xFF; //50mA 

//No need to change these
//but coding them in incase we want to mess around
const int sampleRate = 50;
const byte sampleAverage = 4;
const byte ledMode = 3;
const int pulseWidth = 411;
const int adcRange = 4096;

#const int SIZE_OF_DATA = 200;
#int RED_ARRAY[SIZE_OF_DATA];
#int IR_ARRAY[SIZE_OF_DATA];
#int array_location = 0;

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

#String convertArrayToString(int Data[], int size = SIZE_OF_DATA) {

 #   String arrayString = "[";

  #  for(int i = 0; i < size - 1; i++) {
   #     arrayString += String(Data[i]) + ",";
    }

    # arrayString += String(Data[size-1]) + "]";

    # return arrayString;

}

void sendArrayToServer(int IR[], int RED[], int size = SIZE_OF_DATA) {

    //Try connecting to host
    WiFiClient client;
    if(!client.connect(host,httpPort)) {
        Serial.println("Cannot connect");
        return;
    }

    //Create the URL
    String url = "/";
    url += streamId;
    url += "?IR=" + convertArrayToString(IR);
    url += "&RED=" + convertArrayToString(RED);

    //Request URL
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                "Host: " + host + "\r\n" +
                "Connection: close\r\n\r\n");

    //Stop client
    client.stop(); 

}

void setupSensorBoard() {

    Serial.println("Initializing sensor board...");
  
    // Initialize sensor
    while (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
    {
      Serial.println("MAX30105 was not found. Please check wiring/power. ");
      delay(500);
    }
  
    //Configure sensor.
    particleSensor.setup(powerLevel1, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); 
}

void setup()
{  
    Serial.begin(115200);
    
    printMacAddress();
    setupSensorBoard();

}

void loop()
{
  int RED = particleSensor.getRed();
  int IR = particleSensor.getIR();

#  RED_ARRAY[array_location] = RED;
#  IR_ARRAY[array_location] = IR;
 # array_location++;

 # if(array_location == SIZE_OF_DATA) {
  #    Serial.println("Sending off data...");
     # array_location = 0;
   #   sendArrayToServer(IR_ARRAY, RED_ARRAY);
    #  Serial.println("Done sending.");
  }

}

