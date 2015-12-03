#include <Bridge.h>
#include <Temboo.h>
#include "TembooAccount.h" // contains Temboo account information
#include "TwitterAccount.h"

/*** INPUT PINS ***/
const int catSensor = 2;
const int litterChangeButton = 3;

int litterRobotTriggered = 0;
int changeLitter = 0;
int timesUsed = 0;

void setup() {
  pinMode(catSensor, INPUT);
  pinMode(litterChangeButton, INPUT);

  Serial.begin(9600);
  Bridge.begin();

  Serial.println("Running Litter Robot Notifications");
}

void loop()
{

    litterRobotTriggered = digitalRead(catSensor);
    changeLitter = digitalRead(litterChangeButton);

    /***If catSensor is Triggered or Button is pressed***/
    if (litterRobotTriggered == HIGH || changeLitter == LOW) {
        Serial.println("Litter Robot Cat Sensor Tripped");
        timesUsed++;
        

        if (changeLitter == LOW) {
          tweetText = "Thanks for changing the litter! " + String(millis());
          timesUsed = 0;
        } else {
			String tweetText("I just used the litter robot! :) #" + String(timesUsed) + " " + String(millis()));
		}
        
        TembooChoreo StatusesUpdateChoreo;
    
        
        StatusesUpdateChoreo.begin();
        
        // set Temboo account credentials
        StatusesUpdateChoreo.setAccountName(TEMBOO_ACCOUNT);
        StatusesUpdateChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
        StatusesUpdateChoreo.setAppKey(TEMBOO_APP_KEY);
    
        // identify the Temboo Library choreo to run (Twitter > Tweets > StatusesUpdate)
        StatusesUpdateChoreo.setChoreo("/Library/Twitter/Tweets/StatusesUpdate");
    
        // set the required choreo inputs
        // see https://www.temboo.com/library/Library/Twitter/Tweets/StatusesUpdate/ 
        // for complete details about the inputs for this Choreo
     
        // add the Twitter account information
        StatusesUpdateChoreo.addInput("AccessToken", TWITTER_ACCESS_TOKEN);
        StatusesUpdateChoreo.addInput("AccessTokenSecret", TWITTER_ACCESS_TOKEN_SECRET);
        StatusesUpdateChoreo.addInput("ConsumerKey", TWITTER_API_KEY);    
        StatusesUpdateChoreo.addInput("ConsumerSecret", TWITTER_API_SECRET);
    
        // and the tweet we want to send
        StatusesUpdateChoreo.addInput("StatusUpdate", tweetText);
    
        // tell the Process to run and wait for the results. The 
        // return code (returnCode) will tell us whether the Temboo client 
        // was able to send our request to the Temboo servers
        unsigned int returnCode = StatusesUpdateChoreo.run();
    
        // a return code of zero (0) means everything worked
        if (returnCode == 0) {
            Serial.println("Success! Tweet sent!");
        } else {
          // a non-zero return code means there was an error
          // read and print the error message
          while (StatusesUpdateChoreo.available()) {
            char c = StatusesUpdateChoreo.read();
            Serial.print(c);
          }
        } 
        
        StatusesUpdateChoreo.close();
        /***Sleep for 15 minutes***/
        delay(900000);
    }
}
