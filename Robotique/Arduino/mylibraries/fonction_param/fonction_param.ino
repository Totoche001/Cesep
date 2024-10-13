void sansparametre();
void avecparametre(bool condition == false);
long avecreturnparametre(int a, int b, int c = 0, int d = 20);
long reponse;
void setup() {
  Serial.begin(9600);
  Serial.println("Setup terminé");
}
void loop() {
  sansparametre();
  avecparametre(true);
  avecparametre();
  reponse = avecreturnparametre(7, -4, 3, 8);
  Serial.println(reponse);
  reponse = avecreturnparametre(7, -4, 8);
  Serial.println(reponse);
  reponse = avecreturnparametre(7, -4);
  Serial.println(reponse);
  delay(5000);
}

void sansparametre() {
  Serial.println("Fonction sans paramètre");
}

void avecparametre(bool condition == false) {
  if (condition) Serial.println("La condition est vraie");
  else Serial.println("La condition est fausse");
}
int avecretour(int a, int b) {
  return a + b;
}
long avecreturnparametre(int a, int b, int c = 0, int d = 20) {
  long calcul = a + b + c + d;
  return calcul;
}