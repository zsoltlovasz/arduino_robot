//basic deifinitions

int pwm_engine_port=9;
int angle=0;
int angle_step=5;
int min_angle=-255;
int max_angle=255;
int step_delay=30;
int m1f=2;
int m1b=4;
int m2f=7;
int m2b=8;

void setup() {
  // put your setup code here, to run once:
  // blinking the internal led is a MUST :)
  pinMode(LED_BUILTIN, OUTPUT);
  // output that drives the pwm_engine
  pinMode(pwm_engine_port, OUTPUT);
  pinMode(m1f, OUTPUT);
  pinMode(m1b, OUTPUT);
  pinMode(m2f, OUTPUT);
  pinMode(m2b, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  analogWrite(pwm_engine_port, angle);
  angle+=angle_step;
  if(angle<=0 || angle>=max_angle)angle_step=-angle_step;
  if(angle_step>0){
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(m1f, HIGH);
    digitalWrite(m1b, LOW);
    digitalWrite(m2f, HIGH);
    digitalWrite(m2b, LOW);
  }
  else {
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(m1f, LOW);
    digitalWrite(m1b, HIGH);
    digitalWrite(m2f, LOW);
    digitalWrite(m2b, HIGH);
  }
  delay(step_delay);
}
