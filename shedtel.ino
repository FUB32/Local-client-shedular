#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>   // Universal Telegram Bot Library written by Brian Lough: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
#include <AsyncTCP.h>
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <ESP_Mail_Client.h>
#include "time.h"
AsyncWebServer server(80);

// Replace with your network credentials
const char* ssid = "M";
const char* password = "00000000";
unsigned long previousMillis = 0;// constants won't change:
const long interval = 3000;  // interval at which to blink (milliseconds)

  SMTP_Message message;
// Initialize Telegram BOT
#define BOTtoken "5986829065:AAH_jRmhNj9KdKgEI4tyom05ItHQnZgnZDk"  // your Bot Token (Get from Botfather)

// Use @myidbot to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can
// message you
#define CHAT_ID "333907988"

#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465
#define AUTHOR_EMAIL "mohamedbaqer1932@gmail.com"
#define AUTHOR_PASSWORD "czswejuqozhdngyz"
#define RXD2 17
#define TXD2 16

//#define RECIPIENT_EMAIL "mohamedbaqer1994@gmail.com"
SMTPSession smtp;
String resEmail ;
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

  ESP_Mail_Session session;
// Checks for new messages every 1 second.
int botRequestDelay = 5000;
unsigned long lastTimeBotRan;

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 10800;
const int   daylightOffset_sec = 0;

String inputName;
String inputNumber;
String inputPassword;
char *numbers[]={"7834202829","7712181424"};
char *passwords[]={"a123a123","a1a2a3a4"};
// HTML web page to handle 3 input fields (input1, input2, input3)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
<title>Bootstrap Simple Login Form</title>
<link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.5.0/css/bootstrap.min.css">
<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/font-awesome/4.7.0/css/font-awesome.min.css">
<script src="https://code.jquery.com/jquery-3.5.1.min.js"></script>
<script src="https://cdn.jsdelivr.net/npm/popper.js@1.16.0/dist/umd/popper.min.js"></script>
<script src="https://stackpath.bootstrapcdn.com/bootstrap/4.5.0/js/bootstrap.min.js"></script>
<style>
.login-form {
    width: 340px;
    margin: 50px auto;
    font-size: 15px;
}
.login-form form {
    margin-bottom: 15px;
    background: #f7f7f7;
    box-shadow: 0px 2px 2px rgba(0, 0, 0, 0.3);
    padding: 30px;
}
.login-form h2 {
    margin: 0 0 15px;
}
.form-control, .btn {
    min-height: 38px;
    border-radius: 2px;
}
.btn {        
    font-size: 15px;
    font-weight: bold;
}
</style>
</head>
<body>
<div class="login-form">
    <form action="/post" method="post">
        <h2 class="text-center">Log in</h2>       
        <div class="form-group">
            <input name="name" type="text" class="form-control" placeholder="Username" required="required">
        </div>
        <div class="form-group">
            <input name="number" type="tel" class="form-control" placeholder="Phone number" required="required">
        </div>
        <div class="form-group">
            <input name="email" type="email" class="form-control" placeholder="Email" required="required">
        </div>
        <div class="form-group">
            <input name="password" type="password" class="form-control" placeholder="Password" required="required">
        </div>
        <div class="form-group">
            <button name="sub" type="submit" class="btn btn-primary btn-block">Submit</button>
        </div>      
    </form>
</div>
</body>
</html>)rawliteral";

const char response1_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
<title>Bootstrap Simple Login Form</title>
</head>
<body>
<center>
<h1>Your reservation has been registered</h1>
</center>
</body>
</html>)rawliteral";


const char response2_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
<title>Bootstrap Simple Login Form</title>
</head>
<body>
<center>
<h1>Your reservation has been rejected</h1>
<h1>Number or password is incorrect</h1>
</center>
</body>
</html>)rawliteral";


void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}
void sendMail(String textMsg);
void printLocalTime();
void smtpCallback(SMTP_Status status);
void handleNewMessages(int numNewMessages);
void sendSMS(String number, String message);
void setup() {
 Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  delay(3000);

  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  // Set your Static IP address
//IPAddress local_IP(192, 168, 150, 184);
// Set your Gateway IP address
//IPAddress gateway(192, 168, 150, 1);
//IPAddress subnet(255, 255, 0, 0);
//
//IPAddress primaryDNS(8, 8, 8, 8);   // optional
//IPAddress secondaryDNS(8, 8, 4, 4); // optional
// Configures static IP address
//if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
//  Serial.println("STA Failed to configure");
//}
  WiFi.begin(ssid, password);
  
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();
  // Send web page with input fields to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
    Serial.println("get");
  });

  // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
  server.on("/post", HTTP_POST, [] (AsyncWebServerRequest *request) {
    Serial.println("post");
    inputName=request->getParam(0)->value().c_str();
    Serial.print("Name: ");
    Serial.println(inputName);
    inputNumber=request->getParam(1)->value().c_str();
    Serial.print("Number: ");
    Serial.println(inputNumber);
    resEmail=request->getParam(2)->value().c_str();
    Serial.print("Email: ");
    Serial.println(resEmail);
    inputPassword=request->getParam(3)->value().c_str();
    Serial.print("Password: ");
    Serial.println(inputPassword);

    if((inputNumber == numbers[0] && inputPassword == passwords[0])||(inputNumber == numbers[1] && inputPassword == passwords[1]))
    {
      printLocalTime();
      request->send_P(200, "text/html", response1_html);
    }
    else
    {
      request->send_P(200, "text/html", response2_html);
    }
  
  });
  
  server.onNotFound(notFound);
  server.begin();
    smtp.debug(1);
  smtp.callback(smtpCallback);
  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;
  session.login.user_domain = "";
  message.sender.name = "FUB";
  message.sender.email = AUTHOR_EMAIL;
  message.subject = "Reservation details";
    

}

void loop() {
if (millis() > lastTimeBotRan + botRequestDelay) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
}

  
}
