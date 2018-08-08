#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <string.h>

/* TODO: SMTP. Plenty of broken, non-working libraries out there.
   NOTE: deepSleep() wake (GPIO16->RST) seems not to work on my device.
         RST->GND works, but no state saved.
*/

// Aliases. I prefer shorter, cleaner code.
#define log         Serial.println
#define gettime     timeClient.getFormattedTime

// WLAN Info
#define NETSSID     "SSID"
#define NETPASSWD   "PASSWORD"


/* NTPClient setup. Logs to serial, will add to send.
   timeclient requires localvar, server, and offset in sec.
   GMT+2 atm, implement DST.
*/
WiFiUDP   ntpUDP;
NTPClient timeClient(ntpUDP, "ntp1.rwth-aachen.de", 2 * 60 * 60);

// connect to the WLAN defined above. This is done after wakeups.
void connectWLAN()
{
  log("[connectWLAN] connecting to Network");

  WiFi.begin(NETSSID, NETPASSWD);

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


/*
    setup() and loop().
*/


void setup() {
  // init console
  Serial.begin(115200);
  Serial.setTimeout(2000);
  log("[setup] Serial console started");

  // init NTP
  timeClient.begin();
}

void loop() {
  // RESET! or boot.. deepSleep() left and device awake.
  // TODO: Save state with a latching relay, EEPROM or the likes.

  log("[loop] Someone woke me up or I have turned on for the first time!");
  connectWLAN();
  timeClient.update();
  log((String)"[loop] Triggered at: " + gettime());
  // TODO: SMTP Mail
  ESP.deepSleep(0); // sleep until wakeup
}

