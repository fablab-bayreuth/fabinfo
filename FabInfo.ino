/*
  FabInfo Version 0.2  by JTL / thirsch

  Configurator Engine based on ESP_WebConfig
  Special thanks to John Lassen

  The Parola library to scroll text on the display is used
  greetings to down under send to Marco Colli

  Thanks also to Nick Downie for the Chart.js library
*/


#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <MD_Parola.h>
#include <Ticker.h>
#include <EEPROM.h>
#include <WiFiUdp.h>
#include <SPI.h>
#include <ThingerWifi.h>
#include "helpers.h"
#include "global.h"

// NOTE: MD_MAX72xx library must be installed and configured for the LED
// matrix type USE_FC16_HW being used. Refer documentation included in 
// the MD_MAX72xx library or see this link:
// https://majicdesigns.github.io/MD_MAX72XX/page_hardware.html

// define program settings
#define PGNV "0.2"
#define ACCESS_POINT_NAME  "FabInfo"
#define ACCESS_POINT_PASSWORD  "12345678"
#define AdminTimeOut 600  // Defines the Time in Seconds, when the Admin-Mode will be diabled

// PAROLA define setting of FC-16 LED matrix modules and hardware SPI
// Vcc       3v3     LED matrix work at 3.3V
// GND       GND     GND
// DIN        D7     HSPID or HMOSI
// CS or LD   D4     HSPICS or HCS (D8)
// CLK        D5     CLK or HCLK
#define MAX_DEVICES 8
#define CS_PIN      D8 // or LD,SS


/*
  Include the HTML, STYLE and Script "Pages"
*/
#include "page_root.h"
#include "page_admin.h"
#include "page_script.js.h"
#include "page_chart.h"
#include "page_style.css.h"
#include "page_ntpsettings.h"
#include "page_information.h"
#include "page_general.h"
#include "page_story1.h"
#include "page_networkconfiguration.h"
#include "page_fabinfo.h"

void setup ( void ) {
  EEPROM.begin(1024);
  Serial.begin(115200);
  delay(500);

  pinMode(BUILTIN_LED, OUTPUT);


// set pins Auto Intensity Sensor(AIS)
// LDRV       D3      LDR 3.3V
// LDRG       D2      LDG GND
// LDRA       A0      LDR analog
  pinMode(A0, INPUT); // Auto Intensity Sensor Analog Output
  pinMode(D2, OUTPUT); // Auto Intensity Sensor GND
  pinMode(D3, OUTPUT); // Auto Intensity Sensor VCC
  digitalWrite(D2, LOW);
  digitalWrite(D3, LOW); // Auto Intensity Sensor OFF

// set pins DHT-11 Sensor(TMP & HUM)
// DHTD       D1       DHT-11 Data
  pinMode(D1, INPUT); // DHT-11 Data Digital Input

  Serial.printf("Starting FabInfo %s\n", PGNV);

 // SetDefaultConfig();

  SetConfig(); // set defaults if no configuration found before
  
   if (AdminEnabled)
  {
    WiFi.mode(WIFI_AP_STA);

    char buffer[33];
    sprintf(buffer, "%s_%06x", ACCESS_POINT_NAME, ESP.getChipId());
 
    WiFi.softAP(buffer, ACCESS_POINT_PASSWORD);
  }
  else
  {
    WiFi.mode(WIFI_STA);
  }
  ConfigureWifi();

  server.on ( "/", processFabInfo  );
  
  server.on ( "/admin/filldynamicdata", filldynamicdata );

  server.on ( "/favicon.ico",   []() {
    Serial.println("favicon.ico");
    sendCacheHeader();
    server.send ( 200, "text/html", "" );
  }  );

  server.on ( "/admin.html", []() {
    Serial.println("admin.html");
    sendCacheHeader();
    server.send_P ( 200, "text/html", PAGE_AdminMainPage );
  }  );
  
  server.on ( "/config.html", send_network_configuration_html );

  server.on ( "/story1.html", send_story1_html );
  
  server.on ( "/info.html", []() {
    Serial.println("info.html");
    sendCacheHeader();
    server.send_P ( 200, "text/html", PAGE_Information );
  }  );
  
  server.on ( "/story1values.html", send_story1_configuration_values_html );
  
  server.on ( "/ntp.html", send_NTP_configuration_html  );
  
  server.on ( "/general.html", send_general_html  );
  
  server.on ( "/fabinfo.html", []() {
    sendCacheHeader();
    server.send_P ( 200, "text/html", PAGE_FabInfo );
  } );
  
  server.on ( "/style.css", []() {
    Serial.println("style.css");
    sendCacheHeader();
    server.send_P ( 200, "text/css", PAGE_Style_css );
  } );
  
  server.on ( "/microajax.js", []() {
    Serial.println("microajax.js");
    sendCacheHeader();
    server.send_P ( 200, "application/javascript", PAGE_microajax_js );
  } );
  
  server.on ( "/chart.min.js", []() {
    Serial.println("chart.min.js");
    sendCacheHeader();
    server.send_P ( 200, "application/javascript", PAGE_chart_js );
  } );
  
  server.on ( "/admin/values", send_network_configuration_values_html );
  
  server.on ( "/admin/connectionstate", send_connection_state_values_html );
  
  server.on ( "/admin/sensrefreshtimes", send_information_values_html );
  
  server.on ( "/admin/ntpvalues", send_NTP_configuration_values_html );
  
  server.on ( "/admin/generalvalues", send_general_configuration_values_html);

  server.on ( "/admin/story1values", send_story1_configuration_values_html);
  
  server.on ( "/admin/IoTRLS",     send_IoTRLS_value_html);

  server.onNotFound ( []() {
    Serial.println("Page Not Found");
    server.send ( 400, "text/html", "404 Error ... Page not found" );
  }  );
  
  server.begin();
  Serial.println( "HTTP server started" );
  tkSecond.attach(1, Second_Tick);
  UDPNTPClient.begin(2390);  // Port for NTP receive

  // Ausgabe der eigenen Client IP-Adresse
  char curIP[20];
  sprintf(curIP, "%03d:%03d:%03d:%03d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3]);
  Serial.println("\nAssigned IP ");
  Serial.println(curIP);
  ConfigureIoT();
}


void loop ( void ) {
  // Als erstes wieder eine evtl. neue Seite "bereitstellen", danach werden ggf. Admin abgeschaltet, NTP aktualisiert etc.
  server.handleClient();

  SetDefaultConfig();

  if (AdminEnabled)
    {
  	if (AdminTimeOutCounter > AdminTimeOut)
  	{
  		 AdminEnabled = false;
  		 Serial.println("Admin Mode disabled!");
  		 WiFi.mode(WIFI_STA);
  	}
    }

  
  if (config.Update_Time_Via_NTP_Every  > 0 )
  {
    if (syncNtp) {
      if (NTPRefresh() > 0) {
        syncNtp = false;
      }
    }

    if ((cNTP_Update > 5 && firstStart) || cNTP_Update > (config.Update_Time_Via_NTP_Every * 60))
    {
      syncNtp = true;
      NTPRefresh();

      cNTP_Update = 0;
      firstStart = false;
    }
  }

  if (DateTime.minute != Minute_Old)
  {
    Minute_Old = DateTime.minute;
    if (config.AutoTurnOn)
    {
      if (DateTime.hour == config.TurnOnHour && DateTime.minute == config.TurnOnMinute)
      {
        Time_On = true;
        Serial.println("Enabled FabInfo IoT Communication");
      }

      if (DateTime.hour == config.TurnOffHour && DateTime.minute == config.TurnOffMinute)
      {
        Time_On = false;
        Serial.println("Disabled FabInfo IoT Communication");
      }
    }
  }

  if (config.SensRefreshTime > 0 && (cSens_Update > config.SensRefreshTime * 60 || Sens_Value == -1))
  {
    Sens_Value = readprobe();
    Serial.println("Read sensor value - start");
    Serial.println(Sens_Value);
    cSens_Update = 0;
  }
  
  if (Time_On && config.IoTOn && thing && WiFi.status() == WL_CONNECTED)
  {
    thing->handle();
  }

  if (Refresh)
  {
    Refresh = false;
    ///Serial.println("Refreshing...");
    //Serial.printf("FreeMem:%d %d:%d:%d %d.%d.%d \n",ESP.getFreeHeap() , DateTime.hour,DateTime.minute, DateTime.second, DateTime.year, DateTime.month, DateTime.day);
  }

  if(WiFi.status() == WL_CONNECTED) {
    cWiFi_Connect = 0;
  } else if(cWiFi_Connect > 10 && WiFi.status() == WL_NO_SSID_AVAIL) {
    Serial.println("WiFi-Data wrong? Stop trying to connect!");
    WiFi.disconnect();
  }

  blinkLED();

/*
 Effekt Zahl beginnend mit 1, PA_PRINT
  <option value="PA_PRINT">PRINT</option>
  <option value="PA_SLICE">SLICE</option>
  <option value="PA_MESH">MESH</option>
  <option value="PA_FADE">FADE</option>
  <option value="PA_WIPE">WIPE</option>
  <option value="PA_WIPE_CURSOR">WPE_C</option>
  <option value="PA_OPENING">OPEN</option>
  <option value="PA_OPENING_CURSOR">OPENING CURSOR</option>
  <option value="PA_CLOSING">CLOSE</option>
  <option value="PA_CLOSING_CURSOR">CLOSING CURSOR</option>
  <option value="PA_RANDOM">RAND</option>
  <option value="PA_BLINDS">BLIND</option>
  <option value="PA_DISSOLVE">DISSOLVE</option>
  <option value="PA_SCROLL_UP">SCROLL UP</option>
  <option value="PA_SCROLL_DOWN">SCROLL DOWN</option>
  <option value="PA_SCROLL_LEFT">SCROLL LEFT</option>
  <option value="PA_SCROLL_RIGHT">SCROLL RIGHT</option>
  <option value="PA_SCROLL_UP_LEFT">SCROLL UP LEFT</option>
  <option value="PA_SCROLL_UP_RIGHT">SCROLL UP RIGHT</option>
  <option value="PA_SCROLL_DOWN_LEFT">SCROLL DOWN LEFT</option>
  <option value="PA_SCROLL_DOWN_RIGHT">SCROLL DOWN RIGHT</option>
  <option value="PA_SCAN_HORIZ">SCAN HORIZ</option>
  <option value="PA_SCAN_HORIZX">SCAN HORIZX</option>
  <option value="PA_SCAN_VERT">SCAN VERT</option>
  <option value="PA_SCAN_VERTX">SCAN VERTX</option>
  <option value="PA_GROW_UP">GROW UP</option>
  <option value="PA_GROW_DOWN">GROW DOWN</option>   

 */

  
}


