#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/uart.h"
#include "hardware/gpio.h"
#include "board.h"
#include "sx1509.h"

const uint8_t keyMap[SX1509_ROWS][SX1509_COLS] = {
  { 0, 1, 2, 3 },
  { 4, 5, 6, 7 },
  { 8, 9, 10, 11 },
  { 12, 13, 14, 15 }
};

uint8_t key_data1, key_data2;
uint8_t key_pressed = 0; // Flag to indicate if a key event has been triggered

// SX1509 Interrupt Handler
static void sx1509_interrupt_handler(uint gpio, uint32_t events) {
    // Read the keypad event data from SX1509   
    
    if (!sx1509_read_keypad(I2C_PORT, SX1509_I2C_ADDR, &key_data1, &key_data2)) {
    printf("SX1509: Failed to read keypad data\n");   
    }
    else
    {
        key_pressed = 1; // Set the flag to indicate a key event has occurred
        //printf("Key Event: (0x%02X 0x%02X) , %d\n", key_data1, key_data2, key_pressed);
    }
      
}


int main()
{
    stdio_init_all();

    uint8_t row = 0;
    uint8_t col = 0;
    uint8_t n = 0;
     char keyFunction[10] = {0};
    uint8_t test =0;

    // I2C Initialisation.
    i2c_init(I2C_PORT, I2C_BAUDRATE);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    // For more examples of I2C use see https://github.com/raspberrypi/pico-examples/tree/master/i2c


    // Set up our UART
 //   uart_init(UART_ID, UART_BAUDRATE);
    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
 //   gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
 //   gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    
    // Use some the various UART functions to send out data
    // In a default system, printf will also output via the default UART
    
    // Send out a string, with CR/LF conversions
 //   uart_puts(UART_ID, " Hello, UART!\n");
    
    // For more examples of UART use see https://github.com/raspberrypi/pico-examples/tree/master/uart


    // Set up the LED pin
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    //sleep_ms(20000);
    // Initialize SX1509 GPIO Expander / Keypad
    printf("\nInitializing SX1509 at address 0x%02X...\n", SX1509_I2C_ADDR);
    if (!sx1509_init_keypad(I2C_PORT, SX1509_I2C_ADDR)) {
        printf("ERROR: Failed to initialize SX1509!\n");
        test = 1;
    } else {
        printf("SX1509 ready for keypad input (%d rows x %d columns)\n", SX1509_ROWS, SX1509_COLS);
        test = 2;
    }

    // Set up SX1509 interrupt handler on GP2
    gpio_init(SX1509_INT_PIN);
    gpio_set_dir(SX1509_INT_PIN, GPIO_IN);
//    gpio_pull_up(SX1509_INT_PIN); // Optional pull-up
    gpio_set_irq_enabled_with_callback(SX1509_INT_PIN, GPIO_IRQ_EDGE_FALL, true, &sx1509_interrupt_handler);

    while (true) {
        n=0; // Reset the counter for LED blinks
        
        //printf("Key pressed: %d\n", key_pressed);
        
        if(key_pressed == 1) {
            
            row = SX1509_getRow(&key_data2);
            col = SX1509_getCol(&key_data1);

            //strncpy(keyFunction, *keyFunctions[keyMap[row][col]], sizeof(keyFunction) - 1);
            //keyFunction[sizeof(keyFunction) - 1] = '\0';

            //printf("Key Event: Key #%d (0x%02X 0x%02X) - Function: %s\n", keyMap[row][col], key_data1, key_data2, keyFunction);
            //printf("Row: %d, Column: %d\n", row, col);
            //printf("Key Event: Key #%d (0x%02X 0x%02X)\n", keyMap[row][col], key_data1, key_data2);
            
            switch (keyMap[row][col]) {
                case 0:
                    // Handle key press for key 0
                    strcpy(keyFunction, "X");
                    break;
                case 1:
                    // Handle key press for key 1
                    strcpy(keyFunction, "RUN");
                    break;
                case 2:
                    // Handle key press for key 2
                    strcpy(keyFunction, "CW");
                    break;
                case 3:
                    // Handle key press for key 3
                    strcpy(keyFunction, "MIST");
                    break;
                case 4:
                    // Handle key press for key 4
                    strcpy(keyFunction, "Y");
                    break;
                case 5:
                    // Handle key press for key 5
                    strcpy(keyFunction, "PAUSE");
                    break;
                case 6:
                    // Handle key press for key 6
                    strcpy(keyFunction, "ACW");
                    break;
                case 7:
                    // Handle key press for key 7
                    strcpy(keyFunction, "WATER");
                    break;
                case 8:
                    // Handle key press for key 8
                    strcpy(keyFunction, "Z");
                    break;
                case 9:
                    // Handle key press for key 9
                    strcpy(keyFunction, "ZERO");
                    break;
                case 10:
                    // Handle key press for key 10
                    strcpy(keyFunction, "NULL");
                    break;
                case 11:
                    // Handle key press for key 11
                    strcpy(keyFunction, "NULL");
                    break;
                case 12:
                    // Handle key press for key 12
                    strcpy(keyFunction, "W");
                    break;
                case 13:
                    // Handle key press for key 13
                    strcpy(keyFunction, "STOP");
                    break;
                case 14:
                    // Handle key press for key 14
                    strcpy(keyFunction, "SPINOFF");
                    break;
                case 15:
                    // Handle key press for key 15
                    strcpy(keyFunction, "COOLOFF");
                    break;
                default:
                    printf("Key Unmapped\n");
                    break;
            }
            printf("%s\n", keyFunction);
            key_pressed = 0; // Reset the flag    
        }


        //printf("Hello, world!, %d\n", test);
        //sleep_ms(1000);
        sleep_ms(1);
    }
}
