#include <Seesaw.h>

I2CIP_DEVICE_INIT_STATIC_ID(_Seesaw, I2CIP_SEESAW_ID)

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