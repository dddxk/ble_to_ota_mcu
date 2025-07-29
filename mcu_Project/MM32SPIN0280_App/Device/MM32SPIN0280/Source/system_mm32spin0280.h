///
/// @file     system_STAR.h
/// @brief    CMSIS Device System Header File
///

#ifndef SYSTEM_MM32_H
#define SYSTEM_MM32_H

#ifdef __cplusplus
extern "C" {
#endif

///
/// brief System Clock Frequency (Core Clock)
///
extern u32 SystemCoreClock;

///
/// brief Setup the microcontroller system.
///
/// Initialize the System and update the SystemCoreClock variable.
///
extern void SystemInit (void);

///
/// brief  Update SystemCoreClock variable.
///
/// Updates the SystemCoreClock with current core Clock retrieved from cpu registers.
///
extern void SystemCoreClockUpdate (void);

#ifdef __cplusplus
}
#endif

#endif // SYSTEM_MM32_H
