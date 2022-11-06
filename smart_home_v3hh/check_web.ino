void createWebServer()
{
 {
    server.on("/", []() {

      IPAddress ip = WiFi.softAPIP();
      String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
         String now = "";
  for (int i = 96; i < 134; ++i)
  {
    now += char(EEPROM.read(i));
  }
      content = "<!DOCTYPE HTML><html>MAKED BY YATIN ";
      content += "<form action=\"/scan\" method=\"POST\"><input type=\"submit\" value=\"scan\"></form>";
      content += ipStr;
      content += "<p>";
      content += st;
      content += now;
      content += "</p><form method='get' action='setting'><label>SSID: </label><input name='ssid' length=32><label>password: </label><input name='pass' length=64><input type='submit'></form>";
      content += "</p><form method='get' action='reset'><input type='submit' value='RESET'></form>";
      content += "</p><form method='get' action='set'><input type='submit' value='settings'></form>";
      content += "</p><form method='get' action='offline'><input type='submit' value='offline'></form>";
      content += "</html>";
      server.send(200, "text/html", content);
    });
    server.on("/scan", []() {
      //setupAP();
      IPAddress ip = WiFi.softAPIP();
      String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);

      content= "<!DOCTYPE HTML>\r\n<html>go back";
      server.send(200, "text/html", content);
    });
    server.on("/reset", []() {
      //setupAP();
      IPAddress ip = WiFi.softAPIP();
      String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);

      content = "<!DOCTYPE HTML>\r\n<html>go back";
      server.send(200, "text/html", content);
      Serial.printf("reseting");
      ESP.reset();
    });

    server.on("/setting", []() {
      String qsid = server.arg("ssid");
      String qpass = server.arg("pass");
      if (qsid.length() > 0 ) {
        Serial.println("clearing eeprom");
        for (int i = 0; i < 96; ++i) {
          EEPROM.write(i, 0);
        }
        Serial.println(qsid);
        Serial.println("");
        Serial.println(qpass);
        Serial.println("");

        Serial.println("writing eeprom ssid:");
        for (int i = 0; i < qsid.length(); ++i)
        {
          EEPROM.write(i, qsid[i]);
          Serial.print("Wrote: ");
          Serial.println(qsid[i]);
        }
        Serial.println("writing eeprom pass:");
        for (int i = 0; i < qpass.length(); ++i)
        {
          EEPROM.write(32 + i, qpass[i]);
          Serial.print("Wrote: ");
          Serial.println(qpass[i]);
        }
        EEPROM.commit();

        content = "{\"Success\":\"saved to eeprom... reset to boot into new wifi\"}";
        statusCode = 200;
        ESP.reset();
      } else {
        content = "{\"Error\":\"404 not found\"}";
        statusCode = 404;
        Serial.println("Sending 404");
      }
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.send(statusCode, "application/json", content);

    });
    server.on("/set", []() {
      //setupAP();
      IPAddress ip = WiFi.softAPIP();
      String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);

      content = "<!DOCTYPE HTML>\r\n<html>settings";
      content += "</p><form method='get' action='set/na'><label>NAME: </label><input name='na' length=32><input type='submit'></form>";
      server.send(200, "text/html", content);
     
  }); 
         server.on("/set/na", []() {
      //setupAP();
      IPAddress ip = WiFi.softAPIP();
      String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);

      String naa = server.arg("na");
      for (int i = 0; i < 40; ++i)
        {
          EEPROM.write( 96 + i , naa[i]);
          Serial.print("Wrote: ");
          Serial.println(naa[i]);
        }
         EEPROM.commit();
      content += "<!DOCTYPE HTML><html>applyed";
      server.send(200, "text/html", content);
         });

           server.on("/offline", []() {
      //setupAP();
      IPAddress ip = WiFi.softAPIP();
      String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
      
      content += "<!DOCTYPE HTML><html>controll";
      server.send(200, "text/html", content);
         });
      
      
}
}
