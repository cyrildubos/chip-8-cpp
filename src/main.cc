#include <stdint.h>

#include <iostream>

#include "chip_8.h"

int main() {
  Chip8 chip_8;

  chip_8.ClearMemory();
  chip_8.ClearDisplay();
  chip_8.LoadSprites();

  chip_8.LoadROM("res/roms/ibm_logo.ch8");

  while (true) {
    chip_8.FetchOperationCode();
    chip_8.ExecuteOperationCode();

    chip_8.DebugDisplay();
  }

  return 0;
}