/* Aluno: Gabriel Assun��o Domene
   Disciplina: Microprocessadores e Microcontroladores
   Professor: Roderval
   Data: 22/11/2018
   Projeto: Spray Dryer
   Objetivo: Realizar o processo de secamento por atomiza��o de produtos l�quidos
   utilizando o PIC 16F877A para o controle do sistema embarcado da m�quina.
   
*/


#include <secagem.h>
#include <lcd.c>
#FUSES WDT,PUT

int1 flag;
int1 ventilador = 1;
int1 bomba = 1;
int1 alimento;
int temperatura;
#int_ext
void trata_ext()
{
   //Interrup��o de seguran�a
   restart_wdt();
   set_timer1(0);
   char c;
   flag = 1;              // Vari?vel auxiliar para indicar que a int aconteceu 
   output_high(pin_c7);
   while(input(pin_b0)==0)
   {
      restart_wdt();
      set_timer1(0);
      c='\f';
      lcd_putc(c);
      
      printf(LCD_PUTC, "INTERRUPCAO");
      delay_ms(400);
      output_low(pin_c3);
      output_low(pin_c5);
      output_low(pin_c6);
   }  //fica esperando mudar situa??o do bot?o da interrup.
    c='\f';
    lcd_putc(c);
}

#int_timer0
void trata_t0()
{  
   restart_wdt();
   char c;
   c='\f';
   lcd_putc(c);

   temperatura = LER_TEMPERATURA();
   printf(LCD_PUTC, "TEMP = %u'",temperatura);
   printf(LCD_PUTC, "\n ready");
   
   
   set_timer0(0);
}

void main()
{
   //Setup inicial
   Port_b_pullups(true);
   set_tris_c(0B0000000);
   output_low(pin_b1);
   output_low(pin_c3);
   output_low(pin_c5);
   output_low(pin_c6);
   //Iniciar LCD com mensagem
   lcd_init();
   printf(LCD_PUTC, "PROCESSO SPRAY");
   delay_ms(1000);
   
   setup_adc_ports(AN0);             //configura porta AN0 com entrada anal?gica
   setup_adc(ADC_CLOCK_INTERNAL);    //usar? o clock interno do AD(mais comum)
   
   //Interrup��es e timers
   setup_timer_0 ( RTCC_INTERNAL | RTCC_DIV_64 );  // timer0 com clock interno e dividido por 64
   set_timer0(0);  // carrega o valor 0 no registrador do timer
   enable_interrupts (global | int_timer0);
   enable_interrupts(int_ext);
   ext_int_edge(H_to_L);
   
   setup_wdt(WDT_2304MS);                                                        //Seta watch dog timer
   while(TRUE)
   {  
      restart_wdt();
      //Comando para leitura analogica
      //set_adc_channel(0);        //Seta o canal AN0 com entrada anal?gica
      //delay_us(10);              //tempo necess?rio para o hardware ficar pronto
                                          //Leitura analogica
      // & bitwise retorna
      // && operador l�gico de eq false/true
      //ALIMENTO PINO B1
      //BOMBA PINO B2
      //VENTILADOR PINO B3
      //INTERRUP��O PINO B0
      //Verificar se existe alimento para prosseguir inicialmente, as outras s�o para alimentar o LCD a princ�pio
      alimento = input(pin_b1);
      delay_ms(250);
      
      //Primeira etapa para chave 1
      if((alimento == 0) && (flag == 0))
      {
         bomba = input(pin_b2);
         delay_ms(250);
         //Segunda etapa para chave 2
         restart_wdt();
         if((alimento == 0) && (bomba == 0) && (flag == 0))
         {
            restart_wdt();
            ventilador = input(pin_b3);
            delay_ms(250);
            //Terceira etapa para chave 3
            if((alimento == 0) && (bomba == 0) && (ventilador == 0) && (flag == 0))
            {
               restart_wdt();
               set_adc_channel(0);        //Seta o canal AN0 com entrada anal?gica
               delay_us(10);              //tempo necess?rio para o hardware ficar pronto
                                          //Leitura analogica
               temperatura = LER_TEMPERATURA();
              
               //J� inicia a rota��o do disco spray
               output_high(pin_c5);
               delay_ms(500);
               //Liga os ventiladores e exaustores
               output_high(pin_c3);            // Exaustor
               delay_ms(500);
               //Espera atingir a temperatura m�nima de processo
               while(temperatura < 100)
               {
                  restart_wdt();
                  if(flag == 0)
                  {                  //Aquecendo vento
                     set_adc_channel(0);        //Seta o canal AN0 com entrada anal?gica
                     delay_us(10);              //tempo necess?rio para o hardware ficar pronto
                                                //Leitura analogica
                     temperatura = LER_TEMPERATURA();
                     delay_ms(400);
                     //Disco de RPM girando
                     output_high(pin_c4);
                  }else{
                     
                     output_low(pin_c7);
                     temperatura = 101;
                  }
               }
               //Apaga sinaliza��o de temperatura baixa
               output_low(pin_c4);
               delay_ms(200);
               
               //Verifica��o para ver se chaves n�o foram desligadas
               alimento = input(pin_b1);
               delay_ms(100);
               bomba = input(pin_b2);
               delay_ms(100);
               restart_wdt();
               ventilador = input(pin_b3);
               //Inicio do bombeamento do produto 
               if((flag == 0) && (alimento == 0) && (bomba == 0) && (ventilador == 0) ){
                  restart_wdt();
                  //Inicia o transporte pela bomba helicoidal ap�s temperatura correta
                  output_high(pin_c6);
                  delay_ms(500);
                  //Ate aqui todos requerimentos foram feitos para iniciar o processo!!
                  delay_ms(2000);
                  output_low(pin_c3);
                  output_low(pin_c5);
                  output_low(pin_c6);
               }else{
               //Trata caso se n�o estiver ligado e/ou interrup��o
                 output_low(pin_c3);
                  output_low(pin_c5);
                  output_low(pin_c6);
                  output_low(pin_c7);
                  
               }
            }else{
            //Voltando da interrup��o, apaga por seguran�a
               output_low(pin_c3);
               output_low(pin_c5);
               output_low(pin_c6);  
               output_low(pin_c7);
               
            }
            
         }else{
         //Voltando da interrup��o, apaga por seguran�a
            output_low(pin_c3);
            output_low(pin_c5);
            output_low(pin_c6); 
            output_low(pin_c7);
         }
      }else{//Limpa interrup��o e inicia o processo novamente
         output_low(pin_c3);
         output_low(pin_c5);
         output_low(pin_c6);
         flag = 0;
         output_low(pin_c7);
      }
      delay_ms(1000);
   }

}
