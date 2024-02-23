#include <LoRa.h>

#define LED_G 9
#define LED_B 8
#define BUZZ 7
#define SW_1 4
#define SW_2 3
#define SW_3 21


String inputString = "";
int counter = 0;


void setup() {
  Serial.begin(9600);
  
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  pinMode(BUZZ, OUTPUT);
  pinMode(SW_1, INPUT_PULLUP);
  pinMode(SW_2, INPUT_PULLUP);
  pinMode(SW_3, INPUT_PULLUP);

  digitalWrite(LED_G, LOW);
  digitalWrite(LED_B, LOW);

  while (!Serial);
  Serial.println("LoRa TX");
  
  if (!LoRa.begin(433E6)){
    Serial.println("Starting LoRa Failed!");
    while (1){
      digitalWrite(LED_B, HIGH);
      delay(100);
      digitalWrite(LED_B, LOW);
      delay(100);
    }
  }
  for (int i =0; i < 5; i++) {
    digitalWrite(LED_G, HIGH);
    delay(100);
    digitalWrite(LED_G, LOW);
    delay(100);
  }
  LoRa.setSyncWord(0xA5);
  Serial.println("LoRa Initializing OK!");
  digitalWrite(LED_G, HIGH);
}

void loop() {
  while (Serial.available()) {
      LoRa.beginPacket();
      LoRa.print
      ((char)Serial.read());
      LoRa.endPacket();
  }
  Serial.print("Switch-1: ");
  Serial.println(digitalRead(SW_1));
  Serial.print("Switch-2: ");
  Serial.println(digitalRead(SW_2));
  Serial.print("Switch-3: ");
  Serial.println(digitalRead(SW_3));
  Serial.println("---------------");
  delay(50);
}
