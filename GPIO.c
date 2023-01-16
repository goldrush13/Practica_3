// CECILIA RAMOS
#include "lib/include.h"
extern void Configurar_GPIO(void)
{
    //Paso 1 Enceder el periferico Run Clock Gate Control GPIO
    //SYSCTL->RCGCGPIO |= (1<<1) | (1<<5) | (1<<8) | (1<<12);//enable GPIOF,GPION,GPIOJ
    SYSCTL->RCGCGPIO |= (1<<2);//enable GPIOC pag. 382 y 384
    while((SYSCTL->PRGPIO&0x20)==0){};
    
    //Paso 2
    //GPIO_PORTC_AHB_LOCK_R = 0x4C4F434B; //Desbloquea los puertos. A write of the value 0x4C4F.434B unlocks the GPIO Commit (GPIOCR) register for write access
    //GPIO_PORTC_AHB_CR_R = (1<<3);       // allow changes to PF4-0

    //SALIDAS (?)

    //Paso 3 (program each bit as a GPIO or alternate pin.)
    GPIOC_AHB->AFSEL |= 0x00;           // Value 0 = The associated pin functions as a GPIO and is controlled by the GPIO registers.
    GPIOC_AHB->PCTL|= 0x00;             // The GPIOPCTL register is used in conjunction with the GPIOAFSEL r
    GPIOC_AHB->DEN |= (1<<6) | (1<<7);  // The GPIODEN register is the digital enable register
    GPIOC_AHB->AMSEL = 0x00;            // Disable analog pins


}


extern void Delay(void)
{
  unsigned long volatile time;
  time = 1600000;
  while(time)
  {
		time--;
  }
}