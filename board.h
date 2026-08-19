#ifndef BOARD_H
#define BOARD_H

//
// Pin and Hardware Configuration
// Modify these definitions when porting firmware to a different board
//

// ============================================================================
// LED Configuration
// ============================================================================
#define LED_PIN 6                   // GPIO pin for LED (or use PICO_DEFAULT_LED_PIN)

// ============================================================================
// I2C Configuration
// ============================================================================
#define I2C_PORT        i2c0        // I2C peripheral (i2c0 or i2c1)
#define I2C_SDA         4           // GPIO pin for SDA (data line)
#define I2C_SCL         5           // GPIO pin for SCL (clock line)
#define I2C_BAUDRATE    400000      // I2C speed in Hz (400kHz standard)

// ============================================================================
// UART Configuration
// ============================================================================
//#define UART_ID         uart1       // UART peripheral (uart0 or uart1)
//#define UART_TX_PIN     0           // GPIO pin for TX (transmit)
//#define UART_RX_PIN     1           // GPIO pin for RX (receive)
//#define UART_BAUDRATE   115200      // Baud rate for serial communication



// ============================================================================
// SX1509 GPIO Expander / Keypad Configuration
// ============================================================================
#define SX1509_KEYPAD_I2C_ADDR     0x3E    // I2C address of SX1509
#define SX1509_LED_I2C_ADDR     0x3F    // I2C address of SX1509
#define SX1509_ROWS         4       // Number of keypad rows
#define SX1509_COLS         4       // Number of keypad columns
#define SX1509_INT_PIN      2       // GPIO pin for SX1509 interrupt (active low)

#endif // BOARD_H
