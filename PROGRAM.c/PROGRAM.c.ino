#include <Wire.h>                 
#include <Adafruit_GFX.h>         
#include <Adafruit_SSD1306.h>     
#include <WiFi.h>                 
const int gasSensorPin = A0;      
const int relayPin = 2;           
const int buzzerPin = 3;          
const int ledPin = 4;             
const int threshold = 400;
float gasConcentration = 0.0;
const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";
#define SCREEN_WIDTH 128          
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); 
void readGasSensor() {
    int sensorValue = analogRead(gasSensorPin); 
    gasConcentration = map(sensorValue, 0, 1023, 0, 1000); 
}
void triggerAlerts() {
    if (gasConcentration > threshold) {
        digitalWrite(relayPin, HIGH); 
        digitalWrite(buzzerPin, HIGH);
        digitalWrite(ledPin, HIGH); 
    } else {
        digitalWrite(relayPin, LOW); 
        digitalWrite(buzzerPin, LOW); 
        digitalWrite(ledPin, LOW); 
    }
}
void displayGasLevel() {
    display.clearDisplay();  
    display.setTextSize(1);  
    display.setTextColor(SSD1306_WHITE);  
    display.setCursor(0, 0);  
    display.print("Gas Level: "); 
    display.print(gasConcentration); 
    display.println(" ppm");  
    if (gasConcentration > threshold) {
        display.setCursor(0, 20);
        display.print("Status: ALERT!"); 
    } else {
        display.setCursor(0, 20);
        display.print("Status: Safe");  
    }
    display.display();  
}
void sendWiFiAlert() {
    if (gasConcentration > threshold) {
       
        Serial.println("ALERT! High Gas Concentration Detected!");
    }
}
void setup() {
    Serial.begin(115200);  
    pinMode(gasSensorPin, INPUT); 
    pinMode(relayPin, OUTPUT);  
    pinMode(buzzerPin, OUTPUT);  
    pinMode(ledPin, OUTPUT);  

    
    if (!display.begin(SSD1306_I2C_ADDRESS, 0x3C)) { 
        Serial.println("OLED initialization failed");
        while (true);  
    }
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("System Initializing...");
    display.display();
    delay(2000);

    
    WiFi.begin(ssid, password);
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Connecting to Wi-Fi...");
    display.display();

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Wi-Fi Connected!");
    display.display();
    Serial.println("\nWi-Fi Connected.");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    delay(2000);
}
void loop() {
    readGasSensor();         
    triggerAlerts();         
    displayGasLevel();      
    sendWiFiAlert();        
    delay(1000);             
