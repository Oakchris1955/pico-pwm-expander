#include <hardware/i2c.h>
#include <pico/i2c_slave.h>
#include <pico/stdlib.h>
#include <stdio.h>
#include <string.h>


static const uint I2C_SLAVE_ADDRESS = 0x17;
static const uint I2C_BAUDRATE = 100000; // 100 kHz

static const uint I2C_SLAVE_SDA_PIN = 2;
static const uint I2C_SLAVE_SCL_PIN = 3;

static struct {
	uint8_t pwm_registers[16];
	uint8_t pwm_reg_addr;
	bool pwm_reg_addr_written;
} memory;

static void i2c_slave_handler(i2c_inst_t *i2c, i2c_slave_event_t event) {
	switch (event) {
		case I2C_SLAVE_RECEIVE:
			if(!memory.pwm_reg_addr_written) {
				memory.pwm_reg_addr = i2c_read_byte_raw(i2c);
				memory.pwm_reg_addr_written = true;
			} else {
				memory.pwm_registers[memory.pwm_reg_addr >> 4] = i2c_read_byte_raw(i2c);
			}
			break;
		case I2C_SLAVE_REQUEST:
			i2c_write_byte_raw(i2c, memory.pwm_registers[memory.pwm_reg_addr >> 4]);
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

    i2c_init(i2c1, I2C_BAUDRATE);

    i2c_slave_init(i2c1, I2C_SLAVE_ADDRESS, &i2c_slave_handler);
}


int main() {
	stdio_init_all();
	puts("Initiated stdio_all");
	setup_slave();
	puts("Initiated slave");
	while (1) {};
}
