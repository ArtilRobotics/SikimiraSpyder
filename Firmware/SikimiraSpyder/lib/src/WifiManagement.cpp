#include <WifiManagement.h>
#include <Arduino.h>
#include <WiFi.h>
#include "BluetoothSerial.h"
#include <EEPROM.h>

BluetoothSerial ESP_BT; // Object for Bluetooth
String buffer_in;
unsigned long previousMillisBT = 0;
byte val;
int addr = 0;
byte indS = 0;
byte indP = 0;
String stream;
byte len = 0;
String temp;
String temp2;
unsigned int interval = 30000; // Tiempo de espera
bool WifiStatus;
int ledPinWF=22;

//// Inicio de conexión WiFi por BT

boolean check_wifiUpdate()
{
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillisBT >= interval)
  {
    previousMillisBT = currentMillis;
    Serial.println("60 Seconds Over");
    return true;
  }
  else if (ESP_BT.available())
  { // Check if we receive anything from Bluetooth
    interval = 50000;
    buffer_in = ESP_BT.readStringUntil('\n'); // Read what we recevive
    // Serial.println("Received:"); Serial.println(buffer_in);
    delay(20);
    if (buffer_in.charAt(0) == 'S')
    {
      for (int i = 0; i < buffer_in.length(); i++)
      {
        val = (byte)(buffer_in.charAt(i));
        // Serial.println("val "+val);
        EEPROM.write(addr, val);
        // Serial.println(val);
        addr++;
      }
      // Serial.print("New ");
      // Serial.print(buffer_in);
      EEPROM.write(addr, 10);
      addr++;
      EEPROM.commit();
      ESP_BT.println("SSID Stored");
    }
    else if (buffer_in.charAt(0) == 'P')
    {
      for (int i = 0; i < buffer_in.length(); i++)
      {
        val = (byte)(buffer_in.charAt(i));
        // Serial.println("val "+val);
        EEPROM.write(addr, val);
        // Serial.println(val);
        addr++;
      }
      // Serial.print("New ");
      // Serial.print(buffer_in);
      EEPROM.write(addr, 10);
      EEPROM.commit();
      ESP_BT.println("Password Stored");
      return true;
    }
    return false;
  }
  else
  {
    return false;
  }
}

//// Fin de conexión WiFi por BT


// void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info){
//   Serial.println("Connected to AP successfully!");
// }

// void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info){
//   Serial.println("WiFi connected");
//   Serial.println("IP address: ");
//   Serial.println(WiFi.localIP());
// }

// void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info){
//   Serial.println("Disconnected from WiFi access point");
//   Serial.print("WiFi lost connection. Reason: ");
//   Serial.println(info.wifi_sta_disconnected.reason);
//   Serial.println("Trying to Reconnect");
//   init();
// }

void WifiManagement::init()
{
  pinMode(ledPinWF, OUTPUT);
  /////// Inicio de conexión WiFi por BT

  EEPROM.begin(50);
  // Serial.begin(9600); //Start Serial monitor in 9600
  Serial.println("Bluetooth Device is Ready to Pair");
  Serial.println("Waiting For Wifi Updates 30 seconds");
  ESP_BT.begin("Sikimira"); // Name of your Bluetooth Signal


  while (!check_wifiUpdate() == true)
  {
  }
  Serial.println("The Stored Wifi credetial are : ");
  for (int i = 0; i < 50; i++)
  {
    val = EEPROM.read(i);
    stream += (char)val;
    if ((val == 10) && (indS == 0))
    {
      indS = i;
      // Serial.println("indS"+(String)i);
    }
    else if (val == 10 && indP == 0)
    {
      indP = i;
      break;
      // Serial.println("indP"+(String)i);
    }
  }
  // Serial.println(stream);
  // Serial.println("Stream Ended");
  temp = stream.substring(0, indS);
  temp = temp.substring(5, indS);
  // ssid2=ssid;
  temp2 = stream.substring(indS + 1, indP);
  temp2 = temp2.substring(5, indP - indS);
  int i = temp.length();
  int j = temp2.length();
  char ssid[i];
  char pass[j];
  temp.toCharArray(ssid, i);
  temp2.toCharArray(pass, j);
  Serial.println("Stored SSID");
  Serial.println(ssid);
  Serial.println("Stored PASS");
  Serial.println(pass);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  if (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.println("WiFi Failed");
    WifiStatus = false;
     digitalWrite(ledPinWF, LOW);
  }
  else
  {
    Serial.print("Wifi Connected to ");
    Serial.println(ssid);
    digitalWrite(ledPinWF, HIGH);
    WifiStatus = true;
  }

  ///////  Fin de conexión WiFi por BT
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  WiFi.persistent(false);
	WiFi.mode(WIFI_STA);
  //WiFi.setSleepMode(WIFI_NONE_SLEEP);
	WiFi.setAutoReconnect(true);

  // WiFi.onEvent(WiFiStationConnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_CONNECTED);
  // WiFi.onEvent(WiFiGotIP, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
  // WiFi.onEvent(WiFiStationDisconnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
}

