#include "drivers/intervalTimer.h"
#include "xil_io.h"
#include "xparameters.h"
#include <assert.h>

#define TCSR0_REGISTER_OFFSET 0x00 // Control/Status Register 0
#define TLR0_REGISTER_OFFSET 0x04  // Load Register 0
#define TCR0_REGISTER_OFFSET 0x08  // Timer/Counter Register 0
#define TCSR1_REGISTER_OFFSET 0x10 // Control/Status Register 1
#define TLR1_REGISTER_OFFSET 0x14  // Load Register 1
#define TCR1_REGISTER_OFFSET 0x18  // Timer/Counter Register 1

#define CASC_MASK 0x00000800
#define T0INT_MASK 0x00000100
#define ENT0_MASK 0x00000080
#define ENIT_MASK 0x00000040
#define LOAD0_MASK 0x00000020
#define ARHT0_MASK 0x00000010
#define UDT0_MASK 0x00000002

#define TIMER_0 0
#define TIMER_1 1
#define TIMER_2 2

#define SHIFT_32 32

// helper function to read from the slected timer register.
// takes in an offeset from the slected timer base address, and returns the data
// from the register as a 32 bit value
static uint32_t readRegister(uint8_t timerNumber, uint32_t offset) {
  switch (timerNumber) {
  case TIMER_0:
    return Xil_In32(XPAR_AXI_TIMER_0_BASEADDR + offset);
  case TIMER_1:
    return Xil_In32(XPAR_AXI_TIMER_1_BASEADDR + offset);
  case TIMER_2:
    return Xil_In32(XPAR_AXI_TIMER_2_BASEADDR + offset);
  default:
    return 0;
  }
}

// helper function to write to slected timer base register.
// takes in an offset form the slected timer base address and the value to be
// written to the register. returns void
static void writeRegister(uint8_t timerNumber, uint32_t offset,
                          uint32_t value) {
  switch (timerNumber) {
  case TIMER_0:
    Xil_Out32(XPAR_AXI_TIMER_0_BASEADDR + offset, value);
    break;
  case TIMER_1:
    Xil_Out32(XPAR_AXI_TIMER_1_BASEADDR + offset, value);
    break;
  case TIMER_2:
    Xil_Out32(XPAR_AXI_TIMER_2_BASEADDR + offset, value);
    break;
  default:
    break;
  }
}

void intervalTimer_initCountUp(uint32_t timerNumber) {
  // Reset Control registers
  writeRegister(timerNumber, TCSR0_REGISTER_OFFSET, 0x00);
  writeRegister(timerNumber, TCSR1_REGISTER_OFFSET, 0x00);
  // Write command setting, cascade, count up (already set with 0)
  writeRegister(timerNumber, TCSR0_REGISTER_OFFSET, (CASC_MASK));

  // Set Load Registers to 0
  writeRegister(timerNumber, TLR0_REGISTER_OFFSET, 0x00);
  writeRegister(timerNumber, TLR1_REGISTER_OFFSET, 0x00);

  // Call reload function
  intervalTimer_reload(timerNumber);
}

void intervalTimer_initCountDown(uint32_t timerNumber, double period) {
  // Reset Control registers
  writeRegister(timerNumber, TCSR0_REGISTER_OFFSET, 0x00);
  writeRegister(timerNumber, TCSR1_REGISTER_OFFSET, 0x00);
  // Write command setting, cascade, count down, auto reload
  writeRegister(timerNumber, TCSR0_REGISTER_OFFSET,
                (CASC_MASK | UDT0_MASK | ARHT0_MASK));

  // Calculate period in cycles to pass into the load registers
  uint64_t periodCycles;
  switch (timerNumber) {
  case TIMER_0:
    periodCycles = period * XPAR_AXI_TIMER_0_CLOCK_FREQ_HZ;
    break;
  case TIMER_1:
    periodCycles = period * XPAR_AXI_TIMER_1_CLOCK_FREQ_HZ;
    break;
  case TIMER_2:
    periodCycles = period * XPAR_AXI_TIMER_2_CLOCK_FREQ_HZ;
    break;
  default:
    periodCycles = 0;
    break;
  }
  // Set Load Registers to the period
  writeRegister(timerNumber, TLR0_REGISTER_OFFSET, periodCycles);
  writeRegister(timerNumber, TLR1_REGISTER_OFFSET,
                ((uint64_t)periodCycles >> SHIFT_32));

  // Call reload function
  intervalTimer_reload(timerNumber);
}

void intervalTimer_start(uint32_t timerNumber) {
  // write to the command register with ENT0 set
  writeRegister(timerNumber, TCSR0_REGISTER_OFFSET,
                (readRegister(timerNumber, TCSR0_REGISTER_OFFSET) | ENT0_MASK));
}

void intervalTimer_stop(uint32_t timerNumber) {
  // write to command register with ENT0 reset
  writeRegister(
      timerNumber, TCSR0_REGISTER_OFFSET,
      (readRegister(timerNumber, TCSR0_REGISTER_OFFSET) & (~ENT0_MASK)));
}

void intervalTimer_reload(uint32_t timerNumber) {
  // write to the command register with LOAD0 set
  writeRegister(
      timerNumber, TCSR0_REGISTER_OFFSET,
      (readRegister(timerNumber, TCSR0_REGISTER_OFFSET) | LOAD0_MASK));
  // write to command register with LOAD0 reset
  writeRegister(
      timerNumber, TCSR0_REGISTER_OFFSET,
      (readRegister(timerNumber, TCSR0_REGISTER_OFFSET) & (~LOAD0_MASK)));
}

double intervalTimer_getTotalDurationInSeconds(uint32_t timerNumber) {
  // Read the upper and lower bits of the timer
  uint32_t upperBits = readRegister(timerNumber, TCR1_REGISTER_OFFSET);
  uint32_t lowerBits = readRegister(timerNumber, TCR0_REGISTER_OFFSET);
  // Read the upper bits again to make sure the timer didn't rollover while we
  // read
  if (upperBits != readRegister(timerNumber, TCR1_REGISTER_OFFSET)) {
    // if timer rolled over read again
    upperBits = readRegister(timerNumber, TCR1_REGISTER_OFFSET);
    lowerBits = readRegister(timerNumber, TCR0_REGISTER_OFFSET);
  }
  // combine numbers from both counters.
  double cascadeTimer = lowerBits + ((uint64_t)upperBits << SHIFT_32);
  // Return entire count divided by the frequency of the clock
  switch (timerNumber) {
  case TIMER_0:
    return cascadeTimer / XPAR_AXI_TIMER_0_CLOCK_FREQ_HZ;
  case TIMER_1:
    return cascadeTimer / XPAR_AXI_TIMER_1_CLOCK_FREQ_HZ;
  case TIMER_2:
    return cascadeTimer / XPAR_AXI_TIMER_2_CLOCK_FREQ_HZ;
  default:
    return 0;
  }
}

void intervalTimer_enableInterrupt(uint8_t timerNumber) {
  // write a 1 to the interrupt enable on both counters
  writeRegister(timerNumber, TCSR0_REGISTER_OFFSET,
                (readRegister(timerNumber, TCSR0_REGISTER_OFFSET) | ENIT_MASK));
  writeRegister(timerNumber, TCSR1_REGISTER_OFFSET,
                (readRegister(timerNumber, TCSR1_REGISTER_OFFSET) | ENIT_MASK));
}

void intervalTimer_disableInterrupt(uint8_t timerNumber) {
  // write a 0 to the interrupt enable on both counters
  writeRegister(
      timerNumber, TCSR0_REGISTER_OFFSET,
      (readRegister(timerNumber, TCSR0_REGISTER_OFFSET) & ~ENIT_MASK));
  writeRegister(
      timerNumber, TCSR1_REGISTER_OFFSET,
      (readRegister(timerNumber, TCSR1_REGISTER_OFFSET) & ~ENIT_MASK));
}

void intervalTimer_ackInterrupt(uint8_t timerNumber) {
  // write a 1 to the interupt bit to clear it
  writeRegister(
      timerNumber, TCSR0_REGISTER_OFFSET,
      (readRegister(timerNumber, TCSR0_REGISTER_OFFSET) | T0INT_MASK));
  writeRegister(
      timerNumber, TCSR1_REGISTER_OFFSET,
      (readRegister(timerNumber, TCSR1_REGISTER_OFFSET) | T0INT_MASK));
}