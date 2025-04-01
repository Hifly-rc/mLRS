//*******************************************************
// Copyright (c) MLRS project
// GPL3
// https://www.gnu.org/licenses/gpl-3.0.de.html
//*******************************************************
// hal
//*******************************************************

/*
  Info on DIP switches
   
  1,2 on:   update firmware on main ESP32, USB is connected to UARTO 
  3,4 on:   normal operation mode, USB isn't used, UARTO connected to ESP8285
  5,6,7 on: update firmware on ESP8285, USB is connected to ESP8285's UART

  Flashing ESP8285:
  - Board: Generic ESP8266 Module
  - Upload Speed: can be 921600
  - Reset Method: dtr (aka modemcu)
*/

//-------------------------------------------------------
// ESP32, ELRS BETAFPV MICRO 1W 2400 TX
//-------------------------------------------------------
// https://github.com/ExpressLRS/targets/blob/master/TX/BETAFPV%202400%20Micro%201W.json

#define DEVICE_HAS_JRPIN5
#define DEVICE_HAS_SERIAL_OR_COM // hold 5-way in down direction at boot to enable CLI
#define DEVICE_HAS_IN
#define DEVICE_HAS_NO_DEBUG
#define DEVICE_HAS_SINGLE_LED_RGB
#define DEVICE_HAS_I2C_DISPLAY_ROT180
#define DEVICE_HAS_FAN_ONOFF

// Note on SERIAL_OR_COM:
// The com uart is not initialized, the serial uart is, So, buffers are set as by the RX/TXBUFSIZE defines for serial.
// The TXBUFSIZE setting for the com affects however the CLI's chunkenizer behavior.


//-- UARTS
// UARTB = serial port
// UARTC or USB = COM (CLI)
// UARTD = serial2 BT/ESP port
// UART  = JR bay pin5
// UARTE = in port, SBus or whatever
// UARTF = debug port

#define UARTB_USE_SERIAL // serial
#define UARTB_BAUD                TX_SERIAL_BAUDRATE
#define UARTB_USE_TX_IO           IO_P1
#define UARTB_USE_RX_IO           IO_P3
#define UARTB_TXBUFSIZE           TX_SERIAL_TXBUFSIZE
#define UARTB_RXBUFSIZE           TX_SERIAL_RXBUFSIZE

#define UARTC_USE_SERIAL // COM (CLI)
#define UARTC_BAUD                115200
#define UARTC_USE_TX_IO           IO_P1
#define UARTC_USE_RX_IO           IO_P3
#define UARTC_TXBUFSIZE           0  // TX FIFO = 128
#define UARTC_RXBUFSIZE           TX_COM_RXBUFSIZE

#define UART_USE_SERIAL1 // JR bay pin5
#define UART_BAUD                 400000
#define UART_USE_TX_IO            IO_P13
#define UART_USE_RX_IO            IO_P13
#define UART_TXBUFSIZE            0  // TX FIFO = 128
#define UART_RXBUFSIZE            0  // RX FIFO = 128 + 1

#define UARTE_USE_SERIAL1 // in port, uses JRPin5
#define UARTE_BAUD                 100000
#define UARTE_USE_TX_IO            -1
#define UARTE_USE_RX_IO            IO_P13
#define UARTE_RXBUFSIZE            0 // RX FIFO = 128 + 1


//-- SX1: SX12xx & SPI

#define SPI_CS_IO                 IO_P5
#define SPI_MISO                  IO_P19
#define SPI_MOSI                  IO_P23
#define SPI_SCK                   IO_P18
#define SPI_FREQUENCY             18000000L
#define SX_RESET                  IO_P14
#define SX_BUSY                   IO_P21
#define SX_DIO1                   IO_P4
#define SX_TX_EN                  IO_P26
#define SX_RX_EN                  IO_P27

IRQHANDLER(void SX_DIO_EXTI_IRQHandler(void);)

void sx_init_gpio(void)
{
    gpio_init(SX_DIO1, IO_MODE_INPUT_ANALOG);
    gpio_init(SX_BUSY, IO_MODE_INPUT_PU);
    gpio_init(SX_TX_EN, IO_MODE_OUTPUT_PP_LOW);
    gpio_init(SX_RX_EN, IO_MODE_OUTPUT_PP_LOW);
    gpio_init(SX_RESET, IO_MODE_OUTPUT_PP_HIGH);
}

IRAM_ATTR bool sx_busy_read(void)
{
    return (gpio_read_activehigh(SX_BUSY)) ? true : false;
}

IRAM_ATTR void sx_amp_transmit(void)
{
    gpio_low(SX_RX_EN);
    gpio_high(SX_TX_EN);
}

IRAM_ATTR void sx_amp_receive(void)
{
    gpio_low(SX_TX_EN);
    gpio_high(SX_RX_EN);
}

void sx_dio_enable_exti_isr(void)
{
    attachInterrupt(SX_DIO1, SX_DIO_EXTI_IRQHandler, RISING);
}

void sx_dio_init_exti_isroff(void)
{
    detachInterrupt(SX_DIO1);
}

void sx_dio_exti_isr_clearflag(void) {}


//-- In port

void in_init_gpio(void) {}

void in_set_normal(void) { gpio_matrix_in((gpio_num_t)UARTE_USE_RX_IO, U1RXD_IN_IDX, false); }

void in_set_inverted(void) { gpio_matrix_in((gpio_num_t)UARTE_USE_RX_IO, U1RXD_IN_IDX, true); }


//-- Button

void button_init(void) {}
IRAM_ATTR bool button_pressed(void) { return false; }


//-- LEDs
#include <NeoPixelBus.h>
#define LED_RED                   IO_P16
bool ledRedState;
bool ledGreenState;
bool ledBlueState;

NeoPixelBus<NeoGrbFeature, NeoEsp32I2s0Ws2812xMethod> ledRGB(1, LED_RED);

void leds_init(void)
{
    ledRGB.Begin();
    ledRGB.Show();
}

IRAM_ATTR void led_red_off(void)
{
    if (!ledRedState) return;
    ledRGB.SetPixelColor(0, RgbColor(0, 0, 0));
    ledRGB.Show();
    ledRedState = 0;
}

IRAM_ATTR void led_red_on(void)
{
    if (ledRedState) return;
    ledRGB.SetPixelColor(0, RgbColor(255, 0, 0));
    ledRGB.Show();
    ledRedState = 1;
}

IRAM_ATTR void led_red_toggle(void)
{
    if (ledRedState) { led_red_off(); } else { led_red_on(); }
}

IRAM_ATTR void led_green_off(void)
{
    if (!ledGreenState) return;
    ledRGB.SetPixelColor(0, RgbColor(0, 0, 0));
    ledRGB.Show();
    ledGreenState = 0;
}

IRAM_ATTR void led_green_on(void)
{
    if (ledGreenState) return;
    ledRGB.SetPixelColor(0, RgbColor(0, 255, 0));
    ledRGB.Show();
    ledGreenState = 1;
}

IRAM_ATTR void led_green_toggle(void)
{
    if (ledGreenState) { led_green_off(); } else { led_green_on(); }
}

IRAM_ATTR void led_blue_off(void)
{
    if (!ledBlueState) return;
    ledRGB.SetPixelColor(0, RgbColor(0, 0, 0));
    ledRGB.Show();
    ledBlueState = 0;
}

IRAM_ATTR void led_blue_on(void)
{
    if (ledBlueState) return;
    ledRGB.SetPixelColor(0, RgbColor(0, 0, 255));
    ledRGB.Show();
    ledBlueState = 1;
}

IRAM_ATTR void led_blue_toggle(void)
{
    if (ledBlueState) { led_blue_off(); } else { led_blue_on(); }
}


//-- Display I2C

#define I2C_SDA_IO                IO_P33
#define I2C_SCL_IO                IO_P32
#define I2C_CLOCKSPEED            1000000L  // fix - rather too much, but helps with LQ, ESP32 max speed
#define I2C_BUFFER_SIZE           1024


//-- 5 Way Switch
//"joystick_values": [814,2108,1422,1822,0,4095],
#define FIVEWAY_ADC_IO            IO_P0
#define KEY_DOWN_THRESH           2108
#if defined DEVICE_HAS_I2C_DISPLAY || defined DEVICE_HAS_I2C_DISPLAY_ROT180 || defined DEVICE_HAS_FIVEWAY

void fiveway_init(void) {} // no init needed to read an analog pin in Arduino

IRAM_ATTR uint16_t fiveway_adc_read(void)
{
    return analogRead(FIVEWAY_ADC_IO);
}

IRAM_ATTR uint8_t fiveway_read(void)
{
    int16_t adc = analogRead(FIVEWAY_ADC_IO);
    // BetaFPV 1W Micro seems to have pretty widely varying resistor values, 
    // so we do a less strict method here
    // Attention: this needs to be ordered!
    // uint8_t center_pressed = gpio_read_activelow(FIVEWAY_SWITCH_CENTER);
    if (adc > (814 - 100) && adc < (814 + 100)) return (1 << KEY_UP); // 655
    if (adc > (2108 - 100) && adc < (2108 + 100)) return (1 << KEY_DOWN); // 15552.312.311.921.2
    if (adc > (1422 - 100) && adc < (1422 + 100)) return (1 << KEY_LEFT); // 2555
    if (adc > (1822 - 100) && adc < (1822 + 100)) return (1 << KEY_RIGHT); // 3555
    if (adc < 100) return (1 << KEY_CENTER);
    return 0;
}
#endif


//-- Serial or Com Switch
// use com if FIVEWAY is DOWN during power up, else use serial
// FIVEWAY-DONW becomes bind button later on

#ifdef DEVICE_HAS_SERIAL_OR_COM
bool tx_ser_or_com_serial = true; // we use serial as default

void ser_or_com_init(void)
{
    uint8_t cnt = 0;
    for (uint8_t i = 0; i < 16; i++) {
        uint16_t adc = analogRead(FIVEWAY_ADC_IO);
        if (adc > (KEY_DOWN_THRESH-250) && adc < (KEY_DOWN_THRESH+250)) cnt++;
    }
    tx_ser_or_com_serial = !(cnt > 8);
}

IRAM_ATTR bool ser_or_com_serial(void)
{
    return tx_ser_or_com_serial;
}
#endif


//-- Cooling Fan

#define FAN_IO                    IO_P17

void fan_init(void)
{
    gpio_init(FAN_IO, IO_MODE_OUTPUT_PP_LOW);
    gpio_low(FAN_IO);
}

IRAM_ATTR void fan_set_power(int8_t power_dbm)
{
    if (power_dbm >= POWER_23_DBM) {
        gpio_high(FAN_IO);
    } else {
        gpio_low(FAN_IO);
    }
}


//-- POWER

#define POWER_PA_E28_2G4M27SX
#include "../hal-power-pa.h"

