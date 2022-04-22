#include <Adafruit_SHT31.h>
#include <SPI.h>
#include <RH_RF95.h>
#include <Wire.h>
#include <stdint.h>

#define RFM95_RST 8       // define number of reset pin

#define RF95_FREQ 433.0  // define channel freq const = 433 MHz

RH_RF95 LoRa;
Adafruit_SHT31 sht31;
bool status;

//--------Define stucture of datapacket ------------
typedef struct
{
  float   temperature;      // SHT31 get temp and hum
  float   humidity;
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
  delay(100);
//--------------------------------------------
  Serial.println("Transmit is started!");

//----------Reset transmission config------------------
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);
//-----------------------------------------------------

//----------SX1278 initialization check---------------
  while (!LoRa.init()) {
    Serial.println("SX1278 not detected. Check wire and try again!");
    while (1);
  }
  Serial.println("SX1278 is detected!");
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
      Serial.println("SHT31 not detected. Check wire!");
  } else {
      Serial.print("Starting SHT31... result of .begin(): 0x");
      Serial.println(status, HEX);
  }
}
//------------------------------------------------------------------------------
void loop()
{
//--------------Get SHT31 data--------------------
  float temperature = sht31.readTemperature();
  Serial.print("Temperature: ");
  Serial.print(temperature, 2);
  Serial.println(" degrees C");

  float humidity = sht31.readHumidity();
  Serial.print("%RH: ");
  Serial.print(humidity, 2);
  Serial.println(" %");

//----------------DataPack-------------------
  LoRaPack.temperature = temperature;
  LoRaPack.humidity = humidity;
 
  Serial.println("Sending "); 
    Serial.print((uint8_t) temperature, HEX);
  
  Serial.println("Sending..."); delay(10);
  LoRa.send((uint8_t *)&LoRaPack, sizeof(LoRaPack));

  Serial.println("Waiting for packet to complete..."); delay(10);
  LoRa.waitPacketSent();

 
  delay(3000);
}
