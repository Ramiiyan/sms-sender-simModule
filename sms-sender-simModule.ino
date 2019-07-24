#define TINY_GSM_MODEM_SIM7000
#include <Arduino.h>
#include <TinyGsmClient.h>
#include <SoftwareSerial.h>
#include <Wire.h>

#define NumberD "+94777333702"

SoftwareSerial SerialGSM(7, 8);
TinyGsm modem(SerialGSM);

String number = "+94";
String ccid;
void done();

void setup()
{
  Serial.begin(115200);
  delay(300);
  SerialGSM.begin(4800);
  modem.setNetworkMode(13); // 38-LTE, 13-GSM
  modem.setPreferredMode(2); // 2-NBIOT, 1-GSM

  Serial.print(F("Finding Network..."));
  modem.restart();
  delay(4000);
  Serial.print("Testing..");
  if (!modem.waitForNetwork())
  {
    Serial.println(F("Network Fail."));
    while (true)
      ;
  }
  else
  {
    Serial.println(F("Network identified."));
    Serial.print(F("Signal Strength : "));
    Serial.println(modem.getSignalQuality());
    if (!modem.gprsConnect("dialogbb", "", ""))
    {
      Serial.println(F("GPRS Fail"));
    }
    else
    {
      Serial.println(F("GPRS connected."));
      SerialGSM.println("AT+CCID");
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

  if (Serial.available())
  {
    number += Serial.readString();
    Serial.print("number :");
    Serial.println(number);
    delay(1000);
    modem.sendSMS(number, ccid);
  }
  modem.sendSMS(NumberD, ccid);
}
