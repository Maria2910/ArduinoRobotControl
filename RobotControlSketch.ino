const int TRIG = 3;
const int ECHO = 4;
const int ENA = 5;
const int ENB = 6;
const int IN1 = 7;
const int IN2 = 8;
const int IN3 = 9;
const int IN4 = 10;

int pwm = 120;
int safeDistance = 20; // минимальное расстояние в см
bool autoStopEnabled = true; // флаг автостопа

void setup() {
  Serial.begin(9600);
  
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  robot_stop();
  
  Serial.println("Robot Ready! Commands: F,B,L,R,S,A,a");
  Serial.println("A - auto stop ON, a - auto stop OFF");
}

void loop() {
  // Проверяем расстояние
  int distance = getDistance();
  
  // Автоматическая остановка если включена и движемся вперед
  if (autoStopEnabled && distance > 0 && distance < safeDistance) {
    if (isMovingForward()) {
      robot_stop();
      Serial.println("OBSTACLE DETECTED! AUTO STOP");
    }
  }
  
  // Обработка команд с Bluetooth
  if (Serial.available()) {
    char val = Serial.read();
    
    if (val == 'F') forward();
    else if (val == 'B') backward();
    else if (val == 'L') left();
    else if (val == 'R') right();
    else if (val == 'S') robot_stop();
    else if (val == 'A') { 
      autoStopEnabled = true; 
      Serial.println("Auto stop: ON");
    }
    else if (val == 'a') { 
      autoStopEnabled = false; 
      Serial.println("Auto stop: OFF");
    }
  }
  
  delay(50); // небольшая задержка для стабильности
}

// Функция измерения расстояния
int getDistance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  
  long duration = pulseIn(ECHO, HIGH);
  int distance = duration * 0.034 / 2;
  
  return distance;
}

// Проверка, движется ли робот вперед
bool isMovingForward() {
  return (digitalRead(IN1) == HIGH && digitalRead(IN2) == LOW && 
          digitalRead(IN3) == HIGH && digitalRead(IN4) == LOW);
}

void forward() {
  // Проверяем расстояние перед движением
  int distance = getDistance();
  if (autoStopEnabled && distance > 0 && distance < safeDistance) {
    Serial.println("Cannot move forward - obstacle too close!");
    return;
  }
  
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, pwm);
  analogWrite(ENB, pwm);
}

void backward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, pwm);
  analogWrite(ENB, pwm);
}

void right() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, pwm);
  analogWrite(ENB, pwm);
}

void left() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, pwm);
  analogWrite(ENB, pwm);
}

void robot_stop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}