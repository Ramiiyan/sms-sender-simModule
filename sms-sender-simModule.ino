/*
 * CREATED BY.RAMIYAN
 */

#define TINY_GSM_MODEM_SIM7000  //defining Your SimModule for TinyGSM library Purpose.
#include <Arduino.h>
#include <TinyGsmClient.h> //Install TimGsm Library
#include <SoftwareSerial.h>
#include <Wire.h>

#define NumberD "*************"  //Enter Sender Number with contry code.(DEFAULT)
#define GSM_APN "dialogbb"
#define NB_APN  "nbiot"

SoftwareSerial SerialGSM(7, 8);
TinyGsm modem(SerialGSM);

String number = "+94"; // Enter Your Country code For use as At command
String ccid;
void done();

void setup()
{
  Serial.begin(115200);
  delay(300);
  SerialGSM.begin(4800);      // 4800 is defalut sim7000 baud. change if you have any Exceptions
  modem.setNetworkMode(38); // 38-LTE, 13-GSM
  modem.setPreferredMode(2); // 2-NBIOT, 1-CAT

  Serial.print(F("Finding Network...")); 
  modem.restart();  
  delay(2000);
  
  if (!modem.waitForNetwork())
  {
    Serial.println(F("Network Fail."));  // Check your SIM7000 Module 
    while (true);
  }
  else
  {
    Serial.println(F("Network identified."));
    Serial.print(F("Signal Strength : "));
    Serial.println(modem.getSignalQuality());
    if (!modem.gprsConnect(NB_APN, "", ""))  // use As "Sim APN", "userName", "password" //for default username, pwd will be null("","") 
    {
      Serial.println(F("GPRS Fail"));
    }
    else
    {
      Serial.println(F("GPRS connected."));
      SerialGSM.println("AT+CCID");  // AT command for getting CCID sim Number(printed on the sim) FOR sim7000
      done();
    }
  }
}

void loop()
{
}
void done()
{
  delay(1000);

  while (SerialGSM.available())
  {
    ccid = SerialGSM.readString();
  }
  Serial.println(ccid);
  ccid.remove(0, 4);
  ccid.remove(16, 2);
  delay(300);
  Serial.print("CCID: ");
  Serial.println(ccid);

// if you enter the number via AT command....
  if (Serial.available())
  {
    number += Serial.readString();
    Serial.print("number :");
    Serial.println(number);
    delay(1000);
    modem.sendSMS(number, ccid);  //Sending message using TinyGSM library 
    Serial.println("Sent !");
  }else{
    modem.sendSMS(NumberD, ccid); //Defaultly Sending message To Defined number above.
    Serial.println("Sent !");
  }
}
