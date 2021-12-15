#include <Servo.h>
#include <Adafruit_NeoPixel.h>

//valtozok
unsigned int hatra_jobb=3;
unsigned int elore_jobb=4;
unsigned int gazpedal_jobb=5;
unsigned int gazpedal_bal=6;
unsigned int elore_bal=7;
unsigned int hatra_bal=8;

unsigned int fejmozgato_lab=9;
Servo fejmozgato;
unsigned int gazpedal=0;

unsigned int ledszalag_lab=2;
unsigned int ledszalag_ledszam=12;
Adafruit_NeoPixel ledszalag(ledszalag_ledszam, ledszalag_lab, NEO_GRB + NEO_KHZ800);

unsigned int ledmatrix_adat=12;
unsigned int ledmatrix_ora=13;
volatile byte ledmatrix_puffer[8]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
byte ledmatrix_intenzitas;
//ledmatrix kepek
byte _mc_sziv[]={0x00, 0x66, 0x99, 0x81, 0x42, 0x24, 0x18, 0x00};
byte _mc_ures[]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
//ledmatrix betuk - forras: https://github.com/dhepper/font8x8/blob/master/font8x8_basic.h
byte _mc_a[]={0x0C, 0x1E, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x00};
byte _mc_b[]={0x3F, 0x66, 0x66, 0x3E, 0x66, 0x66, 0x3F, 0x00};
byte _mc_d[]={0x1F, 0x36, 0x66, 0x66, 0x66, 0x36, 0x1F, 0x00};
byte _mc_e[]={0x7F, 0x46, 0x16, 0x1E, 0x16, 0x46, 0x7F, 0x00};
byte _mc_i[]={0x1E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00};
byte _mc_k[]={0x67, 0x66, 0x36, 0x1E, 0x36, 0x66, 0x67, 0x00};
byte _mc_l[]={0x0F, 0x06, 0x06, 0x06, 0x46, 0x66, 0x7F, 0x00};
byte _mc_m[]={0x63, 0x77, 0x7F, 0x7F, 0x6B, 0x63, 0x63, 0x00};
byte _mc_n[]={0x63, 0x67, 0x6F, 0x7B, 0x73, 0x63, 0x63, 0x00};
byte _mc_o[]={0x1C, 0x36, 0x63, 0x63, 0x63, 0x36, 0x1C, 0x00};
byte _mc_r[]={0x3F, 0x66, 0x66, 0x3E, 0x36, 0x66, 0x67, 0x00};

unsigned int ultrahang_keres=10;
unsigned int ultrahang_valasz=11;
unsigned int tavolsag=0;

void motor_beallitas(){
  pinMode(hatra_jobb, OUTPUT);
  pinMode(elore_jobb, OUTPUT);
  pinMode(gazpedal_jobb, OUTPUT);
  pinMode(gazpedal_bal, OUTPUT);
  pinMode(elore_bal, OUTPUT);
  pinMode(hatra_bal, OUTPUT);
}

//meghajto motorok fuggvenyei
void motor_forgatas(unsigned int millisec, unsigned int gazpedal_lepes_kesleltetes=50){
  unsigned int gazpedal_max=255;
  unsigned int gazpedal_min=0;
  unsigned int gazpedal_lepes_koz=10;
  //felporges
  unsigned int gazpedal_aktualis_max=floor(gazpedal_lepes_koz*(millisec/(2*gazpedal_lepes_kesleltetes)));
  if(gazpedal_aktualis_max>gazpedal_max)gazpedal_aktualis_max=gazpedal_max;
  for(gazpedal=gazpedal_min; gazpedal<gazpedal_aktualis_max; gazpedal+=gazpedal_lepes_koz){
    if(gazpedal>gazpedal_max)gazpedal=gazpedal_max;
    analogWrite(gazpedal_jobb, gazpedal);
    analogWrite(gazpedal_bal, gazpedal);
    delay(gazpedal_lepes_kesleltetes);
  }
  //max gaz
  unsigned int maradek_ido=millisec-2*(gazpedal_aktualis_max-gazpedal_min)/gazpedal_lepes_koz*gazpedal_lepes_kesleltetes;
  if(maradek_ido>0)delay(maradek_ido);
  //leallas
  for(;gazpedal>gazpedal_min; gazpedal-=gazpedal_lepes_koz){
    if(gazpedal<gazpedal_min)gazpedal=gazpedal_min;
    analogWrite(gazpedal_jobb, gazpedal);
    analogWrite(gazpedal_bal, gazpedal);
    delay(gazpedal_lepes_kesleltetes);
  }
}

void motor_elore_ido(unsigned int millisec){
  digitalWrite(elore_jobb, HIGH);
  digitalWrite(elore_bal, HIGH);
  digitalWrite(hatra_jobb, LOW);
  digitalWrite(hatra_bal, LOW);
  motor_forgatas(millisec);    
}

void motor_hatra_ido(unsigned int millisec){
  digitalWrite(elore_jobb, LOW);
  digitalWrite(elore_bal, LOW);
  digitalWrite(hatra_jobb, HIGH);
  digitalWrite(hatra_bal, HIGH);
  motor_forgatas(millisec);    
}

void motor_jobbra_ido(unsigned int millisec){
  digitalWrite(elore_jobb, LOW);
  digitalWrite(elore_bal, HIGH);
  digitalWrite(hatra_jobb, HIGH);
  digitalWrite(hatra_bal, LOW);
  motor_forgatas(millisec, 5);    
}

void motor_balra_ido(unsigned int millisec){
  digitalWrite(elore_jobb, HIGH);
  digitalWrite(elore_bal, LOW);
  digitalWrite(hatra_jobb, LOW);
  digitalWrite(hatra_bal, HIGH);
  motor_forgatas(millisec, 5);    
}

void motor_elore(){
  motor_elore_ido(2000);
}

void motor_hatra(){
  motor_hatra_ido(2000);
}

void motor_jobbra(){
  motor_jobbra_ido(1000);
}

void motor_balra(){
  motor_balra_ido(1000);
}

void motor_megallas(){
  analogWrite(gazpedal_jobb, 0);
  analogWrite(gazpedal_bal, 0);
}

void fejmozgato_elore(){
  fejmozgato.write(90);
  delay(500);
}

void fejmozgato_balra(){
  fejmozgato.write(135);
  delay(500);
}

void fejmozgato_jobbra(){
  fejmozgato.write(45);
  delay(500);
}

void fejmozgato_beallitas(){
  fejmozgato.attach(fejmozgato_lab);
  delay(500);
  fejmozgato_balra();
  fejmozgato_jobbra();
  fejmozgato_elore();
}

//ledszalag fuggvenyek
void ledszalag_mutat(unsigned int millisec=0){
  ledszalag.show();
  delay(millisec);
}

void ledszalag_led_szin(unsigned int i, unsigned int r, unsigned int g, unsigned int b){
  ledszalag.setPixelColor(i, ledszalag.Color(r, g, b));
}

void ledszalag_led_szin(unsigned int i, unsigned int h, unsigned int v){
  unsigned int s=255;
  ledszalag.setPixelColor(i, ledszalag.ColorHSV(h, s, v));
}

void ledszalag_szin(unsigned int r, unsigned int g, unsigned int b){
  for(unsigned int i=0; i<ledszalag_ledszam; i++){
    ledszalag_led_szin(i, r, g, b);
  }
}

void ledszalag_szin(unsigned int h, unsigned int v){
  for(unsigned int i=0; i<ledszalag_ledszam; i++){
    ledszalag_led_szin(i, h, v);
  }
}

void ledszalag_torles(){
  ledszalag.clear();
  ledszalag_mutat();
}

void ledszalag_torles_kiveve_tavolsagjezo(){
  for(unsigned int i=0; i<ledszalag_ledszam; i++){
    if(i==3 || i==9)continue;
    ledszalag_led_szin(i, 0, 0, 0);
  }
}

void ledszalag_elore(bool mutat=true){
  ledszalag_torles_kiveve_tavolsagjezo();
  ledszalag_led_szin(1, 127, 0, 0);
  ledszalag_led_szin(5, 127, 0, 0);
  ledszalag_led_szin(8, 43, 43, 43);
  ledszalag_led_szin(10, 43, 43, 43);
  if(mutat)ledszalag_mutat();
}

void ledszalag_jobbra(){
  ledszalag_elore(false);
  ledszalag_led_szin(0, 85, 53, 0);
  ledszalag_led_szin(11, 85, 53, 0);
  ledszalag_mutat();
}

void ledszalag_balra(){
  ledszalag_elore(false);
  ledszalag_led_szin(6, 85, 53, 0);
  ledszalag_led_szin(7, 85, 53, 0);
  ledszalag_mutat();
}

void ledszalag_hatra(){
  ledszalag_elore(false);
  ledszalag_led_szin(2, 43, 43, 43);
  ledszalag_led_szin(4, 43, 43, 43);
  ledszalag_mutat();
}

void ledszalag_megallas(){
  ledszalag_szin(255, 0, 0);
  ledszalag_mutat();
}

void ledszalag_tavolsagjelzok(unsigned int tavolsag){
  //a tavolsag valalhol 2 es 400 cm kozott lesz
  //ezt osszenyomjuk 2-102 koze
  if(tavolsag<2)tavolsag=2;
  if(tavolsag>102)tavolsag=102;
  //majd atrakjuk 0-100 koze
  tavolsag-=2;
  //konvertalas 100-0cm -> 18000-0 szin (zold-sarga-piros), 18000/400=45
  unsigned int szin=tavolsag*180;
  ledszalag_led_szin(3, szin, 30);
  ledszalag_led_szin(9, szin, 30);
  ledszalag_mutat();
}

void ledszalag_beallitas(){
  ledszalag.begin();
  ledszalag_torles();
  ledszalag_szin(127, 0, 0);
  ledszalag_mutat(500);
  ledszalag_szin(0, 127, 0);
  ledszalag_mutat(500);
  ledszalag_szin(0, 0, 127);
  ledszalag_mutat(500);
  for(unsigned int k=0; k<12; k++){
    ledszalag.setPixelColor(k, ledszalag.ColorHSV(k*1500, 255, 5));
  }
  ledszalag_mutat(500);
}

void ledmatrix_torles(){
  for(byte i=0;i<8;i++){
    ledmatrix_puffer[i]=0x00;
  }
  ledmatrix_mutat();
}

void ledmatrix_megallas(){
  for(byte i=0;i<8;i++){
    ledmatrix_puffer[i]=0xFF;
  }
  ledmatrix_mutat();
}

void ledmatrix_betu(byte betu[]){
  for(byte i=0;i<8;i++){
    ledmatrix_puffer[i]=betu[7-i];
  }
  ledmatrix_mutat();
}

void ledmatrix_byte_kuldes(byte adat){
  for (unsigned int i = 0; i < 8; i++) {
    digitalWrite(ledmatrix_ora, LOW);
    digitalWrite(ledmatrix_adat, adat & 1 ? HIGH : LOW);
    adat >>= 1;
    digitalWrite(ledmatrix_ora, HIGH);
  }
}

void ledmatrix_adat_kuldes(byte cim, byte adat){
  ledmatrix_parancs_kuldes(0x44);
  digitalWrite(ledmatrix_adat, LOW);
  ledmatrix_byte_kuldes(0xC0 | cim);
  ledmatrix_byte_kuldes(adat);
  digitalWrite(ledmatrix_adat, HIGH);
}

void ledmatrix_parancs_kuldes(byte parancs){
    digitalWrite(ledmatrix_adat, LOW);
  ledmatrix_byte_kuldes(parancs);
  digitalWrite(ledmatrix_adat, HIGH);
}

void ledmatrix_mutat(){
  for(byte i=0;i<8;i++){
    ledmatrix_adat_kuldes(i,ledmatrix_puffer[i]);
    digitalWrite(ledmatrix_adat, LOW);
    digitalWrite(ledmatrix_ora, LOW);
    digitalWrite(ledmatrix_ora, HIGH);
    digitalWrite(ledmatrix_adat, HIGH);
  }
  ledmatrix_parancs_kuldes(0x88|(ledmatrix_intenzitas));
}

void ledmatrix_byte_beusztat(byte kovetkezo_betu[]){
  for(unsigned int bitszamlalo=7; bitszamlalo>-1; bitszamlalo--){
    for(unsigned int byteszamlalo=0; byteszamlalo<8; byteszamlalo++){
      //print_byte_arr(disbuf);
      byte ideiglenes_tar=ledmatrix_puffer[byteszamlalo];
      //prnt_bits(tmp);
      //tmp<<1;
      //prnt_bits(tmp);
      //Serial.println();
      ledmatrix_puffer[byteszamlalo]=ideiglenes_tar<<1;
      //7-bytecounter is because the display is upside down
      //if the given bit in nextchar is not 0
      //we need to shif in a 1 instead of the 0
      if(kovetkezo_betu[7-byteszamlalo]&(1<<bitszamlalo))ledmatrix_puffer[byteszamlalo]++;
      //print_byte_arr(disbuf);
      //Serial.println();
    }
    ledmatrix_mutat();
    delay(2500);
  }
}


void ledmatrix_beallitas(){
  ledmatrix_intenzitas=4;
  pinMode(ledmatrix_ora, OUTPUT);
  pinMode(ledmatrix_adat, OUTPUT);
  digitalWrite(ledmatrix_adat, HIGH);
  digitalWrite(ledmatrix_ora, HIGH);
  ledmatrix_byte_beusztat(_mc_sziv);
  ledmatrix_byte_beusztat(_mc_b);
  ledmatrix_byte_beusztat(_mc_k);
  ledmatrix_byte_beusztat(_mc_a);
  ledmatrix_betu(_mc_sziv);
  delay(250);
}

void ultrahang_beallitas(){
  pinMode(ultrahang_keres, OUTPUT);
  pinMode(ultrahang_valasz, INPUT);
}

unsigned int ultrahang_meres(){
  long idotartam;
  unsigned int cm;
  digitalWrite(ultrahang_keres, LOW);
  delayMicroseconds(2);
  digitalWrite(ultrahang_keres, HIGH);
  delayMicroseconds(5);
  digitalWrite(ultrahang_keres, LOW);
  idotartam=pulseIn(ultrahang_valasz, HIGH);
  cm=(unsigned int)(idotartam/29/2);
  return cm;
}

void setup() {
  // put your setup code here, to run once:
  motor_beallitas();
  fejmozgato_beallitas();
  ledszalag_beallitas();
  ledmatrix_beallitas();
  ultrahang_beallitas();
}

void loop() {
  // put your main code here, to run repeatedly:
  ledszalag_torles();
  ledmatrix_torles();
  
  fejmozgato_elore();
  tavolsag=ultrahang_meres();
  ledszalag_tavolsagjelzok(tavolsag);

  if(tavolsag>50){
    ledszalag_elore();
    ledmatrix_betu(_mc_sziv);
    motor_elore();
    return;
  }
  
  fejmozgato_jobbra();
  tavolsag=ultrahang_meres();
  ledszalag_tavolsagjelzok(tavolsag);
  
  if(tavolsag>50){
    ledszalag_jobbra();
    motor_jobbra();
    return;
  }

  fejmozgato_balra();
  tavolsag=ultrahang_meres();
  ledszalag_tavolsagjelzok(tavolsag);

  if(tavolsag>50){
    ledszalag_balra();
    motor_balra();
    return;
  }

  fejmozgato_elore();
  ledszalag_megallas();
  ledmatrix_megallas();
  motor_megallas();
  delay(10000);

}
