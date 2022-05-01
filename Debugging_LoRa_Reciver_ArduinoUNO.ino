// BME280_LoRa_Receiver
 
#include <SPI.h>
#include <RH_RF95.h>
 
#define RFM95_RST 9
#define RF95_FREQ 433.0
 
RH_RF95 rf95;
 


// data struct
typedef struct
{
  float   temperature;
  float   humidity;
} s_bme_data;
  
s_bme_data radiopacket;
uint8_t datalen = sizeof(radiopacket);
int16_t last_rssi;
bool recv_state = false;
uint16_t counter = 0;

#define LED 13



void setup() 
{
  pinMode(LED, OUTPUT);     
  pinMode(RFM95_RST, OUTPUT);

  
  digitalWrite(RFM95_RST, HIGH);
 
  while (!Serial);
  Serial.begin(9600);
  delay(100);
 
  Serial.println("BME280_LoRa_Receiver started!");
  
  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);
 
  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }
  Serial.println("LoRa radio init OK!");
 
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
 
  rf95.setTxPower(23, false);

}
 
void loop()
{


  if (rf95.available())
  {
    if (rf95.recv((uint8_t*)&radiopacket, &datalen))
    {

      recv_state = true;

      digitalWrite(LED, HIGH);
      //RH_RF95::printBuffer("Received: ", radiopacket, datalen);
      Serial.print("Count: ");
      Serial.println(counter);

      Serial.print("Temperature: ");
      Serial.print(radiopacket.temperature, 2);
      Serial.println(" degrees C");

      Serial.print("%RH: ");
      Serial.print(radiopacket.humidity, 2);
      Serial.println(" %");
      
      //Serial.println((char*)radiopacket);
      Serial.print("RSSI: ");
      last_rssi = rf95.lastRssi();
      Serial.println(last_rssi, DEC);
      
      digitalWrite(LED, LOW);

      Serial.println("---");

      counter++;
    }
    else
    {
      Serial.println("Receive failed");
      recv_state = false;
    }
  }



}
