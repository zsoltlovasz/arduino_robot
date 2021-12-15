int mclk=13;
int mdin=12;
volatile byte disbuf[8]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
byte intensity;

//shapes - by us
byte _mc_heart[]={0x00, 0x66, 0x99, 0x81, 0x42, 0x24, 0x18, 0x00};
byte _mc_empty[]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
//byte _mc_i[]={0x3c, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3c};
//byte _mc_a[]={0x81, 0x81, 0x42, 0x7e, 0x24, 0x24, 0x18, 0x18};

//letters from https://github.com/dhepper/font8x8/blob/master/font8x8_basic.h
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


void setup() {
  // put your setup code here, to run once:
  intensity=4;
  pinMode(mclk, OUTPUT);
  pinMode(mdin, OUTPUT);
  digitalWrite(mdin, HIGH);
  digitalWrite(mclk, HIGH);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);

}

void mdisplay(){
  for(byte i=0;i<8;i++){
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
  for(byte i=0;i<8;i++){
    disbuf[i]=0x00;
  }
}

void prnt_bits(byte b){
  for(int i = 7; i >= 0; i--)
    Serial.print(bitRead(b,i));
  Serial.println(); 
}

void print_byte_arr(byte arr[]){
  for(int i=0; i<8; i++){
    Serial.print(i);
    Serial.print(':');
    prnt_bits(arr[i]);
    //Serial.print(arr[i], BIN);
    //Serial.println();
  }
  Serial.println();
}

void mdisbuf_inshift(byte nextchar[]){
  for(int bitcounter=7; bitcounter>-1; bitcounter--){
    for(int bytecounter=0; bytecounter<8; bytecounter++){
      //print_byte_arr(disbuf);
      byte tmp=disbuf[bytecounter];
      //prnt_bits(tmp);
      //tmp<<1;
      //prnt_bits(tmp);
      //Serial.println();
      disbuf[bytecounter]=tmp<<1;
      //7-bytecounter is because the display is upside down
      //if the given bit in nextchar is not 0
      //we need to shif in a 1 instead of the 0
      if(nextchar[7-bytecounter]&(1<<bitcounter))disbuf[bytecounter]++;
      //print_byte_arr(disbuf);
      //Serial.println();
    }
    mdisplay();
    delay(250);
  }
}


void loop() {
  // put your main code here, to run repeatedly:
  mclear();
  mdisplay();
  mdisbuf_inshift(_mc_i);
  delay(1000);
  mdisbuf_inshift(_mc_empty);
  delay(1000);
  mdisbuf_inshift(_mc_heart);
  delay(1000);
  mdisbuf_inshift(_mc_empty);
  delay(1000);
  mdisbuf_inshift(_mc_a);
  delay(1000);
  mdisbuf_inshift(_mc_d);
  delay(1000);
  mdisbuf_inshift(_mc_a);
  delay(1000);
  mdisbuf_inshift(_mc_m);
  delay(1000);
  mdisbuf_inshift(_mc_empty);
  delay(1000);
  mdisbuf_inshift(_mc_k);
  delay(1000);
  mdisbuf_inshift(_mc_o);
  delay(1000);
  mdisbuf_inshift(_mc_r);
  delay(1000);
  mdisbuf_inshift(_mc_n);
  delay(1000);
  mdisbuf_inshift(_mc_e);
  delay(1000);
  mdisbuf_inshift(_mc_l);
  delay(1000);
  mdisbuf_inshift(_mc_empty);
  delay(1000);
  mdisbuf_inshift(_mc_b);
  delay(1000);
  mdisbuf_inshift(_mc_o);
  delay(1000);
  mdisbuf_inshift(_mc_b);
  delay(1000);
  mdisbuf_inshift(_mc_o);
  delay(1000);
  mdisbuf_inshift(_mc_empty);
  delay(1000);
  
  //previous stuff below
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
      break;
    }
    break;
  }

}
