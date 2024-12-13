#include <Seesaw.h>

bool Seesaw_RotaryEncoder::_id_set = false;
char Seesaw_RotaryEncoder::_id[I2CIP_ID_SIZE];

void Seesaw_RotaryEncoder::loadID() {
  uint8_t idlen = strlen_P(i2cip_seesaw_rotaryencoder_id_progmem);

  // Read in PROGMEM
  for (uint8_t k = 0; k < idlen; k++) {
    char c = pgm_read_byte_near(i2cip_seesaw_rotaryencoder_id_progmem + k);
    Seesaw_RotaryEncoder::_id[k] = c;
  }

  Seesaw_RotaryEncoder::_id[idlen] = '\0';
  Seesaw_RotaryEncoder::_id_set = true;

  #ifdef I2CIP_DEBUG_SERIAL
    DEBUG_DELAY();
    I2CIP_DEBUG_SERIAL.print(F("Seesaw RotaryEncoder ID Loaded: '"));
    I2CIP_DEBUG_SERIAL.print(Seesaw_RotaryEncoder::_id);
    I2CIP_DEBUG_SERIAL.print(F("' @"));
    I2CIP_DEBUG_SERIAL.println((uintptr_t)(&Seesaw_RotaryEncoder::_id[0]), HEX);
    DEBUG_DELAY();
  #endif
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
  i2cip_errorlevel_t errlev = I2CIP_ERR_NONE;
  if(!initialized) {
    errlev = setGPIOPinMode(encoder, INPUT);
    I2CIP_ERR_BREAK(errlev);
    errlev = setGPIOInterrupts((uint32_t)1 << I2CIP_ROTARY_BUTTON_PIN);
    I2CIP_ERR_BREAK(errlev);
    // enableEncoderInterrupt
    errlev = setEncoderInterrupt(encoder);
    I2CIP_ERR_BREAK(errlev);
    // initialized = true;

    delay(I2CIP_SEESAW_DELAY);;
  }
  int32_t epos = 0;
  errlev = getEncoderPosition(epos, encoder);
  I2CIP_ERR_BREAK(errlev);
  value.encoder = epos;

  i2cip_state_pin_t button = PIN_UNDEF;
  errlev = getGPIOPin(button, I2CIP_ROTARY_BUTTON_PIN, false, true);
  I2CIP_ERR_BREAK(errlev);
  value.button = button == PIN_ON ? PIN_OFF : (button == PIN_OFF ? PIN_ON : PIN_UNDEF); // Inverted logic; press-ground

  return errlev;
}