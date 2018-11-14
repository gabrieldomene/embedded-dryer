#include <16F877A.h>
#device ADC=10
#use delay(crystal=4000000)

// Constantes
// Tempo entre leituras do sensor
#define TEMPO_ATUALIZACAO_MS 1000

// 50 Hz s?o o suficiente para o olho humano n?o perceber os displays piscarem.

// 4 MHz / 4 = 1 MHz
// Temos ent?o que ividir frequ?ncia de 1 MHz para 50 Hz
// Assim, 1 MHz / 50 Hz = 20.000
// Com o timer configurado para 20.000 estouros, portanto, os displays
// j? n?o ir?o piscar.
// 65536 - 20000 = 45536
#define CONT_T1 45536

// Pinos de entrada
#define SENSOR  0

// Pinos de sa?da
#define DEZENA  PIN_C1
#define UNIDADE PIN_C0

// Macros
// Macro para selecionar um canal do conversor AD e aguardar at? que ele esteja pronto para leitura
#define CANAL(x) set_adc_channel(x); delay_us(10)

// Macro para ler o canal ADC e retornar a temperatura, em ?C.
#define LER_TEMPERATURA() 5.0 * read_adc() * 100.0 / 1023.0

