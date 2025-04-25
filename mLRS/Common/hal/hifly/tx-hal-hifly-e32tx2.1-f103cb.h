//*******************************************************
// Copyright (c) MLRS project
// GPL3
// https://www.gnu.org/licenses/gpl-3.0.de.html
// OlliW @ www.olliw.eu
//*******************************************************
// hal
//*********************************************************

//-------------------------------------------------------
// TX DIY HIFLY-DE28 v2.0 STM32F103CB
//-------------------------------------------------------
// #define DEVICE_HAS_SERIAL_OR_COM
#define DEVICE_HAS_NO_COM
#define DEVICE_HAS_JRPIN5 // requires diode from Tx to Rx soldered on the board
// #define DEVICE_HAS_DIVERSITYs
#define DEVICE_HAS_IN
#define DEVICE_HAS_BUZZER
#define DEVICE_HAS_FAN_ONOFF

//-- Timers, Timing, EEPROM, and such stuff

#define DELAY_USE_DWT

#define EE_START_PAGE             124 // 128 kB flash, 1 kB page

#define MICROS_TIMx               TIM3

//#define CLOCK_TIMx TIM2
//#define CLOCK_IRQn TIM2_IRQn
//#define CLOCK_IRQHandler TIM2_IRQHandler
// #define CLOCK_IRQ_PRIORITY        10

//-- UARTS
// UARTB = serial port
// UARTC = USB (CLI) or debug port
// UART = JR bay pin5 (SPORT)
// UARTE = in port, SBus or whatever

#define UARTB_USE_UART1_PA9PA10 // serial
#define UARTB_BAUD TX_SERIAL_BAUDRATE
#define UARTB_USE_TX
#define UARTB_TXBUFSIZE TX_SERIAL_TXBUFSIZE
#define UARTB_USE_TX_ISR
#define UARTB_USE_RX
#define UARTB_RXBUFSIZE TX_SERIAL_RXBUFSIZE

#ifdef DEVICE_HAS_IN
#define UARTE_USE_UART3_PB10PB11   // in pin
#define UARTE_BAUD 100000 // SBus normal baud rate, is being set later anyhow
#define UARTE_USE_RX
#define UARTE_RXBUFSIZE 512
#endif

#define UART_USE_UART2_PA2PA3 // JR pin5, MBridge
#define UART_BAUD 400000
#define UART_USE_TX
#define UART_TXBUFSIZE 512
#define UART_USE_TX_ISR
#define UART_USE_RX
#define UART_RXBUFSIZE 512

// #define JRPIN5_TX_XOR             IO_PB9
// #define JRPIN5_TX_SET_NORMAL      gpio_low(JRPIN5_TX_XOR)
// #define JRPIN5_TX_SET_INVERTED    gpio_high(JRPIN5_TX_XOR)
// #define JRPIN5_RX_XOR             IO_PC15
// #define JRPIN5_RX_SET_NORMAL      gpio_low(JRPIN5_RX_XOR)
// #define JRPIN5_RX_SET_INVERTED    gpio_high(JRPIN5_RX_XOR)

// #define UARTF_USE_UART1 // debug
// #define UARTF_BAUD                115200
// #define UARTF_USE_TX
// #define UARTF_TXBUFSIZE           512
// #define UARTF_USE_TX_ISR
// #define UARTF_USE_RX
// #define UARTF_RXBUFSIZE           512

//-- SX12xx II & SPI

#define SPI_USE_SPI2 // PB13, PB14, PB15
#define SPI_CS_IO IO_PB12
#define SPI_USE_CLK_LOW_1EDGE // datasheet says CPHA = 0  CPOL = 0
#define SPI_USE_CLOCKSPEED_9MHZ

#define SX_RESET IO_PB3
#define SX_DIO0 IO_PB9
#define SX_RX_EN IO_PB4
#define SX_TX_EN IO_PB5

#define SX_DIO0_GPIO_AF_EXTI_PORTx LL_GPIO_AF_EXTI_PORTB
#define SX_DIO0_GPIO_AF_EXTI_LINEx LL_GPIO_AF_EXTI_LINE9
#define SX_DIO_EXTI_LINE_x LL_EXTI_LINE_9
#define SX_DIO_EXTI_IRQn EXTI9_5_IRQn
#define SX_DIO_EXTI_IRQHandler EXTI9_5_IRQHandler
// #define SX_DIO_EXTI_IRQ_PRIORITY   11

void sx_init_gpio(void)
{
    gpio_init(SX_RESET, IO_MODE_OUTPUT_PP_HIGH, IO_SPEED_VERYFAST);
    gpio_init(SX_DIO0, IO_MODE_INPUT_PD, IO_SPEED_VERYFAST);
    gpio_init(SX_TX_EN, IO_MODE_OUTPUT_PP_LOW, IO_SPEED_VERYFAST);
    gpio_init(SX_RX_EN, IO_MODE_OUTPUT_PP_LOW, IO_SPEED_VERYFAST);
}

void sx_amp_transmit(void)
{
    gpio_low(SX_RX_EN);
    gpio_high(SX_TX_EN);
}

void sx_amp_receive(void)
{
    gpio_low(SX_TX_EN);
    gpio_high(SX_RX_EN);
}

void sx_dio_init_exti_isroff(void)
{
    LL_GPIO_AF_SetEXTISource(SX_DIO0_GPIO_AF_EXTI_PORTx, SX_DIO0_GPIO_AF_EXTI_LINEx);

    // let's not use LL_EXTI_Init(), but let's do it by hand, is easier to allow enabling isr later
    LL_EXTI_DisableEvent_0_31(SX_DIO_EXTI_LINE_x);
    LL_EXTI_DisableIT_0_31(SX_DIO_EXTI_LINE_x);
    LL_EXTI_DisableFallingTrig_0_31(SX_DIO_EXTI_LINE_x);
    LL_EXTI_EnableRisingTrig_0_31(SX_DIO_EXTI_LINE_x);

    NVIC_SetPriority(SX_DIO_EXTI_IRQn, SX_DIO_EXTI_IRQ_PRIORITY);
    NVIC_EnableIRQ(SX_DIO_EXTI_IRQn);
}

void sx_dio_enable_exti_isr(void)
{
    LL_EXTI_ClearFlag_0_31(SX_DIO_EXTI_LINE_x);
    LL_EXTI_EnableIT_0_31(SX_DIO_EXTI_LINE_x);
}

void sx_dio_exti_isr_clearflag(void)
{
    LL_EXTI_ClearFlag_0_31(SX_DIO_EXTI_LINE_x);
}

//-- In port

// #define IN_XOR                    IO_PC15

void in_init_gpio(void)
{
    // gpio_init(IN_XOR, IO_MODE_OUTPUT_PP_LOW, IO_SPEED_VERYFAST);
    // gpio_low(IN_XOR);
}

void in_set_normal(void)
{
    // gpio_low(IN_XOR);
}

void in_set_inverted(void)
{
    // gpio_high(IN_XOR);
}

//-- Button

#define BUTTON IO_PA0

void button_init(void)
{
    // gpio_init(BUTTON, IO_MODE_INPUT_PU, IO_SPEED_DEFAULT);
}

bool button_pressed(void)
{
    return 0;
    // return gpio_read_activelow(BUTTON);
}

//-- LEDs

#define LED_GREEN IO_PC15
#define LED_RED IO_PC14

void leds_init(void)
{
    gpio_init(LED_GREEN, IO_MODE_OUTPUT_PP_LOW, IO_SPEED_DEFAULT);
    gpio_init(LED_RED, IO_MODE_OUTPUT_PP_LOW, IO_SPEED_DEFAULT);
    gpio_low(LED_GREEN); // LED_GREEN_OFF
    gpio_low(LED_RED);   // LED_RED_OFF
}

void led_green_off(void) { gpio_low(LED_GREEN); }
void led_green_on(void) { gpio_high(LED_GREEN); }
void led_green_toggle(void) { gpio_toggle(LED_GREEN); }

void led_red_off(void) { gpio_low(LED_RED); }
void led_red_on(void) { gpio_high(LED_RED); }
void led_red_toggle(void) { gpio_toggle(LED_RED); }

//-- Position Switch

void pos_switch_init(void)
{
}

//-- 5 Way Switch
// PC2: resistor chain Vcc - 3.3k - down - 3.3k - left - 3.3k - right - 3.3k - up
// PC13: center

// #define FIVEWAY_SWITCH_CENTER     IO_PC13
#define FIVEWAY_ADCx ADC1     // could also be ADC1
#define FIVEWAY_ADC_IO IO_PA0 // ADC_IN0
#define FIVEWAY_ADC_CHANNELx LL_ADC_CHANNEL_0

#ifdef DEVICE_HAS_I2C_DISPLAY_ROT180
extern "C"
{
    void delay_us(uint32_t us);
}

void fiveway_init(void)
{
    // gpio_init(FIVEWAY_SWITCH_CENTER, IO_MODE_INPUT_PU, IO_SPEED_DEFAULT);
    adc_init_begin(FIVEWAY_ADCx);
    adc_init_one_channel(FIVEWAY_ADCx);
    adc_config_channel(FIVEWAY_ADCx, LL_ADC_REG_RANK_1, FIVEWAY_ADC_CHANNELx, FIVEWAY_ADC_IO);
    adc_enable(FIVEWAY_ADCx);
    delay_us(100);
    adc_start_conversion(FIVEWAY_ADCx);
}

uint8_t fiveway_read(void)
{
    // uint8_t center_pressed = gpio_read_activelow(FIVEWAY_SWITCH_CENTER);
    uint16_t adc = LL_ADC_REG_ReadConversionData12(FIVEWAY_ADCx);
    if (adc > (1842 - 100) && adc < (1842 + 100))
        return (1 << KEY_UP); // 655
    if (adc > (2894 - 100) && adc < (2894 + 100))
        return (1 << KEY_DOWN); // 15552.312.311.921.2
    if (adc > (2431 - 100) && adc < (2431 + 100))
        return (1 << KEY_LEFT); // 2555
    if (adc > (2720 - 100) && adc < (2720 + 100))
        return (1 << KEY_RIGHT); // 3555
    return ((adc < (0 + 200)) << KEY_CENTER);
}
#endif

//-- Display I2C

#define I2C_USE_I2C1          // PB6, PB7
#define I2C_CLOCKSPEED_400KHZ // not all displays seem to work well with I2C_CLOCKSPEED_1000KHZ
#define I2C_USE_DMAMODE

//-- Buzzer

#define BUZZER                    IO_PA8
#define BUZZER_TIMx               TIM1
#define BUZZER_IRQn               TIM1_UP_TIM16_IRQn
#define BUZZER_IRQHandler         TIM1_UP_TIM16_IRQHandler
#define BUZZER_TIM_CHANNEL        LL_TIM_CHANNEL_CH1
// #define BUZZER_TIM_IRQ_PRIORITY   14s

//-- Cooling Fan

#define FAN_IO IO_PC13

void fan_init(void)
{
    gpio_init(FAN_IO, IO_MODE_OUTPUT_PP_LOW, IO_SPEED_DEFAULT);
    gpio_low(FAN_IO);
}

void fan_set_power(int8_t power_dbm)
{
    if (power_dbm >= POWER_23_DBM)
    {
        gpio_high(FAN_IO);
    }
    else
    {
        gpio_low(FAN_IO);
    }
}

//-- POWER
#define POWER_GAIN_DBM            18 //Use 17 for E23-900M30S
#define POWER_SX1276_MAX_DBM      SX1276_OUTPUT_POWER_MAX // maximum allowed sx power, 17 dBm for SX127x
#define POWER_USE_DEFAULT_RFPOWER_CALC

#define RFPOWER_DEFAULT 1 // index into rfpower_list array

const rfpower_t rfpower_list[] = {
    {.dbm = POWER_10_DBM, .mW = 10},
    {.dbm = POWER_20_DBM, .mW = 100},
    {.dbm = POWER_24_DBM, .mW = 250},
    {.dbm = POWER_27_DBM, .mW = 500},
    {.dbm = POWER_30_DBM, .mW = 1000},
};

//-- TEST

uint32_t porta[] = {
    LL_GPIO_PIN_0,
    LL_GPIO_PIN_1,
    LL_GPIO_PIN_2,
    LL_GPIO_PIN_3,
    LL_GPIO_PIN_4,
    LL_GPIO_PIN_5,
    LL_GPIO_PIN_6,
    LL_GPIO_PIN_7,
    LL_GPIO_PIN_8,
    LL_GPIO_PIN_9,
    LL_GPIO_PIN_10,
    LL_GPIO_PIN_15,
};

uint32_t portb[] = {
    LL_GPIO_PIN_0,
    LL_GPIO_PIN_1,
    LL_GPIO_PIN_3,
    LL_GPIO_PIN_4,
    LL_GPIO_PIN_5,
    LL_GPIO_PIN_6,
    LL_GPIO_PIN_7,
    LL_GPIO_PIN_8,
    LL_GPIO_PIN_9,
    LL_GPIO_PIN_10,
    LL_GPIO_PIN_11,
    LL_GPIO_PIN_12,
    LL_GPIO_PIN_13,
    LL_GPIO_PIN_14,
    LL_GPIO_PIN_15,
};

uint32_t portc[] = {
    LL_GPIO_PIN_13,
    LL_GPIO_PIN_14,
    LL_GPIO_PIN_15,
};
