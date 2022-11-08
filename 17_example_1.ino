#include <Servo.h>
#define PIN_LED 9
// Arduino pin assignment

#define PIN_POTENTIOMETER 3 // Potentiometer at Pin A3
// Add IR Sensor Definition Here !!!
#define PIN_IR 0
#define PIN_SERVO 10
#define _EMA_ALPHA 0.1

#define _DIST_MIN 100.0   // minimum distance to be measured (unit: mm)
#define _DIST_MAX 250.0   // maximum distance to be measured (unit: mm)




#define _DUTY_MIN 550  // servo full clock-wise position (0 degree)
#define _DUTY_NEU 1480 // servo neutral position (90 degree)e;
#define _DUTY_MAX 2410 // servo full counter-clockwise position (180 degree)

#define LOOP_INTERVAL 20   // Loop Interval (unit: msec)

float  dist_ema, dist_prev=_DIST_MAX;

Servo myservo;
unsigned long last_loop_time;   // unit: msec

void setup()
{
  myservo.attach(PIN_SERVO); 
  myservo.writeMicroseconds(_DUTY_MIN);
  pinMode(PIN_LED, OUTPUT);
  Serial.begin(2000000);
}

void loop()
{
  unsigned long time_curr = millis();
  int a_value;
  float duty;

  // wait until next event time
  if (time_curr < (last_loop_time + LOOP_INTERVAL))
    return;
  last_loop_time += LOOP_INTERVAL;

  // Remove Next line !!!
  //a_value = analogRead(PIN_POTENTIOMETER);
  // Read IR Sensor value !!!
  a_value = analogRead(PIN_IR);
  // Convert IR sensor value into distance !!!
  unsigned long dist_raw = (6762.0/(a_value-9)-4.0)*10.0 - 60.0;
  // we need distance range filter here !!!
    
  if (dist_raw < _DIST_MIN) {
    dist_raw = dist_prev;           // cut lower than minimum
    digitalWrite(PIN_LED, 1);       // LED OFF
  } else if (dist_raw > _DIST_MAX) {
    dist_raw = dist_prev;           // Cut higher than maximum
    digitalWrite(PIN_LED, 1);       // LED OFF
  } else {    // In desired Range
    digitalWrite(PIN_LED, 0);       // LED ON      
    dist_prev = dist_raw;
  }

  dist_ema =  _EMA_ALPHA * dist_raw + (1-_EMA_ALPHA) * dist_ema;


  if (dist_ema < _DIST_MIN) {
    // dist_ema = dist_prev;
    myservo.writeMicroseconds(_DUTY_MIN); 
    //delay(100);
  }
  else if (dist_ema > _DIST_MAX) {
    // dist_ema = dist_prev;
    myservo.writeMicroseconds(_DUTY_MAX);
    //delay(100);
    
  }
  else if(dist_ema>100.0 && dist_ema < 250.0){
   // dist_prev = dist_ema;
    
    //duty = (1860.0 / 180.0) * (dist_ema - 100.0) + 550.0;

    duty = _DUTY_MIN + ((dist_ema - 100.0) / 150.0) * (_DUTY_MAX - _DUTY_MIN);
    myservo.writeMicroseconds(duty); 
  }

  
  // we need EMA filter here !!!
  //dist_ema =  _EMA_ALPHA * dist_raw + (1-_EMA_ALPHA) * dist_ema;

   

  // map distance into duty
  
  //duty = map(a_value, 0, 1023, _DUTY_MIN, _DUTY_MAX);
  //myservo.writeMicroseconds(duty);

  // print IR sensor value, distnace, duty !!!
  Serial.print("MIN: "); Serial.print(_DIST_MIN);
  Serial.print(", IR: "); Serial.print(a_value);
  Serial.print(", dist: "); Serial.print(dist_raw);
  Serial.print(", ema: "); Serial.print(dist_ema);
  Serial.print(", servo: "); Serial.print(duty);
  Serial.print(", MAX: "); Serial.print(_DIST_MAX);
  Serial.println("");
}

