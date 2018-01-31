#ifndef PAGE_INFOMATION_H
#define PAGE_INFOMATION_H


//
//   The HTML PAGE
//
const char PAGE_Information[] PROGMEM = R"=====(
<meta name="viewport" content="width=device-width, initial-scale=1" />
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<link rel="stylesheet" href="style.css" type="text/css" />
<script src="microajax.js"></script> 
<a href="admin.html"  class="btn btn--s"><</a>&nbsp;&nbsp;<strong>FabInfo Informationen</strong>
<hr>
<table border="0"  cellspacing="0" cellpadding="3" style="width:310px" >
<tr><td align="right">SSID :</td><td><span id="x_ssid"></span></td></tr>
<tr><td align="right">IP :</td><td><span id="x_ip"></span></td></tr>
<tr><td align="right">Netmask :</td><td><span id="x_netmask"></span></td></tr>
<tr><td align="right">Gateway :</td><td><span id="x_gateway"></span></td></tr>
<tr><td align="right">Mac :</td><td><span id="x_mac"></span></td></tr>

<tr><td colspan="2"><hr></span></td></tr>
<tr><td align="right">NTP Zeit:</td><td><span id="x_ntpt"></span></td></tr>
<tr><td align="right">NTP Datum:</td><td><span id="x_ntpd"></span></td></tr>

<tr><td colspan="2"><hr></span></td></tr>
<tr><td align="right">SW Version:</td><td>V<span id="x_version">...</span></td></tr>
<tr><td align="middle">by JTL & thirsch</td><td align="middle">FabLab Bayreuth 2018</td>
<tr><img align="left" width="235" height="75" alt="FLBT Logo" src="data:image/png;base64,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"</td><tr><img align="right"width="114" height="75" alt="by JTL Logo" src="data:image/png;base64,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"</td>
<tr><td colspan="2" align="center"><a href="javascript:GetState()" class="btn btn--m btn--red">AKTUALISIEREN</a></td></tr>
<tr><td colspan="2"><hr></span></td></tr>
</table>
<script>

function GetState()
{
	setValues("/admin/sensrefreshtimes");
}

window.onload = function ()
{
	load("style.css","css", function() 
	{
		load("microajax.js","js", function() 
		{
				GetState();
		});
	});
}
function load(e,t,n){if("js"==t){var a=document.createElement("script");a.src=e,a.type="text/javascript",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}else if("css"==t){var a=document.createElement("link");a.href=e,a.rel="stylesheet",a.type="text/css",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}}



</script>
)=====" ;


//
// FILL WITH INFOMATION
// 

void send_information_values_html ()
{

	String values ="";

	values += "x_ssid|" + (String)WiFi.SSID() +  "|div\n";
	values += "x_ip|" +  (String) WiFi.localIP()[0] + "." +  (String) WiFi.localIP()[1] + "." +  (String) WiFi.localIP()[2] + "." + (String) WiFi.localIP()[3] +  "|div\n";
	values += "x_gateway|" +  (String) WiFi.gatewayIP()[0] + "." +  (String) WiFi.gatewayIP()[1] + "." +  (String) WiFi.gatewayIP()[2] + "." + (String) WiFi.gatewayIP()[3] +  "|div\n";
	values += "x_netmask|" +  (String) WiFi.subnetMask()[0] + "." +  (String) WiFi.subnetMask()[1] + "." +  (String) WiFi.subnetMask()[2] + "." + (String) WiFi.subnetMask()[3] +  "|div\n";
	values += "x_mac|" + GetMacAddress() +  "|div\n";
  values += "x_version|" + (String)PGNV + "|div\n";
  char buffer[50];
 
  sprintf(buffer, "x_ntpt|%02d:%02d:%02d|div\n", DateTime.hour, DateTime.minute, DateTime.second);
  values += buffer;

  sprintf(buffer, "x_ntpd|%02d.%02d.%04d|div\n", DateTime.day, DateTime.month, DateTime.year);
  values += buffer;

  server.send ( 200, "text/plain", values);

	Serial.println(__FUNCTION__);
}


#endif
