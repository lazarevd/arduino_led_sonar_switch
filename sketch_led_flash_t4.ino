int echoPin = 20; //A2
int trigPin = 21; //A3
int led = 10; //10
int rxled = 17; //led on board
int work_brightness = 200;
int sonar_time_delay = 200;
unsigned long switch_off_delay = 50000; //unsigned long for compare to switch_off_time

int switch_off_delta = 10;//less delta - faster decay
int switch_on_delta = 10;//less delta - faster brighten
int max_brightness = 200;
int distance_switch = 80; //in cm






//work variables
int state = 0; //0 - выкл, 1 включение, 2 выключение, 3 вкл
int current_brightness = 0;
unsigned long last_time = 0;
unsigned long switch_off_time = 0;
unsigned long timer_sonar = 0;
int tmp_sonar_distance = 1000;



int switch_on(int current_bright, int time_delta, int max_bright) {
digitalWrite(rxled, LOW);
int ret_bright = current_bright;
if (ret_bright < max_bright) {
  if (millis()- last_time > time_delta) {
    ret_bright++;
    last_time = millis();
  }
} else {
  ret_bright = max_bright;
  state = 3;
}
return ret_bright;
}

int switch_off(int current_bright, int time_delta) {
digitalWrite(rxled, HIGH);
int ret_bright = current_bright;
if (ret_bright > 0) {
  if (millis()- last_time > time_delta) {
    ret_bright--;
    last_time = millis();
  }
} else {
  ret_bright = 0;
  state = 0;
}
return ret_bright;
}



int normal_working(int max_bright, unsigned long switch_off_delay) {
  if (millis() - switch_off_time > switch_off_delay) {
    state = 2;
  }
  return work_brightness;
}



int measure_sonar() {
int duration, cm;    
digitalWrite(trigPin, LOW); 
long time_store = micros();
while (micros() - time_store < 2) {
//do nothing
}
  digitalWrite(trigPin, HIGH);
while (micros() - time_store < 10) {
//do nothing
}  
  digitalWrite(trigPin, LOW); 
  duration = pulseIn(echoPin, HIGH);
  return duration / 58;
}

void setup() {
  pinMode(led, OUTPUT);
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  pinMode(rxled, OUTPUT);
}




//if (millis() - switch_off_time > switch_off_delay) {

void loop() { 
 int time_from_last_state = millis()-timer_sonar;
 if (time_from_last_state > sonar_time_delay) {//sonar measure
  tmp_sonar_distance = measure_sonar();
  timer_sonar = millis();

  Serial.println((millis() - switch_off_time));
  Serial.println((millis() - switch_off_time) > switch_off_delay);

  if (state==0) {//0 - выкл, 1 включение, 2 выключение, 3 вкл
  Serial.println("откл");
  } 
  else if (state==1) {
  Serial.println("включение");
  } 
  else if (state==2) {
  Serial.println("выключение");
  } 
  else if (state==3) {
  Serial.println("вкл");
  }
  } 
 
  if (tmp_sonar_distance < distance_switch) {
   if (state != 3) {
   state = 1;
   }  
   switch_off_time = millis();
  
 }
 
 
 if (state == 1) {//main logic
 current_brightness = switch_on(current_brightness, switch_on_delta, max_brightness);
 } else if (state == 2) {
 current_brightness = switch_off(current_brightness, switch_off_delta);  
 } else if (state == 3) {
 current_brightness = normal_working(work_brightness, switch_off_delay);
 }
 

 analogWrite(led,current_brightness);
 
  }
