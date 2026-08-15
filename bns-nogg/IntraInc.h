#pragma once

constexpr auto OPCODE_JMP = 0xE9;
constexpr auto OPCODE_CALL = 0xE8;

void Install_CorsairPatches();
void Install_ReworkPatches();
