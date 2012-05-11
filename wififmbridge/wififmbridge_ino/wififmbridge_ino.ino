/*
  Web Server
 
 A simple web server that shows the value of the analog input pins.
 using an Arduino Wiznet Ethernet shield. 
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)
 
 created 18 Dec 2009
 by David A. Mellis
 modified 4 Sep 2010
 by Tom Igoe
 
 */

#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,0, 177);

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(80);

void setup()
{
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
}

void loop()
{
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          client.println("<html>");
          client.println("<head><meta name=\"viewport\" content=\"initial-scale = 1.60,maximum-scale = 1.60\" /><meta name=\"apple-mobile-web-app-capable\" content=\"yes\" /><title>Radio Control</title>");
          client.println("<style type=\"text/css\"> button.punch {background: #4162a8;border-top: 1px solid #38538c;border-right: 1px solid #1f2d4d;border-bottom: 1px solid #151e33;border-left: 1px solid #1f2d4d;-webkit-border-radius: 4px;-moz-border-radius: 4px;border-radius: 4px;-webkit-box-shadow: inset 0 1px 10px 1px #5c8bee, 0px 1px 0 #1d2c4d, 0 6px 0px #1f3053, 0 8px 4px 1px #111111;-moz-box-shadow: inset 0 1px 10px 1px #5c8bee, 0px 1px 0 #1d2c4d, 0 6px 0px #1f3053, 0 8px 4px 1px #111111;box-shadow: inset 0 1px 10px 1px #5c8bee, 0px 1px 0 #1d2c4d, 0 6px 0px #1f3053, 0 8px 4px 1px #111111;color: #fff;font: bold 20px \"helvetica neue\", helvetica, arial, sans-serif;line-height: 1;margin-bottom: 10px;padding: 10px 0 12px 0;text-align: center;text-shadow: 0px -1px 1px #1e2d4d;width: 150px;-webkit-background-clip: padding-box; }button.punch:hover {-webkit-box-shadow: inset 0 0px 20px 1px #87adff, 0px 1px 0 #1d2c4d, 0 6px 0px #1f3053, 0 8px 4px 1px #111111;-moz-box-shadow: inset 0 0px 20px 1px #87adff, 0px 1px 0 #1d2c4d, 0 6px 0px #1f3053, 0 8px 4px 1px #111111;box-shadow: inset 0 0px 20px 1px #87adff, 0px 1px 0 #1d2c4d, 0 6px 0px #1f3053, 0 8px 4px 1px #111111;cursor: pointer; }button.punch:active {-webkit-box-shadow: inset 0 1px 10px 1px #5c8bee, 0 1px 0 #1d2c4d, 0 2px 0 #1f3053, 0 4px 3px 0 #111111;-moz-box-shadow: inset 0 1px 10px 1px #5c8bee, 0 1px 0 #1d2c4d, 0 2px 0 #1f3053, 0 4px 3px 0 #111111;box-shadow: inset 0 1px 10px 1px #5c8bee, 0 1px 0 #1d2c4d, 0 2px 0 #1f3053, 0 4px 3px 0 #111111;margin-top: 58px; }</style>");
         client.println("<script type=\"text/javascript\">function blockMove() {event.preventDefault() ;}</script>");
         client.println("</head><body ontouchmove=\"blockMove()\">");
         client.println("<br><br><br><form action=\"/control\" target=\"display\" method=\"get\" type=\"submit\" value=\"submit\"><table><tr><td>");
         client.println("<button name=\"fm\" value=\"1\" class=\"punch\">FM</button></td>");
         client.println("<td><center><button name=\"am\" value=\"1\" class=\"punch\">AM</button></center></td>"); 
         client.println("<td><button name=\"sw\" value=\"1\" class=\"punch\">SW</button></td></tr><tr>");
         client.println("<td><button name=\"prev\" value=\"1\" class=\"punch\"><<</button></td>");
         client.println("<td><iframe name=\"display\"></iframe></td>");
         client.println("<td><button name=\"next\" value=\"1\" class=\"punch\">>></button></td></tr><tr>");
         client.println("<td><button name=\"mute\" value=\"1\" class=\"punch\">Mute</button></td>");
         client.println("<td><center><button name=\"volup\" value=\"1\" class=\"punch\">Vol Up</button></center></td>");
         client.println("<td><button name=\"voldown\" value=\"1\" class=\"punch\">VolDown</button></td></tr></table></form>");
         client.println("</body></html>");
          client.println();

          // output the value of each analog input pin
          for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
            
          }
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
  }
}
