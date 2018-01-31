/*
  FabInfo Version 0.1  by JTL / thirsch

  Configurator Engine based on ESP_WebConfig
  Special thanks to John Lassen

  The Parola library to scroll text on the display is used
  greetings to down under send to Marco Colli

  Thanks also to Nick Downie for the Chart.js library
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Ticker.h>
#include <EEPROM.h>
#include <WiFiUdp.h>
#include <SPI.h>
#include <ThingerWifi.h>
#include "helpers.h"
#include "global.h"
#include <MD_Parola.h>

// NOTE: MD_MAX72xx library must be installed and configured for the LED
// matrix type being used. Refer documentation included in the MD_MAX72xx
// library or see this link:
// https://majicdesigns.github.io/MD_MAX72XX/page_hardware.html

// define program settings
#define PGNV "0.1"
#define ACCESS_POINT_NAME  "FabInfo"
#define ACCESS_POINT_PASSWORD  "12345678"
#define AdminTimeOut 600  // Defines the Time in Seconds, when the Admin-Mode will be diabled


// PAROLA define setting of FC-16 LED matrix modules and SPI
// Vcc       3v3     LED matrix work at 3.3V
// GND       GND     GND
// DIN        D7     HSPID or HMOSI
// CS or LD   D4     HSPICS or HCS (D8)
// CLK        D5     CLK or HCLK
#define MAX_DEVICES 8
#define CLK_PIN   D5 // or SCK
#define DATA_PIN  D7 // or MOSI
#define CS_PIN    D4 // or LD,SS

// PAROLA HARDWARE SPI
MD_Parola P = MD_Parola(CS_PIN, MAX_DEVICES);
// SOFTWARE SPI
//MD_Parola P = MD_Parola(DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

/*
// PAROLA WiFi login parameters - network name and password
const char* ssid = "SSID";
const char* password = "PWD";

// PAROLA WiFi Server object and parameters
WiFiServer server(80);
*/

// PAROLA Scrolling parameters
uint8_t frameDelay = 25;  // default frame delay value
textEffect_t  scrollEffect = PA_SCROLL_LEFT;

// PAROLA Global message buffers shared by Wifi and Scrolling functions
#define BUF_SIZE  512
char curMessage[BUF_SIZE];
char newMessage[BUF_SIZE];
bool newMessageAvailable = false;


/*
  Include the HTML, STYLE and Script "Pages"
*/
#include "Page_Root.h"
#include "Page_Admin.h"
#include "Page_Script.js.h"
#include "Page_Chart.h"
#include "Page_Style.css.h"
#include "Page_NTPsettings.h"
#include "Page_Information.h"
#include "Page_General.h"
#include "PAGE_NetworkConfiguration.h"
#include "PAGE_FabInfo.h"






void setup ( void ) {
  EEPROM.begin(512);
  Serial.begin(115200);
  delay(500);

  pinMode(BUILTIN_LED, OUTPUT);


// set pins Auto Light Sensor(ALS)
// LDRV       D3      LDR 3.3V
// LDRG       D2      LDG GND
// LDRA       A0      LDR analog
  pinMode(A0, INPUT); // Auto Light Sensor Analog Output
  pinMode(D2, OUTPUT); // Auto Light Sensor GND
  pinMode(D3, OUTPUT); // Auto Light Sensor VCC
  digitalWrite(D2, LOW);
  digitalWrite(D3, LOW); // Auto Light Sensor OFF

// set pins DHT-11 Sensor(TMP & HUM)
// DHTD       D1       DHT-11 Data
  pinMode(D1, INPUT); // DHT-11 Data Digital Input

  Serial.printf("Starting FabInfo %s\n", PGNV);

  if (!ReadConfig())
  {
    // DEFAULT CONFIG
    config.ssid = "SSID";
    config.password = "PWD";
    config.dhcp = true;
    config.IP[0] = 192; config.IP[1] = 168; config.IP[2] = 1; config.IP[3] = 100;
    config.Netmask[0] = 255; config.Netmask[1] = 255; config.Netmask[2] = 255; config.Netmask[3] = 0;
    config.Gateway[0] = 192; config.Gateway[1] = 168; config.Gateway[2] = 1; config.Gateway[3] = 1;
    config.ntpServerName = "0.de.pool.ntp.org";
    config.Update_Time_Via_NTP_Every = 1;
    config.timezone = 10;
    config.daylight = true;
    config.DeviceName = "Not Named FabInfo";
    config.IoTOn = false;
    config.IoTUserName = "Not Named IoT User";
    config.IoTDeviceID = "Not Named IoT ID";
    config.IoTCredential = "Not Named IoT Credential";
    config.AutoTurnOff = false; // currently not used
    config.AutoTurnOn = false;
    config.TurnOffHour = 0;
    config.TurnOffMinute = 0;
    config.TurnOnHour = 0;
    config.TurnOnMinute = 0;
    config.SensCalMin = 60;
    config.SensCalMax = 900;
    config.SensCalc = 100;
    config.InfoOn = false; // currently not used -> LED
    config.LEDOn = true; // enable switch LED on
    config.SensRefreshTime = 1;
    config.MinWarn = 0; // currently not used
    WriteConfig();
    Serial.println("General config applied");
  }


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
  server.on ( "/info.html", []() {
    Serial.println("info.html");
    sendCacheHeader();
    server.send_P ( 200, "text/html", PAGE_Information );
  }  );
  server.on ( "/ntp.html", send_NTP_configuration_html  );
  server.on ( "/general.html", send_general_html  );
  server.on ( "/FabInfo.html", []() {
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
  server.on ( "/admin/devicename",     send_devicename_value_html);

  server.onNotFound ( []() {
    Serial.println("Page Not Found");
    server.send ( 400, "text/html", "404 Error ... Page not found" );
  }  );
  server.begin();
  Serial.println( "HTTP server started" );
  tkSecond.attach(1, Second_Tick);
  UDPNTPClient.begin(2390);  // Port for NTP receive
  ConfigureIoT();



  // PAROLA Setup code
  //Serial.begin(57600);
  //PRINTS("\n[MD_Parola WiFi Message Display]\nType a message for the scrolling display from your internet browser");

  P.begin();
  P.displayClear();
  P.displaySuspend(false);

  P.displayScroll(curMessage, PA_LEFT, scrollEffect, frameDelay);

  curMessage[0] = newMessage[0] = '\0';

  // Connect to and initialise WiFi network
  PRINT("\nConnecting to ", ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    PRINT("\n", err2Str(WiFi.status()));
    delay(500);
  }
  PRINTS("\nWiFi connected");

  // Start the server
  server.begin();
  PRINTS("\nServer started");

  // Set up first message as the IP address
  sprintf(curMessage, "%03d:%03d:%03d:%03d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3]);
  PRINT("\nAssigned IP ", curMessage);

  // End of PAROLA Setup code
  
}


char *err2Str(wl_status_t code)
{
  switch (code)
  {
  case WL_IDLE_STATUS:    return("IDLE");           break; // WiFi is in process of changing between statuses
  case WL_NO_SSID_AVAIL:  return("NO_SSID_AVAIL");  break; // case configured SSID cannot be reached
  case WL_CONNECTED:      return("CONNECTED");      break; // successful connection is established
  case WL_CONNECT_FAILED: return("CONNECT_FAILED"); break; // password is incorrect
  case WL_DISCONNECTED:   return("CONNECT_FAILED"); break; // module is not configured in station mode
  default: return("??");
  }
}

uint8_t htoi(char c)
{
  c = toupper(c);
  if ((c >= '0') && (c <= '9')) return(c - '0');
  if ((c >= 'A') && (c <= 'F')) return(c - 'A' + 0xa);
  return(0);
}








void getData(char *szMesg, uint8_t len)
// Message may contain data for:
// New text (/&MSG=)
// Scroll direction (/&SD=)
// Invert (/&I=)
// Speed (/&SP=)
{
  char *pStart, *pEnd;      // pointer to start and end of text

  // check text message
  pStart = strstr(szMesg, "/&MSG=");
  if (pStart != NULL)
  {
    char *psz = newMessage;

    pStart += 6;  // skip to start of data
    pEnd = strstr(pStart, "/&");

    if (pEnd != NULL)
    {
      while (pStart != pEnd)
      {
        if ((*pStart == '%') && isdigit(*(pStart + 1)))
        {
          // replace %xx hex code with the ASCII character
          char c = 0;
          pStart++;
          c += (htoi(*pStart++) << 4);
          c += htoi(*pStart++);
          *psz++ = c;
        }
        else
          *psz++ = *pStart++;
      }

      *psz = '\0'; // terminate the string
      newMessageAvailable = (strlen(newMessage) != 0);
      PRINT("\nNew Msg: ", newMessage);
    }
  }

  // check scroll direction
  pStart = strstr(szMesg, "/&SD=");
  if (pStart != NULL)
  {
    pStart += 5;  // skip to start of data

    PRINT("\nScroll direction: ", *pStart);
    scrollEffect = (*pStart == 'R' ? PA_SCROLL_RIGHT : PA_SCROLL_LEFT);
    P.setTextEffect(scrollEffect, scrollEffect);
    P.displayReset();
  }

  // check invert
  pStart = strstr(szMesg, "/&I=");
  if (pStart != NULL)
  {
    pStart += 4;  // skip to start of data

    PRINT("\nInvert mode: ", *pStart);
    P.setInvert(*pStart == '1');
  }

  // check speed
  pStart = strstr(szMesg, "/&SP=");
  if (pStart != NULL)
  {
    pStart += 5;  // skip to start of data

    int16_t speed = atoi(pStart);
    PRINT("\nSpeed: ", P.getSpeed());
    P.setSpeed(speed);
    frameDelay = speed;
  }
}



void handleWiFi(void)
{
  static enum { S_IDLE, S_WAIT_CONN, S_READ, S_EXTRACT, S_RESPONSE, S_DISCONN } state = S_IDLE;
  static char szBuf[1024];
  static uint16_t idxBuf = 0;
  static WiFiClient client;
  static uint32_t timeStart;

  switch (state)
  {
  case S_IDLE:   // initialise
    PRINTS("\nS_IDLE");
    idxBuf = 0;
    state = S_WAIT_CONN;
    break;

  case S_WAIT_CONN:   // waiting for connection
  {
    client = server.available();
    if (!client) break;
    if (!client.connected()) break;

#if DEBUG
    char szTxt[20];
    sprintf(szTxt, "%03d:%03d:%03d:%03d", client.remoteIP()[0], client.remoteIP()[1], client.remoteIP()[2], client.remoteIP()[3]);
    PRINT("\nNew client @ ", szTxt);
#endif

    timeStart = millis();
    state = S_READ;
  }
  break;

  case S_READ: // get the first line of data
    PRINTS("\nS_READ ");

    while (client.available())
    {
      char c = client.read();

      if ((c == '\r') || (c == '\n'))
      {
        szBuf[idxBuf] = '\0';
        client.flush();
        PRINT("\nRecv: ", szBuf);
        state = S_EXTRACT;
      }
      else
        szBuf[idxBuf++] = (char)c;
    }
    if (millis() - timeStart > 1000)
    {
      PRINTS("\nWait timeout");
      state = S_DISCONN;
    }
    break;

  case S_EXTRACT: // extract data
    PRINTS("\nS_EXTRACT");
    // Extract the string from the message if there is one
    getData(szBuf, BUF_SIZE);
    state = S_RESPONSE;
    break;

  case S_RESPONSE: // send the response to the client
    PRINTS("\nS_RESPONSE");
    // Return the response to the client (web page)
    client.print(WebResponse);
    client.print(WebPage);
    state = S_DISCONN;
    break;

  case S_DISCONN: // disconnect client
    PRINTS("\nS_DISCONN");
    client.flush();
    client.stop();
    state = S_IDLE;
    break;

  default:  state = S_IDLE;
  }
}








void loop ( void ) {
  // Als erstes wieder eine evtl. neue Seite "bereitstellen", danach werden ggf. Admin abgeschaltet, NTP aktualisiert etc.
  server.handleClient();

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


  //PAROLA Loop code  
  handleWiFi();

  if (P.displayAnimate())
  {
    if (newMessageAvailable)
    {
      strcpy(curMessage, newMessage);
      newMessageAvailable = false;
    }
    P.displayReset();
  // End off PAROLA Loop code  
}









/*PAROLA Code



const char WebResponse[] = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n";

char WebPage[] =
"<!DOCTYPE html>" \
"<html>" \
"<head>" \
"<title>MajicDesigns Test Page</title>" \

"<script>" \
"strLine = \"\";" \

"function SendData()" \
"{" \
"  nocache = \"/&nocache=\" + Math.random() * 1000000;" \
"  var request = new XMLHttpRequest();" \
"  strLine = \"&MSG=\" + document.getElementById(\"data_form\").Message.value;" \
"  strLine = strLine + \"/&SD=\" + document.getElementById(\"data_form\").ScrollType.value;" \
"  strLine = strLine + \"/&I=\" + document.getElementById(\"data_form\").Invert.value;" \
"  strLine = strLine + \"/&SP=\" + document.getElementById(\"data_form\").Speed.value;" \
"  request.open(\"GET\", strLine + nocache, false);" \
"  request.send(null);" \
"}" \
"</script>" \
"</head>" \

"<body>" \
"<p><b>MD_Parola set message</b></p>" \

"<form id=\"data_form\" name=\"frmText\">" \
"<label>Message:<br><input type=\"text\" name=\"Message\" maxlength=\"255\"></label>" \
"<br><br>" \
"<input type = \"radio\" name = \"Invert\" value = \"0\" checked> Normal" \
"<input type = \"radio\" name = \"Invert\" value = \"1\"> Inverse" \
"<br>" \
"<input type = \"radio\" name = \"ScrollType\" value = \"L\" checked> Left Scroll" \
"<input type = \"radio\" name = \"ScrollType\" value = \"R\"> Right Scroll" \
"<br><br>" \
"<label>Speed:<br>Fast<input type=\"range\" name=\"Speed\"min=\"10\" max=\"200\">Slow"\
"<br>" \
"</form>" \
"<br>" \
"<input type=\"submit\" value=\"Send Data\" onclick=\"SendData()\">" \
"</body>" \
"</html>";



void setup()
{

}

*/
