#include "Seesaw.h"

struct i2cip_rotaryencoder_s _i2cip_rotaryencoder_default = { .button = PIN_UNDEF, .encoder = 0x00000000 };

// #define ROTARYENCODER_DEFAULT_CACHE ({ .button = PIN_UNDEF, .encoder = 0 })

I2CIP_DEVICE_INIT_STATIC_ID(_Seesaw, I2CIP_SEESAW_ID);
I2CIP_INPUT_INIT_RESET(Seesaw_RotaryEncoder, i2cip_rotaryencoder_t, _i2cip_rotaryencoder_default, void*, nullptr);
I2CIP_OUTPUT_INIT_FAILSAFE(Seesaw_RotaryEncoder, void*, nullptr, void*, nullptr); // Outputs are NOP (Return software error)


i2cip_errorlevel_t _Seesaw::getEncoderPosition(const i2cip_fqa_t& fqa, int32_t& dest, uint8_t encoder, bool setbus, bool resetbus)
{
  // this->read(SEESAW_ENCODER_BASE, SEESAW_ENCODER_POSITION + encoder, buf, 4);
  
  size_t len = 4; uint8_t buf[len] = {0};
  i2cip_errorlevel_t errlev = Device::readRegister(fqa, (uint16_t)(((uint16_t)SEESAW_ENCODER_BASE << 8) | (SEESAW_ENCODER_POSITION + encoder)), buf, len, false, resetbus, setbus); // No null terminate!
  I2CIP_ERR_BREAK(errlev);
  if(len != 4) { return I2CIP::I2CIP_ERR_SOFT; }
  dest = (int32_t)(((uint32_t)buf[0] << 24) | ((uint32_t)buf[1] << 16) | ((uint32_t)buf[2] << 8) | (uint32_t)buf[3]);
  Serial.print("Seesaw Encoder: "); Serial.println(dest, DEC);

  return errlev;
}

i2cip_errorlevel_t _Seesaw::setEncoderInterrupt(const i2cip_fqa_t& fqa, uint8_t encoder, bool enabled, bool setbus) {
  return Device::writeRegister(fqa, (uint16_t)(((uint16_t)SEESAW_ENCODER_BASE << 8) | (SEESAW_ENCODER_INTENSET + encoder)), (enabled ? (uint8_t)0x01 : (uint8_t)0x00), setbus);
}

// i2cip_errorlevel_t _Seesaw::getGPIO(uint64_t& dest, bool setbus, bool resetbus) {
i2cip_errorlevel_t _Seesaw::getGPIO(const i2cip_fqa_t& fqa, uint32_t& dest, bool setbus, bool resetbus) {
  // uint8_t buf[8] = {0}; size_t len = 8;
  uint8_t buf[4] = {0}; size_t len = 4;
  i2cip_errorlevel_t errlev = Device::readRegister(fqa, (uint16_t)(((uint16_t)SEESAW_GPIO_BASE << 8) | SEESAW_GPIO_BULK), buf, len, false, resetbus, setbus); // No null terminate!
  I2CIP_ERR_BREAK(errlev);
  // if(len != 8) { return I2CIP::I2CIP_ERR_SOFT; }
  if(len != 4) { return I2CIP::I2CIP_ERR_SOFT; }
  for(uint8_t i = 0; i < len; i++) {
    dest <<= 8;
    dest |= buf[i];
  }
  return errlev;
}

i2cip_errorlevel_t _Seesaw::getGPIOPin(const i2cip_fqa_t& fqa, i2cip_state_pin_t& dest, uint8_t pin, bool setbus, bool resetbus) {
  // uint32_t maska = (1ul << pin); uint32_t maskb = (1ul << (pin - 32));
  // uint64_t mask = ((uint64_t)1 << pin);
  uint32_t mask = ((uint32_t)1 << pin);
  // uint64_t gpio = 0;
  uint32_t gpio = 0;
  // Serial.print("Seesaw GPIO Pin: "); Serial.print(pin); Serial.print(", Mask: 0b"); Serial.println(mask, BIN);
  i2cip_errorlevel_t errlev = _Seesaw::getGPIO(fqa, gpio, setbus, resetbus);
  I2CIP_ERR_BREAK(errlev);
  Serial.print("Seesaw GPIO: 0b"); Serial.print(gpio >> (pin+1), BIN); Serial.print('['); Serial.print((gpio & mask) ? 1 : 0); Serial.print(']'); Serial.print(gpio & ~(~0 >> (32 - pin)), BIN);
  dest = (gpio & mask) ? PIN_ON : PIN_OFF;
  return errlev;
}

i2cip_errorlevel_t _Seesaw::setGPIOInterrupts(const i2cip_fqa_t& fqa, uint32_t mask, bool enabled, bool setbus) {
  // uint8_t cmd[4] = {(uint8_t)mask, (uint8_t)(mask >> 8), (uint8_t)(mask >> 16), (uint8_t)(mask >> 24)};
  uint8_t cmd[4] = {(uint8_t)(mask >> 24), (uint8_t)(mask >> 16), (uint8_t)(mask >> 8), (uint8_t)mask}; // uint32_t MSB
  size_t len = 4;

  // return write(SEESAW_GPIO_BASE, enabled ? SEESAW_GPIO_INTENSET : SEESAW_GPIO_INTENCLR, cmd, 4);
  i2cip_errorlevel_t errlev = Device::writeRegister(fqa, (uint16_t)(((uint16_t)SEESAW_GPIO_BASE << 8) | (enabled ? SEESAW_GPIO_INTENSET : SEESAW_GPIO_INTENCLR)), cmd, len, setbus);
  I2CIP_ERR_BREAK(errlev);
  if(len != 4) { return I2CIP::I2CIP_ERR_SOFT; }
  return errlev;
}

// template <typename G, typename A, typename S, typename B> void Seesaw<G, A, S, B>::loadID() {
//   uint8_t idlen = strlen_P(i2cip_seesaw_id_progmem);

//   // Read in PROGMEM
//   for (uint8_t k = 0; k < idlen; k++) {
//     char c = pgm_read_byte_near(i2cip_seesaw_id_progmem + k);
//     Seesaw::_id[k] = c;
//   }

//   Seesaw::_id[idlen] = '\0';
//   Seesaw::_id_set = true;

//   #ifdef I2CIP_DEBUG_SERIAL
//     DEBUG_DELAY();
//     I2CIP_DEBUG_SERIAL.print(F("Seesaw ID Loaded: '"));
//     I2CIP_DEBUG_SERIAL.print(Seesaw::_id);
//     I2CIP_DEBUG_SERIAL.print(F("' @"));
//     I2CIP_DEBUG_SERIAL.println((uintptr_t)(&Seesaw::_id[0]), HEX);
//     DEBUG_DELAY();
//   #endif
// }

i2cip_errorlevel_t _Seesaw::setGPIOPinMode(const i2cip_fqa_t& fqa, uint8_t pin, uint8_t mode, bool setbus) {
  if(pin > 31) { return I2CIP::I2CIP_ERR_SOFT; } 
  i2cip_errorlevel_t errlev;
  // uint32_t pinsa = (pin >= 32 ? 0 : (uint32_t)1 << pin);
  // uint32_t pinsb = (pin >= 32 ? (uint32_t)1 << (pin - 32) : 0);
  uint32_t pins = (pin >= 32 ? 0 : (uint32_t)1 << pin);
  // uint8_t cmd[8] = {(uint8_t)(pinsa), (uint8_t)(pinsa >> 8), (uint8_t)(pinsa >> 16), (uint8_t)(pinsa >> 24), (uint8_t)(pinsb), (uint8_t)(pinsb >> 8), (uint8_t)(pinsb >> 16), (uint8_t)(pinsb >> 24)};
  // uint8_t cmd[8] = {(uint8_t)(pinsa >> 24), (uint8_t)(pinsa >> 16), (uint8_t)(pinsa >> 8), (uint8_t)(pinsa), (uint8_t)(pinsb >> 24), (uint8_t)(pinsb >> 16), (uint8_t)(pinsb >> 8), (uint8_t)(pinsb)}; // uint32_t/uint64_t MSB
  uint8_t cmd[4] = {(uint8_t)(pins >> 24), (uint8_t)(pins >> 16), (uint8_t)(pins >> 8), (uint8_t)pins}; // uint32_t MSB
  // size_t len = (pin >= 32) ? 8 : 4; // 2 bytes for register address
  size_t len = 4; // 2 bytes for register address
  switch (mode) {
    case OUTPUT:
      // errlev = this->Device::writeRegister(fqa, (((uint16_t)SEESAW_GPIO_BASE << 8) | (uint16_t)SEESAW_GPIO_DIRSET_BULK), (uint8_t*)cmd, len, setbus);
      errlev = Device::writeRegister(fqa, (uint16_t)(((uint16_t)SEESAW_GPIO_BASE << 8) | SEESAW_GPIO_DIRSET_BULK), (uint8_t*)cmd, len, setbus);
      break;
    case INPUT:
      // errlev = Device::writeRegister(fqa, (((uint16_t)SEESAW_GPIO_BASE << 8) | (uint16_t)SEESAW_GPIO_DIRCLR_BULK), (uint8_t*)cmd, len, setbus);
      errlev = Device::writeRegister(fqa, (uint16_t)(((uint16_t)SEESAW_GPIO_BASE << 8) | SEESAW_GPIO_DIRCLR_BULK), (uint8_t*)cmd, len, setbus);
      break;
    case INPUT_PULLUP:
      // errlev = Device::writeRegister(fqa, (((uint16_t)SEESAW_GPIO_BASE << 8) | (uint16_t)SEESAW_GPIO_DIRCLR_BULK), (uint8_t*)cmd, len, setbus);
      errlev = Device::writeRegister(fqa, (uint16_t)(((uint16_t)SEESAW_GPIO_BASE << 8) | SEESAW_GPIO_DIRCLR_BULK), (uint8_t*)cmd, len, setbus);
      I2CIP_ERR_BREAK(errlev);
      // errlev = Device::writeRegister(fqa, (((uint16_t)SEESAW_GPIO_BASE << 8) | (uint16_t)SEESAW_GPIO_PULLENSET), (uint8_t*)cmd, len, setbus);
      errlev = Device::writeRegister(fqa, (uint16_t)(((uint16_t)SEESAW_GPIO_BASE << 8) | SEESAW_GPIO_PULLENSET), (uint8_t*)cmd, len, setbus);
      I2CIP_ERR_BREAK(errlev);
      // errlev = Device::writeRegister(fqa, (((uint16_t)SEESAW_GPIO_BASE << 8) | (uint16_t)SEESAW_GPIO_BULK_SET), (uint8_t*)cmd, len, setbus);
      errlev = Device::writeRegister(fqa, (uint16_t)(((uint16_t)SEESAW_GPIO_BASE << 8) | SEESAW_GPIO_BULK_SET), (uint8_t*)cmd, len, setbus);
      break;
    case INPUT_PULLDOWN:
      // errlev = Device::writeRegister(fqa, (((uint16_t)SEESAW_GPIO_BASE << 8) | (uint16_t)SEESAW_GPIO_DIRCLR_BULK), (uint8_t*)cmd, len, setbus);
      errlev = Device::writeRegister(fqa, (uint16_t)(((uint16_t)SEESAW_GPIO_BASE << 8) | SEESAW_GPIO_DIRCLR_BULK), (uint8_t*)cmd, len, setbus);
      I2CIP_ERR_BREAK(errlev);
      // errlev = Device::writeRegister(fqa, (((uint16_t)SEESAW_GPIO_BASE << 8) | (uint16_t)SEESAW_GPIO_PULLENSET), (uint8_t*)cmd, len, setbus);
      errlev = Device::writeRegister(fqa, (uint16_t)(((uint16_t)SEESAW_GPIO_BASE << 8) | SEESAW_GPIO_PULLENSET), (uint8_t*)cmd, len, setbus);
      I2CIP_ERR_BREAK(errlev);
      // errlev = Device::writeRegister(fqa, (((uint16_t)SEESAW_GPIO_BASE << 8) | (uint16_t)SEESAW_GPIO_BULK_CLR), (uint8_t*)cmd, len, setbus);
      errlev = Device::writeRegister(fqa, (uint16_t)(((uint16_t)SEESAW_GPIO_BASE << 8) | SEESAW_GPIO_BULK_CLR), (uint8_t*)cmd, len, setbus);
      break;
    default:
      return I2CIP::I2CIP_ERR_SOFT;
  }
  if(errlev == I2CIP::I2CIP_ERR_NONE && len != ((pin >= 32) ? 8 : 4)) {
    return I2CIP::I2CIP_ERR_SOFT;
  }
  return errlev;
}

// Seesaw::Seesaw(const i2cip_fqa_t& fqa, const i2cip_id_t& id) : Device(fqa, id), InputInterface<state_seesaw_t, args_seesaw_t>((Device*)this) { }

// Seesaw::Seesaw(const i2cip_fqa_t& fqa) : Seesaw(fqa, getStaticIDBuffer()) { }

// Handles ID pointer assignment too
// NEVER returns nullptr, unless out of memory
// Device* Seesaw::seesawFactory(const i2cip_fqa_t& fqa) { return seesawFactory(fqa, getStaticIDBuffer()); }
// Device* Seesaw::seesawFactory(const i2cip_fqa_t& fqa, const i2cip_id_t& id) {
//   if(!_id_set || id == nullptr) {
//     loadID();
//     return (Device*)(new Seesaw(fqa, _id));
//   }

//   return (Device*)(new Seesaw(fqa, id == nullptr ? _id : id));
// }

i2cip_errorlevel_t Seesaw_RotaryEncoder::get(i2cip_rotaryencoder_t& value, void* const& args) {
  // i2cip_errorlevel_t errlev = pingTimeout(true, false);
  // I2CIP_ERR_BREAK(errlev);
  i2cip_errorlevel_t errlev = I2CIP::I2CIP_ERR_NONE;
  if(!this->initialized) {
    errlev = _Seesaw::setGPIOPinMode(fqa, encoder, INPUT_PULLUP);
    I2CIP_ERR_BREAK(errlev);
    delayMicroseconds(I2CIP_SEESAW_DELAY);
    errlev = _Seesaw::setGPIOInterrupts(fqa, (uint32_t)1 << I2CIP_ROTARY_BUTTON_PIN, false); // Don't enable button interrupt - sampling UX is in frames
    I2CIP_ERR_BREAK(errlev);
    delayMicroseconds(I2CIP_SEESAW_DELAY);
    // enableEncoderInterrupt
    errlev = _Seesaw::setEncoderInterrupt(fqa, encoder, true); // Do enable encoder interrupt - sampling UX is in frames; want to catch every tick
    I2CIP_ERR_BREAK(errlev);
    // this->initialized = true;

    delayMicroseconds(I2CIP_SEESAW_DELAY);
  }
  int32_t epos = 0;
  errlev = _Seesaw::getEncoderPosition(fqa, epos, encoder);
  I2CIP_ERR_BREAK(errlev);
  value.encoder = epos;
  delayMicroseconds(I2CIP_SEESAW_DELAY);

  i2cip_state_pin_t button = PIN_UNDEF;
  errlev = _Seesaw::getGPIOPin(fqa, button, I2CIP_ROTARY_BUTTON_PIN, false, true);
  value.button = (button == PIN_ON ? PIN_OFF : (button == PIN_OFF ? PIN_ON : PIN_UNDEF)); // Inverted logic; press-ground

  return errlev;
}