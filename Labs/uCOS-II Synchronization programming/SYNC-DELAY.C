// ======================================================================
// File: sync-delay.c
// ======================================================================

#include <includes.h>
#include <uart.h>

#define N_TASKS	3
#define TASK_STK_SIZE 512

// ======================================================================

OS_STK TaskStk[N_TASKS][TASK_STK_SIZE];
OS_STK TaskStartStk[TASK_STK_SIZE];
unsigned int var1;
unsigned int var2;
unsigned int sum;
void Task1(void *pdata);
void Task2(void *pdata);
void Task3(void *pdata);
// ======================================================================

int main(void)
{
  OSInit();

  OSTaskCreate(Task1,(void *)0,&TaskStk[0],11);
  OSTaskCreate(Task2,(void *)0,&TaskStk[1],12);
  OSTaskCreate(Task3,(void *)0,&TaskStk[2],13);

  OSStart();

  return 0;
}

// ======================================================================

void Task1(void *pdata)
{
  TIMER_enable();		// The first Task should call this.
  UART_clear();
  for(;;){
  var1 = 0;
  var2 = 0;
  sum = 0;
  UART_printf("Hexa-decimal adder! (using delay)\n");
  int i = 0;
  UART_printf("? ");
  var1 = UART_gethex();
  UART_printf("? ");
  var2 = UART_gethex();
  OSTimeDly(10);
  }

}

// ======================================================================

void Task2(void *pdata)
{
  for(;;){
  sum = var1 + var2;
  OSTimeDly(10);
  }
}

// ======================================================================

void Task3(void *pdata)
{
  for(;;){
  UART_printf("0x%04x + 0x%04x = 0x%04x\n\n",var1,var2, sum);
  OSTimeDly(10);
  }
}

// ======================================================================
