#include <xc.h>
#include <string.h>
#include "config.h"
#define _XTAL_FREQ 8000000
#define DP PORTDbits.RD7

  const unsigned char seven_seg_digits_decode_gfedcba[75]= {
/*  0     1     2     3     4     5     6     7     8     9     :     ;     */
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x00, 0x00, 
/*  <     =     >     ?     @     A     B     C     D     E     F     G     */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x00, 0x39, 0x00, 0x79, 0x71, 0x3D, 
/*  H     I     J     K     L     M     N     O     P     Q     R     S     */
    0x76, 0x30, 0x1E, 0x00, 0x38, 0x00, 0x00, 0x3F, 0x73, 0x00, 0x00, 0x6D, 
/*  T     U     V     W     X     Y     Z     [     \     ]     ^     _     */
    0x00, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
/*  `     a     b     c     d     e     f     g     h     i     j     k     */
    0x00, 0x00, 0x7C, 0x58, 0x5E, 0x00, 0x00, 0x00, 0x74, 0x00, 0x00, 0x00, 
/*  l     m     n     o     p     q     r     s     t     u     v     w     */
    0x00, 0x00, 0x54, 0x5C, 0x00, 0x67, 0x50, 0x00, 0x78, 0x1C, 0x00, 0x00, 
/*  x     y     z     */
    0x00, 0x6E, 0x00
};



void config_pic(){
   
   
   ANSELA=0x00; // All pins as digital
   ANSELD=0x00;
   TRISA=0x00;
   TRISD=0x00;
   PORTA=0x00;
   PORTD=0x00;
      
   ADCON1 = 0x0F; 
   GIEH = 1;	
   GIEL = 1;	
   INT2IF = 0;	
   INT1IF = 0;	
   INT2IE = 1;	
   INT1IE = 1;
   INT2IP = 0;	
   INT1IP = 1;	
   IPEN = 1;
}

/* Invalid letters are mapped to all segments off (0x00). */
unsigned char decode_7seg(unsigned char chr)
{ /* assuming implementation uses ASCII */
    if (chr > (unsigned char)'z')
        return 0x00;
    return seven_seg_digits_decode_gfedcba[chr - '0'];
    
}

void escribir_string(unsigned char* frase){
   int a =strlen(frase) - 4;
   int b;
   for(int i=0; i<a; ++i){
      for(int h=0; h<200; h++){	 
	 b=0x01;
      for (int j =4; j>0; j--){
	 PORTD = seven_seg_digits_decode_gfedcba[frase[i+j] - 0x30];
	 PORTA=b;
	 __delay_ms(1);
	 PORTA=0x00;
	 b = b<<1;
	}
     }
   }
}

void interrupt service_routine_HP(void){// INT1 routine
   if (INT1IF) {
      INT1IF = 0;
      ++cont;
      unsigned char frase[] = itoa(cont);
      escribir_string(frase);
      }
}

void interrupt low_priority service_routine_LP(void){// INT2 routine
   if (INT2IF) {	
      INT2IF = 0;
      --cont;
      while(INT2);
      unsigned char frase[]  = itoa(cont);
      escribir_string(frase);
      }
}




void main(void)
 {
   config_pic();
   while (1);
      
 }