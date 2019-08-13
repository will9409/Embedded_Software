// ======================================================================
// File: handlers.c
// ======================================================================

#include <uart.h>

// ======================================================================

char *get_current_mode(void);
extern unsigned int prefetch_abort_pc_save;

// ======================================================================
// exception handler for undefined instruction

void handle_undefined_instruction(unsigned int *sp, unsigned int spsr)
{
  unsigned int k = 0;
  UART_printf("Undefined instruction, mode=%s\n", get_current_mode());
  asm volatile("	ldr %0,[%1]":"=r"(k):"r"(*(sp+13)-4));
  UART_printf("lr = %08x, spsr = %08x, pc = %08x, inst = %08x\n",*(sp+13),spsr,*(sp+13)-4,k);
  

}

// ======================================================================
// exception handler for software interrupt

void handle_software_interrupt(unsigned int *sp, unsigned int spsr)
{

  unsigned int temp = 0;
  unsigned int k = 0;
  unsigned int mask = 0x00FFFFFF;
  UART_printf("Software interrupt, mode=%s\n", get_current_mode());
  asm volatile("	ldr %0,[%1]":"=r"(k):"r"(*(sp+13)-4));
  temp = k & mask;
  UART_printf("lr = %08x, spsr = %08x, pc = %08x, inst = %08x,  arg=%d\n",*(sp+13),spsr,*(sp+13)-4,k,temp);
 

}

// ======================================================================
// exception handler for prefetch abort

void handle_prefetch_abort(unsigned int *sp, unsigned int spsr)
{
  unsigned int temp = *(sp+13);
  unsigned int temp2 = *(sp+13)-4;
  prefetch_abort_pc_save = prefetch_abort_pc_save +  16;
  UART_printf("Prefetch abort, mode=%s\n", get_current_mode());
  asm volatile("	mov r14,%[prefetch_abort_pc_save]"::[prefetch_abort_pc_save]"r"(prefetch_abort_pc_save):"r14");
  asm volatile("	mov %0,r14":"=r"(*(sp+13)):);
  UART_printf("lr = %08x, spsr = %08x, pc = %08x, new lr=%08x\n",temp,spsr,temp2,*(sp+13));
}

// ======================================================================
// exception handler for data abort

void handle_data_abort(unsigned int *sp, unsigned int spsr)
{

  unsigned int k = 0;
  asm volatile("	ldr %0,[%1]":"=r"(k):"r"(*(sp+13)-4));
  UART_printf("Data abort, mode=%s\n", get_current_mode());
  UART_printf("lr = %08x, spsr = %08x, pc = %08x, inst = %08x\n",*(sp+13),spsr,*(sp+13)-4,k);

}

// ======================================================================
// exception handler for interrupt

void handle_irq(unsigned int *sp, unsigned int spsr)
{
  UART_printf("IRQ, mode=%s\n", get_current_mode());
}

// ======================================================================
// exception handler for fast interrupt

void handle_fiq(unsigned int *sp, unsigned int spsr)
{
  UART_printf("FIQ, mode=%s\n", get_current_mode());
}

// ======================================================================

char *get_current_mode(void)
{
   unsigned int d;
   unsigned int mask = 0x0000001f;
   asm volatile("	mrs	r3, cpsr");
   asm volatile("	mov	%0, r3":"=r" (d):);
   d = mask & d;
   if(d == 0x00000010)return "usr";
   if(d == 0x00000011)return "irq";
   if(d == 0x00000012)return "fiq";
   if(d == 0x00000013)return "svc";
   if(d == 0x00000017)return "Abt";
   if(d == 0x0000001b)return "und";
   if(d == 0x0000001f)return "sys";
   else return "no mode";
}

// ======================================================================
