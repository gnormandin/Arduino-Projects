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
	    } else if (strncmp_P(buf, PSTR("POST"), 4) == 0) {
	        /* Form POST */
	        char buttonpress[16];
		/* Get posted field value */
		if (wifly.match(F("user="))) {
		    wifly.gets(buttonpress, sizeof(buttonpress));
		    wifly.flushRx();		// discard rest of input
		 //   parseInput(buttonpress);
                      
		}
	    } else {
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
    wifly.sendChunkln(F("<title>WiFly HTTP Server Example</title>"));
    wifly.sendChunkln(F("<h1>"));
    wifly.sendChunkln(F("<p>Hello</p>"));
    wifly.sendChunkln(F("</h1>"));
    wifly.sendChunkln(F("<form name=\"input\" action=\"/\" method=\"post\">"));
    wifly.sendChunkln(F("Username:"));
    wifly.sendChunkln(F("<input type=\"text\" name=\"user\" />"));
    wifly.sendChunkln(F("<input type=\"submit\" value=\"Submit\" />"));
    wifly.sendChunkln(F("</form>")); 
    wifly.sendChunkln(F("</html>"));
    wifly.sendChunkln();
}

/** Send a greeting HTML page with the user's name and an analog reading */
void sendRadioData()
{
    /* Send the header directly with print */
    wifly.println(F("HTTP/1.1 200 OK"));
    wifly.println(F("Content-Type: text/html"));
    wifly.println(F("Transfer-Encoding: chunked"));
    wifly.println();

    /* Send the body using the chunked protocol so the client knows when
     * the message is finished.
     */
    wifly.sendChunkln(F("<html>"));
    wifly.sendChunkln(F("<title>WiFly HTTP Server Example</title>"));
    /* No newlines on the next parts */
    wifly.sendChunk(F("<h1><p>Hello "));
    wifly.sendChunkln();
    /* Finish the paragraph and heading */
    wifly.sendChunkln(F("</p></h1>"));

    /* Include a reading from Analog pin 0 */
    snprintf_P(buf, sizeof(buf), PSTR("<p>Analog0=%d</p>"), analogRead(A0));
    wifly.sendChunkln(buf);

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

//void parseInput(char *input)
//{
// //check passed input and perform radio operations
//switch(input){
//      case 'v': 
//        radio.volumeDown();
//        break;
//      case 'V':
//        radio.volumeUp();
//        break;
//      case 's': 
//        radio.seekDown();
//        break;
//      case 'S': 
//        radio.seekUp();
//        break;
//      case 'm': 
//        radio.mute();
//        break;
//      case 'M': 
//        radio.unMute();
//        break;
//      
//      case 'L':
//      case 'A':
//      case 'W':
//      case 'F': 
//        Serial.print("Switching mode to ");
//        switch(command) {
//          case 'L': 
//            Serial.println("LW");
//            radio.setMode(SI4735_MODE_LW);
//            break;
//          case 'A': 
//            Serial.println("AM");
//            radio.setMode(SI4735_MODE_AM);
//            break;
//          case 'W': 
//            Serial.println("SW");
//            radio.setMode(SI4735_MODE_SW);
//            break;
//          case 'F': 
//            Serial.println("FM");
//            radio.setMode(SI4735_MODE_FM);
//            break;
//        }
//        Serial.flush();        
//        break;
//      
//      case 'R': 
//        if(radio.isRDSCapable()) {
//          decoder.getRDSData(&station);
//          Serial.println("RDS information {");
//          Serial.print("PI: ");
//          Serial.println(station.programIdentifier, HEX);
//          Serial.print("PTY: ");
//          Serial.println(station.PTY);
//          Serial.println("DI {");
//          if(station.DICC & SI4735_RDS_DI_DYNAMIC_PTY)
//            Serial.println("* Dynamic PTY");
//          if(station.DICC & SI4735_RDS_DI_COMPRESSED)
//            Serial.println("* Audio Compression");
//          if(station.DICC & SI4735_RDS_DI_ARTIFICIAL_HEAD)
//            Serial.println("* Artificial Head Recording");
//          if(station.DICC & SI4735_RDS_DI_STEREO)
//            Serial.println("* Stereo Encoding");
//          Serial.println("}");
//          if(station.TP) Serial.println("Traffic programme carried");
//          if(station.TA) Serial.println("Traffic announcement underway");
//          Serial.print("Currently broadcasting ");
//          if(station.MS) Serial.println("music");
//          else Serial.println("speech");
//          Serial.print("PS: ");
//          Serial.println(station.programService);
//          Serial.print("PTYN: ");
//          Serial.println(station.programTypeName);
//          Serial.print("RT: ");
//          Serial.println(station.radioText);
//          Serial.println("}");
//        } else Serial.println("RDS not available.");       
//        break;
//}

//}
