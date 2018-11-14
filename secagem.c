#include <secagem.h>
#include <lcd.c>
int x = 0;
int y;
int1 flag;
int1 fire = 1;
int1 bomba;
int1 alimento;
#int_ext
void trata_ext()
{
 char c;
   flag =1;              // Vari?vel auxiliar para indicar que a int aconteceu 
   while(input(pin_b0)==0)
   {
      c='\f';
      lcd_putc(c);
      
      printf(LCD_PUTC, "INTERRUPCAO");
      delay_ms(400);
   }  //fica esperando mudar situa??o do bot?o da interrup.
    c='\f';
    lcd_putc(c);
    y = 0;
}
#int_timer0
void trata_t0()
{  
   y = y + 1;
   int valor;
   char c;
   set_timer0(131);
   if(flag == 0){
      
      if((y == 10) && (x == 0)){
   
         //Limpa lcd
         c='\f';
         lcd_putc(c);
         
         //Escolhe canal
         set_adc_channel(0);        //Seta o canal AN0 com entrada anal?gica
         delay_us(10);              //tempo necess?rio para o hardware ficar pronto
         //Leitura analogica
         valor = LER_TEMPERATURA();
         //Escreve no LCD
         printf(LCD_PUTC, "TEMP = %u'",valor);
         delay_ms(500);
         x = x+1;
         y = 0;
      }else if(y == 10 && x == 1){
         //Limpa lcd
         c='\f';
         lcd_putc(c);
         if(alimento == 0)
         {
            printf(LCD_PUTC, "PRODUTO OK");
            delay_ms(500);
         }else{
            printf(LCD_PUTC, "SEM PRODUTO");
            delay_ms(500);
         }
         //Escreve no LCD
         
         x = 0;
         y = 0;
      }
   }
}
void main()
{
   Port_b_pullups(true);
   set_tris_c(0B0000000);
   output_low(pin_b1);
   lcd_init();
   setup_adc_ports(AN0);             //configura porta AN0 com entrada anal?gica
   setup_adc(ADC_CLOCK_INTERNAL);    //usar? o clock interno do AD(mais comum)
   
   
   //setup_timer_1(T1_INTERNAL);
   //setup_timer_1(RTCC_DIV_8);
   setup_timer_0 ( RTCC_INTERNAL | RTCC_DIV_64 );  // timer0 com clock interno e dividido por 64
   set_timer0(131);                                         // carrega o valor 131 no registrador do timer
   enable_interrupts (global | int_timer0);
   enable_interrupts(int_ext);
   ext_int_edge(H_to_L);
   //habilita as interrup??es
   
   int1 reserva;
   int1 safety;
   int temperatura;
   char c;
   while(TRUE)
   {  
      
      //Verificar se existe alimento para prosseguir
      alimento = input(pin_b1);
      delay_ms(250);
      if(alimento == 0 )
      {
         output_high(pin_c7);
         //Checar
      }else{
         output_low(pin_c7);
      }
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      flag = 0;
      /*while(flag == 0)
      {
         reserva = input(pin_b2);
         delay_ms(200);
         safety = input(pin_b3);
         //Ver se reservatório tem produto, lógica inversa 0 = S // 1 = N
         if(reserva == 0 && flag != 1)
         {
            output_high(pin_b3);
         }else{
            output_low(pin_b3);
         }
      }*/
   }

}
