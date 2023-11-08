#include "drivers/buttons.h"
#include "xil_io.h"
#include "xparameters.h"
#include <assert.h>

#define TRI_REGISTER_OFFSET 0x04
#define DATA_REGISTER_OFFSET 0x00

// helper function to read from the buttons register.
// takes in an offeset from the buttons base address, and returns the data from
// the register as a 32 bit value
static uint32_t readRegister(uint32_t offset) {
  return Xil_In32(XPAR_PUSH_BUTTONS_BASEADDR + offset);
}

// helper function to write to buttons base register plus an offset
// takes in an offset form the buttons base address and the value to be written
// to the register. returns void
static void writeRegister(uint32_t offset, uint32_t value) {
  Xil_Out32(XPAR_PUSH_BUTTONS_BASEADDR + offset, value);
}

// Initializes the button driver software and hardware.
void buttons_init() {
  // set tri state registers for buttons to be inputs.
  writeRegister(TRI_REGISTER_OFFSET, 0xF);

  // test to make sure the write was completed.
  assert(readRegister(TRI_REGISTER_OFFSET) == 0xF);
}

// return value retrieved from reading the button data register.
uint8_t buttons_read() { return readRegister(DATA_REGISTER_OFFSET); }