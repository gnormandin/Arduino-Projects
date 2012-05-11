#include <SPI.h>
#include <Wire.h>
#include <WiFlyHQ.h>
#include <Si4735-private.h>
#include <Si4735.h>
WiFly wifly;
Si4735 radio;
Si4735RDSDecoder decoder;
char input;
char command;
byte mode, status;
word frequency, rdsblock[4];
bool goodtune;
Si4735_RX_Metrics RSQ;
Si4735_RDS_Data station;
Si4735_RDS_Time rdstime;
char FW[3], REV;


void sendIndex();
void sendRadioData();
void parseInput(char *name);
void send404();
char buf[80];

void setup()
{
    

    Serial.begin(9600);
    radio.begin(SI4735_MODE_FM);
    wifly.begin(&Serial, NULL);
    wifly.setBroadcastInterval(0);	// Turn off UPD broadcast
    wifly.setDeviceID("Wifly-WebServer");
    wifly.setProtocol(WIFLY_PROTOCOL_TCP);
    if (wifly.getPort() != 80) {
        wifly.setPort(80);
	/* local port does not take effect until the WiFly has rebooted (2.32) */
	wifly.save();
		wifly.reboot();
	delay(3000);
    }
}

void loop()
{
    if (wifly.available() > 0) {

        /* See if there is a request */
	if (wifly.gets(buf, sizeof(buf))) {
	    if (strncmp_P(buf, PSTR("GET / "), 6) == 0) {
		/* GET request */
                //check to see what page is being requested, respond with page
		while (wifly.gets(buf, sizeof(buf)) > 0) {
		    /* Skip rest of request */
		}
		sendIndex();
	    } else if (strncmp_P(buf, PSTR("GET /up? "), 6) == 0) {
	       wifly.flushRx();		// discard rest of input
                radio.seekUp();
		sendIndex();
	    } 
               else if (strncmp_P(buf, PSTR("GET /down? "), 6) == 0) {
	       wifly.flushRx();		// discard rest of input
                radio.seekDown();
		sendIndex();
	    }
                else if (strncmp_P(buf, PSTR("GET /hjy? "), 6) == 0) {
	       wifly.flushRx();		// discard rest of input
                radio.setFrequency(9410);
		sendIndex();
	    }
                else {
	        /* Unexpected request */
		wifly.flushRx();		// discard rest of input
		send404();
                
	    }
	}
    }
}

/** Send an index HTML page with an input box for a username */
void sendIndex()
{
    /* Send the header direclty with print */
    wifly.println(F("HTTP/1.1 200 OK"));
    wifly.println(F("Content-Type: text/html"));
    wifly.println(F("Transfer-Encoding: chunked"));
    wifly.println();
    /* Send the body using the chunked protocol so the client knows when
     * the message is finished.
     * Note: we're not simply doing a close() because in version 2.32
     * firmware the close() does not work for client TCP streams.
     */
    wifly.sendChunkln(F("<html>"));
    wifly.sendChunkln(F("<title>Fm Radio</title>"));
    wifly.sendChunkln(F("<h1>"));
    wifly.sendChunkln(F("<center><p>Stupid FM radio</p></center>"));
    wifly.sendChunkln(F("</h1>"));
    wifly.sendChunkln(F("<FORM METHOD=\"LINK\" ACTION=\"up\"><INPUT TYPE=\"submit\" VALUE=\"Seek Up\"></FORM>"));
    wifly.sendChunkln(F("<FORM METHOD=\"LINK\" ACTION=\"down\"><INPUT TYPE=\"submit\" VALUE=\"Seek Down\"></FORM>"));
    wifly.sendChunkln(F("<FORM METHOD=\"LINK\" ACTION=\"hjy\"><INPUT TYPE=\"submit\" VALUE=\"94.1 HJY\"></FORM>"));
    wifly.sendChunkln(F("</html>"));
    wifly.sendChunkln();
}



/** Send a 404 error */
void send404()
{
    wifly.println(F("HTTP/1.1 404 Not Found"));
    wifly.println(F("Content-Type: text/html"));
    wifly.println(F("Transfer-Encoding: chunked"));
    wifly.println();
    wifly.sendChunkln(F("<html><head>"));
    wifly.sendChunkln(F("<title>404 Not Found, but it works</title>"));
    wifly.sendChunkln(F("</head><body>"));
    
    wifly.sendChunkln(F("<h1>Not Found</h1>"));
    wifly.sendChunkln(F("<hr>"));
    wifly.sendChunkln(F("</body></html>"));
   wifly.sendChunkln();
    
}


