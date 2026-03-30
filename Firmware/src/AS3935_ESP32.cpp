// AS3935 Lightning Detector - ESP32 optimized library (I2C only)
// Based on AMS AS3935 datasheet v2

#include "AS3935_ESP32.h"
#include "driver/pcnt.h"

#define PCNT_UNIT PCNT_UNIT_0

AS3935_ESP32::AS3935_ESP32(uint8_t address) : _address(address), _irqPin(0), _wire(nullptr) {}

bool AS3935_ESP32::begin(uint8_t irqPin, TwoWire &wirePort) {
  _wire = &wirePort;
  _irqPin = irqPin;

  // Datasheet: 2ms LCO startup + 2ms RC oscillator startup
  delay(4);

  _wire->beginTransmission(_address);
  return (_wire->endTransmission() == 0);
}

void AS3935_ESP32::resetSettings() {
  _writeReg(REG_RESET, 0x00, DIRECT_COMMAND, 0);
  delay(2);
}

// --- AFE configuratie ---

void AS3935_ESP32::setIndoorOutdoor(uint8_t setting) {
  if (setting != AS3935_INDOOR && setting != AS3935_OUTDOOR) return;
  _writeReg(REG_AFE_GAIN, 0xC1, setting, 1);  // bits [5:1]
}

uint8_t AS3935_ESP32::readIndoorOutdoor() {
  return (_readReg(REG_AFE_GAIN) & 0x3E) >> 1;
}

// --- Gevoeligheid ---

void AS3935_ESP32::setNoiseLevel(uint8_t level) {
  if (level > 7) return;
  _writeReg(REG_THRESHOLD, 0x8F, level, 4);    // bits [6:4]
}

uint8_t AS3935_ESP32::readNoiseLevel() {
  return (_readReg(REG_THRESHOLD) & 0x70) >> 4;
}

void AS3935_ESP32::setWatchdogThreshold(uint8_t threshold) {
  if (threshold > 10) return;
  _writeReg(REG_THRESHOLD, 0xF0, threshold, 0); // bits [3:0]
}

uint8_t AS3935_ESP32::readWatchdogThreshold() {
  return _readReg(REG_THRESHOLD) & 0x0F;
}

void AS3935_ESP32::setSpikeRejection(uint8_t rejection) {
  if (rejection > 15) return;
  _writeReg(REG_LIGHTNING, 0xF0, rejection, 0); // bits [3:0]
}

uint8_t AS3935_ESP32::readSpikeRejection() {
  return _readReg(REG_LIGHTNING) & 0x0F;
}

// --- Lightning threshold ---

void AS3935_ESP32::setLightningThreshold(uint8_t strikes) {
  uint8_t bits;
  if (strikes == 1) bits = 0;
  else if (strikes == 5) bits = 1;
  else if (strikes == 9) bits = 2;
  else if (strikes == 16) bits = 3;
  else return;
  _writeReg(REG_LIGHTNING, 0xCF, bits, 4);      // bits [5:4]
}

uint8_t AS3935_ESP32::readLightningThreshold() {
  uint8_t val = (_readReg(REG_LIGHTNING) & 0x30) >> 4;
  const uint8_t map[] = {1, 5, 9, 16};
  return (val < 4) ? map[val] : val;
}

void AS3935_ESP32::clearStatistics() {
  _writeReg(REG_LIGHTNING, 0xBF, 1, 6);         // bit [6] HIGH
  _writeReg(REG_LIGHTNING, 0xBF, 0, 6);         // bit [6] LOW
  _writeReg(REG_LIGHTNING, 0xBF, 1, 6);         // bit [6] HIGH
}

// --- Interrupt ---

uint8_t AS3935_ESP32::readInterruptReg() {
  delay(2); // Datasheet: 2ms na IRQ HIGH voordat register gelezen mag worden
  return _readReg(REG_INT_MASK_ANT) & 0x0F;
}

void AS3935_ESP32::setMaskDisturber(bool mask) {
  _writeReg(REG_INT_MASK_ANT, 0xDF, mask ? 1 : 0, 5); // bit [5]
}

uint8_t AS3935_ESP32::readMaskDisturber() {
  return (_readReg(REG_INT_MASK_ANT) & 0x20) >> 5;
}

// --- Detectie data ---

uint8_t AS3935_ESP32::distanceToStorm() {
  return _readReg(REG_DISTANCE) & 0x3F;
}

uint32_t AS3935_ESP32::lightningEnergy() {
  uint32_t energy = _readReg(REG_ENERGY_MMSB) & 0x1F;
  energy <<= 8;
  energy |= _readReg(REG_ENERGY_MSB);
  energy <<= 8;
  energy |= _readReg(REG_ENERGY_LSB);
  return energy;
}

// --- Antenna tuning met ESP32 PCNT ---

AS3935_TuneResult AS3935_ESP32::tuneAntenna() {
  AS3935_TuneResult result = {0, 0, 0, false};

  // Stap 1: Divisieratio instellen op 16
  _writeReg(REG_INT_MASK_ANT, 0x3F, 0, 6);     // bits [7:6] = 00 → ratio 16

  // Stap 2: LCO op IRQ pin zetten
  _writeReg(REG_FREQ_DISP_IRQ, 0x1F, 1, 7);    // DISP_LCO = 1 (bit 7)
  delay(2);

  float closestDiff = 1000.0;
  int bestCap = 0;

  Serial.println("Antenne tuning gestart (doel: 500.0 kHz):");

  for (int i = 0; i < 16; i++) {
    // Schrijf TUN_CAP direct, behoud DISP_LCO bit
    uint8_t reg = _readReg(REG_FREQ_DISP_IRQ);
    reg = (reg & 0xF0) | (i & 0x0F);            // Behoud bits [7:4], schrijf bits [3:0]
    _wire->beginTransmission(_address);
    _wire->write(REG_FREQ_DISP_IRQ);
    _wire->write(reg);
    _wire->endTransmission();

    delay(50); // Wacht op oscillator stabilisatie

    float freq = _measureFrequencyPCNT();
    if (freq == 0) {
      Serial.println("  PCNT meting mislukt!");
      _writeReg(REG_FREQ_DISP_IRQ, 0x1F, 0, 7); // DISP_LCO uit
      return result;
    }

    float diff = abs(500.0 - freq);

    Serial.print("  Cap ");
    if (i * 8 < 100) Serial.print(" ");
    if (i * 8 < 10) Serial.print(" ");
    Serial.print(i * 8);
    Serial.print(" pF: ");
    Serial.print(freq, 1);
    Serial.print(" kHz (afwijking: ");
    Serial.print(diff, 1);
    Serial.println(" kHz)");

    if (diff < closestDiff) {
      closestDiff = diff;
      bestCap = i;
    }
  }

  // Stap 3: Beste condensator instellen
  uint8_t reg = _readReg(REG_FREQ_DISP_IRQ);
  reg = (reg & 0xF0) | (bestCap & 0x0F);
  _wire->beginTransmission(_address);
  _wire->write(REG_FREQ_DISP_IRQ);
  _wire->write(reg);
  _wire->endTransmission();
  delay(50);

  // Meet finale frequentie
  float finalFreq = _measureFrequencyPCNT();

  // Stap 4: DISP_LCO uitzetten VOOR readTuneCap
  _writeReg(REG_FREQ_DISP_IRQ, 0x1F, 0, 7);    // DISP_LCO = 0

  Serial.print("Gekozen condensator: ");
  Serial.print(bestCap * 8);
  Serial.print(" pF, frequentie: ");
  Serial.print(finalFreq, 1);
  Serial.print(" kHz (afwijking: ");
  Serial.print(closestDiff, 1);
  Serial.println(" kHz)");

  // Stap 5: RCO calibratie (datasheet: MOET na antenna tuning)
  bool oscOk = calibrateOscillators();

  result.capPF = bestCap * 8;
  result.frequency = finalFreq;
  result.deviation = closestDiff;
  result.success = oscOk;

  return result;
}

float AS3935_ESP32::_measureFrequencyPCNT() {
  // Configureer PCNT hardware pulse counter
  pcnt_config_t pcnt_config = {};
  pcnt_config.pulse_gpio_num = _irqPin;
  pcnt_config.ctrl_gpio_num = PCNT_PIN_NOT_USED;
  pcnt_config.channel = PCNT_CHANNEL_0;
  pcnt_config.unit = PCNT_UNIT;
  pcnt_config.pos_mode = PCNT_COUNT_INC;         // Tel op rising edge
  pcnt_config.neg_mode = PCNT_COUNT_DIS;         // Negeer falling edge
  pcnt_config.lctrl_mode = PCNT_MODE_KEEP;
  pcnt_config.hctrl_mode = PCNT_MODE_KEEP;
  pcnt_config.counter_h_lim = 32767;
  pcnt_config.counter_l_lim = 0;

  pcnt_unit_config(&pcnt_config);

  // Filter: negeer pulsen korter dan 1 µs (ruis)
  pcnt_set_filter_value(PCNT_UNIT, 80);          // 80 APB klokpulsen = 1 µs bij 80 MHz
  pcnt_filter_enable(PCNT_UNIT);

  pcnt_counter_pause(PCNT_UNIT);
  pcnt_counter_clear(PCNT_UNIT);
  pcnt_counter_resume(PCNT_UNIT);

  delay(100); // Tel pulsen gedurende 100ms

  pcnt_counter_pause(PCNT_UNIT);

  int16_t count = 0;
  pcnt_get_counter_value(PCNT_UNIT, &count);

  // Opruimen
  pcnt_counter_clear(PCNT_UNIT);

  if (count <= 0) return 0;

  // Frequentie berekenen: count pulsen in 100ms, divisieratio = 16
  // IRQ freq = count / 0.1s → LCO freq = IRQ_freq * 16
  return (float)count * 0.16;  // count / 100ms * 16 / 1000 = count * 0.16 kHz
}

// --- Handmatige condensator instelling ---

void AS3935_ESP32::setTuneCap(uint8_t pF) {
  if (pF > 120 || pF % 8 != 0) return;
  _writeReg(REG_FREQ_DISP_IRQ, 0xF0, pF / 8, 0); // bits [3:0]
}

uint8_t AS3935_ESP32::readTuneCap() {
  // DISP bits moeten UIT zijn voor correcte lezing (SparkFun bug fix)
  return (_readReg(REG_FREQ_DISP_IRQ) & 0x0F) * 8;
}

// --- Oscillator calibratie ---

bool AS3935_ESP32::calibrateOscillators() {
  // Datasheet sectie 8.11: CALIB_RCO procedure
  // 1. Stuur Direct Command 0x96 naar REG0x3D
  _writeReg(REG_CALIB_RCO, 0x00, DIRECT_COMMAND, 0);

  // 2. Zet DISP_TRCO = 1 (REG0x08 bit 5)
  _writeReg(REG_FREQ_DISP_IRQ, 0x1F, 1, 5);

  // 3. Wacht 2ms
  delay(2);

  // 4. Zet DISP_TRCO = 0
  _writeReg(REG_FREQ_DISP_IRQ, 0x1F, 0, 5);

  // 5. Verifieer calibratie
  //    REG0x3A bit[7] = TRCO_CALIB_DONE (moet 1 zijn)
  //    REG0x3A bit[6] = TRCO_CALIB_NOK  (moet 0 zijn)
  //    REG0x3B bit[7] = SRCO_CALIB_DONE (moet 1 zijn)
  //    REG0x3B bit[6] = SRCO_CALIB_NOK  (moet 0 zijn)
  uint8_t trco = _readReg(REG_CALIB_TRCO);
  uint8_t srco = _readReg(REG_CALIB_SRCO);

  bool trcoOk = (trco & 0x80) && !(trco & 0x40);  // DONE=1 en NOK=0
  bool srcoOk = (srco & 0x80) && !(srco & 0x40);  // DONE=1 en NOK=0

  return trcoOk && srcoOk;
}

// --- Power management ---

void AS3935_ESP32::powerDown() {
  _writeReg(REG_AFE_GAIN, 0xFE, 1, 0);          // bit [0] = 1
}

bool AS3935_ESP32::wakeUp() {
  _writeReg(REG_AFE_GAIN, 0xFE, 0, 0);          // bit [0] = 0
  return calibrateOscillators();
}

// --- I2C register operaties ---

uint8_t AS3935_ESP32::_readReg(uint8_t reg) {
  _wire->beginTransmission(_address);
  _wire->write(reg);
  _wire->endTransmission(false);                  // Restart, niet stoppen
  _wire->requestFrom(_address, (uint8_t)1);
  return _wire->read();
}

void AS3935_ESP32::_writeReg(uint8_t reg, uint8_t mask, uint8_t value, uint8_t startBit) {
  uint8_t current = _readReg(reg);
  current &= mask;                                // Bewaar gemaskeerde bits
  current |= (value << startBit);                 // Schrijf nieuwe bits
  _wire->beginTransmission(_address);
  _wire->write(reg);
  _wire->write(current);
  _wire->endTransmission();
}
