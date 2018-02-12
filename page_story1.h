
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
<td><input type="text" id="StoryOrderNo" name="StoryOrderNo" size="2" value="01"></td>
<hr>
<form action="" method="post">
<table border="0"  cellspacing="0" cellpadding="3" >


<tr>
  <td align="left" colspan="2"><hr></td>
</tr>

<tr>
  <td align="left" colspan="2"><b>Szenenaktion:</b></td>
  <select name="ActionNo">
  <option value="0">-keine Aktion-</option>
  <option value="1">Textinformation</option>
  <option value="2">Temperaturanzeige</option>
  <option value="3">Luftfeuchtigkeitsanzeige</option>
  <option value="4">Helligkeitsanzeige</option>
  <option value="5">Aktuelle Zeitanzeige</option>
  <option value="6">Aktuelle Datumsanzeige</option>
  <option value="7">Anzeige Zieltermin</option>
  <option value="8">Countdown Zieltermin Tage</option>
  <option value="9">Countdown Zieltermin Stunden</option>
  <option value="10">Countdown Zieltermin Minuten</option>
  <option value="11">Anzeige MQTT Topic</option>
  <option value="12">Anzeige Topic FabLab Bayreuth</option>
  <option value="13">Pfeil rechts</option>
  <option value="14">Pfeil links</option>
  <option value="15">Pfeil oben</option>
  <option value="16">Pfeil unten</option>
  <option value="17">Animation Herz</option>
  <option value="18">Animation PacMan</option>
  <option value="19">Animation Knight Rider</option>
  <option value="20">Animation Bouncing Ball</option>
  <option value="21">Animation Wurm</option>
  <option value="22">Animation Pixel</option>
  <option value="23">Animation Flash</option>
  <option value="24">Animation Vorhang</option>
</select>
</tr>

<tr>
  <td align="left" colspan="2"><b>Anzeigeninhalt:</b></td>
</tr>

<tr>
  <td align="right">Anzeigen Text:</td>
  <td><input type="text" id="Textmessage" name="Textmessage" size="128" value="...Textnachricht"></td>
</tr>

<tr>
  <td align="right"> Textgeschwindigkeit:</td>
  <td><input type="text" id="TextSpeed" name="TextSpeed" size="4" value="100"> ms</td>
</tr>

<tr>
  <td align="right"> Textrichtung:</td>
<form><td>
  <input type="radio" name="TextDirection" value="0" checked> links<br>
  <input type="radio" name="TextDirection" value="1"> rechts<br>
</form></td>
</tr>

<tr>
  <td align="right"> Textausrichtung:</td>
<form><td>
  <input type="radio" name="TextOrientation" value="0" checked> links<br>
    <input type="radio" name="TextOrientation" value="1"> Mitte<br>
  <input type="radio" name="TextOrientation" value="1"> rechts<br>
</form></td>
</tr>

<tr>
  <td align="right"> Anzeigezeit:</td>
  <td><input type="text" id="DisplayTime" name="DisplayTime" size="6" value="3000"> ms</td>
</tr>

<tr>
  <td align="right"> Pause nach Aktion:</td>
  <td><input type="text" id="NextTime" name="NextTime" size="6" value="500"> ms</td>
</tr>

<tr>
  <td align="left" colspan="2"><b>Eingangseffekt:</b></td>
  <select name="InEffect">
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
</select>
</tr>

<tr>
  <td align="left" colspan="2"><b>Ausgangseffekt:</b></td>
  <select name="OutEffect">
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
</select>
</tr>

<tr>
  <td align="left" colspan="2"><hr></td>
</tr>

<tr>
  <td align="left" colspan="2"><b>Zähler Einstellungen:</b></td>
</tr>
  <td align="right"> (Countdown)</td>

<tr>
  <td align="right"> Zähler Einheit:</td>
<form><td>
  <input type="radio" name="CountTime" value="1" checked> Sekunden<br>
  <input type="radio" name="CountTime" value="2"> Minuten<br>
  <input type="radio" name="CountTime" value="3"> Stunden<br>
  <input type="radio" name="CountTime" value="4"> Tage<br>  
</form></td>
</tr>  

<tr>
  <td align="right"> Zielzeit:</td>
  <td><input type="text" id="DateHour" name="DateHour" size="2" value="13">:<input type="text" id="DateMinute" name="DateMinute" size="2" value="00"> Uhr</td>
</tr>
<tr>
  <td align="right"> Zieldatum:</td>
  <td><input type="text" id="DateDay" name="DateDay" size="2" value="07">.<input type="text" id="DateMonth" name="DateMonth" size="2" value="04">.<input type="text" id="DateYear" name="DateYear" size="4" value="2018"></td>
</tr>

<tr>
  <td align="left" colspan="2"><hr></td>
</tr>

<tr>
  <td align="left" colspan="2"><b>IoT MQTT Inhalt:</b></td>
</tr>

<tr>
	<td align="right">MQTT Topic:</td>
	<td><input type="text" id="MQTTtopic" name="MQTTtopic" size="32" value="/FabInfo/news"></td>
</tr>
<tr>
  <td align="right"> MQTT broker host:</td>
  <td><input type="checkbox" id="MQTThost" name="MQTThost" size="32" value="...fablab-bayreuth.de"></td>
</tr>
<tr>
  <td align="right">MQTT User data:</td>
  <td><input type="text" id="MQTTuser" name="MQTTuser" value="" size="32" value="...MQTT user"></td>
</tr>
<tr>
  <td align="right">MQTT Port:</td>
  <td><input type="text" id="MQTTport" name="MQTTport" size="4" value="1883"></td>
</tr>

<tr>
  <td align="right"> SSL aktiv:</td>
  <td><input type="checkbox" id="MQTTssl" name="MQTTssl"></td>
</tr>

<tr>
  <td align="right">MQTT SSL Port:</td>
  <td><input type="text" id="MQTTsslport" name="MQTTsslport" size="4" value="8883"></td>
</tr>

<tr>
  <td align="right">TSL Websocket Port:</td>
  <td><input type="text" id="MQTTwebsport" name="MQTTwebsport" size="4" value="8883"></td>
</tr>


<tr>
  <td align="left" colspan="2"><hr></td>
</tr>

<tr>
  <td align="left" colspan="2"><b>Anzeigeneinstellung:</b></td>
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

<tr>
  <td align="left" colspan="2"><hr></td>
</tr>


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
