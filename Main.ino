/*
 * Controle de velocidade com PWM para motor DC e MOSFET
 * 
 * Autora: Ana Paula Messina - tecdicas
 * https://tecdicas.com/
 * 
 * 28/05/2019
 */

// Pino PWM
#define CONTROLE 9 
#define ENTRADA 8

// Valor máximo 255
//int velocidade = 255;
//int tempo = 30;
int referencia = 0;
void setup()
{
  Serial.begin(9600);
  pinMode(CONTROLE, OUTPUT);
}
void loop()
{
  referencia = analogRead(ENTRADA);
  analogWrite(CONTROLE, referencia);
  Serial.print(referencia); Serial.print("\t");
  Serial.println(CONTROLE);
  delay(0.5);
  /*// Utilize o Monitor serial
  for(int x = 0; x < velocidade; x++)
  {
    analogWrite(CONTROLE, x);
    Serial.print("Aumentando..");
    Serial.println(x);
    delay(tempo);
  }

  for(int y = velocidade; y > 0; y--)
  {
    analogWrite(CONTROLE, y);
    Serial.print("Diminuindo..");
    Serial.println(y);
    delay(tempo);
  }*/
}
