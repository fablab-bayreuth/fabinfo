
//
//  HTML PAGE
//

const char PAGE_Story1Settings[] PROGMEM =  R"=====(
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1" />
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<link href="style.css" rel="stylesheet" type="text/css" />
</head>
<body>
<a href="admin.html"  class="btn btn--red">ZURÜCK</a>&nbsp;&nbsp;<strong>Szene 01</strong>&nbsp;&nbsp;
<a href="admin.html"  class="btn btn--s"><</a>&nbsp;
<a href="story2.html"  class="btn btn--s">></a>
<td align="right">Ordnungsnummer:</td>
<td><input type="text" id="StoryOrderNo" name="StoryOrderNo" value=""></td>
<hr>
<form action="" method="post">
<table border="0"  cellspacing="0" cellpadding="3" >

<tr>
  <td align="left" colspan="2"><hr></td>
</tr>

<tr>
  <td align="left" colspan="2">Anzeigeneinstellung:</td>
</tr>

<tr>
  <td align="right"> Automatische Helligkeitsregelung aktiv:</td>
  <td><input type="checkbox" id="AISOn" name="AISOn"></td>
</tr>

<tr>
  <td align="right"> Minimalwert Helligkeitssensor:</td>
  <td><input type="text" id="AISmin" name="AISmin" size="2" value="06"></td>
</tr>

<tr>
  <td align="right"> Maximalwert Helligkeitssensor:</td>
  <td><input type="text" id="AISmax" name="AISmax" size="2" value="15"></td>
</tr>

<tr>
  <td align="right"> Statische Anzeigenhelligkeit:</td>
  <td><input type="text" id="DisplayIntensity" name="DisplayIntensity" size="2" value="10"></td>
</tr>

<select>
  <option ActionNo="0">-keine Aktion-</option>
  <option ActionNo="1">Textinformation</option>
  <option ActionNo="2">Temperaturanzeige</option>
  <option ActionNo="3">Luftfeuchtigkeitsanzeige</option>
  <option ActionNo="4">Helligkeitsanzeige</option>
  <option ActionNo="5">Aktuelle Zeitanzeige</option>
  <option ActionNo="6">Aktuelle Datumsanzeige</option>
  <option ActionNo="7">Anzeige Zieltermin</option>
  <option ActionNo="8">Countdown Zieltermin Tage</option>
  <option ActionNo="9">Countdown Zieltermin Stunden</option>
  <option ActionNo="10">Countdown Zieltermin Minuten</option>
  <option ActionNo="11">Anzeige MQTT Topic</option>
  <option ActionNo="12">Anzeige Topic FabLab Bayreuth</option>
  <option ActionNo="13">Pfeil rechts</option>
  <option ActionNo="14">Pfeil links</option>
  <option ActionNo="15">Pfeil oben</option>
  <option ActionNo="16">Pfeil unten</option>
  <option ActionNo="17">Animation Herz</option>
  <option ActionNo="18">Animation PacMan</option>
  <option ActionNo="19">Animation Knight Rider</option>
  <option ActionNo="20">Animation Bouncing Ball</option>
  <option ActionNo="21">Animation Wurm</option>
  <option ActionNo="22">Animation Pixel</option>
  <option ActionNo="23">Animation Flash</option>
  <option ActionNo="24">Animation Vorhang</option>
</select>

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
	<td><input type="text" id="senscalmaxL" name="senscalmaxL" size="3" value="900"></td>
</tr>

<tr>
  <td align="right"> Messwert für Minimalanzeige:</td>
  <td><input type="text" id="senscalminL" name="senscalminL" size="3" value="50"></td>
</tr>

<tr>
  <td align="right"> Kalibrierfaktor:</td>
  <td><input type="text" id="senscalcH" name="senscalcH" size="3" value=""></td>
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
	  setValues("/admin/story1values");
}

</script>
</body>
</html>
)=====";




void send_story1_html()
{
	
	if (server.args() > 0 )  // Save Settings
	{
		config.AutoTurnOn = false;
    config.IoTOn = false;
    config.LEDOn = false;
    
		String temp = "";
		for ( uint8_t i = 0; i < server.args(); i++ ) {

		}
		WriteConfig();
    ConfigureIoT();
		firstStart = true;
    Sens_Value = -1;
	}

	server.send_P ( 200, "text/html", PAGE_Story1Settings ); 
	Serial.println(__FUNCTION__); 
	
	
}

void send_story1_configuration_values_html()
{

}
