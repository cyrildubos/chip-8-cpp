#ifndef CHIP_8_H_
#define CHIP_8_H_

#include <stdint.h>

#include <filesystem>
#include <fstream>
#include <iostream>

#define MEMORY_SIZE 0x1000
#define SRPITE_COUNT 0xF
#define SRPITE_HEIGHT 0x5
#define REGISTERS_SIZE 0xF
#define STACK_SIZE 0x1000
#define PROGRAM_LOCATION 0x200
#define DISPLAY_WIDTH 0x40
#define DISPLAY_HEIGHT 0x20
#define KEYPAD_SIZE 0xF

class Chip8 {
 private:
  // memory
  uint8_t m[MEMORY_SIZE];

  // registers
  uint8_t v[REGISTERS_SIZE];
  uint16_t i = 0x0;

  // delay & sound timer registers
  uint8_t dt = 0x0;
  uint8_t st = 0x0;

  // stack & stack pointer
  uint16_t s[STACK_SIZE];
  uint16_t sp = 0x0;

  // program counter
  uint16_t pc = PROGRAM_LOCATION;

  // operation code
  uint16_t oc = 0x0;

  // display
  uint8_t d[DISPLAY_WIDTH * DISPLAY_HEIGHT];

  // keypad
  uint8_t k[KEYPAD_SIZE];

  uint16_t GetNNN();
  uint8_t GetN();
  uint8_t GetX();
  uint8_t GetY();
  uint8_t GetKK();

  uint8_t GetRandomByte();

 public:
  void DebugMemory();
  void ClearMemory();

  void LoadSprites();
  void LoadROM(std::string);

  void DebugRegisters();

  void DebugDisplay();
  void ClearDisplay();

  void DrawByte(uint8_t, uint8_t, uint8_t);

  void DebugOperationCode();
  void FetchOperationCode();
  void ExecuteOperationCode();
};

#endif