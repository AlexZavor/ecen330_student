#include "interrupts.h"
#include "armInterrupts.h"
#include "xil_io.h"
#include "xparameters.h"

#define IPR_OFFSET 0x04 // Interrupt Pending Register
#define IER_OFFSET 0x08 // Interrupt Enable Register
#define IAR_OFFSET 0x0C // Interrupt Acknowledge Register
#define SIE_OFFSET 0x10 // Set Interrupt Enables
#define CIE_OFFSET 0x14 // Clear Interrupt Enables
#define MER_OFFSET 0x1C // Master Enable Register

#define MASTER_ENABLE 0x03
#define DISABLE_ALL_INPUTS 0x00

#define NUMBER_OF_INTERRUPT_LINES 32

// placeholding for the addresses of registered isr functions
static void (*isrFcnPtrs[NUMBER_OF_INTERRUPT_LINES])() = {NULL};

// helper function to read from the buttons register.
// takes in an offeset from the buttons base address, and returns the data
// from the register as a 32 bit value
static uint32_t readRegister(uint32_t offset) {
  return Xil_In32(XPAR_AXI_INTC_0_BASEADDR + offset);
}

// helper function to write to buttons base register plus an offset
// takes in an offset form the buttons base address and the value to be written
// to the register. returns void.
static void writeRegister(uint32_t offset, uint32_t value) {
  Xil_Out32(XPAR_AXI_INTC_0_BASEADDR + offset, value);
}

// helper function to direct the flow of interrupts. when receving a valid
// interrupt this function decides what interrupt line on the controller is
// being called and proceeds to call the registered interrupt sub routiene for
// that interrupt. after the isr is returns this function also acknowleges the
// interrupt in the interrupt controller. Returns void.
static void interrupts_isr() {
  // Loop through each interrupt line
  for (uint8_t interrupt = 0; interrupt < NUMBER_OF_INTERRUPT_LINES;
       interrupt++) {
    // check if line is requesting an interrupt
    if (readRegister(IPR_OFFSET) & (0x01 << interrupt)) {
      // If the interrupt has a vallid registerd function, call that function
      if (isrFcnPtrs[interrupt] != NULL) {
        isrFcnPtrs[interrupt]();
      }
      // acknoledge the interrupt in the interrupt controller.
      writeRegister(IAR_OFFSET, (0x01 << interrupt));
    }
  }
}

// Initialize interrupt hardware
void interrupts_init() {
  // Enable hardware interupts in the interupt controller
  writeRegister(MER_OFFSET, MASTER_ENABLE);
  writeRegister(IER_OFFSET, DISABLE_ALL_INPUTS);
  // Enable ARM Interrupt System
  armInterrupts_init();
  armInterrupts_enable();
  armInterrupts_setupIntc(interrupts_isr);
}

// Register a callback function (fcn is a function pointer to this callback
// function) for a given interrupt input number (irq).  When this interrupt
// input is active, fcn will be called.
void interrupts_register(uint8_t irq, void (*fcn)()) { isrFcnPtrs[irq] = fcn; }

// Enable single input interrupt line, given by irq number.
void interrupts_irq_enable(uint8_t irq) {
  writeRegister(SIE_OFFSET, (0x01 << irq));
}

// Disable single input interrupt line, given by irq number.
void interrupts_irq_disable(uint8_t irq) {
  writeRegister(CIE_OFFSET, (0x01 << irq));
}
