#include <Arduino.h>
#include <Webserver.h>
#include <WiFi.h> 
#include <Firebase_ESP_Client.h> //this is just the library
#include "website.h"

#define DOOR_PIN 4

#define USE_LOCAL_WIFI // comment this line out to do AP stuff
//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h" //RTDB = RealTime DataBase (this is just the tools you need for this kind of stuff)

#define WIFI_SSID "ChumbakaWifi"
#define WIFI_PASSWORD "Chumbakapassword"

//#define WIFI_SSID "Kareem's communication boi"
//#define WIFI_PASSWORD "12345678"

#define AP_SSID "esp32door"
#define AP_PASSWORD "1234567890"

#define API_KEY "AIzaSyA4LKUXmrNnRwCjMDqtOUsfZhAF7BYmecs"
#define DATABASE_URL "https://chumbakadooropener-default-rtdb.asia-southeast1.firebasedatabase.app"

//Define Firebase Data object
FirebaseData fbdo; //this is the firebase data object
FirebaseAuth auth;
FirebaseConfig config;

WebServer server(80);

bool signupOK = false;

String username = "";
String password = "";

char XML[200]; // the XML array size needs to be bigger that your maximum expected size

char buf[32]; // just some buffer holder for char operations

void SendWebsite() {
  Serial.println("sending web page");
  server.send(200, "text/html", PAGE_MAIN); //200ms is the timeout
}

void UsernameCheck() {
  if (Firebase.ready() && signupOK){ //if firebase ready
    username = server.arg("VALUE");
    if (Firebase.RTDB.pathExisted(&fbdo, "users/" + username)) {
      server.send(200, "text/plain", "found"); //Send web page
    } else {
      server.send(200, "text/plain", "nope"); //Send web page
    }
  }
}

void PasswordCheck() {
  password = server.arg("VALUE");
  
  Firebase.RTDB.getString(&fbdo, "users/" + username);
  if (fbdo.stringData() == password) {
    server.send(200, "text/plain", "open"); //Send web page
    delay(1500);
    digitalWrite(DOOR_PIN, HIGH);
    delay(100);
    digitalWrite(DOOR_PIN, LOW);
  } else {
   server.send(200, "text/plain", "nope");
  }
}

void setup(){
  // WIFI STUFF
  Serial.begin(115200);
  
  pinMode(DOOR_PIN, OUTPUT);
  digitalWrite(DOOR_PIN, LOW);

  #ifdef USE_LOCAL_WIFI
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED){
      Serial.print(".");
      delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();
  #endif

  disableCore0WDT();
  //disableCore1WDT();
  //DATABASE CONFIG STUFF

  config.api_key = API_KEY;  /* Assign the api key (required) */
  config.database_url = DATABASE_URL;   /* Assign the RTDB URL (required) */

  //THIS TAKES CARE OF THE SIGNUP
  if (Firebase.signUp(&config, &auth, "", "")){ //the & is the same as *, you are just putting a reference to the config and auth objects
    Serial.println("firebase all good to go");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str()); //printf is just print but with the %s things which is a string insert
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
   //A token is just an identifier to attach to a user in order to know who's doing what
   
  Firebase.begin(&config, &auth);  //this starts the Firebasething
  Firebase.reconnectWiFi(true); //and I guess this just periodically reconnects wifi

  //and now for webserver stuff
  server.on("/", SendWebsite);
  server.on("/USERNAME_CHECK", UsernameCheck);
  server.on("/PASSWORD_CHECK", PasswordCheck);

  server.begin();
}

void loop() {
  server.handleClient(); //this needs to be called to keep the server running
}

