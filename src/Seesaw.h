#ifndef I2CIP_SEESAW_H_
#define I2CIP_SEESAW_H_

#include <Arduino.h>
#include <I2CIP.h>

// MACROS

// Registers
// #define Seesaw_MEAS_HIGHREP_STRETCH  (uint16_t)0x2C06  // Measurement High Repeatability with Clock Stretch Enabled
// #define Seesaw_MEAS_MEDREP_STRETCH   (uint16_t)0x2C0D  // Measurement Medium Repeatability with Clock Stretch Enabled
// #define Seesaw_MEAS_LOWREP_STRETCH   (uint16_t)0x2C10  // Measurement Low Repeatability with Clock Stretch Enabled
// #define Seesaw_MEAS_HIGHREP          (uint16_t)0x2400  // Measurement High Repeatability with Clock Stretch Disabled
// #define Seesaw_MEAS_MEDREP           (uint16_t)0x240B  // Measurement Medium Repeatability with Clock Stretch Disabled
// #define Seesaw_MEAS_LOWREP           (uint16_t)0x2416  // Measurement Low Repeatability with Clock Stretch Disabled
// #define Seesaw_READSTATUS            (uint16_t)0xF32D  // Read Out of Status Register
// #define Seesaw_CLEARSTATUS           (uint16_t)0x3041  // Clear Status
// #define Seesaw_SOFTRESET             (uint16_t)0x30A2  // Soft Reset
// #define Seesaw_HEATEREN              (uint16_t)0x306D  // Heater Enable
// #define Seesaw_HEATERDIS             (uint16_t)0x3066  // Heater Disable
// #define Seesaw_REG_HEATER_BIT        (uint16_t)0x000d  // Status Register Heater Bit

// Settings
// #define I2CIP_SEESAW_ADDRESS 0x49 // Default
// #define I2CIP_SEESAW_ADDRESS 0x32 // Default
#define I2CIP_SEESAW_ADDRESS 0x36 // Default
#define I2CIP_SEESAW_DELAY 10 // Write/Read Sensing Delay

/** Module Base Addreses
 *  The module base addresses for different seesaw modules.
 */
enum {
  SEESAW_STATUS_BASE = 0x00,
  SEESAW_GPIO_BASE = 0x01,
  SEESAW_SERCOM0_BASE = 0x02,

  SEESAW_TIMER_BASE = 0x08,
  SEESAW_ADC_BASE = 0x09,
  SEESAW_DAC_BASE = 0x0A,
  SEESAW_INTERRUPT_BASE = 0x0B,
  SEESAW_DAP_BASE = 0x0C,
  SEESAW_EEPROM_BASE = 0x0D,
  SEESAW_NEOPIXEL_BASE = 0x0E,
  SEESAW_TOUCH_BASE = 0x0F,
  SEESAW_KEYPAD_BASE = 0x10,
  SEESAW_ENCODER_BASE = 0x11,
  SEESAW_SPECTRUM_BASE = 0x12,
};

/** GPIO module function address registers
 */
enum {
  SEESAW_GPIO_DIRSET_BULK = 0x02,
  SEESAW_GPIO_DIRCLR_BULK = 0x03,
  SEESAW_GPIO_BULK = 0x04,
  SEESAW_GPIO_BULK_SET = 0x05,
  SEESAW_GPIO_BULK_CLR = 0x06,
  SEESAW_GPIO_BULK_TOGGLE = 0x07,
  SEESAW_GPIO_INTENSET = 0x08,
  SEESAW_GPIO_INTENCLR = 0x09,
  SEESAW_GPIO_INTFLAG = 0x0A,
  SEESAW_GPIO_PULLENSET = 0x0B,
  SEESAW_GPIO_PULLENCLR = 0x0C,
};

/** status module function address registers
 */
enum {
  SEESAW_STATUS_HW_ID = 0x01,
  SEESAW_STATUS_VERSION = 0x02,
  SEESAW_STATUS_OPTIONS = 0x03,
  SEESAW_STATUS_TEMP = 0x04,
  SEESAW_STATUS_SWRST = 0x7F,
};

/** timer module function address registers
 */
enum {
  SEESAW_TIMER_STATUS = 0x00,
  SEESAW_TIMER_PWM = 0x01,
  SEESAW_TIMER_FREQ = 0x02,
};

/** ADC module function address registers
 */
enum {
  SEESAW_ADC_STATUS = 0x00,
  SEESAW_ADC_INTEN = 0x02,
  SEESAW_ADC_INTENCLR = 0x03,
  SEESAW_ADC_WINMODE = 0x04,
  SEESAW_ADC_WINTHRESH = 0x05,
  SEESAW_ADC_CHANNEL_OFFSET = 0x07,
};

/** Sercom module function address registers
 */
enum {
  SEESAW_SERCOM_STATUS = 0x00,
  SEESAW_SERCOM_INTEN = 0x02,
  SEESAW_SERCOM_INTENCLR = 0x03,
  SEESAW_SERCOM_BAUD = 0x04,
  SEESAW_SERCOM_DATA = 0x05,
};

/** neopixel module function address registers
 */
enum {
  SEESAW_NEOPIXEL_STATUS = 0x00,
  SEESAW_NEOPIXEL_PIN = 0x01,
  SEESAW_NEOPIXEL_SPEED = 0x02,
  SEESAW_NEOPIXEL_BUF_LENGTH = 0x03,
  SEESAW_NEOPIXEL_BUF = 0x04,
  SEESAW_NEOPIXEL_SHOW = 0x05,
};

/** touch module function address registers
 */
enum {
  SEESAW_TOUCH_CHANNEL_OFFSET = 0x10,
};

/** keypad module function address registers
 */
enum {
  SEESAW_KEYPAD_STATUS = 0x00,
  SEESAW_KEYPAD_EVENT = 0x01,
  SEESAW_KEYPAD_INTENSET = 0x02,
  SEESAW_KEYPAD_INTENCLR = 0x03,
  SEESAW_KEYPAD_COUNT = 0x04,
  SEESAW_KEYPAD_FIFO = 0x10,
};

/** keypad module edge definitions
 */
enum {
  SEESAW_KEYPAD_EDGE_HIGH = 0,
  SEESAW_KEYPAD_EDGE_LOW,
  SEESAW_KEYPAD_EDGE_FALLING,
  SEESAW_KEYPAD_EDGE_RISING,
};

/** encoder module edge definitions
 */
enum {
  SEESAW_ENCODER_STATUS = 0x00,
  SEESAW_ENCODER_INTENSET = 0x10,
  SEESAW_ENCODER_INTENCLR = 0x20,
  SEESAW_ENCODER_POSITION = 0x30,
  SEESAW_ENCODER_DELTA = 0x40,
};

/** Audio spectrum module function address registers
 */
enum {
  SEESAW_SPECTRUM_RESULTS_LOWER = 0x00, // Audio spectrum bins 0-31
  SEESAW_SPECTRUM_RESULTS_UPPER = 0x01, // Audio spectrum bins 32-63
  // If some future device supports a larger spectrum, can add additional
  // "bins" working upward from here. Configurable setting registers then
  // work downward from the top to avoid collision between spectrum bins
  // and configurables.
  SEESAW_SPECTRUM_CHANNEL = 0xFD,
  SEESAW_SPECTRUM_RATE = 0xFE,
  SEESAW_SPECTRUM_STATUS = 0xFF,
};

#define ADC_INPUT_0_PIN 2 ///< default ADC input pin
#define ADC_INPUT_1_PIN 3 ///< default ADC input pin
#define ADC_INPUT_2_PIN 4 ///< default ADC input pin
#define ADC_INPUT_3_PIN 5 ///< default ADC input pin

#define PWM_0_PIN 4 ///< default PWM output pin
#define PWM_1_PIN 5 ///< default PWM output pin
#define PWM_2_PIN 6 ///< default PWM output pin
#define PWM_3_PIN 7 ///< default PWM output pin

#ifndef INPUT_PULLDOWN
#define INPUT_PULLDOWN                                                         \
  0x03 ///< for compatibility with platforms that do not already define
       ///< INPUT_PULLDOWN
#endif

/*=========================================================================*/
// clang-format off
#define SEESAW_HW_ID_CODE_SAMD09 0x55 ///< seesaw HW ID code for SAMD09
#define SEESAW_HW_ID_CODE_TINY806 0x84 ///< seesaw HW ID code for ATtiny806
#define SEESAW_HW_ID_CODE_TINY807 0x85 ///< seesaw HW ID code for ATtiny807
#define SEESAW_HW_ID_CODE_TINY816 0x86 ///< seesaw HW ID code for ATtiny816
#define SEESAW_HW_ID_CODE_TINY817 0x87 ///< seesaw HW ID code for ATtiny817
#define SEESAW_HW_ID_CODE_TINY1616 0x88 ///< seesaw HW ID code for ATtiny1616
#define SEESAW_HW_ID_CODE_TINY1617 0x89 ///< seesaw HW ID code for ATtiny1617
// clang-format on

/** raw key event stucture for keypad module */
union keyEventRaw {
  struct {
    uint8_t EDGE : 2; ///< the edge that was triggered
    uint8_t NUM : 6;  ///< the event number
  } bit;              ///< bitfield format
  uint8_t reg;        ///< register format
};

/** extended key event stucture for keypad module */
union keyEvent {
  struct {
    uint8_t EDGE : 2;  ///< the edge that was triggered
    uint16_t NUM : 14; ///< the event number
  } bit;               ///< bitfield format
  uint16_t reg;        ///< register format
};

/** key state struct that will be written to seesaw chip keypad module */
union keyState {
  struct {
    uint8_t STATE : 1;  ///< the current state of the key
    uint8_t ACTIVE : 4; ///< the registered events for that key
  } bit;                ///< bitfield format
  uint8_t reg;          ///< register format
};

using namespace I2CIP;

// typedef enum {
//   Seesaw_HEATER_DISABLE  = 0x00,
//   Seesaw_HEATER_ENABLE   = 0x01,
// } args_seesaw_t;

// typedef struct {
//   float temperature;
//   float humidity;
// } state_seesaw_t;

#define I2CIP_ROTARY_BUTTON_PIN 24

const char i2cip_seesaw_rotaryencoder_id_progmem[] PROGMEM = {"ROTARY"};


// Interface class for the Seesaw air temperature and humidity sensor
template <typename G, typename A, typename S, typename B> class Seesaw : public Device, public IOInterface<G, A, S, B> {
  private:
    // static bool _id_set;
    // static char _id[];

    // Seesaw(const i2cip_fqa_t& fqa) : Seesaw(fqa, _id) { loadID(); }

    #ifdef MAIN_CLASS_NAME
    friend class MAIN_CLASS_NAME;
    #endif

  protected:
    i2cip_errorlevel_t setGPIOPinMode(uint8_t pin, uint8_t mode, bool setbus = false);
    i2cip_errorlevel_t setEncoderInterrupt(uint8_t encoder, bool enabled = true, bool setbus = false);
    i2cip_errorlevel_t setGPIOInterrupts(uint32_t mask, bool enabled = true, bool setbus = false); // (uint32_t)1 << SS_SWITCH, 1


    i2cip_errorlevel_t getEncoderPosition(int32_t& dest, uint8_t encoder, bool setbus = false, bool resetbus = false);
    // i2cip_errorlevel_t getGPIO(uint64_t& dest, bool setbus = false, bool resetbus = false);
    i2cip_errorlevel_t getGPIO(uint32_t& dest, bool setbus = false, bool resetbus = false);
    i2cip_errorlevel_t getGPIOPin(i2cip_state_pin_t& dest, uint8_t pin, bool setbus = false, bool resetbus = false);
    
    Seesaw(const i2cip_fqa_t& fqa, const i2cip_id_t& id) : Device(fqa, id), IOInterface<G, A, S, B>((Device*)this) { }
  public:
    // static void loadID(void);

    // static Device* seesawFactory(const i2cip_fqa_t& fqa, const i2cip_id_t& id);
    // static Device* seesawFactory(const i2cip_fqa_t& fqa);
    
    // i2cip_errorlevel_t get(state_seesaw_t& value, const args_seesaw_t& args) override;

    // const args_seesaw_t& getDefaultA(void) const override;
    // void clearCache(void) override;

    // static const char* getStaticIDBuffer() { return Seesaw::_id; }
    // static const char* getStaticIDBuffer() { return Seesaw::_id_set ? Seesaw::_id : nullptr; }
};

typedef struct {
  i2cip_state_pin_t button;
  int32_t encoder;
} i2cip_rotaryencoder_t;

class Seesaw_RotaryEncoder : public Seesaw<i2cip_rotaryencoder_t, void*, void*, void*> {
  private:
    static bool _id_set;
    static char _id[];

    bool initialized = false;

    void* const is_null = nullptr;
    const uint8_t encoder;
  public:
    Seesaw_RotaryEncoder(uint8_t encoder, const i2cip_fqa_t& fqa, const i2cip_id_t& id) : Seesaw<i2cip_rotaryencoder_t, void*, void*, void*>(fqa, id), encoder(encoder) { removeOutput(); }
    Seesaw_RotaryEncoder(const i2cip_fqa_t& fqa, const i2cip_id_t& id) : Seesaw_RotaryEncoder((uint8_t)0, fqa, id) { }
    
    i2cip_errorlevel_t set(void* const& value, void* const& args) override { return I2CIP_ERR_HARD; } // You're clearly new here
    void* const& getDefaultB(void) const override { return is_null; }
    void resetFailsafe(void) override { return; } // No output

    i2cip_errorlevel_t get(i2cip_rotaryencoder_t& value, void* const& args) override;
    void* const& getDefaultA(void) const override { return is_null; }

    void clearCache(void) override { setCache({.button = PIN_UNDEF, .encoder = 0}); } // No output

    static const char* getStaticIDBuffer() { return Seesaw_RotaryEncoder::_id; }
    static void loadID(void);

    static Device* rotaryEncoderFactory(const i2cip_fqa_t& fqa, const i2cip_id_t& id) { loadID(); return new Seesaw_RotaryEncoder(fqa, id); }
    static Device* rotaryEncoderFactory(const i2cip_fqa_t& fqa) { loadID(); return new Seesaw_RotaryEncoder(fqa, _id); }
};

#endif

#include <Seesaw.tpp>