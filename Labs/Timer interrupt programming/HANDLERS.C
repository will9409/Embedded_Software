// ======================================================================
// File: handlers.c
// ======================================================================

#include <uart.h>
#include "handlers.h"

// ======================================================================

char *get_current_mode(void);

// ======================================================================
// exception handler for undefined instruction

void handle_undefined_instruction(unsigned int *sp, unsigned int spsr)
{
  UART_printf("Undefined instruction, mode=%s\n", get_current_mode());
}

// ======================================================================
// exception handler for software interrupt

void handle_software_interrupt(unsigned int *sp, unsigned int spsr)
{
  UART_printf("Software interrupt, mode=%s\n", get_current_mode());
}

// ======================================================================
// exception handler for prefetch abort

void handle_prefetch_abort(unsigned int *sp, unsigned int spsr)
{
  UART_printf("Prefetch abort, mode=%s\n", get_current_mode());
}

// ======================================================================
// exception handler for data abort

void handle_data_abort(unsigned int *sp, unsigned int spsr)
{
  unsigned int pc, inst;

  pc = sp[13] - 4;
  inst = *((unsigned int *) pc);

  UART_printf("Data abort, mode=%s\n", get_current_mode());
  UART_printf("lr=%08x, spsr=%08x, pc=%08x, inst=%08x\n",
	      sp[13], spsr, pc, inst);
}

void handle_data_abort_old(unsigned int *sp, unsigned int spsr)
{
  UART_printf("Data abort, mode=%s\n", get_current_mode());
}

// ======================================================================
// exception handler for interrupt

volatile unsigned int clock = 0;

void handle_irq(unsigned int *sp, unsigned int spsr)
{
   // If DMTIMER1 TISR has overflow event.
 if ((IO_READ(DMTIMER1_BASE+TISR)&0x00000002) == 0x00000002) {
    clock++;
    // DMTIMER1 TISR: clear pending capture/overflow/compare events.
  IO_RCW(DMTIMER1_BASE+TISR,0x00000002,0xffffffff);
  }
  // INTC_CONTROL: reset IRQ generation and enable new IRQ.
  IO_RCW(DMTIMER1_BASE+INTC_CONTROL,0x00000001,0xffffffff);
 // IO_RCW(DMTIMER1_BASE+INTC_CONTROL,0x00000001,0x0);
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
  asm volatile("	mrs	r4, cpsr");
  asm volatile("	mov	%0, r4":"=r" (d):);
  d = mask & d;
  if(d == 0x00000010)return "usr";
  if(d == 0x00000011)return "irq";
  if(d == 0x00000012)return "fiq";
  if(d == 0x00000013)return "svc";
  if(d == 0x00000017)return "abt";
  if(d == 0x0000001b)return "und";
  if(d == 0x0000001f)return "sys";
  else return "no mode";
}

// ======================================================================

void TIMER_init(void)
{
  // CM_WKUP TIMER1_CLKCTRL: make module explicitly enabled.
  IO_WRITE(CM_WKUP_BASE + TIMER1_CLKCTRL, 0x02);

  // CM_DPLL CLKSEL_TIMER1MS_CLK: select CLK_32KHZ clock.
  IO_WRITE(CM_DPLL_BASE + CLKSEL_TIMER1MS_CLK, 0x0);
  IO_WRITE(CM_DPLL_BASE + CLKSEL_TIMER1MS_CLK, 0x01);

  // DMTIMER1 TCLR: stop timer control.
  IO_RCW(DMTIMER1_BASE+TCLR,0x00000001,0x0);

  // DMTIMER1 TCRR, TPIR, TNIR and TLDR: set for 1ms timer.
  IO_WRITE(DMTIMER1_BASE + TCRR, 0xffffffe0);
  IO_WRITE(DMTIMER1_BASE + TPIR, 232000);
  IO_WRITE(DMTIMER1_BASE + TNIR, -768000);
  IO_WRITE(DMTIMER1_BASE + TLDR, 0xffffffe0);

  // DMTIMER1 TCLR: enable auto-reload mode.
  IO_RCW(DMTIMER1_BASE+TCLR,0x00000002,0xffffffff);

  // INTCPS INT_MIR0-3: make all interrupts masked except DMTIMER1.
  IO_WRITE(INTCPS_BASE + INTC_MIR0, 0xffffffff);
  IO_WRITE(INTCPS_BASE + INTC_MIR1, 0xffffffff);
  IO_WRITE(INTCPS_BASE + INTC_MIR2, 0xfffffff7);
  IO_WRITE(INTCPS_BASE + INTC_MIR3, 0xffffffff);

  // DMTIMER1 TISR: clear pending capture/overflow/match events.
  IO_RCW(DMTIMER1_BASE+TISR,0x00000007,0xffffffff);

  // DMTIMER1 TIER: enable overflow interrupt.
  IO_RCW(DMTIMER1_BASE+TIER,0x00000007,0xffffffff);

  // INTCPS INTC_CONTROL: reset IRQ and enable new IRQ.
  IO_RCW(DMTIMER1_BASE+INTC_CONTROL,0x00000001,0xffffffff);

  // Enable interrupt.
  asm volatile ("	cpsie	i");
}

// ======================================================================

void TIMER_enable(void)
{
  // DMTIMER1 TCLR: start timer control.
  //IO_WRITE(DMTIMER1_BASE + TCLR, ((DMTIMER1_BASE+TCLR)+1));
  IO_RCW(DMTIMER1_BASE+TCLR,0x00000001,0xffffffff);
}

// ======================================================================

void TIMER_disable(void)
{
  // DMTIMER1 TCLR: stop timer control.
  //IO_WRITE(DMTIMER1_BASE + TCLR, IO_READ(DMTIMER1_BASE+TCLR)&0xfffffffe);
  IO_RCW(DMTIMER1_BASE+TCLR,0x00000001,0x0);
  
}

// ======================================================================
