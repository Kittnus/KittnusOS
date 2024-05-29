#include "GDT.h"

// TODO: Initialize in GDTLoader instead
ALIGN GDT g_GDT = {
  // LimitLow, BaseLow, BaseMiddle, Access, Granularity, BaseHigh
  { 0, 0, 0, 0x00, 0x00, 0 }, // Null

  { 0, 0, 0, 0b10011010, 0b10100000, 0 }, // Ring0Code
  { 0, 0, 0, 0b10010010, 0b10100000, 0 }, // Ring0Data

  { 0, 0, 0, 0x00, 0x00, 0 }, // Ring3Null
  { 0, 0, 0, 0b11111010, 0b10100000, 0 }, // Ring3Code
  { 0, 0, 0, 0b11110010, 0b10100000, 0 }, // Ring3Data

  { 0, 0, 0, 0b10001001, 0b00100000, 0 }, // TSSLow
  { 0, 0, 0, 0x00, 0x00, 0 }, // TSSHigh
};
