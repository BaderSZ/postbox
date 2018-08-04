#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESPMail.h>
#include <string.h>

// Aliases. I prefer shorter, cleaner code.
#define log         Serial.println
#define gettime     timeClient.getFormattedTime

// WLAN Info
#define NETSSID     "SSID"
#define NETPASSWD   "PASSWORD"

// Email data
#define SERVER      "Server Host"
#define PORT        465
#define LOGIN       "Account Login"
#define SENDER      "Sender Email"
#define MAILPASSWD  "Account password"

#define RECEIVER    "Recepient"

/* NTPClient setup. IDE whines if it's not in this scope. */
WiFiUDP   ntpUDP;
NTPClient timeClient(ntpUDP);


/* connect to the WLAN defined above. This is done after wakeups. */
void connectWLAN()
{
  log("[connectWLAN] connecting to Network");
  
  WiFi.begin(NETSSID,NETPASSWD);

  // error checking  
  while (WiFi.status() != WL_CONNECTED)
  {
    if (WiFi.status() == WL_CONNECT_FAILED)
      log("[connectWLAN] Failed to connect, trying again.");
      delay(10000); 
  }

  log("[connectWLAN] WiFi connected with IP: ");
  log(WiFi.localIP());
}


/* Send EMail with given data. */
void sendMail(char* subject, char* message)
{
  
  log("[sendMail] init and connect");
  
  ESPMail mail;
  mail.begin();

  mail.enableDebugMode();
  
  mail.setSubject(SENDER, subject);
  mail.addTo(RECEIVER);
  mail.setBody(message);
  
  log("[sendMail] SUBJET: " + (String)subject + " MSG: " + (String)message);
  log("[sendMail] sending...");
  
  if (mail.send(SERVER, PORT, LOGIN, MAILPASSWD) == 0)
    log("[sendMail] sent!");
  else
    log("[sendMail] sending failed.");
}

/*****
  * Main/Mandatory functions. Loop for the sleep/wake loop, setup to initialise NTP *
******/

void setup() {
  // init console
  Serial.begin(115200);
  Serial.setTimeout(2000);
  log("[setup] Serial console started");

  // init NTP
  timeClient.begin();
}


void loop() {
  connectWLAN();
  sendMail("[Postman] Initialised", "Booted. Postman is now awake and sleeping until further notice.");
  
  while(true)
  {
    log("[loop] Booted! Sleeping until I have been awoken (again)...");
    ESP.deepSleep(0); // sleep until wakeup
    
    // RESET! deepSleep() left and device awake.

    log("[loop] Someone woke me up!");
    connectWLAN();
    timeClient.update();
    log((String)"[loop] Triggered at: " + gettime());
    sendMail("[Postman] You've got mail!", "You've got mail at "); // TODO + gettime());
  }
}
