Postbox is a simple device that connects to your (physical) mailbox to notify you if you've got mail.

It's mechanism is simple:
 - It connects to a WLAN of your choice, and a physical switch on your mailbox.
 - the device goes to sleep after running its initialisation stage.
 - if the mailbox is opened, a physical switch wakes it from sleep (GPIO16-RST) and the loop continues.
 - after notifying you per email, it goes back to sleep.

The ESP-12F/8266 is an excellent choice as it is compact and has minimal power usage in its deep sleep mode. Usually, the device will run for a few seconds after the trigger. I expect it to last for a few weeks with a decent battery.

Libraries needed:
arduino-libraries/NTPClient
esp8266/Arduino

TODO: 
Email notifications
