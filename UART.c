// CECILIA RAMOS
#include "lib/include.h"

// MI configuración UART5 que corresponde al PC6(23Rx) y PC7(22Tx) pp. 1164
extern void Configurar_UART0(void)
{
    ///////////////////  CONFIGURACIÓN PINES /////////////////////
    // Habilitar RELOJES <UART> y <GPIO> con REGISTROS
    SYSCTL->RCGCUART  = (1<<5);           //Paso 1 (RCGCUART) pag.388 UART/modulo5 0->Disable 1->Enable
    SYSCTL->RCGCGPIO |= (1<<2);           //Paso 2 (RCGCGPIO) pag.382 Enable clock port C
    
    //(GPIOAFSEL) pag.770 Enable alternate function
    GPIOC_AHB->AFSEL = (1<<7) | (1<<6);   // Habilita función alternativa en el pin C6 y C7 pag.787 PIN 6 (recepcion) y PIN 7 (transmisión)
    
    //GPIO Port Control (GPIOPCTL) to assign the UART signals to the appropriate pins
    //PC6-> U5Rx PC7-> U5Tx pag.1808                                  
    //GPIOA_AHB->PCTL = (GPIOA_AHB->PCTL&0xFFFFFF00) | 0x00000011;// (1<<0) | (1<<4);//0x00000011
    GPIOC_AHB->PCTL =  (1<<20) | (1<<24);   //Indica pin 6 y 7 funcionan como UART pag. 787
    
    // GPIO Digital Enable (GPIODEN) pag.781
    GPIOC_AHB->DEN = (1<<6) | (1<<7);      //PC6 PC7 habilito como digitales
    
    
    
    /////////////////// CONFIGURACIÓN UART /////////////////////
    //UART0 UART Control (UARTCTL) pag.1188 DISABLE!!
    UART5->CTL = (0<<9) | (0<<8) | (0<<0);      // Deshabilito UARTEN, TXE, RXE
    //            RXE       TXE    UARTEN (UARTENable)

    // UART Integer Baud-Rate Divisor (UARTIBRD) pag.1172
    /*
    BRD = 20,000,000 / (16 * 9600) = 130.2
        = 40,000,000 / (16 * 28800) = 86.805

    UARTFBRD[DIVFRAC] = integer(0.2 * 64 + 0.5) = 14
                               (0.805 * 64 + 0.5) = 52.02
    */
    UART5->IBRD = 86;
    
    // UART Fractional Baud-Rate Divisor (UARTFBRD) pag.1172
    UART5->FBRD = 52;
    
    //  UART Line Control (UARTLCRH) pag.1186 Serial parameters such as data length, parity, and stop bit selection are implemented in this register.
    UART5->LCRH = (0x3<<5);      //PARA TENER 8 bits(0x3 pp 1186)
    //UART5->LCRH = (0x3<<5)|(1<<4);  |(1<<4) activar el bit de paridad pero solo se emplea con el uart0
    
    //  UART Clock Configuration(UARTCC) pag.1213
    UART5->CC =(0<<0);         //usaremos el reloj PRINCIPAL
    
    //Disable UART0 UART Control (UARTCTL) pag.1188
    UART5->CTL = (1<<9) | (1<<8) | (1<<0);
    //            RXE       TXE      UARTEN

}


//Experimento 2

//El envio es su nombre  (rave) 

// invertirlo y regresarlo con numeros consecutivos
// entre letras (e1v2a3r) 


/////// LEEMOS N CARACTER
extern char readChar(void)          
{
    //UART FR flag pag 1180
    //UART DR data 1175
    int v;      // BANDERA
    char c;
    while((UART5->FR & (1<<4)) != 0 );          // Se mantiene en el ciclo cuando no llega un dato 
    v = UART5->DR & 0xFF;               //DR - registro del dato y 0xff es la mascara 
    c = v;                          // Convierte el entero a caracter
    return c;
}


//////// LEEMOS UNA CADENA 
extern char * readString(char delimitador)  
{

   int i=0;
   char *string = (char *)calloc(10,sizeof(char));
   char c = readChar();
   while(c != delimitador)
   {
       *(string+i) = c;
       i++;
       if(i%10==0)
       {
           string = realloc(string,(i+10)*sizeof(char));
       }
       c = readChar();
   }

   return string;

}

///////// ENVIAMOS CARACTER
extern void printChar(char c)           
{
    while((UART5->FR & (1<<5)) != 0 );    // ¿La posición 5 esta desocupada? SI = continua con la linea de datos y transmite  
    UART5->DR = c;                                
}

/////////// ENVIAMOS CADENA
extern void printString(char* string)       
{
    while(*string)                         // String es la direccion de memoria PARA LA CADENA para saber si esta vacía, si esta vacia se sale del ciclo
    {
        printChar(*(string++));
    }
    printChar('\n');
}

////////// INVERTIMOS CADENA
extern char * invertir(char *str)
{
    uint32_t n = strlen(str);
    uint32_t i = 0;
    char *inv = (char*)calloc(10,sizeof(char));

    while(i<n)
    {
        *(inv + (n-i-1)) * str[i];

    }
    return inv;
}


// calloc y realloc son funciones de memoria