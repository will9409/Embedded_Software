// ======================================================================
// File: stack.c
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
  // Stack area
  fun1();
  fun2();

  return 0;
}

// ======================================================================

int fun1(void)
{
  int x[1000];

  x[0] = 0;
  x[1] = x[0];
  UART_printf("==================================================\n");
  UART_printf("Stack for fun1:\n");
  UART_printf("&x[0]=%x, &x[999]=%x\n",&x[0],&x[999]);
  
  return 0;
}

// ======================================================================

int fun2(void)
{
  int y[2000];

  y[0] = 0;
  y[1] = y[0];
  UART_printf("==================================================\n");
  UART_printf("Stack for fun2:\n");
  UART_printf("&y[0]=%x, &y[1999]=%x\n",&y[0],&y[1999]);
  UART_printf("==================================================\n");

  return 0;
}

// ======================================================================
