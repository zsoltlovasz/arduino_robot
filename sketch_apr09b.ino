int mclk=1;
int mdin=0;
volatile uint8_t disbuf[8]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t intensity;

void setup() {
  // put your setup code here, to run once:
  intensity=4;
  pinMode(mclk, OUTPUT);
  pinMode(mdin, OUTPUT);
  digitalWrite(mdin, HIGH);
  digitalWrite(mclk, HIGH);
  pinMode(LED_BUILTIN, OUTPUT);

}

void mdisplay(){
  for(uint8_t i=0;i<8;i++){
    msenddata(i,disbuf[i]);
    digitalWrite(mdin, LOW);
    digitalWrite(mclk, LOW);
    digitalWrite(mclk, HIGH);
    digitalWrite(mdin, HIGH);
  }
  msendcommand(0x88|(intensity));
}

void msendcommand(byte cmd){
  digitalWrite(mdin, LOW);
  msend(cmd);
  digitalWrite(mdin, HIGH);
}

void msenddata(byte address, byte data){
  msendcommand(0x44);
  digitalWrite(mdin, LOW);
  msend(0xC0 | address);
  msend(data);
  digitalWrite(mdin, HIGH);
}

void msend(byte data){
  for (int i = 0; i < 8; i++) {
    digitalWrite(mclk, LOW);
    digitalWrite(mdin, data & 1 ? HIGH : LOW);
    data >>= 1;
    digitalWrite(mclk, HIGH);
  }
}

void mclear(){
  for(uint8_t i=0;i<8;i++){
    disbuf[i]=0x00;
  }
}


void loop() {
  // put your main code here, to run repeatedly:
  for (int i=0; i<8; i++){
    for (int j=0; j<8; j++){
      mclear();
      disbuf[i]=(1<<j);
      mdisplay();
      if((i+j)%2)digitalWrite(LED_BUILTIN, HIGH);
      else digitalWrite(LED_BUILTIN, LOW);
      delay(1000);
      mclear();
      disbuf[7]=0x00;
      disbuf[6]=0x66;
      disbuf[5]=0x99;
      disbuf[4]=0x81;
      disbuf[3]=0x42;
      disbuf[2]=0x24;
      disbuf[1]=0x18;
      disbuf[0]=0x00;
      mdisplay();
      delay(1000);
    }
  }

}
