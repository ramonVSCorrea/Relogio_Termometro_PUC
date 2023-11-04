/*************************************************************
          --PROJETO: MONITORAMENTO DE TEMPERATURA--
 
 ALUNOS: Luiz Antonio Farias Santos
         Ramon Vinícius Silva Corrêa
 CURSO: Engenharia Eletrônica e de Telecomunicação       
  
***************************************************************/   
          /*--INCLUSÃO DE BIBLIOTECAS--*/
          
#include <lm35.h>    //biblioteca do sensor de temperatura     
#include <DS3231.h>  //biblioteca do módulo RTC 
#include <LiquidCrystal.h>    //biblioteca do display LCD

/***************************************************************/
           /*--DEFINIÇÃO DE OBJETOS--*/
           
LiquidCrystal lcd(2, 3, 4, 5, 6, 7); //Objeto para LCD (pinos 2,3,4,5,6 e 7)
DS3231 rtc(SDA, SCL); //Objeto para módulo RTC
Time t;   //Objeto para tempo do RTC
lmt sensor(A0);  //Objeto para sensor de temperatura

/****************************************************************/
          /*--VARIÁVEIS PARA TEMPERATURA--*/
          
float tempc, tempf;   //Temperatura em Celsius e Fahrenheit 
float menort = 0.0, maiort = 0.0;   //Maior e menor temperatura
float somac=0.0, somaf=0.0, medt=0.0, medc=0.0, medf=0.0;  //Média de temperaturas     
int a = 0;  //Salvar a primeira temperatura registrada

/*****************************************************************/
              /*--VARIÁVEIS PARA RTC--*/
              
int hora , minu , seg = 00;   //Relógio
int dia = 01, mes = 01, ano = 2019;   //Data
int x = 0;   //incrementa horas e datas
int y = 0;   //decrementa horas e datas
int z = 0;   //Alterna entre horas(Z=0), minutos(Z=1), dias(Z=2), meses(Z=3) e anos(Z=4)
int d = 0;   //limpar lcd

/******************************************************************/
            /*--VARIÁVEIS PARA BOTÕES--*/
            
int button1 = 8;  //Define botão 1 como pino 8
int button2 = 9;  //Define botão 2 como pino 9
int button3 = 10; //Define botão 3 como pino 10
int button4 = 11; //Define botão 4 como pino 11
int pres1 = 0;    //Lê o nível lógico do botão 1
int pres2 = 0;    //Lê o nível lógico do botão 2
int pres3 = 0;    //Lê o nível lógico do botão 3
int pres4 = 0;    //Lê o nível lógico do botão 4

/********************************************************************/
              /*--VARIÁVEIS PARA MILLIS--*/
              
unsigned long atual = 0;    //Armazena o millis
unsigned long anterior1 = 0; 
unsigned long anterior2 = 0;
int intervalo = 1000;  //Define o tempo em miliseg. de intervalo
int intervalo2 = 3600000;

/********************************************************************/
              /*--VARIÁVEIS PARA ALARME--*/
              
int liga = 1; //Define quando o alarme estará acionado

/********************************************************************/
              /*--VARIÁVEIS PARA TELA--*/

int tela = 1, w = 0; //Alterna entre telas

/******************************************************************/
void setup() //SETUP DO PROGRAMA
{
                    /*--INICIA OBJETOS--*/
                    
  rtc.begin();    //Inicia módulo RTC
  lcd.begin(16, 2); //Inicia Display LCD 
  
  /****************************************************************/
                    /*--PINOS I/O--*/
                    
  pinMode (13, OUTPUT); //Define pino 13 como saída
  pinMode (12, OUTPUT); //Define pino 12 como saída
  pinMode (button1, INPUT); //Define button 1 como entrada
  pinMode (button2, INPUT); //Define button 2 como entrada
  pinMode (button3, INPUT); //Define button 2 como entrada
  pinMode (button4, INPUT); //Define button 2 como entrada

  /*****************************************************************/
                  /*--CONFIG. RTC--*/
                  
  //rtc.setTime(hora, minu, seg);  //Modifica horário do relógio(DEIXE ESSA LINHA COMO COMENTÁRIO)
  //rtc.setDate(dia, mes, ano);   //Modifica data (DEIXE ESSA LINHA COMO COMENTÉRIO)
  hora = t.hour;  //Define a variável como as horas do RTC
  minu = t.min;   //Define a variável como os minutos do RTC
  seg = t.sec;  //Define a variável como os segundos do RTC
}
/*********************************************************************/
void loop () //INICIA O LOOP DO PROGRAMA 
{
  atual = millis(); //atualiza o millis
  t = rtc.getTime(); //atualiza o relógio
  tempc = sensor.tCelsius(); //lê a temperatura em Celsius
  tempf = sensor.tFahrenheit(); //lê a temperatura em Fahrenheit

  /***********************************************************/
       /*--LEITURA DE NÍVEL LÓGICO DOS BOTÕES--*/
       
  pres1 = digitalRead(button1); //lê o nível lógico do botão 1 
  pres2 = digitalRead(button2); //lê o nível lógico do botão 2 
  pres3 = digitalRead(button3); //lê o nível lógico do botão 3 
  pres4 = digitalRead(button4); //lê o nível lógico do botão 4 

  /**********************************************************/
                   /*--INICIA FUNÇÕES--*/ 
                   
  maiorTemp(); //inicia função que armazena a maior e menor temperetura registrada
  mudaTela();  //inicia função que alterna entre as telas

  if (tempc>=50.00){alarme();} //Se a temp. em °C for maior que 50, inicia função alarme() 

}
/*****************************************************************************************/
                /*--CRIAÇÃO DE FUNÇÕES--*/
                
void alarme() //FUNÇÃO QUE ATIVA O ALARME
{
  if (atual - anterior2  >=  intervalo) //Se o millis for >= ao intervalo:
  {
    if (liga == 1) 
    {
      digitalWrite(13, HIGH); //Liga o LED
      tone(12, 1500); //Liga o Buzzer
      liga = 0;      
    }
    else
    {
      digitalWrite(13, LOW); //Desliga o LED
      noTone(12); //Desliga o Buzzer   
      liga = 1;     
    }
    anterior2 = atual;
  }     
}

/****************************************************************************************/

void atualizac() //FUNÇÃO QUE ATUALIZA A TEMPERATURA EM °C
{
  lcd.setCursor(0, 1); //Move o cursor do LCD para a linha 1 e coluna 0
  lcd.print(tempc); //Imprime a temperatura em Celsius
  lcd.print((char)223); //Imprime o simbolo de graus °
  lcd.print("C"); //Imprime a letra C
}

/****************************************************************************************/

void atualizaf() //FUNÇÃO QUE ATUALIZA A TEMPERATURA EM °F
{
  lcd.setCursor(9, 1); //Move o cursor do LCD para a linha 1 e coluna 9
  lcd.print(tempf); //Imprime a temperatura em °F
  lcd.print((char)223); //Imprime o simbolo de graus °
  lcd.print("F"); //Imprime a letra F
}

/***************************************************************************************/

void tela1() //FUNÇÃO DE EXIBIÇÃO DA PRIMEIRA TELA (DATA E HORA)
{
  lcd.setCursor(0, 0); //Move o cursor para a linha 0 e coluna 0
  lcd.print("Hora: "); //Imprime a palavra "Hora"
  lcd.print(rtc.getTimeStr()); //Imprime as horas
  lcd.setCursor(0 , 1); //Move o cursor para a linha 1 e coluna 0
  lcd.print("Data: ");  //Imprime a palavra "Data"
  lcd.print(t.date); //Imprime o dia 
  lcd.print("/");  //Imprime " / "
  lcd.print(t.mon); //imprime o mê
  lcd.print("/"); //Imprime " / "
  lcd.print((t.year - 2000)); //imprime o ano
  mudaRTC(); // inicia a função que altera a data e hora 
}

/*****************************************************************************************/

void tela2() //FUNÇÃO DE EXIBIÇÃO DA SEGUNDA TELA (TEMP. ATUAL)
{
  lcd.setCursor(2, 0); //Move o cursor para linha 0 e coluna 2
  lcd.print("Temperatura:"); //Imprime a palavra "Temperatura"
  
  if (atual - anterior1 >= intervalo) //aguarda o intervalo de 1 segundo
  {
    atualizac(); //Inicia a função de atualizar a temperatura em °C
    atualizaf();  //Inicia a função de atualizar a temperatura em °F
    somac = somac + tempc; //soma as temperaturas medidas em °C
    somaf = somaf + tempf; //soma as temperaturas medidas em °F 
    medt++; //soma quantas temperaturas forma medidas
    anterior1 = atual; 
  }
}

/*****************************************************************************************/

void tela3() //FUNÇÃO DE EXIBIÇÃO DA TERCEIRA TELA (MAIOR E MENOR TEMP.)
{
    lcd.setCursor(0, 0); //Move o cursor do LCD para a linha 0 e coluna 0 
    lcd.print("> TEMP:"); //Imprime a mensagem " > TEMP "
    lcd.print(maiort); //Imprime a maior temp. registrada em °C
    lcd.print("C"); //Imprime a letra C
    lcd.print((char)223); //Imprime o símbolo de graus °
  
    lcd.setCursor(0, 1); //Move o cursor do LCD para a linha 1 e coluna 0
    lcd.print("< TEMP:"); //Imprime a mensagem " < TEMP "
    lcd.print(menort); //Imprime a menor temp. registrada em °C
    lcd.print("C"); //Imprime a letra C
    lcd.print((char)223); //Imprime o símbolo de graus ° 
}

/*******************************************************************************************/

void tela4() //FUNÇÃO DE EXIBIÇÃO DA QUARTA TELA (MÉDIA DA TEMPERATURA)
{
    if(pres1 == HIGH) //Se o botão 1 for pressionado
  {
    while(digitalRead(button1) == HIGH);
    medc=somac/medt; //calcula a média de temperatura em °C até o momento do pressionamento
    medf=somaf/medt; //calcula a média de temperatura em °F até o momento do pressionamento
  }
  
  lcd.setCursor(3, 0); //Move o curosor do LCD para a linha 0 e coluna 3
  lcd.print("Temp.Media:"); //Imprime a mensagem "Temp. Media"
  lcd.setCursor(0, 1); //Move o curosor do LCD para a linha 1 e coluna 0
  lcd.print(medc); //Imprime média de temperaturas em Celsius
  lcd.print((char)223); //Imprime o símbolo de graus °
  lcd.print("C"); //Imprime a letra C
  lcd.setCursor(9,1); //Move o cursor do LCD para a linha 1 e coluna 9
  lcd.print(medf); //Imprime a média de temperatura em Fahrenheit
  lcd.print((char)223); //Imprime o símbolo de graus °
  lcd.print("F"); //Imprime a letra F

}

/********************************************************************************************/

void mudaTela() //FUNÇÃO QUE ALTERNA ENTRE AS TELAS
{
    if (pres4 == HIGH) //Se o botão 4 for pressionado
  {
    while (digitalRead(button4) == HIGH); //Não faz nada enquanto o botão for pressionado
    tela = tela + 1; //Soma 1 à variável tela
    w = w + 1; //Soma 1 à variável w
    if (tela > 4){tela = 1;} //Se a variável tela for maior que 5 volte para a tela 1
  }
  
  if (w == 1){lcd.clear(); w = 0;}//Se w for igual a 1 limpa a tela
  
  if (tela == 1){tela1();} //Se tela for igual a 1 executa a função tela1() 
  if ((tela == 2) && (w == 0)) {tela2();} //Se tela for igual a 2 e w igual a 0, executa a função tela2()
  if ((tela == 3) && (w == 0)) {tela3();} //Se tela for igual a 3 e w igual a 0, executa a função tela3()
  if ((tela == 4) && (w == 0)) {tela4();} //Se tela for igual a 4 e w igual a 0, executa a função tela4()
}

/***************************************************************************************/

void maiorTemp() //FUNÇÃO QUE ARMAZENA A MAIOR E MENOR TEMP. REGISTRADA
{
    if (a == 0) //Armazena a primeira temperatura registrada
  {
    maiort = tempc;
    menort = tempc;
    a = 1;
  }

  if (tempc > maiort){maiort = tempc;}  //Se a temperatura atual for maior que a anterior, armazena a temp. atual
  if (tempc < menort){menort = tempc;}  //Se a temperatura atual for menor que a anterior, armazena a temp. atual
}

/*******************************************************************************/

void mudaRTC() //FUNÇÃO PARA ALTERAR DATA E HORA 
{
    if (pres3 == HIGH) //Se o botão 3 for pressionado alterna entre hora, min, dia, mes e ano
  {
    while (digitalRead(button3) == HIGH); //Enquanto o botão 3 estiver pressionado, não faz nada
    z++; //incrementa na variável z
    if (z > 4){z = 0;} //Se Z for maior que 4, volta para a posição das horas (0-horas, 1-min, 2-dia, 3-mes, 4-ano)
  }
  /*************************************************************************/
                        /*--INCREMENTA HORA--*/
                        
  if ((pres1 == HIGH) && (z == 0)) //Se o botão 1 for pressionado, e a variável Z estiver em horas
  {
    while (digitalRead(button1) == HIGH); //não faz nada enquanto o botão estiver pressionado
    x++; //Soma 1 a variável X
  }

  if ((x == 1) && (z == 0)) //Se X estiver em 1 e Z em horas
  {
    hora = t.hour; //Iguala a variável "hora" com as horas do RTC 
    minu = t.min; //Iguala a variável "minu" com os minutos do RTC 
    hora = hora + 1; //Soma 1 a variável hora
    if (hora > 23) {hora = 0;} //Se passar de 23 horas, volta para 0
    rtc.setTime(hora, minu, seg); //Atualiza o relógio do RTC
    x = 0; //Finaliza o if 
  }
  
  /****************************************************************************************/
                          /*--DECREMENTA HORA--*/
                          
  if ((pres2 == HIGH) && (z == 0)) //Se o botão 2 estiver pressionado e a variável Z estiver em horas
  {
    while (digitalRead(button2) == HIGH);//não faz nada enquanto o botão estiver pressionado
    y++; //Soma 1 a variável Y
  }

  if ((y == 1) && (z == 0)) //Se Y estiver em 1 e Z em horas
  {
    hora = t.hour;  //Iguala a variável "hora" com as horas do RTC
    minu = t.min; //Iguala a variável "minu" com os minutos do RTC 
    hora = hora - 1; //Decrementa 1 da variável hora 
    if (hora < 0) {hora = 23;} //Se hora for menor que 0 volta para 23 horas
    rtc.setTime(hora, minu, seg); //Atualiza o relógio do RTC
    y = 0; //Finaliza o if
  }
  /****************************************************************************************/
                        /*--INCREMENTA MINUTOS--*/
                        
  if ((pres1 == HIGH) && (z == 1)) //Se o botão 1 for pressionado e Z estiver em minutos
  {
    while (digitalRead(button1) == HIGH); //Não faz nada enquanto o botão estiver pressionado
    x++; //Soma 1 a variável x
  }

  if ((x == 1) && (z == 1)) //Se x estiver em 1 e Z em minutos
  {
    hora = t.hour; //Iguala a variável "hora" com as horas do RTC
    minu = t.min; //Iguala a variável "minu" com os minutos do RTC
    minu = minu + 1; //Incrementa 1 na variável minu
    if (minu > 59) {minu = 0;} //Se os minutos passarem de 59, volte para 0
    rtc.setTime(hora, minu, seg); //Atualiza o relógio do RTC
    x = 0; //Finaliza o if 
  }
  /**************************************************************************************/
                           /*--DECREMENTA MINUTOS--*/
                           
  if ((pres2 == HIGH) && (z == 1)) //Se o botão 2 for pressionado e Z estiver em minutos
  {
    while (digitalRead(button2) == HIGH); //Não faz nada enquanto o botão estiver pressionado
    y++; //Soma 1 a variável Y
  }

  if ((y == 1) && (z == 1)) //Se Y estiver em 1 e Z em minutos
  {
    hora = t.hour; //Iguala a variável "hora" com as horas do RTC
    minu = t.min; //Iguala a variável "minu" com os minutos do RTC
    minu = minu - 1; //Decrementa 1 na variável minu
    if (minu < 00) {minu = 59;} //Se os minu for menor que 0, volte para 59
    rtc.setTime(hora, minu, seg); //Atualiza o relógio do RTC
    y = 0; //Finaliza o if 
  }
  
  /*************************************************************************************/
                          /*--INCREMENTA DIAS--*/
                          
  if ((pres1 == HIGH) && (z == 2)) //Se o botão 1 for pressionado e Z estiver em Dias
  {
    while (digitalRead(button1) == HIGH); //Não faz nada enquanto o botão estiver pressionado
    x++; //Soma 1 a variável x
  } 

  if ((x == 1) && (z == 2)) //Se x estiver em 1 e Z em dias 
  {
    lcd.clear();
    dia = t.date; //Iguala a variável "dia" com os dias do RTC
    mes = t.mon;  //Iguala a variável "mes" com os meses do RTC
    dia = dia + 1; //Incrementa 1 a variável dia
    if (dia > 31) {dia = 1;}  //Se a variável dia for maior que 31 volte dia para 1
    rtc.setDate(dia, mes, ano); //Atualiza a data do RTC
    x = 0;  //Finaliza o if 
  }
  
  /**************************************************************************/
                    /*--DECREMENTA DIAS--*/
                    
  if ((pres2 == HIGH) && (z == 2)) //Se o botão 2 for pressionado e Z estiver em Dias
  {
    while (digitalRead(button2) == HIGH); //Não faz nada enquanto o botão estiver pressionado
    y++; //Soma 1 a variável Y
  }

  if ((y == 1) && (z == 2)) //Se x estiver em 1 e Z em dias 
  {
    lcd.clear();
    dia = t.date; //Iguala a variável "dia" com os dias do RTC
    mes = t.mon;  //Iguala a variável "mes" com os meses do RTC
    dia = dia - 1; //Decrementa 1 a variável dia
    if (dia < 1) {dia = 31;} //Se a variável dia for menor que 1, volte para 31
    if(dia>=10){d=1;}
    if ((dia<10)&&(d==1)){lcd.clear(); d=0;}
    rtc.setDate(dia, mes, ano); //Atualiza a data do RTC
    y = 0; //Finaliza o if
  }
  
  /****************************************************************************************/
                            /*--INCREMENTA MESES--*/
                            
  if ((pres1 == HIGH) && (z == 3)) //Se o botão 1 for pressionado e Z estiver em meses
  {
    while (digitalRead(button1) == HIGH); //Não faz nada enquanto o botão estiver pressionado
    x++; //Soma 1 a variável x
  }

  if ((x == 1) && (z == 3)) //Se x estiver em 1 e Z em meses 
  {
    lcd.clear();
    dia = t.date;  //Iguala a variável "dia" com os dias do RTC
    mes = t.mon;  //Iguala a variável "mes" com os meses do RTC
    mes = mes + 1;  //Incrementa 1 a variável mes
    if (mes > 12) {mes = 01;} //Se a variável mes for maior que 12 volte para 1  
    rtc.setDate(dia, mes, ano); //Atualiza a data do RTC
    x = 0;  //Finaliza o if 
  }
  
  /**************************************************************************/
                        /*--DECREMENTA MESES--*/
                        
  if ((pres2 == HIGH) && (z == 3)) //Se o botão 2 for pressionado e Z estiver em meses
  {
    while (digitalRead(button2) == HIGH);  //Não faz nada enquanto o botão estiver pressionado
    y++;  //Soma 1 a variável Y
  }

  if ((y == 1) && (z == 3))  //Se y estiver em 1 e Z em meses
  {
    lcd.clear();
    dia = t.date;  //Iguala a variável "dia" com os dias do RTC
    mes = t.mon;   //Iguala a variável "mes" com os meses do RTC
    mes = mes - 1;  //Decrementa 1 a variável mes
    if (mes < 1) {mes = 12;}  //Se a variavel mes for menor que 1 volte para 12
    rtc.setDate(dia, mes, ano); //Atualiza a data do RTC
    y = 0;  //Finaliza o if
  }
  
  /****************************************************************************************/
                          /*--INCREMENTA ANOS--*/
                          
  if ((pres1 == HIGH) && (z == 4)) //Se o botão 1 for pressionado e Z estiver em anos
  {
    while (digitalRead(button1) == HIGH);  //Não faz nada enquanto o botão estiver pressionado
    x++;  //Soma 1 a variável x
  }

  if ((x == 1) && (z == 4))  //Se x estiver em 1 e Z em anos 
  {
    dia = t.date;  //Iguala a variável "dia" com os dias do RTC
    mes = t.mon;   //Iguala a variável "mes" com os meses do RTC
    ano = t.year;  //Iguala a variável "ano" com os anos do RTC
    ano = ano + 1;  //Incrementa 1 a variável ano 
    rtc.setDate(dia, mes, ano); //Atualiza a data do RTC

    x = 0; //Finaliza o if 
  }
  
  /**************************************************************************/
                    /*--DECREMENTA ANOS--*/
                    
  if ((pres2 == HIGH) && (z == 4))  //Se o botão 2 for pressionado e Z estiver em anos
  {
    while (digitalRead(button2) == HIGH); //Não faz nada enquanto o botão estiver pressionado
    y++;  //Soma 1 a variável Y
  }

  if ((y == 1) && (z == 4)) //Se y estiver em 1 e Z em anos 
  {
    dia = t.date;  //Iguala a variável "dia" com os dias do RTC
    mes = t.mon;   //Iguala a variável "mes" com os meses do RTC
    ano = t.year;  //Iguala a variável "ano" com os anos do RTC
    ano = ano - 1;  //Decrementa 1 a variável ano 
    rtc.setDate(dia, mes, ano); //Atualiza a data do RTC
    y = 0; //Finaliza o if 
  }
}

/*****************************************************************************/
