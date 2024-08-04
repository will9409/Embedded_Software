// ======================================================================
// File: args.c
// ======================================================================

#include <uart.h>

// ======================================================================

char *hello = "Hello!";		// The string is in rodata section.
char *bye = "Bye!";		// The string is in rodata section.

unsigned int i = 0x12345678;	// i is in data section.
unsigned int j = 0x90abcdef;	// j is in data section.

unsigned int array[1024];	// array[1024] is in bss section.

// ======================================================================

int fun1(void);			// function fun1 that main calls.
int fun2(void);			// function fun2 that main calls.

// ======================================================================

int main(int argc, char *argv[])
{
  int q= 0;
  // Arguments
  UART_printf("==================================================\n");
  UART_printf("Arguments:\n");
  UART_printf("argc=%d\n",argc);
  for( ; q < argc; q++){
    UART_printf("argv[%d]=%s\n",q,argv[q]);
  }


  UART_printf("==================================================\n");

  fun1();
  fun2();


  return  7;
}

// ======================================================================

int fun1(void)
{
  int x[1000];

  x[0] = 0;
  x[1] = x[0];

  return 0;
}

// ======================================================================

int fun2(void)
{
  int y[2000];

  y[0] = 0;
  y[1] = y[0];

  return 0;
}

// ======================================================================
