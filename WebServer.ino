/*************************************************************************************************
  ESP32 Web Server
  Ejemplo de creación de Web server 
  Basándose en los ejemplos de: 
  https://lastminuteengineers.com/creating-esp32-web-server-arduino-ide/
  https://electropeak.com/learn
**************************************************************************************************/
//************************************************************************************************
// Librerías
//************************************************************************************************
#include <WiFi.h>
#include <WebServer.h>
//************************************************************************************************
// Variables globales
//************************************************************************************************
// SSID & Password
const char* ssid = "Iphone de Kathy";  // Enter SSID here
const char* password = "kathy777";  //Enter Password here

WebServer server(80);  // Object of WebServer(HTTP port, 80 is defult)


#define RXD2 16
#define TXD2 17
char RE[4];
int D7;
int AD7;

//************************************************************************************************
// Configuración
//************************************************************************************************
void setup() {
  Serial.begin(115200);
  Serial.println("Try Connecting to ");
  Serial.println(ssid);
  
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
 // para display 7 segmentos
  pinMode(18, OUTPUT);//C
  pinMode(19, OUTPUT);//E
  pinMode(21, OUTPUT);//D
  pinMode(32, OUTPUT);//F
  pinMode(33, OUTPUT);//G
  pinMode(25, OUTPUT);//B
  pinMode(26, OUTPUT);//A

  // Connect to your wi-fi modem
  WiFi.begin(ssid, password);

  // Check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected successfully");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());  //Show ESP32 IP on serial

  server.on("/", handle_OnConnect); // Directamente desde e.g. 192.168.0.
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("HTTP server started");
  delay(100);
}
//************************************************************************************************
// loop principal
//************************************************************************************************
void loop() {
  server.handleClient();
  if (Serial2.available() > 0) {
    Serial2.readBytesUntil(10, RE, 4);
  }

  AD7 = 4;
  D7 = AD7;
  if (RE[0] == 48) {
    D7 = AD7;
  } else {
    AD7--;
  }
  if (RE[1] == 48) {
    D7 = AD7;
  } else {
    AD7--;
  }
  if (RE[2] == 48) {
    D7 = AD7;
  } else {
    AD7--;
  }
  if (RE[3] == 48) {
    D7 = AD7;
  } else {
    AD7--;
  }

  display_(AD7);
  server.handleClient();
}

//************************************************************************************************
// Display
//************************************************************************************************
void display_(int a) {
  switch (a) {
    case 0:
      digitalWrite(26, HIGH); //a
      digitalWrite(25, HIGH); //b
      digitalWrite(18, HIGH); //c
      digitalWrite(21, HIGH); //d
      digitalWrite(19, HIGH); //e
      digitalWrite(32, HIGH); //f
      digitalWrite(33, LOW); //g
      Serial.println("0 PARQUEOS");
      break;
    case 1:
      digitalWrite(26, LOW); //a
      digitalWrite(25, HIGH); //b
      digitalWrite(18, HIGH); //c
      digitalWrite(21, LOW); //d
      digitalWrite(19, LOW); //e
      digitalWrite(32, LOW); //f
      digitalWrite(33, LOW); //g
      Serial.println("1 PARQUEOS");
      break;
    case 2:
      digitalWrite(26, HIGH); //a
      digitalWrite(25, HIGH); //b
      digitalWrite(18, LOW); //c
      digitalWrite(21, HIGH); //d
      digitalWrite(19, HIGH); //e
      digitalWrite(32, LOW); //f
      digitalWrite(33, HIGH); //g
      Serial.println("2 PARQUEOS");
      break;
    case 3:
      digitalWrite(26, HIGH); //a
      digitalWrite(25, HIGH); //b
      digitalWrite(18, HIGH); //c
      digitalWrite(21, HIGH); /-
      digitalWrite(33, HIGH); //g
      Serial.println("3 PARQUEOS");
      break;
    case 4:
      digitalWrite(26, LOW); //a
      digitalWrite(25, HIGH); //b
      digitalWrite(18, HIGH); //c
      digitalWrite(21, LOW); //d
      digitalWrite(19, LOW); //e
      digitalWrite(32, HIGH); //f
      digitalWrite(33, HIGH);
      Serial.println("4 PARQUEOS");
      break;
      default:
      digitalWrite(26, LOW); //a
      digitalWrite(25, HIGH); //b
      digitalWrite(18, HIGH); //c
      digitalWrite(21, LOW); //d
      digitalWrite(19, LOW); //e
      digitalWrite(32, HIGH); //f
      digitalWrite(33, HIGH);
      break;
  }-----------------------
  -
  +
}
//************************************************************************************************
// Handler de Inicio página
//************************************************************************************************
void handle_OnConnect() {
  server.send(200, "text/html", SendHTML());
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

//************************************************************************************************
// Procesador de HTML
//************************************************************************************************
String SendHTML(void) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<!doctype html>\n";
  ptr += "<html>\n";
  ptr += "<head>\n";
  ptr += "  <script type=\"text/javascript\">\n";
  ptr += "  setTimeout(function(){\n";
  ptr += "     window.location.reload(1);\n";
  ptr += "  }, 5000);\t\n";
  ptr += "</script>\n";
  ptr += "<meta charset=\"utf-8\">\n";
  ptr += "<title>Proyecto Digital</title>\n";
  ptr += "<style>\n";
  ptr += "table {\n";
  ptr += "  font-family: arial, sans-serif;\n";
  ptr += "  border-collapse: collapse;\n";
  ptr += "  width: 40%;\n";
  ptr += "}\n";
  ptr += "\n";
  ptr += "td, th {\n";
  ptr += "  border: 3px solid #000000;\n";
  ptr += "  text-align: center;\n";
  ptr += "  padding: 10px;\n";
  ptr += "}\n";
  ptr += "\n";
  ptr += "tr:nth-child(even) {\n";
  ptr += "  background-color: #dddddd;\n";
  ptr += "}\t\n";
  ptr += "</style>\t\n";
  ptr += "\t\n";
  ptr += "</head>\n";
  ptr += "\n";
  ptr += "<body>\n";
  ptr += "<h1>Control de Registro para Parqueo</h1>\n";
  ptr += "<p>&nbsp;</p>\n";
  ptr += "<table>\n";
  ptr += "  <tr>\n";
  ptr += "    <th>PARQUEO</th>\n";
  ptr += "    <th>DISPONIBILIDAD</th>\n";
  ptr += "\n";
  ptr += "  </tr>\n";
  ptr += "  <tr>\n";
  ptr += "    <td>Parqueo A</td>\n";
  if (RE[0] == 48) {
    ptr += "    <td>LIBRE&nbsp;✅</td>\n";
  } else {
    ptr += "    <td>OCUPADO&nbsp;⛔</td>\n";
  }
ptr += "\n";
ptr += "  </tr>\n";
ptr += "  <tr>\n";
ptr += "    <td>Parqueo B</td>\n";
  if (RE[1] == 48) {
    ptr += "    <td>LIBRE&nbsp;✅</td>\n";
  } else {
    ptr += "    <td>OCUPADO&nbsp;⛔</td>\n";
  }
ptr += "\n";
ptr += "  </tr>\n";
ptr += "  <tr>\n";
ptr += "    <td>Parqueo C</td>\n";
  if (RE[2] == 48) {
    ptr += "    <td>LIBRE&nbsp;✅</td>\n";
  } else {
    ptr += "    <td>OCUPADO&nbsp;⛔</td>\n";
  }
ptr += "\n";
ptr += "  </tr>\n";
ptr += "  <tr>\n";
ptr += "    <td>Parqueo D</td>\n";
  if (RE[3] == 48) {
    ptr += "    <td>LIBRE&nbsp;✅</td>\n";
  } else {
    ptr += "    <td>OCUPADO&nbsp;⛔</td>\n";
  }
ptr += "\n";
ptr += "  </tr>\n";
ptr += "</table>\n";
ptr += "\t\n";
ptr += "</body>\t\n";
ptr += "\t\n";
ptr += "\t\n";
ptr += "</html>\n";
ptr += "";
return ptr;
}
