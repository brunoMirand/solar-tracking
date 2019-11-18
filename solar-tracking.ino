#include <ESP32Servo.h>

#include <Array.h>

#include <Arduino.h>

#include <WiFi.h>

#include <AsyncTCP.h>

#include <ESPAsyncWebServer.h>

#include <ESPDash.h>

AsyncWebServer server(80);

const char * ssid = "Residencial";
const char * password = "MPBBcintra";

int LED_BUILTIN = 2;

Servo servoMotorHorizontal;
int servoHorizontalLimiteHigh = 180;
int servoHorizontalLimiteLow = 20;

Servo servoMotorVertical;
int servoVerticalLimiteHigh = 180;
int servoVerticalLimiteLow = 20;

// Get ports sensors
int LDR_TOP_LEFT = 4;
int LDR_BOTTOM_LEFT = 4;
int LDR_TOP_RIGHT = 4;
int LDR_BOTTOM_RIGHT = 4;

int SPEED = 5;
int highestValue;

int freq = 0;
int channel = 0;
int resolution = 8;

int getMaxValueBetweenSensors() {
  int sensorTopLeft = analogRead(LDR_TOP_LEFT);
  int sensorBottomLeft = analogRead(LDR_BOTTOM_LEFT);
  int sensorTopRight = analogRead(LDR_TOP_RIGHT);
  int sensorBottomRight = analogRead(LDR_BOTTOM_RIGHT);

  // Top Average and Bottom Average (vertical rotation)
  int topAverage = (sensorTopLeft + sensorTopRight) / 2;
  int bottomAverage = (sensorBottomLeft + sensorBottomRight) / 2;

  // Left Average and Right Average (horizontal rotation)
  int leftAverage = (sensorTopLeft + sensorBottomLeft) / 2;
  int rightAverage = (sensorTopRight + sensorBottomRight) / 2;
  int sensorArray[4] = {
    topAverage,
    bottomAverage,
    leftAverage,
    rightAverage
  };
  Array<int> array = Array<int>(sensorArray, 4);
  return array.getMax() + 5;
}

void sliderChanged(const char * id, int value) {
  Serial.println("Slider Changed - " + String(id) + " " + String(value));
  servoMotorVertical.write(value);
  ESPDash.updateGaugeChart("gauge1", value);
}

void setup() {
  Serial.begin(115200);
  ledcSetup(channel, freq, resolution);
  pinMode(LED_BUILTIN, OUTPUT);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.printf("WiFi Failed!\n");
    return;
  }
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("Connected to the WIFI");

  highestValue = getMaxValueBetweenSensors();
  Serial.println("highestValue");
  Serial.println(highestValue);
  delay(10);

  servoMotorVertical.attach(13);
  servoMotorVertical.write(110);
  servoMotorHorizontal.attach(5);
  servoMotorHorizontal.write(100);

  ESPDash.init(server); // Initiate ESPDash and attach your Async webserver instance

  // Attach Slider Change Events
  ESPDash.attachSliderChanged(sliderChanged);

  ESPDash.addSliderCard("slider1", "Servo Vertical", 3);
  ESPDash.addSliderCard("slider2", "Servo Vertical", 3);
  ESPDash.addGaugeChart("gauge1", "Sensor 1");
  ESPDash.addGaugeChart("gauge2", "Sensor 2");
  ESPDash.addGaugeChart("gauge3", "Sensor 3", 10);
  ESPDash.addGaugeChart("gauge4", "Sensor 4");
  ESPDash.addTemperatureCard("temp1", "Intensidade", 0, 20);
  server.begin();

  delay(20);
}

int painelSolar() {
  int servoVertical = servoMotorVertical.read();
  int servoHorizontal = servoMotorHorizontal.read();

  int sensorTopLeft = analogRead(LDR_TOP_LEFT);
  int sensorBottomLeft = analogRead(LDR_BOTTOM_LEFT);
  int sensorTopRight = analogRead(LDR_TOP_RIGHT);
  int sensorBottomRight = analogRead(LDR_BOTTOM_RIGHT);

  // Top Average and Bottom Average (vertical rotation)
  int topAverage = (sensorTopLeft + sensorTopRight) / 2;
  int bottomAverage = (sensorBottomLeft + sensorBottomRight) / 2;

  // Left Average and Right Average (horizontal rotation)
  int leftAverage = (sensorTopLeft + sensorBottomLeft) / 2;
  int rightAverage = (sensorTopRight + sensorBottomRight) / 2;

  ESPDash.updateGaugeChart("gauge2", sensorTopLeft);

  if ((topAverage > highestValue) && (topAverage < bottomAverage)) {
    servoMotorVertical.write(servoVertical + SPEED);
    if (servoVertical > servoVerticalLimiteHigh) {
      servoVertical = servoVerticalLimiteHigh;
    }
    delay(10);
  } else if ((bottomAverage > highestValue) && (bottomAverage < topAverage)) {
    servoMotorVertical.write(servoVertical - SPEED);
    if (servoVertical < servoVerticalLimiteLow) {
      servoVertical = servoVerticalLimiteLow;
    }
    delay(10);
  } else {
    servoMotorVertical.write(servoVertical);
  }

  if ((leftAverage > highestValue) && (leftAverage > rightAverage)) {
    servoMotorHorizontal.write(servoHorizontal + SPEED);
    if (servoHorizontal > servoHorizontalLimiteHigh) {
      servoHorizontal = servoHorizontalLimiteHigh;
    }
    delay(10);
  } else if ((rightAverage > highestValue) && (rightAverage > leftAverage)) {
    servoMotorHorizontal.write(servoHorizontal - SPEED);
    if (servoHorizontal < servoHorizontalLimiteLow) {
      servoHorizontal = servoHorizontalLimiteLow;
    }
    delay(10);
  } else {
    servoMotorHorizontal.write(servoHorizontal);
  }

  Serial.println("-----------");
  Serial.println("TOP_AVERAGE");
  Serial.println(sensorTopLeft);
  Serial.println(topAverage);
  Serial.println("-----------");
  Serial.println("BOTTOM_AVERAGE");
  Serial.println(bottomAverage);
  Serial.println("-----------");

  Serial.println("-----------");
  Serial.println("LEFT_AVERAGE");
  Serial.println(leftAverage);
  Serial.println("-----------");
  Serial.println("RIGHT_AVERAGE");
  Serial.println(rightAverage);
  Serial.println("-----------");
  delay(1000);
}

void loop() {
  painelSolar();
  delay(10);
}
