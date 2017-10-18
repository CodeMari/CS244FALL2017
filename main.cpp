/* #include "Arduino.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "MAX30105.h"

String deviceName = "CS244";

// WiFi settings
const char *ssid = "UCInet Mobile Access";
 
//Server details
const char* host = "169.234.47.57";
const int httpPort = 8888;
const char* streamId   = "CS244HW2/Assignment2.php";


static const uint8_t MAX30105_LED1_PULSEAMP = 	0x0C;
static const uint8_t MAX30105_LED2_PULSEAMP = 	0x0D;

//Sensor details
MAX30105 particleSensor;
byte ledBrightness = 0xFF; //0 = Off, 0x02 = 0.4mA, 0x1F = 6.4mA (default), , 255 = 50mA
byte sampleRate = 50;

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

void sendData(int IR, int RED) {

    //Try connecting to host
    WiFiClient client;
    if(!client.connect(host,httpPort)) {
        Serial.println("Cannot connect");
        return;
    }

    //Create the URL
    String url = "/";
    url += streamId;
    url += "?IR=" + String(IR);
    url += "&RED=" + String(RED);

    //Request URL
    Serial.println("Requesting URL: " + url);
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
  
    particleSensor.setup(); //Configure sensor. Use default of 6.4mA for LED drive
}

byte setSensorCurrent() {
    // return 0; //Off
    //TODO: set current values here
    return 0xFF; //255 = 50mA
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

  Serial.print("IR[");
  Serial.print(IR);
  Serial.print("] RED[");
  Serial.print(RED);
  Serial.print("]");
  Serial.println();

  sendData(IR,RED);
  delay(500);

  Serial.println();

} */


#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "MAX30105.h"

String deviceName = "CS244";

// WiFi settings
const char *ssid = "UCInet Mobile Access";
 
//Server details
const char* host = "169.234.47.57";
const int httpPort = 8888;
const char* streamId   = "CS244HW2/Assignment2.php";

static const uint8_t MAX30105_LED1_PULSEAMP = 	0x0C;
static const uint8_t MAX30105_LED2_PULSEAMP = 	0x0D;

//Sensor details
MAX30105 particleSensor;

const byte powerLevel = 0x1F; //6.4mA
//Pwer level settings: 0x02 = 0.4mA, 0x1F = 6.4mA (default), 0x7F = 25.4mA, 0xFF = 50mA
const byte powerLevel1 = 0x02; //0.4mA 
const byte powerLevel2 = 0x1F; //6.4mA
const byte powerLevel3 = 0x7F; //25.4mA
const byte powerLevel4 = 0xFF; //50mA 
//No need to change these
//but coding them in incase we want to mess around
const int sampleRate = 50;
const byte sampleAverage = 4;
const byte ledMode = 3;
const int pulseWidth = 411;
const int adcRange = 4096;

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

void sendData(int IR1, int RED1, int IR2, int RED2,int IR3, int RED3,int IR4, int RED4 ) {

    //Try connecting to host
    WiFiClient client;
    if(!client.connect(host,httpPort)) {
        Serial.println("Cannot connect");
        return;
    }

    //Create the URL
    String url = "/";
    url += streamId;
    url += "?IR1=" + String(IR1);
    url += "&RED1=" + String(RED1);
    url += "&IR2=" + String(IR2);
    url += "&RED2=" + String(RED2);
    url += "&IR3=" + String(IR3);
    url += "&RED3=" + String(RED3);
    url += "&IR4=" + String(IR4);
    url += "&RED4=" + String(RED4); 

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
    particleSensor.setup(powerLevel, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); 
}


void setup()
{  
    Serial.begin(115200);
    
    printMacAddress();
    setupSensorBoard();

}

void loop()
{
    int RED1, RED2, RED3, RED4;
    int IR1, IR2, IR3, IR4;

    particleSensor.setup(powerLevel1, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange);
    
    RED1 = particleSensor.getRed();
    IR1 = particleSensor.getIR();

    particleSensor.setup(powerLevel2, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); 
    
  
    RED2 = particleSensor.getRed();
    IR2 = particleSensor.getIR();

    particleSensor.setup(powerLevel3, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); 
    
  
    RED3 = particleSensor.getRed();
    IR3 = particleSensor.getIR();

    particleSensor.setup(powerLevel4, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); 
    
    RED4 = particleSensor.getRed();
    IR4 = particleSensor.getIR();

    Serial.print("IR1[");
    Serial.print(IR1);
    Serial.print("] RED1[");
    Serial.print(RED1);
    Serial.print("] ");
    Serial.print("IR2[");
    Serial.print(IR2);
    Serial.print("] RED2[");
    Serial.print(RED2);
    Serial.print("] ");
    Serial.print("IR3[");
    Serial.print(IR3);
    Serial.print("] RED3[");
    Serial.print(RED3);
    Serial.print("] ");
    Serial.print("IR4[");
    Serial.print(IR4);
    Serial.print("] RED4[");
    Serial.print(RED4);
    Serial.print("] ");
    Serial.println();
  
    sendData(IR1,RED1, IR2,RED2,IR3,RED3,IR4,RED4);


}