#ifndef SX1509_H
#define SX1509_H

#include <stdint.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

//
// SX1509 I2C GPIO Expander / Keypad Driver
// Reference: SX1509 Datasheet
//

// ============================================================================
// Register Definitions
// ============================================================================
#define SX1509_REG_INPUT_DISABLE_B      0x00
#define SX1509_REG_INPUT_DISABLE_A      0x01
#define SX1509_REG_LONG_SLEW_B          0x02
#define SX1509_REG_LONG_SLEW_A          0x03
#define SX1509_REG_LOW_DRIVE_B          0x04
#define SX1509_REG_LOW_DRIVE_A          0x05
#define SX1509_REG_PULL_UP_B            0x06
#define SX1509_REG_PULL_UP_A            0x07
#define SX1509_REG_PULL_DOWN_B          0x08
#define SX1509_REG_PULL_DOWN_A          0x09
#define SX1509_REG_OPEN_DRAIN_B         0x0A
#define SX1509_REG_OPEN_DRAIN_A         0x0B
#define SX1509_REG_POLARITY_B           0x0C
#define SX1509_REG_POLARITY_A           0x0D
#define SX1509_REG_DIR_B                0x0E
#define SX1509_REG_DIR_A                0x0F
#define SX1509_REG_DATA_B               0x10
#define SX1509_REG_DATA_A               0x11
#define SX1509_REG_INTERRUPT_MASK_B     0x12
#define SX1509_REG_INTERRUPT_MASK_A     0x13
#define SX1509_REG_SENSE_HIGH_B         0x14
#define SX1509_REG_SENSE_LOW_B          0x15
#define SX1509_REG_SENSE_HIGH_A         0x16
#define SX1509_REG_SENSE_LOW_A          0x17
#define SX1509_REG_INTERRUPT_SOURCE_B   0x18
#define SX1509_REG_INTERRUPT_SOURCE_A   0x19
#define SX1509_REG_EVENT_STATUS_B       0x1A
#define SX1509_REG_EVENT_STATUS_A       0x1B
#define SX1509_REG_LEVEL_SHIFTER_1      0x1C
#define SX1509_REG_LEVEL_SHIFTER_2      0x1D
#define SX1509_REG_CLOCK                0x1E
#define SX1509_REG_MISC                 0x1F
#define SX1509_REG_LED_DRIVER_ENABLE_B  0x20
#define SX1509_REG_LED_DRIVER_ENABLE_A  0x21
#define SX1509_REG_DEBOUNCE_CONFIG      0x22
#define SX1509_REG_DEBOUNCE_ENABLE_B    0x23
#define SX1509_REG_DEBOUNCE_ENABLE_A    0x24
#define SX1509_REG_KEY_CONFIG_1         0x25
#define SX1509_REG_KEY_CONFIG_2         0x26
#define SX1509_REG_KEY_DATA_1           0x27
#define SX1509_REG_KEY_DATA_2           0x28

// LED Driver Registers (0x29-0x68)
// Channels 0-3: TOn, IOn, Off (no fade capability)
#define SX1509_REG_T_ON_0               0x29
#define SX1509_REG_I_ON_0               0x2A
#define SX1509_REG_OFF_0                0x2B
#define SX1509_REG_T_ON_1               0x2C
#define SX1509_REG_I_ON_1               0x2D
#define SX1509_REG_OFF_1                0x2E
#define SX1509_REG_T_ON_2               0x2F
#define SX1509_REG_I_ON_2               0x30
#define SX1509_REG_OFF_2                0x31
#define SX1509_REG_T_ON_3               0x32
#define SX1509_REG_I_ON_3               0x33
#define SX1509_REG_OFF_3                0x34

// Channels 4-7: TOn, IOn, Off, TRise, TFall (with fade)
#define SX1509_REG_T_ON_4               0x35
#define SX1509_REG_I_ON_4               0x36
#define SX1509_REG_OFF_4                0x37
#define SX1509_REG_T_RISE_4             0x38
#define SX1509_REG_T_FALL_4             0x39
#define SX1509_REG_T_ON_5               0x3A
#define SX1509_REG_I_ON_5               0x3B
#define SX1509_REG_OFF_5                0x3C
#define SX1509_REG_T_RISE_5             0x3D
#define SX1509_REG_T_FALL_5             0x3E
#define SX1509_REG_T_ON_6               0x3F
#define SX1509_REG_I_ON_6               0x40
#define SX1509_REG_OFF_6                0x41
#define SX1509_REG_T_RISE_6             0x42
#define SX1509_REG_T_FALL_6             0x43
#define SX1509_REG_T_ON_7               0x44
#define SX1509_REG_I_ON_7               0x45
#define SX1509_REG_OFF_7                0x46
#define SX1509_REG_T_RISE_7             0x47
#define SX1509_REG_T_FALL_7             0x48

// Channels 8-11: TOn, IOn, Off (no fade)
#define SX1509_REG_T_ON_8               0x49
#define SX1509_REG_I_ON_8               0x4A
#define SX1509_REG_OFF_8                0x4B
#define SX1509_REG_T_ON_9               0x4C
#define SX1509_REG_I_ON_9               0x4D
#define SX1509_REG_OFF_9                0x4E
#define SX1509_REG_T_ON_10              0x4F
#define SX1509_REG_I_ON_10              0x50
#define SX1509_REG_OFF_10               0x51
#define SX1509_REG_T_ON_11              0x52
#define SX1509_REG_I_ON_11              0x53
#define SX1509_REG_OFF_11               0x54

// Channels 12-15: TOn, IOn, Off, TRise, TFall (with fade)
#define SX1509_REG_T_ON_12              0x55
#define SX1509_REG_I_ON_12              0x56
#define SX1509_REG_OFF_12               0x57
#define SX1509_REG_T_RISE_12            0x58
#define SX1509_REG_T_FALL_12            0x59
#define SX1509_REG_T_ON_13              0x5A
#define SX1509_REG_I_ON_13              0x5B
#define SX1509_REG_OFF_13               0x5C
#define SX1509_REG_T_RISE_13            0x5D
#define SX1509_REG_T_FALL_13            0x5E
#define SX1509_REG_T_ON_14              0x5F
#define SX1509_REG_I_ON_14              0x60
#define SX1509_REG_OFF_14               0x61
#define SX1509_REG_T_RISE_14            0x62
#define SX1509_REG_T_FALL_14            0x63
#define SX1509_REG_T_ON_15              0x64
#define SX1509_REG_I_ON_15              0x65
#define SX1509_REG_OFF_15               0x66
#define SX1509_REG_T_RISE_15            0x67
#define SX1509_REG_T_FALL_15            0x68

// Miscellaneous
#define SX1509_REG_HIGH_INPUT_B         0x69
#define SX1509_REG_HIGH_INPUT_A         0x6A

// Software Reset
#define SX1509_REG_RESET                0x7D

// Test (not to be written)
#define SX1509_REG_TEST_1               0x7E
#define SX1509_REG_TEST_2               0x7F

// ============================================================================
// Function Prototypes
// ============================================================================

/**
 * Initialize SX1509 on I2C bus
 * Sets up the device for keypad scanning with debounce
 * 
 * @param i2c_port The I2C port to use
 * @param addr The I2C address of the SX1509 (typically 0x3E)
 * @return true if initialization successful, false otherwise
 */
bool sx1509_init_keypad(i2c_inst_t *i2c_port, uint8_t addr);

/**
 * Write a single byte to an SX1509 register
 * 
 * @param i2c_port The I2C port to use
 * @param addr I2C address
 * @param reg Register address
 * @param value Value to write
 * @return true if successful, false on I2C error
 */
bool sx1509_write_reg(i2c_inst_t *i2c_port, uint8_t addr, uint8_t reg, uint8_t value);

/**
 * Read a single byte from an SX1509 register
 * 
 * @param i2c_port The I2C port to use
 * @param addr I2C address
 * @param reg Register address
 * @param value Pointer to store the read value
 * @return true if successful, false on I2C error
 */
bool sx1509_read_reg(i2c_inst_t *i2c_port, uint8_t addr, uint8_t reg, uint8_t *value);

/**
 * Read keypad event data
 * Returns the key data from the SX1509 keypad engine
 * 
 * @param i2c_port The I2C port to use
 * @param addr I2C address
 * @param key_data1 Pointer to KEY_DATA_1 register (pressed key information)
 * @param key_data2 Pointer to KEY_DATA_2 register (additional key info)
 * @return true if successful, false on I2C error
 */
bool sx1509_read_keypad(i2c_inst_t *i2c_port, uint8_t addr, uint8_t *key_data1, uint8_t *key_data2);

/**
 * Get the current state of GPIO pins (Port A and B)
 * 
 * @param i2c_port The I2C port to use
 * @param addr I2C address
 * @param port_a Pointer to store Port A data
 * @param port_b Pointer to store Port B data
 * @return true if successful, false on I2C error
 */
bool sx1509_read_gpio(i2c_inst_t *i2c_port, uint8_t addr, uint8_t *port_a, uint8_t *port_b);

/**
 * Set output state on GPIO pins
 * 
 * @param i2c_port The I2C port to use
 * @param addr I2C address
 * @param port_a Value to write to Port A
 * @param port_b Value to write to Port B
 * @return true if successful, false on I2C error
 */
bool sx1509_write_gpio(i2c_inst_t *i2c_port, uint8_t addr, uint8_t port_a, uint8_t port_b);

/**
 * Configure a GPIO pin direction
 * 
 * @param i2c_port The I2C port to use
 * @param addr I2C address
 * @param pin Pin number (0-15)
 * @param is_output true for output, false for input
 * @return true if successful, false on I2C error
 */
bool sx1509_set_pin_direction(i2c_inst_t *i2c_port, uint8_t addr, uint8_t pin, bool is_output);

/**
 * Reset the SX1509 device
 * 
 * @param i2c_port The I2C port to use
 * @param addr I2C address
 * @return true if successful, false on I2C error
 */
bool sx1509_reset(i2c_inst_t *i2c_port, uint8_t addr);


uint8_t SX1509_getRow(uint8_t *keyData2);

uint8_t SX1509_getCol(uint8_t *keyData1);


#endif // SX1509_H
