#include <Servo.h>  //servo library

#include "timer.h"

constexpr int kLineTrackerRight = 10;
constexpr int kLineTrackerMiddle = 4;
constexpr int kLineTrackerLeft = 2;

constexpr int kMotorSpeedControlA = 5;
constexpr int kMotorSpeedControlB = 6;
constexpr int kMotorDirectionA1 = 7;
constexpr int kMotorDirectionA2 = 8;
constexpr int kMotorDirectionB1 = 9;
constexpr int kMotorDirectionB2 = 11;

// constexpr long kCheckInterval = 2800;  // interval to wait for (milliseconds)
constexpr long kCheckInterval = 500;  // interval to wait for (milliseconds)
constexpr long kForwardInterval = 1000;
constexpr int kCarSpeed = 150;

Servo my_servo;  // create servo object to control servo

int echo_pin = A4;
int trig_pin = A5;

class Car {
 public:
  static Car& GetInstance() {
    static Car instance;
    return instance;
  }

  void Setup() {
    my_servo.attach(3);  // attach servo on pin 3 to servo object
    Serial.begin(9600);
    pinMode(echo_pin, INPUT);
    pinMode(trig_pin, OUTPUT);
    pinMode(kMotorDirectionA1, OUTPUT);
    pinMode(kMotorDirectionA2, OUTPUT);
    pinMode(kMotorDirectionB1, OUTPUT);
    pinMode(kMotorDirectionB2, OUTPUT);
    pinMode(kMotorSpeedControlA, OUTPUT);
    pinMode(kMotorSpeedControlB, OUTPUT);
    pinMode(kLineTrackerRight, INPUT);
    pinMode(kLineTrackerMiddle, INPUT);
    pinMode(kLineTrackerLeft, INPUT);
    StopCar();
  }

  void MoveForward() {
    analogWrite(kMotorSpeedControlA, kCarSpeed);
    analogWrite(kMotorSpeedControlB, kCarSpeed);
    digitalWrite(kMotorDirectionA1, HIGH);
    digitalWrite(kMotorDirectionA2, LOW);
    digitalWrite(kMotorDirectionB1, LOW);
    digitalWrite(kMotorDirectionB2, HIGH);
    Serial.println("go forward!");
  }

  void MoveBackward() {
    analogWrite(kMotorSpeedControlA, kCarSpeed);
    analogWrite(kMotorSpeedControlB, kCarSpeed);
    digitalWrite(kMotorDirectionA1, LOW);
    digitalWrite(kMotorDirectionA2, HIGH);
    digitalWrite(kMotorDirectionB1, HIGH);
    digitalWrite(kMotorDirectionB2, LOW);
    Serial.println("go back!");
  }

  void TurnLeft() {
    analogWrite(kMotorSpeedControlA, kCarSpeed);
    analogWrite(kMotorSpeedControlB, kCarSpeed);
    digitalWrite(kMotorDirectionA1, LOW);
    digitalWrite(kMotorDirectionA2, HIGH);
    digitalWrite(kMotorDirectionB1, LOW);
    digitalWrite(kMotorDirectionB2, HIGH);
    Serial.println("go left!");
  }

  void TurnRight() {
    analogWrite(kMotorSpeedControlA, kCarSpeed);
    analogWrite(kMotorSpeedControlB, kCarSpeed);
    digitalWrite(kMotorDirectionA1, HIGH);
    digitalWrite(kMotorDirectionA2, LOW);
    digitalWrite(kMotorDirectionB1, HIGH);
    digitalWrite(kMotorDirectionB2, LOW);
    Serial.println("go right!");
  }

  void StopCar() {
    digitalWrite(kMotorSpeedControlA, LOW);
    digitalWrite(kMotorSpeedControlB, LOW);
    Serial.println("Stop!");
  }

  int MeasureDistance() {
    digitalWrite(trig_pin, LOW);
    delayMicroseconds(2);
    digitalWrite(trig_pin, HIGH);
    delayMicroseconds(20);
    digitalWrite(trig_pin, LOW);
    float distance = pulseIn(echo_pin, HIGH);
    distance = distance / 58;
    return static_cast<int>(distance);
  }

  int CheckObstacle() {
    my_servo.write(90);  // Center
    // delay(250);
    int middle_distance = MeasureDistance();
    // delay(250);

    // my_servo.write(0);  // Left
    // delay(250);
    // distances.left = MeasureDistance();
    // delay(250);

    // my_servo.write(180);  // Right
    // delay(250);
    // distances.right = MeasureDistance();
    // delay(250);

    // my_servo.write(90);  // Center
    // delay(250);

    return middle_distance;
  }

 private:
  Car() {}  // Private constructor

  // Delete copy constructor and assignment operator
  Car(const Car&) = delete;
  Car& operator=(const Car&) = delete;
};

void setup() { Car::GetInstance().Setup(); }

Timer obstacle_timer(kCheckInterval);
Timer forward_timer(kForwardInterval);
bool can_go_forward = true;

void loop() {
  Car& car = Car::GetInstance();

  if (obstacle_timer.Elapsed()) {
    car.StopCar();
    int distance_middle = car.CheckObstacle();

    if (distance_middle < 20 /*  || distances.left < 50 || distances.right < 50 */) {
      can_go_forward = false;
    } else {
      can_go_forward = true;
    }
  }

  if (!can_go_forward) {
    return;
  }

  if (forward_timer.Elapsed()) {
    car.MoveForward();
    delay(100);
    car.StopCar();
  }

  const bool is_left_detected = digitalRead(kLineTrackerLeft);
  const bool is_middle_detected = digitalRead(kLineTrackerMiddle);
  const bool is_right_detected = digitalRead(kLineTrackerRight);

  if (!is_left_detected && !is_middle_detected && !is_right_detected) {
    car.StopCar();
    return;
  }

  if (!is_left_detected) {
    car.TurnLeft();
  } else if (!is_right_detected) {
    car.TurnRight();
  } else if (!is_middle_detected) {
    car.MoveForward();
  }
}
