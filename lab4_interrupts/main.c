#include <stdbool.h>
#include <stdio.h>

#include "arm_interrupts.h"
#include "interrupt_test.h"
#include "interrupts.h"

int main() {
  // Run the interrupt test to test the driver for the AXI Interrupt Controller
  printf("Running the interrupt test\n");
  interrupt_test_run();
}