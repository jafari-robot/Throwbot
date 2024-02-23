#include <LoRa.h>

#define LED_C A0
#define BUZZ_C A1

// Motor A connections
#define enA 6
#define in1 4
#define in2 5

// Motor B connections
#define enB 9
#define in3 7
#define in4 8

#define encoder1Pin 2
#define encoder2Pin 3

volatile long encoder1Count = 0;
volatile long encoder2Count = 0;

long previousTime = 0;
float ePrevious = 0;
float eIntegral = 0;

String LoRaData;
int mv_st = 0;

void setup() {
  Serial.begin(9600);

  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(encoder1Pin, INPUT);
  pinMode(encoder2Pin, INPUT);
  pinMode(LED_C, OUTPUT);
  pinMode(BUZZ_C, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(encoder1Pin), handleEncoder1, RISING);
  attachInterrupt(digitalPinToInterrupt(encoder2Pin), handleEncoder2, RISING);

  while (!Serial);
  Serial.println("LoRa RX");
  
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa Failed!");
    while (1);
  }
  LoRa.setSyncWord(0xA5);
  Serial.println("LoRa Initializing OK!");
}

void loop() {
  LoRaData = "";
  int target = encoder1Count;
  int packetSize = LoRa.parsePacket();
  float p = 2.0;
  float i = 0.0;
  float d = 0.0;
  float u = pidController(target, p, i, d);

  if (packetSize) {
    while (LoRa.available()) {
      LoRaData = LoRa.readString();
      Serial.println(LoRaData);
      if (LoRaData == "0") {
        mv_st = 0;
      }
      else if (LoRaData == "1") {
        mv_st = 1;
      }
      else if (LoRaData == "2") {
        mv_st = 2;
      }
      else if (LoRaData == "3") {
        digitalWrite(BUZZ_C, HIGH);
      }
      else {
        Serial.println("Invalid Data!");
        digitalWrite(BUZZ_C, LOW);
      }
    }
  }

  if (mv_st == 0) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    analogWrite(enA, 0);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
    analogWrite(enB, 0);
  }
  else if (mv_st == 1) {
    moveMotor(u, 1);
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(enA, 255);
  }
  else if (mv_st == 2) {  
    moveMotor(u, -1);
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    analogWrite(enA, 255);
  }
  
  Serial.print(encoder1Count);
  Serial.print(", ");
  Serial.println(encoder2Count);

}

void handleEncoder1() {
  encoder1Count++;
}

void handleEncoder2() {
  encoder2Count++;
}

void moveMotor(float u, int direction) {
  float speed = fabs(u);
  if (speed > 255) {
    speed = 255;
  }

  if (encoder2Count > encoder1Count) {
    speed = 0;
  }

  if (direction == 1) {
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
  }
  else if (direction == -1) {
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
  }
  analogWrite(enB, speed);
}

float pidController(int target, float p, float i, float d) {
  long currentTime = micros();
  float deltaT = ((float)(currentTime - previousTime)) / 1.0e6;

  int e = encoder2Count - target;
  float eDerivative = (e - ePrevious) / deltaT;
  eIntegral = eIntegral + e * deltaT;

  float u = (p * e) + (i * eIntegral) + (d * eDerivative);

  previousTime = currentTime;
  ePrevious = e;

  return u;
}