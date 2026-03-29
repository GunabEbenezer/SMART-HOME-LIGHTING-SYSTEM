#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL2YJ_BI5IE"
#define BLYNK_TEMPLATE_NAME "SMART HOME LIGHTING"
#define BLYNK_AUTH_TOKEN "3ncPhTzLzTG456k7lYzMgxFyzefINmvd"

#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// Setting up Blynk auth_token, Wi-Fi credentials
char auth [] = "3ncPhTzLzTG456k7lYzMgxFyzefINmvd";
char ssid [] = "Wokwi-GUEST";
char pass [] =  "";

bool motionDetected = false;    // Tracks to check for motion detection
bool ledState = false;          // Tracks for the current state for of LED

// Declaration of lcd parameters
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Declaration of Input-Output pins
const int ledPin = 18;
const int pirPin = 13;


void setup() {
  // pinMode declaration
  pinMode(ledPin, OUTPUT);
  pinMode(pirPin, INPUT);
  Serial.begin(115200);

  // Initializing LCD
  lcd.init();
  lcd.backlight();

  // Initial display on LCD
  lcd.setCursor(0, 0);
  lcd.print("SMART LIGHTING...");
  lcd.setCursor(0, 1);
  lcd.print("System Ready...");
  delay(2000);
  lcd.clear();

  // Connecting to Wi-Fi
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin("Wokwi-GUEST", "", 6);
  while (WiFi.status() !=WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("WiFi Connected!");      // Successful connection to Wi-Fi

  // Connecting to Blynk
  Serial.println("Attempting connection with Blynk...");
  Blynk.begin(auth, ssid, pass);
  Serial.println("Blynk connection successful !");

  // Updating LCD to "System Ready"
  lcd.clear();
  lcd.print("System Ready...");
  delay(1000);
}

// Function to handle Voice Commands
BLYNK_WRITE(V1) {
  ledState = param.asInt();     // Reads the switch state form the app
  digitalWrite(ledPin, ledState);   // Turns LED ON/OFF based on the switch

  lcd.clear();
  lcd.print(ledState ? "LED: ON"  : "LED: OFF");    // Updates LED state on the LCD
}

void loop() {
  Blynk.run();    // Kepps Blynk in connection

  int motionState = digitalRead(pirPin);    // Reads PIR motion sensor state

  if (motionState == HIGH && !ledState) {
    // motion is detected AND LED is ON
    motionDetected = true;
    digitalWrite(ledPin, HIGH);   // Turns ON LED
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("MOTION: DETECTED");
    lcd.setCursor(0, 1);
    lcd.print("LED State: ON ");
    Serial.println("Motion Detected! LED turned ON");
    delay(10000);

  } else if (motionState == LOW && motionDetected) {
    motionDetected = false;
    digitalWrite(ledPin, LOW);

    lcd.clear();
    lcd.setCursor(0, 0);
        // Maintains the LED state on the LCD
    lcd.print("MOTION: NONE ");
    lcd.setCursor(0, 1);
    lcd.print("LED State: OFF ");
    Serial.println("No Motion...LED is turned OFF");
  }



  delay(100); 
}
