// ======================================================================
// File: data.c
// ======================================================================

#include <uart.h>

// ======================================================================

extern unsigned int _bss_start, _bss_end;

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
  // Section data
  UART_printf("==================================================\n");
  UART_printf("Section data:\n");
  UART_printf("&i=%x, i=%x\n",&i,i);
  UART_printf("&j=%x, j=%x\n",&j,j);

  // Section bss
  UART_printf("==================================================\n");
  UART_printf("Section bss:\n");
  UART_printf("_bss_start=%x, _bss_end=%x\n",&_bss_start,&_bss_end);
  UART_printf("&array[0]=%x, &array[1023]=%x\n",&array[0],&array[1023]);
  UART_printf("array[0]=%d, array[1]=%d\n",array[0],array[1]);
  UART_printf("array[1022]=%d, array[1023]=%d\n",array[1022],array[1023]);
  UART_printf("==================================================\n");

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
