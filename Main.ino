/*
 * Controle de velocidade com PWM para motor DC e MOSFET
 * 
 * Autora: Ana Paula Messina - tecdicas
 * https://tecdicas.com/
 * 
 * 28/05/2019
 * 
 * 21/05/2020 * Modificação, ajustes e ampliação.
 * 
 * Entrada de referência para o controle PWM
 * Entrada de referência para controle de intervalos entre o acionamento do PWM
 * Entrada de referência de posição do eixo do motor
 * 
 * Jáder Schumann
 */

#define PWM_OUT 9 
#define PWM_IN 4
#define BPM_IN 8
#define POSICAO_IN 5 //pino 6 é referente ao A7 na placa Arduino Pro Micro (mini) e somente pode ser usado como entrada analogia, conforme documentação oficial)

#define PWM_MIN 410 //algum valor entre 0 e 1023 (realizada leitura e teste de bancada, definido valor = 410)
#define PWM_OFF 30

// Valor máximo 255
//int velocidade = 255;
//int tempo = 30;
unsigned int pwm_referencia = 0;
unsigned int bpm_referencia = 0;
unsigned int eixo_referencia = 0;
unsigned int fator_tempo = 10; 
unsigned long tempo_repouso = 1;
unsigned long tempo_millis = 0;
unsigned int estado = 1;//inicia já acionando o motor

void setup()
{
  Serial.begin(9600);
  pinMode(PWM_OUT, OUTPUT);
  pinMode(PWM_IN, INPUT);
  pinMode(BPM_IN, INPUT);
  pinMode(POSICAO_IN, INPUT);
}
void loop()
{
 /* pwm_referencia = analogRead(PWM_IN);
  analogWrite(PWM_OUT, pwm_referencia/4);
  Serial.print(pwm_referencia); Serial.print("\t");
  Serial.println(PWM_OUT);
  delay(150);*/


switch(estado){
  case 1:
  //aciona motor

  pwm_referencia = analogRead(PWM_IN);//analogRead() retorna valor entre 0 e 1023;
  if(pwm_referencia <= PWM_OFF)
    analogWrite(PWM_OUT, 0);//pwm min definido como o valor minimo para evitar o motor trancar por baixa rpm
    else if (pwm_referencia < PWM_MIN)
       analogWrite(PWM_OUT, PWM_MIN/4);
         else      
            analogWrite(PWM_OUT, pwm_referencia/4);//analogWrite() (conversao D/A) tem parametros de entrada com range 0 a 254)
  
  Serial.print("Pino de referencia PWM = ");Serial.println(PWM_IN);
  Serial.print("pwm_referencia = "); Serial.println(pwm_referencia);
  Serial.print("Pino de saida PWM = ");Serial.println(PWM_OUT);
  delay(150);

  eixo_referencia = digitalRead(POSICAO_IN);//retorna HIGH OU LOW
  Serial.print("Pino de entrada, sensor do eixo = "); Serial.println(POSICAO_IN);
  Serial.print("eixo_referencia = "); Serial.println(eixo_referencia);
  if(eixo_referencia == 1)// trocar aqui para 1 e ou 0  no lugarde HIGH E LOW
    estado = 2;
  break;  
  
  case 2:
  //seta tempo
  bpm_referencia = analogRead(BPM_IN);//analogRead() retorna valor entre 0 e 1023;
  Serial.print("Pino de referencia BPM = "); Serial.println(BPM_IN);
  Serial.print("bpm_referencia = "); Serial.println(bpm_referencia);
  if(bpm_referencia <=0)
    bpm_referencia = 1;// evita o tempo 0
  estado = 3;
  break;
  
  case 3:
  //conta tempo
  //delay(ms) tempo em milisegundos que o programa fica parado
  //converter 0 a 1023 em tempo aplicando um fator de conversão;

  /* Em uma condição minima de rpm estavel o acionador estava 4 x mais rapido do que deveria.
   *  contagem ficava proxima de 8 acionamentos em 15s (24 acionamentos por min)
   *  o ALVO é entre 8 e 10 acionamentos por segundo
   *  
   *  considerando assim que o acionador em rpm minima leva meio segundo para completar uma volta (aproximadamente)
   *  na verdade vamos desconsiderar estes meios segundos para contagem do bpm
   *  obs: definir um minimo para o pwm, evitando travamentos.pwm_corte, ou algo assim.
   *  
   *  sendo o minimo 8 acionamentos por minuito, teriamos 60/8 = 7,5 s de pausa a cada acionamento
   *  aumentando um pouco o range, pensando em 6 acionamentos por segundos temos 60/6 = 10s de maior tempo de pausa.
   *  1023 - 10 x 1000 ms.
   *  fator_tempo =~ 10;
   * 
  */
tempo_repouso = bpm_referencia * fator_tempo;
//tempo = 1023 * 10 = 10230 ms (10 segundos)
//tempo = 0 * 10 = 0
//tempo = 1 * 10 = 10ms (não para tempo aglum, praticmanete gira direto , o que nos leva aos ~24 bpm em rpm min
//qual a rpm max?
    //rpm max na velocidade 1 é de ~32 rpm
    //rpm min na velocidade 1 é de ~24 bpm  (PWM ref ~410)
    //rpm max na velocidade 2 é de ~12*4 48 bpm
    //rpm min na velocidade 2 é de ~11*4 44 bpm(PWM ref ~618)

 //DA PARA DEFINIR A RPM MAX AJUSTANDO O TEMPO MINIMO DE REPOUSO!! (ainda nao está em questão)(depende do quao rapido gira o motor)   
    
tempo_millis = millis();
Serial.print("Tempo de repouso = "); Serial.print(tempo_repouso); Serial.println("  ms");
while( (tempo_millis + tempo_repouso) > millis() )
  {
    //aguarda o tempo definido
    estado = 3;
  }
  //fim do tempo
  estado = 1;
  break;
}
   
}
