#ifndef I2CIP_SEESAW_T_
#define I2CIP_SEESAW_T_

#include <Arduino.h>
#include <I2CIP.h>

#include <Seesaw.h>

template <typename G, typename A, typename S, typename B> i2cip_errorlevel_t Seesaw<G, A, S, B>::getEncoderPosition(int32_t& dest, uint8_t encoder, bool setbus, bool resetbus)
{
  // this->read(SEESAW_ENCODER_BASE, SEESAW_ENCODER_POSITION + encoder, buf, 4);
  
  size_t len = 4; uint8_t buf[len] = {0};
  i2cip_errorlevel_t errlev = this->readRegister((uint16_t)(((uint16_t)SEESAW_ENCODER_BASE << 8) | (SEESAW_ENCODER_POSITION + encoder)), buf, len, setbus, resetbus);
  I2CIP_ERR_BREAK(errlev);
  if(len != 4) { return I2CIP::I2CIP_ERR_SOFT; }
  dest = (int32_t)(((uint32_t)buf[0] << 24) | ((uint32_t)buf[1] << 16) | ((uint32_t)buf[2] << 8) | (uint32_t)buf[3]);

  return errlev;
}

template <typename G, typename A, typename S, typename B> i2cip_errorlevel_t Seesaw<G, A, S, B>::setEncoderInterrupt(uint8_t encoder, bool enabled, bool setbus) {
  return writeRegister((uint16_t)(((uint16_t)SEESAW_ENCODER_BASE << 8) | (SEESAW_ENCODER_INTENSET + encoder)), (enabled ? (uint8_t)0x01 : (uint8_t)0x00), setbus);
}

// template <typename G, typename A, typename S, typename B> i2cip_errorlevel_t Seesaw<G, A, S, B>::getGPIO(uint64_t& dest, bool setbus, bool resetbus) {
template <typename G, typename A, typename S, typename B> i2cip_errorlevel_t Seesaw<G, A, S, B>::getGPIO(uint32_t& dest, bool setbus, bool resetbus) {
  // uint8_t buf[8] = {0}; size_t len = 8;
  uint8_t buf[4] = {0}; size_t len = 4;
  i2cip_errorlevel_t errlev = readRegister((uint16_t)(((uint16_t)SEESAW_GPIO_BASE << 8) | SEESAW_GPIO_BULK), buf, len, setbus, resetbus);
  I2CIP_ERR_BREAK(errlev);
  // if(len != 8) { return I2CIP::I2CIP_ERR_SOFT; }
  if(len != 4) { return I2CIP::I2CIP_ERR_SOFT; }
  for(uint8_t i = 0; i < len; i++) {
    dest <<= 8;
    dest |= buf[i];
  }
  return errlev;
}

template <typename G, typename A, typename S, typename B> i2cip_errorlevel_t Seesaw<G, A, S, B>::getGPIOPin(i2cip_state_pin_t& dest, uint8_t pin, bool setbus, bool resetbus) {
  // uint32_t maska = (1ul << pin); uint32_t maskb = (1ul << (pin - 32));
  // uint64_t mask = ((uint64_t)1 << pin);
  uint32_t mask = ((uint32_t)1 << pin);
  // uint64_t gpio = 0;
  uint32_t gpio = 0;
  // Serial.print("Seesaw GPIO Pin: "); Serial.print(pin); Serial.print(", Mask: 0b"); Serial.println(mask, BIN);
  i2cip_errorlevel_t errlev = getGPIO(gpio, setbus, resetbus);
  I2CIP_ERR_BREAK(errlev);
  // Serial.print("Seesaw GPIO: 0b"); Serial.print(gpio >> (pin+1), BIN); Serial.print('['); Serial.print((gpio & mask) ? 1 : 0); Serial.print(']'); Serial.print(gpio & ~(~0 >> (32 - pin)), BIN);
  dest = (gpio & mask) ? PIN_ON : PIN_OFF;
  return errlev;
}

template <typename G, typename A, typename S, typename B> i2cip_errorlevel_t Seesaw<G, A, S, B>::setGPIOInterrupts(uint32_t mask, bool enabled, bool setbus) {
  // uint8_t cmd[4] = {(uint8_t)mask, (uint8_t)(mask >> 8), (uint8_t)(mask >> 16), (uint8_t)(mask >> 24)};
  uint8_t cmd[4] = {(uint8_t)(mask >> 24), (uint8_t)(mask >> 16), (uint8_t)(mask >> 8), (uint8_t)mask}; // uint32_t MSB
  size_t len = 4;

  // return write(SEESAW_GPIO_BASE, enabled ? SEESAW_GPIO_INTENSET : SEESAW_GPIO_INTENCLR, cmd, 4);
  i2cip_errorlevel_t errlev = writeRegister((uint16_t)(((uint16_t)SEESAW_GPIO_BASE << 8) | (enabled ? SEESAW_GPIO_INTENSET : SEESAW_GPIO_INTENCLR)), cmd, len, setbus);
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

template <typename G, typename A, typename S, typename B> i2cip_errorlevel_t Seesaw<G, A, S, B>::setGPIOPinMode(uint8_t pin, uint8_t mode, bool setbus) {
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
      // errlev = this->writeRegister((((uint16_t)SEESAW_GPIO_BASE << 8) | (uint16_t)SEESAW_GPIO_DIRSET_BULK), (uint8_t*)cmd, len, setbus);
      errlev = writeRegister((uint16_t)(((uint16_t)SEESAW_GPIO_BASE << 8) | SEESAW_GPIO_DIRSET_BULK), (uint8_t*)cmd, len, setbus);
      break;
    case INPUT:
      // errlev = writeRegister((((uint16_t)SEESAW_GPIO_BASE << 8) | (uint16_t)SEESAW_GPIO_DIRCLR_BULK), (uint8_t*)cmd, len, setbus);
      errlev = writeRegister((uint16_t)(((uint16_t)SEESAW_GPIO_BASE << 8) | SEESAW_GPIO_DIRCLR_BULK), (uint8_t*)cmd, len, setbus);
      break;
    case INPUT_PULLUP:
      // errlev = writeRegister((((uint16_t)SEESAW_GPIO_BASE << 8) | (uint16_t)SEESAW_GPIO_DIRCLR_BULK), (uint8_t*)cmd, len, setbus);
      errlev = writeRegister((uint16_t)(((uint16_t)SEESAW_GPIO_BASE << 8) | SEESAW_GPIO_DIRCLR_BULK), (uint8_t*)cmd, len, setbus);
      I2CIP_ERR_BREAK(errlev);
      // errlev = writeRegister((((uint16_t)SEESAW_GPIO_BASE << 8) | (uint16_t)SEESAW_GPIO_PULLENSET), (uint8_t*)cmd, len, setbus);
      errlev = writeRegister((uint16_t)(((uint16_t)SEESAW_GPIO_BASE << 8) | SEESAW_GPIO_PULLENSET), (uint8_t*)cmd, len, setbus);
      I2CIP_ERR_BREAK(errlev);
      // errlev = writeRegister((((uint16_t)SEESAW_GPIO_BASE << 8) | (uint16_t)SEESAW_GPIO_BULK_SET), (uint8_t*)cmd, len, setbus);
      errlev = writeRegister((uint16_t)(((uint16_t)SEESAW_GPIO_BASE << 8) | SEESAW_GPIO_BULK_SET), (uint8_t*)cmd, len, setbus);
      break;
    case INPUT_PULLDOWN:
      // errlev = writeRegister((((uint16_t)SEESAW_GPIO_BASE << 8) | (uint16_t)SEESAW_GPIO_DIRCLR_BULK), (uint8_t*)cmd, len, setbus);
      errlev = writeRegister((uint16_t)(((uint16_t)SEESAW_GPIO_BASE << 8) | SEESAW_GPIO_DIRCLR_BULK), (uint8_t*)cmd, len, setbus);
      I2CIP_ERR_BREAK(errlev);
      // errlev = writeRegister((((uint16_t)SEESAW_GPIO_BASE << 8) | (uint16_t)SEESAW_GPIO_PULLENSET), (uint8_t*)cmd, len, setbus);
      errlev = writeRegister((uint16_t)(((uint16_t)SEESAW_GPIO_BASE << 8) | SEESAW_GPIO_PULLENSET), (uint8_t*)cmd, len, setbus);
      I2CIP_ERR_BREAK(errlev);
      // errlev = writeRegister((((uint16_t)SEESAW_GPIO_BASE << 8) | (uint16_t)SEESAW_GPIO_BULK_CLR), (uint8_t*)cmd, len, setbus);
      errlev = writeRegister((uint16_t)(((uint16_t)SEESAW_GPIO_BASE << 8) | SEESAW_GPIO_BULK_CLR), (uint8_t*)cmd, len, setbus);
      break;
    default:
      return I2CIP::I2CIP_ERR_SOFT;
  }
  if(errlev == I2CIP::I2CIP_ERR_NONE && len != ((pin >= 32) ? 8 : 4)) {
    return I2CIP::I2CIP_ERR_SOFT;
  }
  return errlev;
}

#endif