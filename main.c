#include <hardware/i2c.h>
#include <hardware/pwm.h>
#include <pico/i2c_slave.h>
#include <pico/stdlib.h>
#include <stdio.h>
#include <string.h>

#define PWM_BASE_PIN 2

static const uint I2C_SLAVE_ADDRESS = 0x17;
static const uint I2C_BAUDRATE = 100000; // 100 kHz

static const uint I2C_SLAVE_SDA_PIN = 0;
static const uint I2C_SLAVE_SCL_PIN = 1;

static struct {
	uint8_t pwm_registers[16];
	uint8_t pwm_reg_addr;
	bool pwm_reg_addr_written;
} memory;

static void i2c_slave_handler(i2c_inst_t *i2c, i2c_slave_event_t event) {
	switch (event) {
		case I2C_SLAVE_RECEIVE:
			if(!memory.pwm_reg_addr_written) {
				memory.pwm_reg_addr = i2c_read_byte_raw(i2c) & 0b00001111;
				memory.pwm_reg_addr_written = true;
			} else {
				memory.pwm_registers[memory.pwm_reg_addr] = i2c_read_byte_raw(i2c);
			}
			break;
		case I2C_SLAVE_REQUEST:
			i2c_write_byte_raw(i2c, memory.pwm_registers[memory.pwm_reg_addr]);
			memory.pwm_reg_addr++;
			break;
		case I2C_SLAVE_FINISH:
			memory.pwm_reg_addr_written = false;
			break;
		default:
			break;
	}
}

static void setup_slave() {
    gpio_init(I2C_SLAVE_SDA_PIN);
    gpio_set_function(I2C_SLAVE_SDA_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SLAVE_SDA_PIN);

    gpio_init(I2C_SLAVE_SCL_PIN);
    gpio_set_function(I2C_SLAVE_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SLAVE_SCL_PIN);

    i2c_init(i2c0, I2C_BAUDRATE);

    i2c_slave_init(i2c0, I2C_SLAVE_ADDRESS, &i2c_slave_handler);
}

static void setup_pwm() {
	for (int pin_number = PWM_BASE_PIN; pin_number < PWM_BASE_PIN+16; pin_number++) {
		gpio_set_function(pin_number, GPIO_FUNC_PWM);
		pwm_set_gpio_level(pin_number, 0);
	}

	for (int slice_num = 0; slice_num < 8; slice_num++) {
		pwm_set_wrap(slice_num, 255);
		pwm_set_enabled(slice_num, true);
	}
}

int main() {
	stdio_init_all();
	printf("Initiated stdio_all");
	setup_pwm();
	printf("Initiated PWM");
	setup_slave();
	printf("Initiated slave");
	while (1) {
		for (int reg_address = 0; reg_address < 16; reg_address++) {
			pwm_set_gpio_level(PWM_BASE_PIN+reg_address, memory.pwm_registers[reg_address]);
		}
	};
}

