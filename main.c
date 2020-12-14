// Distance Sensor
#include <SoftwareSerial.h>

// All of these are constants associated to which arduino pins each of sensor/wire/etc are plugged into 
#define echoPin 7
#define trigPin 8 

// Bluetooth     //old
#define TXpin 10 //5
#define RXpin 11 //4
#define signalPin 2


long duration;
long distance, distance_inches;
SoftwareSerial BTserial(TXpin,RXpin); // TX | RX
String bt_feedback = "";

bool BT_connected = false;
const byte BTSignalPin = 2;

void setup() {
  Serial.begin(9600);
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(BTSignalPin, INPUT);

  // starts bluetooth output
  BTserial.begin(9600);

  Serial.println("Arduino Booted. Please connected Bluetooth device now.");

//   wait for connection
  while (!BT_connected) {
//    Serial.println(digitalRead(BTSignalPin));
    if (digitalRead(BTSignalPin)==HIGH) 
      {
        BT_connected = true;
      }
  }

  Serial.println("Thank you for choosing ShaZam BT.");

}

void loop() {

  if (digitalRead(BTSignalPin)==HIGH) {
    if (BTserial.available()) {
        bt_feedback = String(BTserial.read());
        String response = "[BLUETOOTH] " + bt_feedback; 
        Serial.println(response);
    }
  
    
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
  
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
  
    duration = pulseIn(echoPin, HIGH);
  
    // distance is in centimeters after this line is ran
    distance = ((duration / 29) / 2); 
  
    // distance is in inches after this line is ran
    // 1 cm = 0.393701 in.
    // distance_inches = distance * 0.393701;
  
    // calculate if adjancent car is in blind spot
    // an American lane is 12 feet wide
    // 150 cm approx 5 ft, 250 cm approx 8 ft
    if (distance  >= 150 && distance <= 399)
      Serial.println("------Adjancent Car within range------");  
    // the range of the ultrasonic sensor is only 400 cm so anything more is wrong
    else if (distance > 399)
      // no obj found, beyond the range of the sensor
      Serial.println("Nearest Object too far away, ignore this");
    else 
      Serial.println(distance);  
  
    BTserial.print(distance);
      
    //Miliseconds
    delay(50);
  } else {
    Serial.println("Bluetooth Disconnected. Please reconnect to continue");
    if (digitalRead(BTSignalPin)==HIGH) {
        BT_connected = true;
    }
  }
}