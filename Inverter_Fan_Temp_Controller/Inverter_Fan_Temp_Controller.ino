#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino_HTS221.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int fanControlPin = 8;
const int OFFSET = 4;
int count = 0;
unsigned long startTime;
int rpm;

void setup() {
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  if (!HTS.begin()) {
    Serial.println("Failed to initialize humidity temperature sensor!");
    while (1);
  }

  // Clear the buffer
  display.clearDisplay();
  display.display();
  display.setRotation(2);

  pinMode(fanControlPin, OUTPUT);
  analogWrite(fanControlPin, 0);
}

void loop() {
  // Passing in FAHRENHEIT as the unit parameter to ENV.readTemperature(...),
  // allows you to read the sensor values in imperial units
  float temperature = HTS.readTemperature(FAHRENHEIT);
  temperature = temperature - OFFSET;
  float humidity    = HTS.readHumidity();

  int pwm = map(int(temperature), 92, 98, 0, 255);
  pwm = constrain(pwm, 0, 255);
  analogWrite(fanControlPin, pwm);

  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 5);     // Start at top-left corner

  display.print("Temperature = "); display.print(temperature); display.print((char)247); display.println("F");
  display.println();
  display.print("Humidity = "); display.print(humidity); display.println("%");
  display.println();
  display.print("PWM (0-255) = "); display.println(pwm);
  display.println(); 
  display.println("92-98 F");
  display.display();
  delay(1000);
}

void counter() {
  count++;
}
