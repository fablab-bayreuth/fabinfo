//
//  HTML PAGE
//

const char PAGE_AdminGeneralSettings[] PROGMEM =  R"=====(
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1" />
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<link href="style.css" rel="stylesheet" type="text/css" />
</head>
<body>
<a href="admin.html"  class="btn btn--red">ZURÜCK</a>&nbsp;&nbsp;<strong>FabInfo Konfiguration</strong>
<hr>
<form action="" method="post">
<table border="0"  cellspacing="0" cellpadding="3" >

<tr>
  <td align="left" colspan="2"><b>FabInfo IoT Konfiguration:</b></td>
</tr>
<tr>
  <td align="right"> IoT Client aktiv:</td>
  <td><input type="checkbox" id="iotenabled" name="iotenabled"></td>
</tr>
<tr>
  <td align="right">IoT User Name:</td>
  <td><input type="text" id="iotusername" name="iotusername" value=""></td>
</tr>
<tr>
  <td align="right">IoT Device ID:</td>
  <td><input type="text" id="iotdeviceid" name="iotdeviceid" value=""></td>
</tr>
<tr>
  <td align="right">IoT Credential:</td>
  <td><input type="text" id="iotcredential" name="iotcredential" value=""></td>
</tr>
<tr>
  <td align="right">IoT Resource Light Sensor:</td>
  <td><input type="text" id="IoTRLS" name="IoTRLS" value=""></td>
</tr>
<tr>
  <td align="right">IoT Resource Humidity Sensor:</td>
  <td><input type="text" id="IoTRHS" name="IoTRHS" value=""></td>
</tr>
<tr>
  <td align="right">IoT Resource Temperatur Sensor:</td>
  <td><input type="text" id="IoTRTS" name="IoTRTS" value=""></td>
</tr>
<tr>
	<td align="left" colspan="2"><hr></td>
</tr>

<tr>
	<td align="left" colspan="2"><b>Sensoreinstellungen:</b></td>
</tr>

<tr>
	<td align="right"> Sensoren auslesen:</td>
	<td><input type="text" id="sensrefreshtime" name="sensrefreshtime" size="3" value="1"> Minuten (0=deaktiv)</td>
</tr>

<tr>
  <td align="left" colspan="2"><hr></td>
</tr>

<tr>
  <td align="left" colspan="2">Helligkeit Sensor Kalibrierung:</td>
</tr>

<tr>
	<td align="right"> Messwert für Maximalanzeige:</td>
	<td><input type="text" id="senscalmaxL" name="senscalmaxL" size="3" value="900"></td>
</tr>

<tr>
  <td align="right"> Messwert für Minimalanzeige:</td>
  <td><input type="text" id="senscalminL" name="senscalminL" size="3" value="50"></td>
</tr>

<tr>
  <td align="left" colspan="2"><hr></td>
</tr>

<tr>
  <td align="left" colspan="2">Luftfeuchtigkeits Sensor Kalibrierung:</td>
</tr>

<tr>
  <td align="right"> Messwert für Maximalanzeige:</td>
  <td><input type="text" id="senscalmaxH" name="senscalmaxH" size="3" value="900"></td>
</tr>

<tr>
  <td align="right"> Messwert für Minimalanzeige:</td>
  <td><input type="text" id="senscalminH" name="senscalminH" size="3" value="50"></td>
</tr>

<tr>
  <td align="right"> Kalibrierfaktor:</td>
  <td><input type="text" id="senscalcH" name="senscalcH" size="3" value=""></td>
</tr>

<tr>
  <td align="left" colspan="2"><hr></td>
</tr>

<tr>
  <td align="left" colspan="2">Temperatur Sensor Kalibrierung:</td>
</tr>

<tr>
  <td align="right"> Messwert für Maximalanzeige:</td>
  <td><input type="text" id="senscalmaxT" name="senscalmaxT" size="3" value="900"></td>
</tr>

<tr>
  <td align="right"> Messwert für Minimalanzeige:</td>
  <td><input type="text" id="senscalminT" name="senscalminT" size="3" value="50"></td>
</tr>

<tr>
  <td align="right"> Kalibrierfaktor:</td>
  <td><input type="text" id="senscalcT" name="senscalcT" size="3" value=""></td>
</tr>

<tr>
  <td align="left" colspan="2"><hr></td>
</tr>

<tr>
  <td align="left" colspan="2"><b>FabInfo IoT Zeitsteuerung:</b></td>
</tr>
<tr>
  <td align="right"> IoT Zeitsteuerung aktiv:</td>
  <td><input type="checkbox" id="tonenabled" name="tonenabled"></td>
</tr>

<tr>
  <td align="right"> Einschaltzeit:</td>
  <td><input type="text" id="tonhour" name="tonhour" size="2" value="00">:<input type="text" id="tonminute" name="tonminute" size="2" value="00"></td>
</tr>
<tr>
  <td align="right"> Ausschaltzeit:</td>
  <td><input type="text" id="toffhour" name="toffhour" size="2" value="00">:<input type="text" id="toffminute" name="toffminute" size="2" value="00"></td>
</tr>

<tr>
  <td align="left" colspan="2"><hr></td>
</tr>

<tr>
  <td align="left" colspan="2"><b>Blaue LED Anzeige:</b></td>
</tr>

<tr>
  <td align="right"> LED Status aktiv:</td>
  <td><input type="checkbox" id="ledenabled" name="ledenabled"></td>
</tr>

<tr><td colspan="2" align="center"><input type="submit" style="width:150px" class="btn btn--m btn--red" value="SPEICHERN"></td></tr>


</table>
</form>

<script src="microajax.js"></script>
<script>

window.onload = function ()
{
	  setValues("/admin/generalvalues");
}

</script>
</body>
</html>
)=====";


// Functions for this Page
void send_IoTRLS_value_html()
{
		
	String values ="";
	values += "IoTRLS|" + (String) config.IoTRLS + "|div\n";
	server.send ( 200, "text/plain", values);
	Serial.println(__FUNCTION__); 
	
}

void send_general_html()
{
	
	if (server.args() > 0 )  // Save Settings
	{
		config.AutoTurnOn = false;
    config.IoTOn = false;
    config.LEDOn = false;
    
		String temp = "";
		for ( uint8_t i = 0; i < server.args(); i++ ) {
      if (server.argName(i) == "iotenabled") config.IoTOn = true; 
      if (server.argName(i) == "iotusername") config.IoTUserName = urldecode(server.arg(i)); 
      if (server.argName(i) == "iotdeviceid") config.IoTDeviceID = urldecode(server.arg(i)); 
      if (server.argName(i) == "iotcredential") config.IoTCredential = urldecode(server.arg(i)); 
      if (server.argName(i) == "IoTRLS") config.IoTRLS = urldecode(server.arg(i)); 
      if (server.argName(i) == "IoTRHS") config.IoTRHS = urldecode(server.arg(i)); 
      if (server.argName(i) == "IoTRTS") config.IoTRTS = urldecode(server.arg(i));       
 			if (server.argName(i) == "tonenabled") config.AutoTurnOn = true; 
			if (server.argName(i) == "tonhour") config.TurnOnHour =  server.arg(i).toInt(); 
			if (server.argName(i) == "tonminute") config.TurnOnMinute =  server.arg(i).toInt(); 
			if (server.argName(i) == "toffhour") config.TurnOffHour =  server.arg(i).toInt(); 
			if (server.argName(i) == "toffminute") config.TurnOffMinute =  server.arg(i).toInt(); 
			if (server.argName(i) == "ledenabled") config.LEDOn = true; // switch enabled status LED
			if (server.argName(i) == "sensrefreshtime") config.SensRefreshTime =  server.arg(i).toInt(); 
			if (server.argName(i) == "senscalmaxL") config.SensCalMaxL =  server.arg(i).toInt(); 
      if (server.argName(i) == "senscalminL") config.SensCalMinL =  server.arg(i).toInt(); 
      if (server.argName(i) == "senscalmaxH") config.SensCalMaxH =  server.arg(i).toInt();  
      if (server.argName(i) == "senscalminH") config.SensCalMinH =  server.arg(i).toInt();     
      if (server.argName(i) == "senscalmaxT") config.SensCalMaxT =  server.arg(i).toInt();  
      if (server.argName(i) == "senscalminT") config.SensCalMinT =  server.arg(i).toInt();   
      if (server.argName(i) == "senscalcH") config.SensCalcH =  server.arg(i).toInt();   
      if (server.argName(i) == "senscalcT") config.SensCalcT =  server.arg(i).toInt();                
		}
		WriteConfig();
    ConfigureIoT();
		firstStart = true;
    Sens_Value = -1;
	}

	server.send_P ( 200, "text/html", PAGE_AdminGeneralSettings ); 
	Serial.println(__FUNCTION__); 
	
	
}

void send_general_configuration_values_html()
{
	String values ="";
  values += "iotenabled|" +  (String) (config.IoTOn ? "checked" : "") + "|chk\n";
  values += "iotusername|" +  (String)  config.IoTUserName +  "|input\n";
  values += "iotdeviceid|" +  (String)  config.IoTDeviceID +  "|input\n";
  values += "iotcredential|" +  (String)  config.IoTCredential +  "|input\n"; 
  values += "IoTRLS|" +  (String)  config.IoTRLS +  "|input\n";
  values += "IoTRHS|" +  (String)  config.IoTRHS +  "|input\n";
  values += "IoTRTS|" +  (String)  config.IoTRTS +  "|input\n";
	values += "tonhour|" +  (String)  config.TurnOnHour +  "|input\n";
	values += "tonminute|" +   (String) config.TurnOnMinute +  "|input\n";
	values += "toffhour|" +  (String)  config.TurnOffHour +  "|input\n";
	values += "toffminute|" +   (String)  config.TurnOffMinute +  "|input\n";
	values += "tonenabled|" +  (String) (config.AutoTurnOn ? "checked" : "") + "|chk\n";
	values += "ledenabled|" +  (String) (config.LEDOn ? "checked" : "") + "|chk\n";
	values += "sensrefreshtime|" +   (String)  config.SensRefreshTime +  "|input\n";
	values += "senscalmaxL|" +   (String)  config.SensCalMaxL +  "|input\n";
  values += "senscalminL|" +   (String)  config.SensCalMinL +  "|input\n";
  values += "senscalmaxH|" +   (String)  config.SensCalMaxH +  "|input\n";
  values += "senscalminH|" +   (String)  config.SensCalMinH +  "|input\n";
  values += "senscalmaxT|" +   (String)  config.SensCalMaxT +  "|input\n";
  values += "senscalminT|" +   (String)  config.SensCalMinT +  "|input\n";    
  values += "senscalcH|" +   (String)  config.SensCalcH +  "|input\n";
  values += "senscalcT|" +   (String)  config.SensCalcT +  "|input\n";  
	server.send ( 200, "text/plain", values);
	Serial.println(__FUNCTION__); 
}
