//basic deifinitions

int hatra_jobb=7;
int elore_jobb=8;
int gazpedal_jobb=9;
int fejmozgato=10;
int gazpedal_bal=11;
int elore_bal=12;
int hatra_bal=13;

int gazpedal=0;
int gazpedal_min=0;
int gazpedal_lepes=10;
int gazpedal_max=100;
int kesleltetes_lepesek_kozott=100;

void setup() {
  // put your setup code here, to run once:
  // blinking the internal led is a MUST :)
  pinMode(LED_BUILTIN, OUTPUT);
  // output that drives the pwm_engine
  pinMode(hatra_jobb, OUTPUT);
  pinMode(elore_jobb, OUTPUT);
  pinMode(gazpedal_jobb, OUTPUT);
  pinMode(fejmozgato, OUTPUT);
  pinMode(gazpedal_bal, OUTPUT);
  pinMode(elore_bal, OUTPUT);
  pinMode(hatra_bal, OUTPUT);
  //Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  analogWrite(gazpedal_jobb, gazpedal);
  analogWrite(fejmozgato, gazpedal);
  analogWrite(gazpedal_bal, gazpedal);
  gazpedal+=gazpedal_lepes;
  if(gazpedal<=gazpedal_min || gazpedal>=gazpedal_max)gazpedal_lepes=-gazpedal_lepes;
  if(gazpedal_lepes>0){
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(elore_jobb, HIGH);
    digitalWrite(hatra_jobb, LOW);
    digitalWrite(elore_bal, HIGH);
    digitalWrite(hatra_bal, LOW);
  }
  else {
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(elore_jobb, LOW);
    digitalWrite(hatra_jobb, HIGH);
    digitalWrite(elore_bal, LOW);
    digitalWrite(hatra_bal, HIGH);
  }
  delay(kesleltetes_lepesek_kozott);
  //Serial.println("serialtst");
}
