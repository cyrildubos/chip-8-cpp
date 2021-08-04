#include "chip_8.h"

uint16_t Chip8::GetNNN() { return oc & 0x0FFF; }
uint8_t Chip8::GetN() { return oc & 0x0F; }
uint8_t Chip8::GetX() { return (oc & 0x0F00) >> 8; }
uint8_t Chip8::GetY() { return (oc & 0x00F0) >> 4; }
uint8_t Chip8::GetKK() { return oc & 0x00FF; }

uint8_t Chip8::GetRandomByte() {
  srand(time(NULL));

  return rand() % 0x100;
}

void Chip8::DebugMemory() {
  for (int i = 0; i < MEMORY_SIZE; i++) std::cout << std::hex << +m[i] << " ";

  std::cout << std::endl;
}

void Chip8::ClearMemory() {
  for (int i = 0; i < MEMORY_SIZE; i++) m[i] = 0x0;
}

void Chip8::LoadSprites() {
  uint8_t s[] = {
      0xF0, 0x90, 0x90, 0x90,
      0xF0,  // 0
      0x20, 0x60, 0x20, 0x20,
      0x70,  // 1
      0xF0, 0x10, 0xF0, 0x80,
      0xF0,  // 2
      0xF0, 0x10, 0xF0, 0x10,
      0xF0,  // 3
      0x90, 0x90, 0xF0, 0x10,
      0x10,  // 4
      0xF0, 0x80, 0xF0, 0x10,
      0xF0,  // 5
      0xF0, 0x80, 0xF0, 0x90,
      0xF0,  // 6
      0xF0, 0x10, 0x20, 0x40,
      0x40,  // 7
      0xF0, 0x90, 0xF0, 0x90,
      0xF0,  // 8
      0xF0, 0x90, 0xF0, 0x10,
      0xF0,  // 9
      0xF0, 0x90, 0xF0, 0x90,
      0x90,  // A
      0xE0, 0x90, 0xE0, 0x90,
      0xE0,  // B
      0xF0, 0x80, 0x80, 0x80,
      0xF0,  // C
      0xE0, 0x90, 0x90, 0x90,
      0xE0,  // D
      0xF0, 0x80, 0xF0, 0x80,
      0xF0,  // E
      0xE0, 0x80, 0xF0, 0x80,
      0x80,  // F
  };

  for (int i = 0; i < SRPITE_COUNT * SRPITE_HEIGHT; i++) m[i] = s[i];
}

void Chip8::LoadROM(std::string path) {
  std::ifstream stream(path, std::ifstream::binary);

  if (stream.is_open()) {
    for (int i = 0x0; i <= std::filesystem::file_size(path); i++)
      stream >> std::noskipws >> m[PROGRAM_LOCATION + i];

    stream.close();
  }
}

void Chip8::DebugRegisters() {
  for (int i = 0x0; i < REGISTERS_SIZE; i++)
    std::cout << std::setw(0x2) << std::setfill('0') << std::hex << +v[i]
              << " ";

  std::cout << std::endl;
}

void Chip8::DebugDisplay() {
  for (int i = 0x0; i < DISPLAY_HEIGHT; i++) {
    for (int j = 0x0; j < DISPLAY_WIDTH; j++)
      std::cout << ((d[i * DISPLAY_WIDTH + j] == 0x0) ? " " : "█");

    std::cout << std::endl;
  }
}

void Chip8::ClearDisplay() {
  for (int i = 0; i < DISPLAY_HEIGHT; i++)
    for (int j = 0; j < DISPLAY_WIDTH; j++) d[i * DISPLAY_WIDTH + j] = 0x0;
}

void Chip8::DrawByte(uint8_t b, uint8_t x, uint8_t y) {
  for (int i = 0x1; i <= 0x8; i++)
    d[y * DISPLAY_WIDTH + (x + i) % DISPLAY_WIDTH] ^= (b >> 0x8 - i) & 0x1;
}

void Chip8::DebugOperationCode() {
  std::cout << std::setw(0x4) << std::setfill('0') << std::hex << oc
            << std::endl;
}

void Chip8::FetchOperationCode() { oc = m[pc++] << 0x8 | m[pc++]; }

void Chip8::ExecuteOperationCode() {
  switch (oc & 0xF000) {
    case 0x0000:
      switch (oc & 0xFFFF) {
        case 0x00E0:  // 00E0
          ClearDisplay();
          break;
        case 0x00EE:  // 00EE
          pc = s[sp--];
          break;
      }
      break;
    case 0x1000:  // 1nnn
      pc = GetNNN();
      break;
    case 0x2000:  // 2nnn
      s[++sp] = pc;
      pc = GetNNN();
      break;
    case 0x3000:  // 3xkk
      if (v[GetX()] == GetKK()) pc += 2;
      break;
    case 0x4000:  // 4xkk
      if (v[GetX()] != GetKK()) pc += 2;
      break;
    case 0x5000:  // 5xkk
      if (v[GetX()] == v[GetY()]) pc += 2;
      break;
    case 0x6000:  // 6xkk
      v[GetX()] = GetKK();
      break;
    case 0x7000:  // 7xkk
      v[GetX()] += GetKK();
      break;
    case 0x8000:
      switch (oc & 0xF00F) {
        case 0x8000:  // 8xy0
          v[GetX()] = v[GetY()];
          break;
        case 0x8001:  // 8xy1
          v[GetX()] |= v[GetY()];
          break;
        case 0x8002:  // 8xy2
          v[GetX()] &= v[GetY()];
          break;
        case 0x8003:  // 8xy3
          v[GetX()] ^= v[GetY()];
          break;
        case 0x8004:  // 8xy4
          v[0xF] = v[GetX()] + v[GetY()] > 0xFF ? 0x1 : 0x0;
          v[GetX()] += v[GetY()];
          break;
        case 0x8005:  // 8xy5
          v[0xF] = v[GetX()] > v[GetY()] ? 0x1 : 0x0;
          v[GetX()] -= v[GetY()];
          break;
        case 0x8006:  // 8xy6
          v[0xF] = v[GetX()] & 0b00000001;
          v[GetX()] >>= 1;
          break;
        case 0x8007:  // 8xy7
          v[0xF] = v[GetY()] > v[GetX()] ? 0x1 : 0x0;
          v[GetX()] = v[GetY()] - v[GetX()];
          break;
        case 0x800E:  // 8xyE
          v[0xF] = v[GetX()] & 0b10000000;
          v[GetX()] <<= 1;
          break;
      }
      break;
    case 0x9000:
      switch (oc & 0xF00F) {
        case 0x9000:  // 9xy0
          if (v[GetX()] != v[GetY()]) pc += 2;
          break;
      }
      break;
    case 0xA000:  // Annn
      i = GetNNN();
      break;
    case 0xB000:  // Bnnn
      pc = GetNNN() + v[0x0];
      break;
    case 0xC000:  // Cxkk
      v[GetX()] = GetRandomByte() & GetKK();
      break;
    case 0xD000:  // Dxyn
      for (int j = 0x0; j < GetN(); j++)
        DrawByte(m[i + j], v[GetX()], v[GetY()] + j);
      break;
    case 0xE000:
      switch (oc & 0xF0FF) {
        case 0xE09E:  // Ex9E
          if (k[v[GetX()]] != 0x0) pc += 2;
          break;
        case 0xE0A1:  // ExA1
          if (k[v[GetX()]] == 0x0) pc += 2;
          break;
      }
      break;
    case 0xF000:
      switch (oc & 0xF0FF) {
        case 0xF007:  // Fx07
          v[GetX()] = dt;
          break;
        case 0xF00A:  // Fx0A
          std::cin.ignore();
          /* code */
          break;
        case 0xF015:  // Fx15
          dt = v[GetX()];
          break;
        case 0xF018:  // Fx18
          st = v[GetX()];
          break;
        case 0xF01E:  // Fx1E
          i += v[GetX()];
          break;
        case 0xF029:  // Fx29
          i = 0x5 * v[GetX()];
          break;
        case 0xF033:  // Fx33
          m[i] = v[GetX()] / 100;
          m[i + 1] = v[GetX()] / 10 % 10;
          m[i + 2] = v[GetX()] % 10;
          break;
        case 0xF055:  // Fx55
          for (int j = 0x0; j <= GetX(); j++) m[i + j] = v[j];
          break;
        case 0xF065:  // Fx65
          for (int j = 0x0; j <= GetX(); j++) v[j] = m[i + j];
          break;
      }
      break;
  }
}
