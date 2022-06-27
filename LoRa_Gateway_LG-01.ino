// Arduino9x_RX
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messaging client (receiver)
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example Arduino9x_TX

#include <Console.h>
#include <SPI.h>
#include <RH_RF95.h>
 
//#define RFM95_CS 10
//#define RFM95_RST 9
//#define RFM95_INT 2
 
// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 433.0
 
// Singleton instance of the radio driver
RH_RF95 rf95;//(RFM95_CS, RFM95_INT);
 
// Blinky on receipt
#define LED A2



// data struct
typedef struct
{
  float   temperature;
  float   humidity;
  float   lng;
  float   lat;
} s_bme_data;
  
s_bme_data radiopacket;
uint8_t datalen = sizeof(radiopacket);
int16_t last_rssi;
bool recv_state = false;
uint16_t counter = 0;

//#define USE_LCD
#define USE_FILE
#define USE_THINGSBOARD

#ifdef USE_LCD
  #include "U8glib.h"
  U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);  // I2C / TWI 

  char *status_text, temp_text[25], s_tmp[15];
#endif

#ifdef USE_FILE
  #include <FileIO.h>
  const char* data_filename = "/mnt/sda1/data/datalog.csv";

  // This function return a string with the time stamp 
  // LG01 will call the Linux "date" command and get the time stamp
  /* from Logger_USB_Flash */
  String getTimeStamp() {
    String result;
    Process time;
    time.begin("date");
    time.addParameter("+%s");
    time.run();  // run the command
  
    // read the output of the command
    while(time.available()>0) {
      char c = time.read();
      if(c != '\n')
        result += c;
    }
    
    return result;
  }
  
  /*Function for data writing*/
  bool write_data(String dataString, const char* file_name)
  {
    File dataFile = FileSystem.open(file_name, FILE_APPEND);
  
    // if the file is available, write to it:
    if (dataFile) {
      dataFile.println(dataString);
      dataFile.close();
      return true;
    } else {
      return false;
    }
  }

#endif

#ifdef USE_THINGSBOARD
  #include <Process.h>
  
  void uploadData(String dataString) {//Upload Data to ThingsBoard
    Console.print("\t\tCall Linux Command to send ");
    Console.print("JSON data: ");
    Console.println(dataString);
    
    Process p;
    p.begin("send_data");
    p.addParameter(dataString);
    p.run();    // Run the process and wait for its termination
    Console.print("ended with " + String(p.exitValue()));
  
    Console.print("\t\tFeedback from Linux: ");
    // If there's output from Linux,
    // send it out the Console:
    while (p.available()>0) 
    {
      char c = p.read();
      Console.write(c);
    }
    Console.println("");
    Console.println("\t\tCall Finished");
  }

#endif

void setup() 
{
  pinMode(LED, OUTPUT);     
//  pinMode(RFM95_RST, OUTPUT);
//  digitalWrite(RFM95_RST, HIGH);
 
  Bridge.begin(115200);
  Console.begin();
//  while (!Console) ; // Wait for console port to be available
  delay(100);

#ifdef USE_FILE  
  FileSystem.begin();
#endif
  Console.println("Arduino LoRa RX Test!");
  
  // manual reset
/*  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);
 */
  while (!rf95.init()) {
    Console.println("LoRa radio init failed");
    while (1);
  }
  Console.println("LoRa radio init OK!");
 
  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Console.println("setFrequency failed");
    while (1);
  }
  Console.print("Set Freq to: "); Console.println(RF95_FREQ);
 
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
 
  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(23, false);

#ifdef USE_LCD
  u8g.setFont(u8g_font_5x7);
  u8g.setFontPosTop();
  status_text = ":)  Receiver started! :)";
#endif
}
 
void loop()
{
#ifdef USE_LCD
  status_text = "...     Receiving     ...";
#endif
//  recv_state = false;

  if (rf95.available())
  {
    // Should be a message for us now   
    //uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    //uint8_t len = sizeof(buf);
    
    if (rf95.recv((uint8_t*)&radiopacket, &datalen))
    {
#ifdef USE_LCD
      status_text = ":)  Receiver got data :) ";
#endif
      recv_state = true;

      digitalWrite(LED, HIGH);
      //RH_RF95::printBuffer("Received: ", radiopacket, datalen);
      Console.print("Count: ");
      Console.println(counter);

      Console.print("Temperature: ");
      Console.print(radiopacket.temperature, 2);
      Console.println(" degrees C");

      Console.print("%RH: ");
      Console.print(radiopacket.humidity, 2);
      Console.println(" %");

      Console.print("GPS lng: ");
      Console.println(radiopacket.lng, 2);

      Console.print("GPS lat: ");
      Console.println(radiopacket.lat, 2);
      
      //Console.println((char*)radiopacket);
      Console.print("RSSI: ");
      last_rssi = rf95.lastRssi();
      Console.println(last_rssi, DEC);
      
      // Send a reply
      /*uint8_t data[] = "And hello back to you";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent();
      Console.println("Sent a reply");*/
      digitalWrite(LED, LOW);

#ifdef USE_FILE
  String timestamp = getTimeStamp();
  
  Console.println("- writing file -");
  String dataString;
  dataString += "{";
  dataString += "\"temperature\":";
  dataString += String(radiopacket.temperature);
  dataString += ", ";
  dataString += "\"humidity\":";
  dataString += String(radiopacket.humidity);
  dataString += ", ";
  dataString += "\"lng\":";
  dataString += String(radiopacket.lng);
  dataString += ", ";  
  dataString += "\"lat\":";
  dataString += String(radiopacket.lng);
  dataString += "}";
  
  write_data(dataString, data_filename);
#endif

#ifdef USE_THINGSBOARD
  String space = "%20";
  Console.println("- uploading to ThingsBoard -");
  uploadData(dataString);
#endif

      Console.println("---");

      counter++;
    }
    else
    {
      Console.println("Receive failed");
      recv_state = false;
    }
  }

#ifdef USE_LCD
    // U8GLIB picture loop
    u8g.firstPage();  
    do {
        u8g.drawStr(0, 0, status_text);
        if (recv_state) {
          sprintf(temp_text, "N: %8d   RSSI: %6s", counter, dtostrf(last_rssi, 5, 0, s_tmp));
          u8g.drawStr(0, 15, temp_text);

          sprintf(temp_text, "Temperature: %10s C", dtostrf(radiopacket.temperature, 5, 2, s_tmp));
          u8g.drawStr(0, 25, temp_text);

          sprintf(temp_text, "Humidity: %13s %%", dtostrf(radiopacket.humidity, 8, 2, s_tmp));
          u8g.drawStr(0, 55, temp_text);
        }
    } while( u8g.nextPage() );
#endif

}
