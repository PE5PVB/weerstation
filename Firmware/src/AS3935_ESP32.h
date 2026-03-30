// AS3935 Lightning Detector - ESP32 optimized library (I2C only)
// Based on AMS AS3935 datasheet v2
// Uses ESP32 PCNT hardware pulse counter for accurate antenna tuning

#ifndef AS3935_ESP32_H
#define AS3935_ESP32_H

#include <Arduino.h>
#include <Wire.h>

// I2C addresses (active-high address pins)
#define AS3935_ADDR_DEFAULT 0x03  // ADD0=HIGH, ADD1=HIGH
#define AS3935_ADDR_01      0x01  // ADD0=HIGH, ADD1=LOW
#define AS3935_ADDR_10      0x02  // ADD0=LOW,  ADD1=HIGH

// Indoor/outdoor AFE gain setting
#define AS3935_INDOOR  0x12
#define AS3935_OUTDOOR 0x0E

// Interrupt types (REG0x03 bits [3:0])
#define AS3935_INT_NOISE     0x01
#define AS3935_INT_DISTURBER 0x04
#define AS3935_INT_LIGHTNING 0x08

struct AS3935_TuneResult {
  uint8_t  capPF;           // Gekozen condensator in pF
  float    frequency;       // Gemeten frequentie in kHz
  float    deviation;       // Afwijking van 500 kHz
  bool     success;         // Calibratie geslaagd
};

class AS3935_ESP32 {
public:
  AS3935_ESP32(uint8_t address = AS3935_ADDR_DEFAULT);

  // Initialisatie
  bool     begin(uint8_t irqPin, TwoWire &wirePort = Wire);
  void     resetSettings();

  // AFE configuratie
  void     setIndoorOutdoor(uint8_t setting);
  uint8_t  readIndoorOutdoor();

  // Gevoeligheid
  void     setNoiseLevel(uint8_t level);
  uint8_t  readNoiseLevel();
  void     setWatchdogThreshold(uint8_t threshold);
  uint8_t  readWatchdogThreshold();
  void     setSpikeRejection(uint8_t rejection);
  uint8_t  readSpikeRejection();

  // Lightning threshold (1, 5, 9, of 16 inslagen binnen 15 min)
  void     setLightningThreshold(uint8_t strikes);
  uint8_t  readLightningThreshold();
  void     clearStatistics();

  // Interrupt
  uint8_t  readInterruptReg();
  void     setMaskDisturber(bool mask);
  uint8_t  readMaskDisturber();

  // Detectie data
  uint8_t  distanceToStorm();
  uint32_t lightningEnergy();

  // Antenna tuning met ESP32 PCNT hardware
  AS3935_TuneResult tuneAntenna();

  // Handmatige condensator instelling (0-120 pF, stappen van 8)
  void     setTuneCap(uint8_t pF);
  uint8_t  readTuneCap();

  // Oscillator calibratie (datasheet vereist: na tuning en na power-on)
  bool     calibrateOscillators();

  // Power management
  void     powerDown();
  bool     wakeUp();

private:
  uint8_t  _address;
  uint8_t  _irqPin;
  TwoWire *_wire;

  // Register lezen/schrijven
  uint8_t  _readReg(uint8_t reg);
  void     _writeReg(uint8_t reg, uint8_t mask, uint8_t value, uint8_t startBit);

  // PCNT frequentiemeting
  float    _measureFrequencyPCNT();

  // Register adressen
  enum Register : uint8_t {
    REG_AFE_GAIN      = 0x00,
    REG_THRESHOLD      = 0x01,
    REG_LIGHTNING      = 0x02,
    REG_INT_MASK_ANT   = 0x03,
    REG_ENERGY_LSB     = 0x04,
    REG_ENERGY_MSB     = 0x05,
    REG_ENERGY_MMSB    = 0x06,
    REG_DISTANCE       = 0x07,
    REG_FREQ_DISP_IRQ  = 0x08,
    REG_CALIB_TRCO     = 0x3A,
    REG_CALIB_SRCO     = 0x3B,
    REG_RESET          = 0x3C,
    REG_CALIB_RCO      = 0x3D
  };

  static const uint8_t DIRECT_COMMAND = 0x96;
};

#endif
