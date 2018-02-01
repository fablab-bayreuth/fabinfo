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
	<td align="right">Gerätename:</td>
	<td><input type="text" id="devicename" name="devicename" value=""></td>
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
	<td align="left" colspan="2"><hr></td>
</tr>

<tr>
	<td align="left" colspan="2">FabInfo IoT Zeitsteuerung:</td>
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
	<td align="left" colspan="2">Sensoreinstellung:</td>
</tr>

<tr>
	<td align="right"> Sensor auslesen:</td>
	<td><input type="text" id="sensrefreshtime" name="sensrefreshtime" size="3" value="1"> Minuten (0=deaktiv)</td>
</tr>

<tr>
  <td align="left" colspan="2"><hr></td>
</tr>

<tr>
  <td align="left" colspan="2">Sensor Kalibrierung:</td>
</tr>

<tr>
	<td align="right"> Messwert für Maximalanzeige:</td>
	<td><input type="text" id="senscalmax" name="senscalmax" size="3" value="900"></td>
</tr>

<tr>
  <td align="right"> Messwert für Minimalanzeige:</td>
  <td><input type="text" id="senscalmin" name="senscalmin" size="3" value="50"></td>
</tr>

<tr>
  <td align="right"> Kalibrierfaktor:</td>
  <td><input type="text" id="senscalc" name="senscalc" size="3" value=""></td>
</tr>

<tr>
  <td align="left" colspan="2"><hr></td>
</tr>

<tr>
  <td align="left" colspan="2">LED Anzeige:</td>
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
void send_devicename_value_html()
{
		
	String values ="";
	values += "devicename|" + (String) config.DeviceName + "|div\n";
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
			if (server.argName(i) == "devicename") config.DeviceName = urldecode(server.arg(i)); 
      if (server.argName(i) == "iotenabled") config.IoTOn = true; 
      if (server.argName(i) == "iotusername") config.IoTUserName = urldecode(server.arg(i)); 
      if (server.argName(i) == "iotdeviceid") config.IoTDeviceID = urldecode(server.arg(i)); 
      if (server.argName(i) == "iotcredential") config.IoTCredential = urldecode(server.arg(i)); 
 			if (server.argName(i) == "tonenabled") config.AutoTurnOn = true; 
			if (server.argName(i) == "tonhour") config.TurnOnHour =  server.arg(i).toInt(); 
			if (server.argName(i) == "tonminute") config.TurnOnMinute =  server.arg(i).toInt(); 
			if (server.argName(i) == "toffhour") config.TurnOffHour =  server.arg(i).toInt(); 
			if (server.argName(i) == "toffminute") config.TurnOffMinute =  server.arg(i).toInt(); 
			if (server.argName(i) == "ledenabled") config.LEDOn = true; // switch enabled status LED
			if (server.argName(i) == "sensrefreshtime") config.SensRefreshTime =  server.arg(i).toInt(); 
			if (server.argName(i) == "senscalmax") config.SensCalMax =  server.arg(i).toInt(); 
      if (server.argName(i) == "senscalmin") config.SensCalMin =  server.arg(i).toInt();  
      if (server.argName(i) == "senscalc") config.SensCalc =  server.arg(i).toInt();         
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
	values += "devicename|" +  (String)  config.DeviceName +  "|input\n";
  values += "iotenabled|" +  (String) (config.IoTOn ? "checked" : "") + "|chk\n";
  values += "iotusername|" +  (String)  config.IoTUserName +  "|input\n";
  values += "iotdeviceid|" +  (String)  config.IoTDeviceID +  "|input\n";
  values += "iotcredential|" +  (String)  config.IoTCredential +  "|input\n"; 
	values += "tonhour|" +  (String)  config.TurnOnHour +  "|input\n";
	values += "tonminute|" +   (String) config.TurnOnMinute +  "|input\n";
	values += "toffhour|" +  (String)  config.TurnOffHour +  "|input\n";
	values += "toffminute|" +   (String)  config.TurnOffMinute +  "|input\n";
	values += "tonenabled|" +  (String) (config.AutoTurnOn ? "checked" : "") + "|chk\n";
	values += "ledenabled|" +  (String) (config.LEDOn ? "checked" : "") + "|chk\n";
	values += "sensrefreshtime|" +   (String)  config.SensRefreshTime +  "|input\n";
	values += "senscalmax|" +   (String)  config.SensCalMax +  "|input\n";
  values += "senscalmin|" +   (String)  config.SensCalMin +  "|input\n";
  values += "senscalc|" +   (String)  config.SensCalc +  "|input\n";
	server.send ( 200, "text/plain", values);
	Serial.println(__FUNCTION__); 
}
