/* include library */
#include <ESP8266WiFi.h>
 
/* define port */
WiFiClient client;
WiFiServer server(80);
 
/* WIFI settings */
const char* ssid = "LYFONTECH";
const char* password = "LYFON5030";
 
/* data received from application */
String  data = "";
 
/* define L298N or L293D motor control pins */
int leftMotorForward = D0;    
int rightMotorForward = D2;  
int leftMotorBackward = D1;    
int rightMotorBackward = D3;  
int switch1 = D6;
int switch2 = D7; 
 int sw_state1 = HIGH;
 int sw_state2 = HIGH;
 
void setup()
{
  Serial.begin(115200);
  /* initialize motor control pins as output */
  pinMode(leftMotorForward, OUTPUT);
  pinMode(rightMotorForward, OUTPUT);
  pinMode(leftMotorBackward, OUTPUT);
  pinMode(rightMotorBackward, OUTPUT);
  pinMode(switch1, OUTPUT);
  pinMode(switch2, OUTPUT);



  //connect to your local wi-fi network
  WiFi.begin(ssid, password);
 
  // Attempt to connect to WiFi network:
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    // Connect to WPA/WPA2 network. Change this line if using open or WEP  network:
    // Wait 3 seconds for connection:
    delay(3000);
  }
 
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());   //You can get IP address assigned to ESP
 
  /* start server communication */
  server.begin();
}
 
void loop()
{
  /* If the server available, run the "checkClient" function */
  client = server.available();
  if (!client) return;
  data = checkClient ();
 
  /************************ Run function according to incoming data from application *************************/
 
  /* If the incoming data is "forward", run the "MotorForward" function */
  if (data == "forward") MotorForward();
  /* If the incoming data is "backward", run the "MotorBackward" function */
  else if (data == "backward") MotorBackward();
  /* If the incoming data is "left", run the "TsturnLeft" function */
  else if (data == "left") TurnLeft();
  /* If the incoming data is "right", run the "TurnRight" function */
  else if (data == "right") TurnRight();
  /* If the incoming data is "stop", run the "MotorStop" function */
  else if (data == "stop") MotorStop();
  
  //for turning on switch 1 relay
  else if (data == "switch1") turnonswitch1();
  //for switch 2
   else if (data == "switch2") turnonswitch2();
  //for turning off switch 1 relay
  else if (data == "switch1off") turnoffswitch1();
  //for switch 2 off
   else if (data == "switch2off") turnoffswitch2();
}
 
/********************************************* FORWARD *****************************************************/
void MotorForward(void)
{
  digitalWrite(leftMotorForward, HIGH);
  digitalWrite(rightMotorForward, HIGH);
  digitalWrite(leftMotorBackward, LOW);
  digitalWrite(rightMotorBackward, LOW);
}
 
/********************************************* BACKWARD *****************************************************/
void MotorBackward(void)
{
  digitalWrite(leftMotorBackward, HIGH);
  digitalWrite(rightMotorBackward, HIGH);
  digitalWrite(leftMotorForward, LOW);
  digitalWrite(rightMotorForward, LOW);
}
 
/********************************************* TURN LEFT *****************************************************/
void TurnLeft(void)
{
  digitalWrite(leftMotorForward, LOW);
  digitalWrite(rightMotorForward, HIGH);
  digitalWrite(rightMotorBackward, LOW);
  digitalWrite(leftMotorBackward, HIGH);
}
 
/********************************************* TURN RIGHT *****************************************************/
void TurnRight(void)
{
  digitalWrite(leftMotorForward, HIGH);
  digitalWrite(rightMotorForward, LOW);
  digitalWrite(rightMotorBackward, HIGH);
  digitalWrite(leftMotorBackward, LOW);
}
 
/********************************************* STOP *****************************************************/
void MotorStop(void)
{
  digitalWrite(leftMotorForward, LOW);
  digitalWrite(leftMotorBackward, LOW);
  digitalWrite(rightMotorForward, LOW);
  digitalWrite(rightMotorBackward, LOW);
}

void turnonswitch1(void)
{
  //sw_state1 = !sw_state1;
  //digitalWrite(switch1, sw_state1);
  digitalWrite(switch1, HIGH);
}
 
void turnonswitch2(void)
{
  //sw_state2 = !sw_state2;
  //digitalWrite(switch2, sw_state2);
  digitalWrite(switch2, HIGH);
}
void turnoffswitch2(void)
{
  digitalWrite(switch2, LOW);
}

void turnoffswitch1(void)
{
  digitalWrite(switch1, LOW);
}
/***************************
******* RECEIVE DATA FROM the APP ******************************************/
String checkClient (void)
{
  while (!client.available()) delay(1);
  String request = client.readStringUntil('\r');
  request.remove(0, 5);
  request.remove(request.length() - 9, 9);
  return request;
}