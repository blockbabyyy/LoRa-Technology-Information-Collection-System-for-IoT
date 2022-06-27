
#include <TinyGPS++.h> // include GPS library for NEO-6M
#include <Adafruit_SHT31.h> //include library for SHT31
#include <SPI.h>  // include library for SPI interface
#include <RH_RF95.h> // include library for SX1276 LoRa
#include <Wire.h> // include library for I2C interface
#include <stdint.h>
#include <SoftwareSerial.h>

#define RFM95_RST 8       // define number of reset pin
#define RF95_FREQ 433.0  // define channel freq const = 433 MHz

static const int RXPin = 3, TXPin = 4;
static const uint32_t GPSBaud = 9600;

//Battery_Shield battery;
TinyGPSPlus gps;
RH_RF95 LoRa;
Adafruit_SHT31 sht31;
bool status;

SoftwareSerial ss(RXPin,TXPin);

//--------Define stucture of datapacket ------------
typedef struct
{
  float   temperature;      // SHT31 get temp and hum
  float   humidity;
  float   longitude;
  float   latitude;
  //  float   charge;
  int satellit;
} send_sht31_data;

send_sht31_data LoRaPack;
//--------------------------------------------

void setup()
{
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
  //-------- Serial Out Rate Config--------------
  while (!Serial);
  Serial.begin(9600);
  ss.begin(9600);
 
  //--------------------------------------------
  Serial.println("Service info:");
  Serial.println("");
  Serial.println("Transmit is started!");

  //----------Reset transmission config------------------
  digitalWrite(RFM95_RST, LOW);
  smartDelay(0);
  digitalWrite(RFM95_RST, HIGH);
  smartDelay(0);
  //-----------------------------------------------------

  //----------SX1278 initialization check---------------
  while (!LoRa.init()) {
    Serial.println("LoRa status: Not detected"); 
    while (1);
  }
  Serial.println("LoRa satus: OK");
  //----------------------------------------------------
  //----------------Set freq check-----------------------
  if (!LoRa.setFrequency(RF95_FREQ)) {
    Serial.println("Set Frequency failed.");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
  LoRa.setTxPower(5, false);
  //---------------------------------------------------------

  //----------------------SHT31 sensor init check----------------------------------
  status = sht31.begin();
  if (!status) {
    Serial.println("SHT31: Not detected");
  } else {
    Serial.println("SHT31: OK");
  }
}
//------------------------------------------------------------------------------
void loop()
{
  Serial.println("----------------------------------");
  
  //--------------Get SHT31 data--------------------
  float temperature = sht31.readTemperature();
  Serial.print("Temperature: ");
  Serial.print(temperature, 2);
  Serial.println(" degrees C");

  float humidity = sht31.readHumidity();
  Serial.print("%RH: ");
  Serial.print(humidity, 2);
  Serial.println(" %");
  int satellit = gps.satellites.value();
  float longitude = gps.location.lng();
  float latitude = gps.location.lat();
  Serial.print("Latitude / longitude:");
  Serial.print(latitude, 6);
  Serial.print(" / ");
  Serial.println(longitude, 6);
  Serial.println("");
  
 
 //---------------------------------------------------------
 /*Serial.println("GPS debugg information:"); 
 Serial.print("CharsProcessed:"); Serial.println(gps.charsProcessed());*/
  if (millis() > 5000 & gps.charsProcessed() < 10){
    Serial.println(F("GPS was not detected: check connection"));
    Serial.println("GPS stream dump:");
      while (true) // infinite loop
        if (ss.available() > 0) // any data coming in?
          Serial.write(ss.read());}
 
   /* Serial.print("Satellites:"); Serial.println(gps.satellites.value());
    Serial.print("Sentences that failed checksum=");
    Serial.println(gps.failedChecksum());
 
    // Testing overflow in SoftwareSerial is sometimes useful too.
    Serial.print("Soft Serial device overflowed? ");
    Serial.println(ss.overflow() ? "YES!" : "No");*/
    
    

  //----------------DataPack-------------------
  LoRaPack.temperature = temperature;
  LoRaPack.humidity = humidity;
  LoRaPack.longitude = longitude;
  LoRaPack.latitude = latitude;
 // LoRaPack.satellit = satellit;

  Serial.println("Sending..."); smartDelay(100);
  LoRa.send((uint8_t *)&LoRaPack, sizeof(LoRaPack));

  Serial.println("Waiting for sending complete..."); smartDelay(100);
  LoRa.waitPacketSent();
  Serial.println("----------------------------------");
  Serial.println("");

  smartDelay(10000);
}
static void smartDelay(unsigned long ms)  //Custom delay that guarantees the availability of GPS data
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}
