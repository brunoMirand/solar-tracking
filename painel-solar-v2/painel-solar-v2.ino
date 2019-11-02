#include <Servo.h>
#include <Array.h>

Servo servoMotorHorizontal;
int servoHorizontalLimiteHigh = 180;
int servoHorizontalLimiteLow = 20;

Servo servoMotorVertical;
int servoVerticalLimiteHigh = 180;
int servoVerticalLimiteLow = 20;

// Get ports sensors
int LDR_TOP_LEFT = A0;
int LDR_BOTTOM_LEFT = A1;
int LDR_TOP_RIGHT = A2;
int LDR_BOTTOM_RIGHT = A3;

int SPEED = 5;
int highestValue;

void setup() {
    Serial.begin(9600);

    highestValue = getMaxValueBetweenSensors();
    Serial.println("highestValue");
    Serial.println(highestValue);
    delay(1000);

    servoMotorVertical.attach(6);
    servoMotorVertical.write(110);
    servoMotorHorizontal.attach(5);
    servoMotorHorizontal.write(100);
    delay(200);
}

void loop() {
    painelSolar();
    delay(10);
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

    if ((topAverage > highestValue) && (topAverage > bottomAverage)) {
        servoMotorVertical.write(servoVertical + SPEED);
        if (servoVertical > servoVerticalLimiteHigh) {
                servoVertical = servoVerticalLimiteHigh;
        }
    }
    else if ((bottomAverage > highestValue) && (bottomAverage > topAverage)) {
        servoMotorVertical.write(servoVertical - SPEED);
        if (servoVertical < servoVerticalLimiteLow) {
                servoVertical = servoVerticalLimiteLow;
        }
    }
    else {
        servoMotorVertical.write(servoVertical);
    }

    if ((leftAverage > highestValue) && (leftAverage > rightAverage)) {
      servoMotorHorizontal.write(servoHorizontal + SPEED);
      if (servoHorizontal > servoHorizontalLimiteHigh) {
            servoHorizontal = servoHorizontalLimiteHigh;
      }
    }
    else if ((rightAverage > highestValue) && (rightAverage > leftAverage)) {
      servoMotorHorizontal.write(servoHorizontal - SPEED);
      if (servoHorizontal < servoHorizontalLimiteLow) {
            servoHorizontal = servoHorizontalLimiteLow;
      }

    }
    else {
      servoMotorHorizontal.write(servoHorizontal);
    }

    Serial.println("-----------");
    Serial.println("TOP_AVERAGE");
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

    int sensorArray[4] = {topAverage, bottomAverage, leftAverage, rightAverage};
    Array<int> array = Array<int>(sensorArray, 4);
    return array.getMax() + 5;
}
