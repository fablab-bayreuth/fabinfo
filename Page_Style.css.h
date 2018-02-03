
//
//  STYLE PAGE
//

const char PAGE_Style_css[] PROGMEM = R"=====(
body { color: #000000; font-family: avenir, helvetica, arial, sans-serif;  letter-spacing: 0.15em;} 
hr {    background-color: #ffffff;    border: 0 none;   color: #ffffff;    height: 1px; } 
.btn, .btn:link, .btn:visited {  
	border-radius: 0.3em;  
	border-style: solid;  
	border-width: 1px;  
color: #111;  
display: inline-block;  
	font-family: avenir, helvetica, arial, sans-serif;  
	letter-spacing: 0.15em;  
	margin-bottom: 0.5em;  
padding: 1em 0.75em;  
	text-decoration: none;  
	text-transform: uppercase;  
	-webkit-transition: color 0.8s, background-color 0.4s, border 0.4s;  
transition: color 0.8s, background-color 0.4s, border 0.4s; 
} 
.btn:hover, .btn:focus {
color: #cd2900;  
border: 1px solid #6f0000;  
	-webkit-transition: background-color 0.3s, color 0.3s, border 0.3s;  
transition: background-color 0.3s, color 0.3s, border 0.3s; 
}
	.btn:active {  
color: #cd2900;  
border: 1px solid #6f0000;  
		-webkit-transition: background-color 0.3s, color 0.3s, border 0.3s;  
transition: background-color 0.3s, color 0.3s, border 0.3s; 
	} 
	.btn--s 
	{  
		font-size: 12px; 
	}
	.btn--m { 
		font-size: 14px; 
	}
	.btn--l {  
		font-size: 20px;  border-radius: 0.25em !important; 
	} 
	.btn--full, .btn--full:link {
		border-radius: 0.25em; 
display: block;  
			margin-left: auto; 
			margin-right: auto; 
			text-align: center; 
width: 100%; 
	} 
	.btn--red:link, .btn--red:visited {
color: #fff;  
		background-color: #6f0000; 
	}
	.btn--red:hover, .btn--red:focus {
color: #fff !important;  
		background-color: #e65208;  
		border-color: #6f0000; 
	}
	.btn--red:active {
color: #fff; 
		background-color: #e65208;  border-color: #e65208; 
	}
	@media screen and (min-width: 32em) {
		.btn--full {  
			max-width: 16em !important; } 
	}
)=====";
 
