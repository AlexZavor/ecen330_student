#include "drivers/switches.h"
#include "xil_io.h"
#include "xparameters.h"
#include <assert.h>

#define TRI_REGISTER_OFFSET 0x04
#define DATA_REGISTER_OFFSET 0x00

// helper function to read from the switches register.
// takes in an offeset from the switches base address, and returns the data from
// the register as a 32 bit value
static uint32_t readRegister(uint32_t offset) {
  return Xil_In32(XPAR_SLIDE_SWITCHES_BASEADDR + offset);
}

// helper function to write to switches base register plus an offset
// takes in an offset form the switches base address and the value to be written
// to the register. returns void
static void writeRegister(uint32_t offset, uint32_t value) {
  Xil_Out32(XPAR_SLIDE_SWITCHES_BASEADDR + offset, value);
}

// Initializes the switch driver software and hardware.
void switches_init() {
  // set tri state registers for switches to be inputs.
  writeRegister(TRI_REGISTER_OFFSET, 0xF);

  // test to make sure the write was completed.
  assert(readRegister(TRI_REGISTER_OFFSET) == 0xF);
}

// return value retrieved from reading the switches data register.
uint8_t switches_read() { return readRegister(DATA_REGISTER_OFFSET); }