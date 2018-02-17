#ifndef GLOBAL_H
#define GLOBAL_H


ESP8266WebServer server(80);									// The Webserver
boolean firstStart = true;										// On firststart = true, NTP will try to get a valid time
int AdminTimeOutCounter = 0;									// Counter for Disabling the AdminMode
strDateTime DateTime;											// Global DateTime structure, will be refreshed every Second
WiFiUDP UDPNTPClient;											// NTP Client
unsigned long UnixTimestamp = 0;								// GLOBALTIME  ( Will be set by NTP)
boolean Refresh = false;                    // For Main Loop, to refresh things like GPIO / WS2812
int cNTP_Update = 0;											// Counter for Updating the time via NTP
int cSens_Update = 0;                     // Counter for Updating the sensor
int cWiFi_Connect = 0;                    // Counter how many seconds the WiFi tried to connect to the AP.
Ticker tkSecond;												// Second - Timer for Updating Datetime Structure
boolean AdminEnabled = true;		// Enable Admin Mode for a given Time
byte Minute_Old = 100;			// Helpvariable for checking, when a new Minute comes up (for Auto Turn On / Off)
boolean Time_On = true;        // For Main Loop, to enable Thinger.io service by time - first autostart
int Sens_Value = -1;

int LEDState = LOW;          // LEDState used to set the LED
int LEDPWM = 2;               // LEDPWM used to set PWM rate
long previousLED = 0;        // will store last time LED was updated
long LEDinterval = 300;     // by PWM variable interval at which to blink (milliseconds)
long LEDpuls = 300;         // interval at which to blink (milliseconds)

byte StoryInstances = 0;    // number of configured story instances for current storybook

ThingerWifi *thing; // Platzhalter für spaetere ThingerWifi Instanz

struct strConfig {
	String ssid;
	String password;
	byte  IP[4];
	byte  Netmask[4];
	byte  Gateway[4];
	boolean dhcp;
	String ntpServerName;
	long Update_Time_Via_NTP_Every;
	long timezone;
	boolean daylight;
	String IoTRLS;
  boolean IoTOn;
  String IoTUserName;
  String IoTDeviceID;
  String IoTCredential;  
	boolean AutoTurnOn;
	byte TurnOffHour;
	byte TurnOffMinute;
	byte TurnOnHour;
	byte TurnOnMinute;
	int SensCalMinL;
	int SensCalMaxL;
	int SensCalcH;
  int SensCalcT;
	boolean LEDOn; // LED enabled switch
  boolean DefaultOn; // Set to default configuration
	byte SensRefreshTime;
  String IoTRHS;
  String IoTRTS;
  int SensCalMinH;
  int SensCalMaxH;
  int SensCalMinT;
  int SensCalMaxT;
}   config;

struct strStory {
  int StoryOrderNo; // Number for order of storybook 
  int LastStoryNo; // Last number for order of storybook 
  boolean AISOn; // Auto Intensity Sensor On (1=active)
  byte AISmin;  // AIS minimum level (0...15) 
  byte AISmax;  // AIS maximum level (0...15)
  byte DisplayIntensity; // static intensity level if AIS disabled (0...15)
  
  int ActionNo; // Display action (Textmessage, Temperature, Humidifier, Light, Current Time, Current Date, Target Date, Countdown to Date, MQTT Topic, Nerd News, Animation: PacMan, Smiley, Bouncing Ball, etc.)
  String Textmessage;
  int  TextSpeed; // Speed (ms/frame)
  boolean TextDirection; // Direction (0=left, 1=right)
  byte TextOrientation; // Text Orientation (1=left, 2=center, 3=right)
  long DisplayTime; // Time for action Parola=[pause] (ms)
  long NextTime; // Blank time to begin next action (ms) 
  byte InEffect; // Incoming text effect (see numbers of texteffect_t)
  byte OutEffect; // Fadeout text effect (see numbers of texteffect_t)
  byte CountTime; // Counter time unit to calculate target date (1=seconds, 2=minutes, 3=hours, 4=days)
  byte DateYear; // Target Date Year
  byte DateMonth; // Target Date Month
  byte DateDay; // Target Date Day
  byte DateHour; // Target Date Hour
  byte DateMinute; // Target Date Minute

  String MQTTtopic; // MQTT Topic to subscribe
  String MQTThost; // MQTT broker host
  String MQTTuser; // MQTT User data
  String MQTTpwd; // MQTT Password data
  String MQTTport; // MQTT Port
  boolean MQTTssl; // SSL active (1=active) 
  String MQTTsslport; // MQTT SSL Port  
  String MQTTwebsport; //Websockets port (for TSL only)
}   story0;






#define MAX_VALUES 12

class DataBuffer {
  private: 
    struct item { 
      strDateTime dateTime;
      int value;
    };
    item data[MAX_VALUES];
    int lastIndex = 0;
    
  public:
    /**
     * Pushes the current light value to the buffer once per hour.
     */
    void push(int value) {
      int prevIndex = lastIndex - 1;
      if(prevIndex < 0) prevIndex = MAX_VALUES - 1;

      //if(data[prevIndex].dateTime.minute < DateTime.minute || data[prevIndex].dateTime.hour < DateTime.hour) {
        data[lastIndex].dateTime = DateTime;
        data[lastIndex++].value = value;

        if(lastIndex > MAX_VALUES - 1) {
          lastIndex = 0;
        }
      //}
    }
    String getCommaSeparatedValues() {
      String res = "";

      for(int i = 0; i < MAX_VALUES; i++) {
        int pos = lastIndex + i;
        if(pos > MAX_VALUES - 1) pos = i - (MAX_VALUES - lastIndex);

        char buffer[9];
        sprintf(buffer, "%02d:%02d:%02d", data[pos].dateTime.hour, data[pos].dateTime.minute, data[pos].dateTime.second);
        res += buffer;
        res += ";";
        res += data[pos].value;
        res += ",";
      }

      return res;
    }
};

DataBuffer db;




int readprobe()
{
    int result;
/*
    digitalWrite(D2, LOW);
    digitalWrite(D3, HIGH); // Sensor ON

    delay(50); // vorher ist der Wert nicht stabil   
    result = map((1023-analogRead(A0)),config.SensCalMinL,config.SensCalMaxL,0,100); // !!!!!! Muss überarbeitet werden
 
    digitalWrite(D2, LOW);
    digitalWrite(D3, LOW); // Sensor OFF
*/
    db.push(result);
    
    return result;  
}




/*
**
** CONFIGURATION HANDLING
**
*/
void ConfigureIoT()
    {
      Serial.println("Configuring IoT");
      if (thing)
        {
          Serial.println( "IoT instance deleted" );
          delete thing; //löscht altes Objekt/Instanz
          thing = 0; //Pointer wieder zurück auf 0
        }
        
      if (config.IoTOn)
        {
        thing = new ThingerWifi (config.IoTUserName.c_str(),config.IoTDeviceID.c_str(),config.IoTCredential.c_str());
        // Connect thinger.io
        thing->add_wifi(config.ssid.c_str(), config.password.c_str());
        // Sensor resource
        (*thing)["SMS"] >> [](pson & out)
          {
        out = Sens_Value;
          };
        }    
    }


void ConfigureWifi()
{
	Serial.println("Configuring Wifi");
	WiFi.begin (config.ssid.c_str(), config.password.c_str());
	if (!config.dhcp)
	{
		WiFi.config(IPAddress(config.IP[0],config.IP[1],config.IP[2],config.IP[3] ),  IPAddress(config.Gateway[0],config.Gateway[1],config.Gateway[2],config.Gateway[3] ) , IPAddress(config.Netmask[0],config.Netmask[1],config.Netmask[2],config.Netmask[3] ));
	}
}






/*
**
**  NTP 
**
*/

boolean syncNtp = false;
const int NTP_PACKET_SIZE = 48; 
byte packetBuffer[ NTP_PACKET_SIZE]; 
unsigned long startMillis;
boolean ntpWait = false;
byte trying = 0;
unsigned long NTPRefresh()
{
	if (WiFi.status() == WL_CONNECTED)
	{
    if(!ntpWait) {
  		IPAddress timeServerIP; 
  		WiFi.hostByName(config.ntpServerName.c_str(), timeServerIP); 
  
  		Serial.println("sending NTP packet...");
  		memset(packetBuffer, 0, NTP_PACKET_SIZE);
  		packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  		packetBuffer[1] = 0;     // Stratum, or type of clock
  		packetBuffer[2] = 6;     // Polling Interval
  		packetBuffer[3] = 0xEC;  // Peer Clock Precision
  		packetBuffer[12]  = 49;
  		packetBuffer[13]  = 0x4E;
  		packetBuffer[14]  = 49;
  		packetBuffer[15]  = 52;
  		UDPNTPClient.beginPacket(timeServerIP, 123); 
  		UDPNTPClient.write(packetBuffer, NTP_PACKET_SIZE);
  		UDPNTPClient.endPacket();
      UDPNTPClient.flush();
      
      startMillis = millis();
      ntpWait = true;
    }

    if(millis() - startMillis > 1000) {
      startMillis = millis();

      trying ++;
      if(trying > 5) ntpWait = false;
      int cb = UDPNTPClient.parsePacket();
      if (!cb) {
        Serial.println("NTP no packet yet");
      }
      else 
      {
        Serial.print("NTP packet received, length=");
        Serial.println(cb);
        UDPNTPClient.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
        unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
        unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
        unsigned long secsSince1900 = highWord << 16 | lowWord;
        const unsigned long seventyYears = 2208988800UL;
        unsigned long epoch = secsSince1900 - seventyYears;
        UnixTimestamp = epoch;

        ntpWait = false;
        
        return epoch;
      }
    }
	}

  return 0;
}




boolean ReadConfig()
{

  Serial.println("Reading Configuration");
  if (EEPROM.read(0) == 'C' && EEPROM.read(1) == 'F'  && EEPROM.read(2) == 'G' )
  {
    Serial.println("Configurarion Found!");

    int pos = 16;
   
    config.dhcp =   EEPROM.read(pos); pos += sizeof(config.dhcp);

    config.daylight = EEPROM.read(pos); pos += sizeof(config.daylight);

    config.Update_Time_Via_NTP_Every = EEPROMReadlong(18); // 4 Byte
    config.timezone = EEPROMReadlong(22); // 4 Byte

    config.SensCalMinL = EEPROMReadint(26);
    config.SensCalMaxL = EEPROMReadint(28);
    config.SensCalcH = EEPROMReadint(30);

    config.IP[0] = EEPROM.read(32);
    config.IP[1] = EEPROM.read(33);
    config.IP[2] = EEPROM.read(34);
    config.IP[3] = EEPROM.read(35);

    config.Netmask[0] = EEPROM.read(36);
    config.Netmask[1] = EEPROM.read(37);
    config.Netmask[2] = EEPROM.read(38);
    config.Netmask[3] = EEPROM.read(39);
    config.Gateway[0] = EEPROM.read(40);
    config.Gateway[1] = EEPROM.read(41);
    config.Gateway[2] = EEPROM.read(42);
    config.Gateway[3] = EEPROM.read(43);

    config.ssid = ReadStringFromEEPROM(64);
    config.password = ReadStringFromEEPROM(96);
    config.ntpServerName = ReadStringFromEEPROM(128);
    
    config.AutoTurnOn = EEPROM.read(300);
    config.TurnOnHour = EEPROM.read(302);
    config.TurnOnMinute = EEPROM.read(303);
    config.TurnOffHour = EEPROM.read(304);
    config.TurnOffMinute = EEPROM.read(305);

    config.LEDOn = EEPROM.read(310);
    config.SensRefreshTime = EEPROM.read(311);
    config.IoTRLS= ReadStringFromEEPROM(313);

    config.IoTOn = EEPROM.read(345);
    config.IoTUserName= ReadStringFromEEPROM(346);
    config.IoTDeviceID= ReadStringFromEEPROM(378);
    config.IoTCredential= ReadStringFromEEPROM(410);
    config.IoTRHS= ReadStringFromEEPROM(342);
    config.IoTRTS= ReadStringFromEEPROM(374); 
    
    config.SensCalMinH = EEPROMReadint(406);
    config.SensCalMaxH = EEPROMReadint(408);
    config.SensCalMinT = EEPROMReadint(410);
    config.SensCalMaxT = EEPROMReadint(412);  
    config.SensCalcT = EEPROMReadint(414);     
    config.DefaultOn = EEPROM.read(316);      
    return true;
  }
  else
  {
    Serial.println("Configurarion NOT FOUND!!!!");
    return false;
  }
}





void WriteConfig()
{

  Serial.println("Writing Config");
  EEPROM.write(0,'C');
  EEPROM.write(1,'F');
  EEPROM.write(2,'G');

  EEPROM.write(16,config.dhcp);
  EEPROM.write(17,config.daylight);
  
  EEPROMWritelong(18,config.Update_Time_Via_NTP_Every); // 4 Byte
  EEPROMWritelong(22,config.timezone);  // 4 Byte

  EEPROMWriteint(26,config.SensCalMinL);
  EEPROMWriteint(28,config.SensCalMaxL);
  EEPROMWriteint(30,config.SensCalcH);

  EEPROM.write(32,config.IP[0]);
  EEPROM.write(33,config.IP[1]);
  EEPROM.write(34,config.IP[2]);
  EEPROM.write(35,config.IP[3]);

  EEPROM.write(36,config.Netmask[0]);
  EEPROM.write(37,config.Netmask[1]);
  EEPROM.write(38,config.Netmask[2]);
  EEPROM.write(39,config.Netmask[3]);

  EEPROM.write(40,config.Gateway[0]);
  EEPROM.write(41,config.Gateway[1]);
  EEPROM.write(42,config.Gateway[2]);
  EEPROM.write(43,config.Gateway[3]);

  WriteStringToEEPROM(64,config.ssid);
  WriteStringToEEPROM(96,config.password);
  WriteStringToEEPROM(128,config.ntpServerName);

  EEPROM.write(300,config.AutoTurnOn);
  EEPROM.write(302,config.TurnOnHour);
  EEPROM.write(303,config.TurnOnMinute);
  EEPROM.write(304,config.TurnOffHour);
  EEPROM.write(305,config.TurnOffMinute); 

  EEPROM.write(310,config.LEDOn);
  EEPROM.write(311,config.SensRefreshTime);
  WriteStringToEEPROM(313,config.IoTRLS);

  EEPROM.write(345,config.IoTOn);
  WriteStringToEEPROM(346,config.IoTUserName);
  WriteStringToEEPROM(378,config.IoTDeviceID);
  WriteStringToEEPROM(410,config.IoTCredential);
  WriteStringToEEPROM(342,config.IoTRHS);
  WriteStringToEEPROM(374,config.IoTRTS);
  EEPROMWriteint(406,config.SensCalMinH);
  EEPROMWriteint(408,config.SensCalMaxH);
  EEPROMWriteint(410,config.SensCalMinT);
  EEPROMWriteint(412,config.SensCalMaxT);   
  EEPROMWriteint(414,config.SensCalcT);  
  EEPROM.write(416,config.DefaultOn); 
  EEPROM.commit();
}


boolean ReadStory()
{

  Serial.println("Reading Storybook");
  if ( StoryInstances >> 0 )
  {
    Serial.println("Story Found!");

    story0.StoryOrderNo = EEPROMReadint(512);
    story0.LastStoryNo = EEPROMReadint(514);
    story0.AISOn = EEPROM.read(516);
    story0.AISmin = EEPROM.read(517);
    story0.AISmax = EEPROM.read(518);
    story0.DisplayIntensity = EEPROM.read(519);
    story0.ActionNo = EEPROMReadint(520);
    story0.Textmessage = ReadStringFromEEPROM(522);
    story0.TextSpeed = EEPROMReadint(650);
    story0.TextDirection = EEPROM.read(652);
    story0.TextOrientation = EEPROM.read(653);
    story0.DisplayTime = EEPROMReadlong(654); // 4 Byte
    story0.NextTime = EEPROMReadlong(658); // 4 Byte
    story0.InEffect = EEPROM.read(662);
    story0.OutEffect = EEPROM.read(663);
    story0.CountTime = EEPROM.read(664);
    story0.DateYear = EEPROM.read(665);
    story0.DateMonth = EEPROM.read(666);
    story0.DateDay = EEPROM.read(667);
    story0.DateHour = EEPROM.read(668);
    story0.DateMinute = EEPROM.read(669);
    story0.MQTTtopic = ReadStringFromEEPROM(670);
    story0.MQTThost = ReadStringFromEEPROM(798);
    story0.MQTTuser = ReadStringFromEEPROM(926);
    story0.MQTTpwd = ReadStringFromEEPROM(950);
    story0.MQTTport = ReadStringFromEEPROM(962);
    story0.MQTTssl = EEPROM.read(967); //1byte
    story0.MQTTsslport = ReadStringFromEEPROM(968); //5bytes 
    story0.MQTTwebsport = ReadStringFromEEPROM(973); //5bytes 
    return true;
  }
  else
  {
    Serial.println("Storybook NOT FOUND!!!!");
    return false;
  }
}

void WriteStory()
{
  Serial.println("Writing Storybook");
 
  EEPROMWriteint(512,story0.StoryOrderNo);
  EEPROMWriteint(514,story0.LastStoryNo);
  EEPROM.write(516,story0.AISOn);
  EEPROM.write(517,story0.AISmin);
  EEPROM.write(518,story0.AISmax);
  EEPROM.write(519,story0.DisplayIntensity);  
  EEPROMWriteint(520,story0.ActionNo);
  WriteStringToEEPROM(522,story0.Textmessage);
  EEPROMWriteint(650,story0.TextSpeed);
  EEPROM.write(652,story0.TextDirection);
  EEPROM.write(653,story0.TextOrientation);
  EEPROMWritelong(654,story0.DisplayTime);
  EEPROMWritelong(658,story0.NextTime);  
  EEPROM.write(662,story0.InEffect);
  EEPROM.write(663,story0.OutEffect);
  EEPROM.write(664,story0.CountTime);
  EEPROM.write(665,story0.DateYear);
  EEPROM.write(666,story0.DateMonth);
  EEPROM.write(667,story0.DateDay);
  EEPROM.write(668,story0.DateHour);
  EEPROM.write(669,story0.DateMinute);  
  WriteStringToEEPROM(670,story0.MQTTtopic); //128bytes
  WriteStringToEEPROM(798,story0.MQTThost); //128bytes
  WriteStringToEEPROM(926,story0.MQTTuser); //24bytes
  WriteStringToEEPROM(950,story0.MQTTpwd); //12bytes
  WriteStringToEEPROM(962,story0.MQTTport); //5bytes
  EEPROM.write(967,story0.MQTTssl); //1byte
  WriteStringToEEPROM(968,story0.MQTTsslport); //5bytes  
  WriteStringToEEPROM(973,story0.MQTTwebsport); //5bytes
  EEPROM.commit();
}




void SetConfig()
{
   if (!ReadConfig()) // set defaults if no configuration found before
  {
    // DEFAULT CONFIG
    config.ssid = "ArduinoGast";
    config.password = "arduinoUNO123";
    config.dhcp = true;
    config.IP[0] = 192; config.IP[1] = 168; config.IP[2] = 1; config.IP[3] = 100;
    config.Netmask[0] = 255; config.Netmask[1] = 255; config.Netmask[2] = 255; config.Netmask[3] = 0;
    config.Gateway[0] = 192; config.Gateway[1] = 168; config.Gateway[2] = 1; config.Gateway[3] = 1;
    config.ntpServerName = "0.de.pool.ntp.org";
    config.Update_Time_Via_NTP_Every = 1;
    config.timezone = 10;
    config.daylight = true;
    config.IoTOn = false;
    config.IoTUserName = "Not Named IoT User";
    config.IoTDeviceID = "Not Named IoT Device";
    config.IoTCredential = "Not Named IoT Credential";
    config.IoTRLS = "RLS";
    config.IoTRHS = "RHS";
    config.IoTRTS = "RTS";
    config.AutoTurnOn = false;
    config.TurnOffHour = 0;
    config.TurnOffMinute = 0;
    config.TurnOnHour = 0;
    config.TurnOnMinute = 0;
    config.SensCalMinL = 60;
    config.SensCalMaxL = 900;
    config.SensCalMinH = 60;
    config.SensCalMaxH = 900;
    config.SensCalMinT = 60;
    config.SensCalMaxT = 900;   
    config.SensCalcH = 100;
    config.SensCalcT = 100;    
    config.LEDOn = true; // enable switch LED on
    config.DefaultOn = false; // disable set to default configuration
    config.SensRefreshTime = 1;
    WriteConfig();
    Serial.println("General config applied");
  }

}






void SetDefaultConfig()
{
  if (config.DefaultOn)
  {
     for (int i = 0 ; i < 1024 ; i++) 
     {
      EEPROM.write(i, 0);
     }
    SetConfig(); // set default configuration
    config.DefaultOn = false;
    ConfigureWifi();
  }
}



void Second_Tick()
{
	strDateTime tempDateTime;
	AdminTimeOutCounter++;
	cNTP_Update++;
  cSens_Update++;
  cWiFi_Connect++;
	UnixTimestamp++;
	ConvertUnixTimeStamp(UnixTimestamp +  (config.timezone *  360) , &tempDateTime);
	if (config.daylight) // Sommerzeit beachten
		if (summertime(tempDateTime.year,tempDateTime.month,tempDateTime.day,tempDateTime.hour,0))
		{
			ConvertUnixTimeStamp(UnixTimestamp +  (config.timezone *  360) + 3600, &DateTime);
		}
		else
		{
			DateTime = tempDateTime;
		}
	else
	{
			DateTime = tempDateTime;
	}
	Refresh = true;
}


void sendCacheHeader() {
  server.sendHeader("Cache-Control", "public, max-age=3600", false);
}

void blinkLED () 
  {
  unsigned long currentLED = millis();
  if(currentLED - previousLED > LEDinterval) 
    {
      previousLED = currentLED;   // save the last time you blinked the LED 
    
      if ((LEDState == HIGH) && config.LEDOn && Time_On)  // if the LED switch is enabled and LED is off turn it on and reverse
        {     
   
          if (AdminEnabled)
            {
              LEDpuls = 200;
              LEDPWM = 1;
            }
          else
            {
              if (WiFi.status() == WL_CONNECTED)
                {
                LEDpuls = 1000;
                LEDPWM = 1;
                }
              if (thing)
                {
              LEDpuls = 300;
              LEDPWM = 1000;
                }
         }
      
          LEDState = LOW;
          LEDinterval = LEDpuls;
        }
      else
        {
          LEDState = HIGH;
          LEDinterval = LEDpuls * LEDPWM;
        }

      digitalWrite(BUILTIN_LED, LEDState);     // set the LED with the LEDState of the variable:
    }
}


#endif
