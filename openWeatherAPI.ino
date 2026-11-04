#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <Arduino_JSON.h>
#include <M5Core2.h>

const char* ssid = "Scytale";
const char* password = "Chaumurky26";

// Domain name with URL path
String openWeatherMapApiKey = "a50e5ef4d92a134e0ea70e24c7929523";

// Country code and city
String city = "Tempe";
String countryCode = "US";

// Default timer to set 10 seconds for testing
unsigned long lastTime = 0;
// set timer to 10 secs
unsigned long timerDelay = 10000;

String jsonBuffer;

String httpGETRequest(const char* serverName);
void changeToHometown();
void changeToTempe();

void setup() {
  Serial.begin(115200);
  M5.begin();
  M5.Lcd.setTextSize(2);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  M5.Lcd.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    M5.Lcd.println(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  M5.Lcd.println("");
  M5.Lcd.println("Connected to WiFi network with IP Address: ");
  M5.Lcd.println(WiFi.localIP());

  Serial.println("Timer set to 10 seconds, it will take 10 seconds before publishing the first reading.");
  M5.Lcd.println("Timer set to 10 seconds, it will take 10 seconds before publishing the first reading.");

  M5.Lcd.clear();
  M5.Lcd.setCursor(0, 0);
}

void loop() {
  M5.update(); 
  
  if((millis() - lastTime) > timerDelay) {
    lastTime = millis();
    
    if(WiFi.status() == WL_CONNECTED) {
      String serverPath = "https://api.openweathermap.org/data/2.5/weather?q=" + city + "," + countryCode + "&APPID=" + openWeatherMapApiKey;

      jsonBuffer = httpGETRequest(serverPath.c_str());
      Serial.println(jsonBuffer);
      JSONVar myObject = JSON.parse(jsonBuffer);

      M5.Lcd.clear();
      M5.Lcd.setCursor(10, 20);

      // Error Handlers for Invalid API response configurations
      if(JSON.typeof(myObject) == "undefined") {
        M5.Lcd.setTextSize(2);
        M5.Lcd.println("Parsing failed!");
        return; 
      }



      M5.Lcd.print("JSON object = ");
      M5.Lcd.println(myObject);
      M5.Lcd.println("Temperature: ");
      M5.Lcd.println(myObject["main"]["temp"]);
      M5.Lcd.println("Pressure");
      M5.Lcd.println(myObject["main"]["pressure"]);
      M5.Lcd.println("Humidity");
      M5.Lcd.println(myObject["main"]["humidity"]);
      M5.Lcd.println("Wind speed");
      M5.Lcd.println(myObject["wind"]["speed"]);

      delay(1000); 

      M5.Lcd.clear();
      M5.Lcd.setCursor(10, 20);

      // Extract values
      double tempKelvin = (double)myObject["main"]["temp"];
      int humidity = (int)myObject["main"]["humidity"];

      // Convert Kelvin to Fahrenheit
      double tempFahrenheit = (tempKelvin - 273.15) * 9.0 / 5.0 + 32.0;

      // Print data to screen 
      M5.Lcd.setTextSize(3);
      M5.Lcd.println(city + ", " + countryCode);
      M5.Lcd.setTextSize(2);
      M5.Lcd.println("-------------------");
      M5.Lcd.println("");

      M5.Lcd.print("Temp: ");
      M5.Lcd.print(tempFahrenheit, 1); 
      M5.Lcd.println(" F");

      M5.Lcd.print("Humidity: ");
      M5.Lcd.print(humidity);
      M5.Lcd.println(" %");
    }
    else {
      M5.Lcd.clear();
      M5.Lcd.setCursor(10, 20);
      M5.Lcd.println("WiFi Disconnected");
    }
  }

  if(M5.BtnA.wasReleased() || M5.BtnA.pressedFor(1000, 200)) {
    changeToHometown();
    lastTime = 0; 
  } else if(M5.BtnC.wasReleased() || M5.BtnC.pressedFor(1000, 200)) {
    changeToTempe();
    lastTime = 0; 
  }

  delay(50); 
}

// GET request function
String httpGETRequest(const char* serverName) {
  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient http;

  // Domain name with path
  http.begin(client, serverName);

  // Send HTTP POST Request
  int httpResponseCode = http.GET();

  String payload = "{}";

  if(httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error Code: ");
    Serial.println(httpResponseCode);
  }

  // Free up resources
  http.end();
  return payload;
}

void changeToHometown (){
    // Country code and city
  city = "Los Angeles";
  countryCode = "US";
}

void changeToTempe () {
    // Country code and city
  city = "Tempe";
  countryCode = "US";
}

