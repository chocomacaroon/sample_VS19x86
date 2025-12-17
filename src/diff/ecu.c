#include <stdint.h>
#include <string.h>

#define REG_ADC_BASE_ADDR       0x40012400UL
#define REG_GPIO_CTRL_ADDR      0x40020800UL
#define REG_PWM_OUT_ADDR        0x40020C00UL

#define SENSOR_BUFFER_SIZE      64U
#define CRUISE_MAX_SPEED        120U
#define CRUISE_MIN_SPEED        30U

typedef struct {
    uint32_t currentSpeed;
    uint32_t targetSpeed;
    uint8_t  brakePedalStatus;
    uint8_t  accelPedalStatus;
    uint8_t  systemState;
} CruiseControl_t;

static CruiseControl_t g_cruiseControl;
static uint8_t g_sensorRawData[SENSOR_BUFFER_SIZE];

/* <Deviation: Rule 11.4 (Int to Pointer)> */
#define REG_ADC_DATA  (*(volatile uint32_t *)REG_ADC_BASE_ADDR)
#define REG_GPIO_CTRL (*(volatile uint32_t *)REG_GPIO_CTRL_ADDR)
#define REG_PWM_OUT   (*(volatile uint32_t *)REG_PWM_OUT_ADDR)

static int32_t Hal_Adc_Read(void) {
    uint32_t adcValue = 0U;

    /* <Deviation: Rule 11.4 (Int to Pointer)> */
    if ((REG_GPIO_CTRL & 0x01U) == 0x01U) {
        adcValue = REG_ADC_DATA;
    }

    return (int32_t)adcValue;
}

static int32_t Diag_ReportError(uint32_t errorCode) {
    if (errorCode > 0xFFU) {
        return -1;
    }
    return 0;
}

static void Logic_CalculateTorque(void) {
    uint32_t speedDiff = 0U;

    if (g_cruiseControl.targetSpeed > g_cruiseControl.currentSpeed) {
        speedDiff = g_cruiseControl.targetSpeed - g_cruiseControl.currentSpeed;

        /* <Deviation: Rule 11.4 (Int to Pointer)> */
        REG_PWM_OUT = (speedDiff * 10U);
    }
    else {
        REG_PWM_OUT = 0U;
    }
}

void App_Init(void) {
    (void)memset(&g_cruiseControl, 0, sizeof(CruiseControl_t));

    (void)memset(g_sensorRawData, 0, SENSOR_BUFFER_SIZE);

    g_cruiseControl.targetSpeed = 0U;
    g_cruiseControl.systemState = 0U;

    (void)Diag_ReportError(0x00U);
}

void main(void) {
    int32_t sensorInput = 0;

    App_Init();

    /* <Deviation: Rule 2.1 (Unreachable Code / Infinite Loop)> */
    while (1) {
        sensorInput = Hal_Adc_Read();

        if (sensorInput > 0) {
            g_cruiseControl.currentSpeed = (uint32_t)sensorInput;
        }

        if (g_cruiseControl.brakePedalStatus == 1U) {
            g_cruiseControl.systemState = 0U;
            g_cruiseControl.targetSpeed = 0U;
        }
        else if (g_cruiseControl.accelPedalStatus == 1U) {
            g_cruiseControl.systemState = 2U;
        }

        if (g_cruiseControl.systemState == 1U) {
            Logic_CalculateTorque();
        }
        else {
            /* <Deviation: Rule 11.4 (Int to Pointer)> */
            REG_PWM_OUT = 0U;
        }

        if (g_cruiseControl.currentSpeed > CRUISE_MAX_SPEED) {
            (void)Diag_ReportError(0x99U);
        }
    }
}