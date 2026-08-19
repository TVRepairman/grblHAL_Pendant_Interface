#include <stdio.h>
#include "sx1509.h"
#include "board.h"

//
// SX1509 Driver Implementation
//

bool sx1509_write_reg(i2c_inst_t *i2c_port, uint8_t addr, uint8_t reg, uint8_t value) {
    uint8_t buf[2] = {reg, value};
    int result = i2c_write_blocking(i2c_port, addr, buf, 2, false);
    return result == 2;  // Should write 2 bytes on success
}

bool sx1509_read_reg(i2c_inst_t *i2c_port, uint8_t addr, uint8_t reg, uint8_t *value) {
    if (!value) return false;
    
    // Write register address
    int write_result = i2c_write_blocking(i2c_port, addr, &reg, 1, true);
    if (write_result != 1) return false;
    
    // Read value
    int read_result = i2c_read_blocking(i2c_port, addr, value, 1, false);
    return read_result == 1;
}

bool sx1509_reset(i2c_inst_t *i2c_port, uint8_t addr) {
    // Reset via MISC register (write 0x80 to trigger reset)
    return sx1509_write_reg(i2c_port, addr, SX1509_REG_MISC, 0x80);
}

bool sx1509_init_keypad(i2c_inst_t *i2c_port, uint8_t addr) {

    // Configure the oscillator (keypad won't work without it):
    if (!sx1509_write_reg(i2c_port, addr, SX1509_REG_CLOCK, 0x40)) {
        printf("SX1509: Failed to reset device\n");
        return false;
    }

    // Configure for keypad operation, following p16 of the datasheet:
    // 1a. Set Set RegDir for port A as output, port B as input (DIR = 0xFF for inputs, 0x00 for outputs)
    if (!sx1509_write_reg(i2c_port, addr, SX1509_REG_DIR_A, 0x00)) {
        printf("SX1509: Failed to set direction A\n");
        return false;
    }
    
    if (!sx1509_write_reg(i2c_port, addr, SX1509_REG_DIR_B, 0xFF)) {
        printf("SX1509: Failed to set direction B\n");
        return false;
    }

    // 1b. Set the Open Drain register so that the outputs are open drain (Open drain = 0x00)
    if (!sx1509_write_reg(i2c_port, addr, SX1509_REG_OPEN_DRAIN_A, 0xFF)) {
        printf("SX1509: Failed to set open drain A\n");
        return false;
    }

    if (!sx1509_write_reg(i2c_port, addr, SX1509_REG_OPEN_DRAIN_B, 0x00)) {
        printf("SX1509: Failed to set open drain B\n");
        return false;
    }

    // 1c. Enable pull-ups on the port that is an input
    if (!sx1509_write_reg(i2c_port, addr, SX1509_REG_PULL_UP_A, 0x00)) {
        printf("SX1509: Failed to enable pull-ups A\n");
        return false;
    }
    
    if (!sx1509_write_reg(i2c_port, addr, SX1509_REG_PULL_UP_B, 0xFF)) {
        printf("SX1509: Failed to enable pull-ups B\n");
        return false;
    }
    
    // 2a. Enable debounce on input pins
    if (!sx1509_write_reg(i2c_port, addr, SX1509_REG_DEBOUNCE_ENABLE_A, 0x00)) {
        printf("SX1509: Failed to enable debounce A\n");
        return false;
    }
    
    if (!sx1509_write_reg(i2c_port, addr, SX1509_REG_DEBOUNCE_ENABLE_B, 0xFF)) {
        printf("SX1509: Failed to enable debounce B\n");
        return false;
    }

    // 2b. Debounce configuration (REG_DEBOUNCE_CONFIG): set debounce time
    // Bits 0-2: Time, Bit 3: enable for reg A, Bit 4: enable for reg B
    if (!sx1509_write_reg(i2c_port, addr, SX1509_REG_DEBOUNCE_CONFIG, 0x05)) {
        printf("SX1509: Failed to configure debounce\n");
        return false;
    }
    
    // 3. Configure keypad engine
    
    // KEY_CONFIG_1: sleep time and scan frequency
    // Bits 2-0: scan time per row, Bit 3 unused, Bits 6-4: auto sleep time  (no key press within this time will set keypad engine to sleep) 
    if (!sx1509_write_reg(i2c_port, addr, SX1509_REG_KEY_CONFIG_1, 0x75)) {
        printf("SX1509: Failed to configure keypad scan\n");
        return false;
    }

    // KEY_CONFIG_2: number of rows and columns
    // Bits 2-0: columns (0 = 1 column, 1 = 2 columns, etc.), Bits 5-3: rows
    uint8_t key_config2 = ((SX1509_ROWS - 1) << 3) | (SX1509_COLS - 1);
    if (!sx1509_write_reg(i2c_port, addr, SX1509_REG_KEY_CONFIG_2, key_config2)) {
        printf("SX1509: Failed to configure keypad rows/cols\n");
        return false;
    }
    
    printf("SX1509:Initialization successful\n");
    return true;
}

bool sx1509_init_led(i2c_inst_t *i2c_port, uint8_t addr) {

    // Disable input buffer (RegInputDisable)
    if (!sx1509_write_reg(i2c_port, addr, SX1509_REG_INPUT_DISABLE_A, 0xFF)) {
        printf("SX1509: Failed to set input disable A\n");
        return false;
    }
    if (!sx1509_write_reg(i2c_port, addr, SX1509_REG_INPUT_DISABLE_B, 0xFF)) {
        printf("SX1509: Failed to set input disable B\n");
        return false;
    }
    // Disable pull-up (RegPullUp)
    if (!sx1509_write_reg(i2c_port, addr, SX1509_REG_PULL_UP_A, 0x00)) {
        printf("SX1509: Failed to reset device\n");
        return false;
    }
    if (!sx1509_write_reg(i2c_port, addr, SX1509_REG_PULL_UP_B, 0x00)) {
        printf("SX1509: Failed to reset device\n");
        return false;
    }
    // Enable open drain (RegOpenDrain)
    if (!sx1509_write_reg(i2c_port, addr, SX1509_REG_OPEN_DRAIN_A, 0xFF)) {
        printf("SX1509: Failed to reset device\n");
        return false;
    }
    if (!sx1509_write_reg(i2c_port, addr, SX1509_REG_OPEN_DRAIN_B, 0xFF)) {
        printf("SX1509: Failed to reset device\n");
        return false;
    }
    // Set direction to output (RegDir) – by default RegData is set high => LED OFF
    if (!sx1509_write_reg(i2c_port, addr, SX1509_REG_DIR_A, 0x00)) {
        printf("SX1509: Failed to reset device\n");
        return false;
    }
    if (!sx1509_write_reg(i2c_port, addr, SX1509_REG_DIR_B, 0x00)) {
        printf("SX1509: Failed to reset device\n");
        return false;
    }
    // Enable oscillator (RegClock)
    if (!sx1509_write_reg(i2c_port, addr, SX1509_REG_CLOCK, 0x40)) {
        printf("SX1509: Failed to reset device\n");
        return false;
    }
    // Configure LED driver clock and mode if relevant (RegMisc)
    if (!sx1509_write_reg(i2c_port, addr, SX1509_REG_MISC, 0xAC)) {
        printf("SX1509: Failed to reset device\n");
        return false;
    }
    //Enable LED driver operation (RegLEDDriverEnable)
    if (!sx1509_write_reg(i2c_port, addr, SX1509_REG_LED_DRIVER_ENABLE_A, 0xFF)) {
        printf("SX1509: Failed to reset device\n");
        return false;
    }
    if (!sx1509_write_reg(i2c_port, addr, SX1509_REG_LED_DRIVER_ENABLE_B, 0xFF)) {
        printf("SX1509: Failed to reset device\n");
        return false;
    }
    // Configure LED driver parameters (RegTOn, RegIOn, RegOff, RegTRise, RegTFall)
    if (!sx1509_write_reg(i2c_port, addr, SX1509_REG_T_ON_4, 0x00)) {
        printf("SX1509: Failed to reset device\n");
        return false;        
    }
    if (!sx1509_write_reg(i2c_port, addr, SX1509_REG_I_ON_4, 0xFF)) {
        printf("SX1509: Failed to reset device\n");
        return false;
    }
    if (!sx1509_write_reg(i2c_port, addr, SX1509_REG_OFF_4, 0x00)) {
        printf("SX1509: Failed to reset device\n");
        return false;
    }    
    if (!sx1509_write_reg(i2c_port, addr, SX1509_REG_T_RISE_4, 0x08)) {
        printf("SX1509: Failed to reset device\n");
        return false;
    }     
    if (!sx1509_write_reg(i2c_port, addr, SX1509_REG_T_FALL_4, 0x08)) {
        printf("SX1509: Failed to reset device\n");
        return false;
    }  
 //   // Set RegData bit low => LED driver started 
 //   if (!sx1509_write_reg(i2c_port, addr, SX1509_REG_DATA_A, 0x10)) {
 //       printf("SX1509: Failed to reset device\n");
 //       return false;
 //   }  
    

    
    printf("SX1509:Initialization successful\n");
    return true;
}

bool sx1509_read_keypad(i2c_inst_t *i2c_port, uint8_t addr, uint8_t *key_data1, uint8_t *key_data2) {
    if (!key_data1 || !key_data2) return false;
    
    if (!sx1509_read_reg(i2c_port, addr, SX1509_REG_KEY_DATA_1, key_data1)) {
        return false;
    }
    
    if (!sx1509_read_reg(i2c_port, addr, SX1509_REG_KEY_DATA_2, key_data2)) {
        return false;
    }
    
    return true;
}

bool sx1509_read_gpio(i2c_inst_t *i2c_port, uint8_t addr, uint8_t *port_a, uint8_t *port_b) {
    if (!port_a || !port_b) return false;
    
    if (!sx1509_read_reg(i2c_port, addr, SX1509_REG_DATA_A, port_a)) {
        return false;
    }
    
    if (!sx1509_read_reg(i2c_port, addr, SX1509_REG_DATA_B, port_b)) {
        return false;
    }
    
    return true;
}

bool sx1509_write_gpio(i2c_inst_t *i2c_port, uint8_t addr, uint8_t port_a, uint8_t port_b) {
    if (!sx1509_write_reg(i2c_port, addr, SX1509_REG_DATA_A, port_a)) {
        return false;
    }
    
    if (!sx1509_write_reg(i2c_port, addr, SX1509_REG_DATA_B, port_b)) {
        return false;
    }
    
    return true;
}

bool sx1509_set_pin_direction(i2c_inst_t *i2c_port, uint8_t addr, uint8_t pin, bool is_output) {
    if (pin > 15) return false;
    
    uint8_t port_reg = (pin < 8) ? SX1509_REG_DIR_A : SX1509_REG_DIR_B;
    uint8_t pin_bit = pin % 8;
    
    // Read current direction
    uint8_t current;
    if (!sx1509_read_reg(i2c_port, addr, port_reg, &current)) {
        return false;
    }
    
    // Modify bit (0 = output, 1 = input)
    if (is_output) {
        current &= ~(1 << pin_bit);  // Clear bit for output
    } else {
        current |= (1 << pin_bit);   // Set bit for input
    }
    
    return sx1509_write_reg(i2c_port, addr, port_reg, current);
}

uint8_t SX1509_getRow(uint8_t *keyData2)
{
    uint8_t rowData = *keyData2;

    for (uint8_t i = 0; i < 8; i++)
    {
        if (!(rowData & (1 << i)))  // Check if bit is 0
            return i;
    }
    return 0;
}

uint8_t SX1509_getCol(uint8_t *keyData1)
{
	uint8_t colData = *keyData1;

	for (uint8_t i = 0; i < 8; i++)
	{
		if (!(colData & (1 << i)))  // Check if bit is 0
			return i;
	}
	return 0;
}