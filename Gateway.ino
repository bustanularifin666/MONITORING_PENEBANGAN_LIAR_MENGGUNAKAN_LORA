#include <RHGenericDriver.h>
#include <RHReliableDatagram.h>
#include <RH_RF69.h>
#include <RH_RF95.h>
#include <SPI.h>
#include <Console.h>
#include <Process.h>

int led = A2;
int i, counter, counterr, counterr2;
String url;
String dataurl = "";
String tx, gpsla, gpslo, suara, acx, acy, acz, adxl;
#define BAUDRATE 115200

// Our RF95 module 3 Configuration 
#define RF95_2_NODE_ID    2
#define RF95_2_FREQUENCY  915.00

// Our RFM69 module 3 Configuration
#define RF95_3_NODE_ID    3
#define RF95_3_FREQUENCY  915.00

RH_RF95 rf95_2;
RH_RF95 rf95_3;

String dataString = "";

uint8_t buf;
uint8_t len;

String data;
char x[60];


void setup() 
{
  counterr = 0;
  counterr2 = 0;
  Bridge.begin(BAUDRATE);
  while (!Console) ; // Wait for console port to be available
  pinMode(led, OUTPUT);     
  Console.begin();
  Console.println("Start Sketch");

  if (!rf95_2.init())
    Console.println("init failed node 2");

   if (!rf95_3.init())
    Console.println("init failed node 3");
    
  // Setup ISM frequency
  rf95_2.setFrequency(RF95_2_FREQUENCY);
  rf95_3.setFrequency(RF95_3_FREQUENCY);

  rf95_2.setTxPower(14, false);
  rf95_2.setThisAddress(RF95_2_NODE_ID);
  rf95_2.setHeaderFrom(RF95_2_NODE_ID);

  rf95_3.setTxPower(14, false);
  rf95_3.setThisAddress(RF95_3_NODE_ID);
  rf95_3.setHeaderFrom(RF95_3_NODE_ID);

  rf95_2.setSpreadingFactor(7);
  rf95_3.setSpreadingFactor(7);
  
  rf95_2.setSignalBandwidth(125000);
  rf95_3.setSignalBandwidth(125000);
  
  rf95_2.setCodingRate4(5);
  rf95_3.setCodingRate4(5);
  
  rf95_2.setPromiscuous(true);
  rf95_3.setPromiscuous(true);

  rf95_2.setModeRx();
  rf95_3.setModeRx();
}


void loop()
{
  if (rf95_3.available())
  {
    // Should be a message for us now   
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    String data = (char*)buf;
    int datal = data.length() + 1;
    char x[datal];
    data.toCharArray(x, datal);
    
    if (rf95_3.recv(buf, &len))
    {
      RH_RF95::printBuffer("request: ", buf, len);
      
      tx = "";
      gpsla = "";
      gpslo = "";
      suara = "";
      adxl = "";
      
      counter = 1;
      for(i = 0; i  <= datal; i++){
        if(x[i] == ' '){
          counter++;
        }
        else if(x[i] != ' ' && counter == 1){
          tx = String(x[i]);
        }
        else if(x[i] != ' ' && counter == 2){
          gpsla = String(gpsla + x[i]);
        }
        else if(x[i] != ' ' && counter == 3){
          gpslo = String(gpslo + x[i]);
        }
        else if(x[i] != ' ' && counter == 5){
          suara = String(suara + x[i]);
        }
        else if(x[i] != ' ' && counter == 6){
          adxl = String(adxl + x[i]);
        }
      }
      counterr++;
      counterr2++;
      //<<<<<Output>>>>>
    }
    else
    {
      Console.println("recv failed");
    }
    Console.println(tx);
    Console.println(gpsla);
    Console.println(gpslo);
    Console.println(suara);
    Console.println(adxl);
    
    Console.print("tx = ");
    Console.println(tx);
    Console.print("Counterr = ");
    Console.println(counterr);
    Console.print("Counterr2 = ");
    Console.println(counterr2);
    
    if (counterr >= 10 && tx == "1"){
      Process p;
      p.begin("lua");
      p.addParameter("/root/revisi.lua");
      p.addParameter((String)suara);
      p.addParameter((String)gpsla);
      p.addParameter((String)gpslo);
      p.addParameter((String)adxl);
      p.run();
      counterr = 0;
      Console.println("Data 1 stored");
    }

    else if (counterr2 >= 10 && tx == "2"){
      Process q;
      q.begin("lua");
      q.addParameter("/root/revisi2.lua");
      q.addParameter(suara);
      q.addParameter(gpsla);
      q.addParameter(gpslo);
      q.addParameter(adxl);
      q.run();
      counterr2 = 0;
      Console.println("Data 2 stored");
    }    
  }
  
}
