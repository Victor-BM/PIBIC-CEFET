float tensao;
float dado;
void setup() {
  pinMode (A8, INPUT);
  Serial.begin (9600);
}

void loop() {
  dado = analogRead(A8);
  tensao = dado*5/1023;
  Serial.print ("Dado: ");
  Serial.println (dado);
  Serial.print ("Tensão: ");
  Serial.println (tensao);
  delay (2000);
}