// ======================================================================
// File: uart.c
// Note that only UART1 of BeagleBone is used here!
// ======================================================================

#include <uart.h>
// ======================================================================

#define IO_READ(addr)		(*(volatile unsigned int *)(addr))
#define IO_WRITE(addr, val)	(*(volatile unsigned int *)(addr) = (val))

// ======================================================================

#define UART1_RHR_REG	0x44e09000	// UART1 Receive holding register
#define UART1_THR_REG	0x44e09000	// UART1 Transmit holding register
#define UART1_LSR_REG	0x44e09014	// UART1 Line status register

#define LSR_RX_FIFO_E	0x01	// LSR receive fifo empty
#define LSR_TX_FIFO_E	0x20	// LSR transmit fifo empty

// ======================================================================

int UART_getc(void)
{
  unsigned int c;

  while ((IO_READ(UART1_LSR_REG) & LSR_RX_FIFO_E) == 0);

  c = IO_READ(UART1_RHR_REG);

  UART_putc(c);

  return (int)c;
}

// ======================================================================

int UART_getc_noblock(void)
{
  unsigned int c;
  
  c = IO_READ(UART1_RHR_REG);

  UART_putc(c);

  return (int)c;
}

// ======================================================================

unsigned int UART_gethex(void)
{
  unsigned int temp;
  unsigned int hex= 0x00000000;
  int i = 0;
  while(1){

  while((IO_READ(UART1_LSR_REG) & LSR_RX_FIFO_E) == 0);
    temp = IO_READ(UART1_RHR_REG);
    UART_putc(temp);
    if(temp == '\n' || temp == '\r')return hex;
    else if(temp == '0')temp = 0x00000000;
    else if(temp == '1')temp = 0x00000001;
    else if(temp == '2')temp = 0x00000002;
    else if(temp == '3')temp = 0x00000003;
    else if(temp == '4')temp = 0x00000004;
    else if(temp == '5')temp = 0x00000005;
    else if(temp == '6')temp = 0x00000006;
    else if(temp == '7')temp = 0x00000007;
    else if(temp == '8')temp = 0x00000008;
    else if(temp == '9')temp = 0x00000009;
    else if(temp == 'A' || temp == 'a')temp = 0x0000000A;
    else if(temp == 'B' || temp == 'b')temp = 0x0000000B;
    else if(temp == 'C' || temp == 'c')temp = 0x0000000C;
    else if(temp == 'D' || temp == 'd')temp = 0x0000000D;
    else if(temp == 'E' || temp == 'e')temp = 0x0000000E;
    else if(temp == 'F' || temp == 'f')temp = 0x0000000F;
    else return temp;
    if( i == 0 ){
      hex = hex + temp;
      i++;
    }
    else{
      hex = hex << 4;
      hex = hex + temp;
      i++;
    }
  }
  return hex;
}

// ======================================================================

char *UART_gets(char *str)
{
  int j = 0;
  int i = 0;
  int q;

  for(j; j < 512; j++){
     str[j] = '\0';
  }

  while(1){

    while((IO_READ(UART1_LSR_REG) & LSR_RX_FIFO_E) == 0);

    q = IO_READ(UART1_RHR_REG);
    UART_putc(q);
    if(q == '\n' || q == '\r'){
        return str;
    }
    str[i] = q;
    i++;
  }  
}

// ======================================================================

int UART_putc(int c)
{
  if ((c == '\r') || (c == '\n')){
    c = '\r';
 
  while ((IO_READ(UART1_LSR_REG) & LSR_TX_FIFO_E) == 0);

    IO_WRITE(UART1_THR_REG, c);

    c = '\n';
  }

  while ((IO_READ(UART1_LSR_REG) & LSR_TX_FIFO_E) == 0);

  IO_WRITE(UART1_THR_REG, c);

  return c;
  

}

// ======================================================================

int UART_puts(const char *s)
{
    int i = 0;
    while(s[i] != '\0'){
    IO_WRITE(UART1_THR_REG,s[i]);
    i++;
    }
  return (int)s;
}

// ======================================================================

void UART_clear(void)
{
  char escsqnce[20] = "\033[2J\033[0;0H";
  UART_puts(escsqnce);
}

// ======================================================================

void UART_goto(int x, int y)
{
  char xnumber[3];
  char ynumber[3];
  char gotosequence1[5]= "\033[";
  char semicolon = ';';
  char end = 'H';
  int i = 0;
  for(i; i< 3; i++){
    if(i == 0){
    xnumber[i] = x/100+48;
    ynumber[i] = y/100+48;
    }
    else if(i == 1){
    xnumber[i] = ((x/10)%10)+48;
    ynumber[i] = ((y/10)%10)+48;
    }
    else if(i == 2){
    xnumber[i] = (x%10)+48;
    ynumber[i] = (y%10)+48;
    }
  }
  


  UART_puts(gotosequence1);
  UART_putc(ynumber[0]);
  UART_putc(ynumber[1]);
  UART_putc(ynumber[2]);
  UART_putc(semicolon);
  UART_putc(xnumber[0]);
  UART_putc(xnumber[1]);
  UART_putc(xnumber[2]);
  UART_putc(end);
}

// ======================================================================

int UART_sprintf(char *out, const char *format, ...)
{
  extern int print(char **out, int *varg);

  register int *varg = (int *) (&format);
  return print(&out, varg);
}

// ======================================================================

int UART_printf(const char *format, ...)
{
  extern int print(char **out, int *varg);

  register int *varg = (int *) (&format);
  return print(0, varg);
}

// ======================================================================
