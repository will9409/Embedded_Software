// ======================================================================
// File: tasks-statistic.c
// ======================================================================

#include <includes.h>
#include <uart.h>

// ======================================================================

#define N_TASKS		10
#define TASK_STK_SIZE	512

// ======================================================================

OS_STK TaskStk[N_TASKS][TASK_STK_SIZE];
OS_STK TaskStartStk[TASK_STK_SIZE];

char TaskData[N_TASKS];

OS_EVENT *RandomSem;

// ======================================================================

void TaskStart(void *data);
void Task(void *data);
void TaskStatDisp(void);
unsigned short myrandom(unsigned short);


// ======================================================================

int main(void)
{
  OSInit();

  RandomSem = OSSemCreate(1);
  OSTaskCreate(TaskStart,(void *)0, &TaskStartStk[TASK_STK_SIZE-1],10);

  OSStart();

  return 0;
}

// ======================================================================

void TaskStart(void *pdata)
{
  unsigned char i;

  TIMER_enable();		// The first Task should call this.
  UART_clear();
  OSStatInit();
  for(i = 0; i < 10; i++){
  TaskData[i] = i;
  OSTaskCreate(Task,(void *)&TaskData[i], &TaskStk[i][TASK_STK_SIZE-1],i+11);
  }
  for(;;){
  UART_goto(0,0);
  UART_printf("LAB-08: task-statistic.c");
  TaskStatDisp();
  OSTimeDlyHMSM(0,0,1,0);
  }

}

// ======================================================================

void Task(void *pdata)
{
  for(;;){
  OSSemPend(RandomSem, 0, OS_NO_ERR);
  unsigned int x = myrandom(80);
  unsigned int y = myrandom(20);
  OSSemPost(RandomSem);

  UART_goto(x,y+2);
  UART_printf("%d",*(char *)pdata);
  OSTimeDly(5);
  }
}

// ======================================================================

void TaskStatDisp(void)
{
  UART_goto(0,23);
  UART_printf("#Tasks: %d",OSTaskCtr);
  UART_printf(", CPU USage(%%): %d",OSCPUUsage);
  UART_printf(", #CtsSW/Sec: %d",OSCtxSwCtr);
 // OSCtxSwCtr = 0;
}

unsigned short myrandom(unsigned short n)
{
  static unsigned int rand = 0;

  rand = rand * 1103515245 + 12345;
  return (unsigned short) ((rand / 65536) % n);
}

// ======================================================================
