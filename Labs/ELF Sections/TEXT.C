// ======================================================================
// File: text.c
// ======================================================================

#include <uart.h>

// ======================================================================

extern unsigned int _start;

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
  // Section text
  UART_printf("==================================================\n");
  UART_printf("Section text:\n");
  UART_printf("&_start=%x\n",&_start);
  UART_printf("main=%x\n",&main);
  UART_printf("fun1=%x\n",&fun1);
  UART_printf("fun2=%x\n",&fun2);
  // Section rodata
  UART_printf("==================================================\n");
  UART_printf("Section rodata:\n");
  UART_printf("hello=%x, *hello=%s\n",&hello,hello);
  UART_printf("bye=%x, *bye=%s\n",&bye,bye);
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
