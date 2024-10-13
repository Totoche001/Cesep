#define ROUGE 9
#define VERT 10
#define BLEU 11

void ledRGB(byte r, byte v, byte b, word pause=1000);


void setup() {
  // put your setup code here, to run once:
  pinMode(ROUGE, OUTPUT);
  pinMode(VERT, OUTPUT);
  pinMode(BLEU, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  ledRGB(0,100,100);
  ledRGB(100,0,100);
  ledRGB(100,100,100,2000);
  ledRGB(0,100,0,500);
  //#define pause 10;
}
void ledRGB(byte r, byte v, byte b, word pause=1000){
  analogWrite(ROUGE, r);
  analogWrite(VERT, v);
  analogWrite(BLEU, b);
  delay(pause);
}
