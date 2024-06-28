#ifndef NRF_I2C_DEFS
#define NRF_I2C_DEFS

#if defined(USE_I2C0)
#define I2C_IRQn UARTE0_SPIM0_SPIS0_TWIM0_TWIS0_IRQn
#define I2C_DEV NRF_TWIM0
#elif defined(USE_I2C1)
#define I2C_IRQn UARTE1_SPIM1_SPIS1_TWIM1_TWIS1_IRQn
#define I2C_DEV NRF_TWIM1
#elif defined(USE_I2C2)
#define I2C_IRQn UARTE2_SPIM2_SPIS2_TWIM2_TWIS2_IRQn
#define I2C_DEV NRF_TWIM2
#elif defined(USE_I2C3)
#define I2C_IRQn UARTE3_SPIM3_SPIS3_TWIM3_TWIS3_IRQn
#define I2C_DEV NRF_TWIM3
#else
#error USE_I2Cx (x=0-3) must be defined
#endif

#endif // NRF_I2C_DEFS
