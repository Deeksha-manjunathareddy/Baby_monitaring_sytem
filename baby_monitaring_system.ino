#include <Servo.h> 

#include <ESP8266WiFi.h> 

#include <ESP8266SMTP.h>  

const int soundSensorPin = A0; 

const int threshold = 500;  // Adjust based on sensor sensitivity 

const int servoPin = 9;  

Servo servo; 

unsigned long cryStartTime = 0; 

bool crying = false;  

const char* ssid = "your_SSID";       // Replace with your WiFi SSID 

const char* password = "your_PASSWORD";// Replace with your WiFi password  

SMTPData smtpData;  

void setup () { 

    Serial.begin(9600); 

    servo.attach(servoPin); 

    pinMode(soundSensorPin, INPUT);     

    WiFi.begin(ssid, password); 

    while (WiFi.status() != WL_CONNECTED) { 

        delay (1000); 

        Serial.println("Connecting to WiFi..."); 

    } 
     Serial.println("Connected to WiFi"); 

}  

void loop() { 

    int soundLevel = analogRead(soundSensorPin);     

    if (soundLevel > threshold) { 

        if (!crying) { 

            crying = true; 

            cryStartTime = millis(); 

        } 

        moveCradle(); 

    } else { 

        crying = false; 

        servo.write(90);  // Stop moving cradle 

    }     

    if (crying && (millis() - cryStartTime) > 600000) {  // 10 minutes 

        sendEmail(); 

        crying = false;  // Reset crying status 

    }     

    delay(100);  // Small delay for stability 

}  

void moveCradle() { 

    for (int pos = 0; pos <= 180; pos += 1) { 

        servo.write(pos); 

        delay(15); 

    } 

    for (int pos = 180; pos >= 0; pos -= 1) { 
       servo.write(pos); 

        delay(15); 

    } 

}  

void sendEmail() { 

    smtpData.setLogin("smtp.yourprovider.com", 465, "your_email@example.com", "your_password"); 

    smtpData.setSender("Baby Monitor", "your_email@example.com"); 

    smtpData.setPriority("High"); 

    smtpData.setSubject("Baby Crying Alert"); 

    smtpData.setMessage("The baby has been crying for more than 10 minutes.", false); 

    smtpData.addRecipient("recipient@example.com"); 

  

    if (!MailClient.sendMail(smtpData)) { 

        Serial.println("Error sending email, " + MailClient.smtpErrorReason()); 

    } 

  

    smtpData.empty(); 

} 