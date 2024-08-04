// ======================================================================
// File: sync-semaphore.c
// ======================================================================

#include <includes.h>
#include <uart.h>

#define N_TASKS 3
#define TASK_STK_SIZE 512
// ======================================================================

OS_STK TaskStk[N_TASKS][TASK_STK_SIZE];
unsigned int var1;
unsigned int var2;
unsigned int sum;
void Task1(void *pdata);
void Task2(void *pdata);
void Task3(void *pdata);
OS_EVENT *RandomSem1;
OS_EVENT *RandomSem2;
OS_EVENT *RandomSem3;
// ======================================================================

int main(void)
{
  OSInit();

  RandomSem1 = OSSemCreate(1);
  RandomSem2 = OSSemCreate(0);
  RandomSem3 = OSSemCreate(0);
  OSTaskCreate(Task1,(void *)0,&TaskStk[0][TASK_STK_SIZE-1],11);
  OSTaskCreate(Task2,(void *)0,&TaskStk[1][TASK_STK_SIZE-1],12);
  OSTaskCreate(Task3,(void *)0,&TaskStk[2][TASK_STK_SIZE-1],13);

  OSStart();

  return 0;
}

// ======================================================================

void Task1(void *pdata)
{
  UART_clear();
  UART_printf("Hexa-decimal addr! (using semaphore)\n\n");
  for(;;){
  INT8U err;  
  OSSemPend(RandomSem1,0,&err);
  var1 = 0;
  var2 = 0;
  sum = 0;
  UART_printf("? ");
  var1 = UART_gethex();
  UART_printf("? ");
  var2 = UART_gethex();
  OSSemPost(RandomSem2);
  }
}

// ======================================================================

void Task2(void *pdata)
{
  for(;;){
  INT8U err;  
  OSSemPend(RandomSem2,0,&err);
  sum = var1 + var2;
  OSSemPost(RandomSem3);
  }
}

// ======================================================================

void Task3(void *pdata)
{
  for(;;){
  INT8U err;  
  OSSemPend(RandomSem3,0, &err);
  UART_printf("0x%04x + 0x%04x = 0x%04x\n\n",var1,var2,sum);
  OSSemPost(RandomSem1);
  }
}

// ======================================================================
