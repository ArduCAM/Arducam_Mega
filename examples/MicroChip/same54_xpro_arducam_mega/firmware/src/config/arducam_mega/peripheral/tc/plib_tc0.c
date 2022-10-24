/*******************************************************************************
  Timer/Counter(TC0) PLIB

  Company
    Microchip Technology Inc.

  File Name
    plib_tc0.c

  Summary
    TC0 PLIB Implementation File.

  Description
    This file defines the interface to the TC peripheral library. This
    library provides access to and control of the associated peripheral
    instance.

  Remarks:
    None.

*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
/* This section lists the other files that are included in this file.
*/

#include "interrupts.h"
#include "plib_tc0.h" 

// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************

static TC_COMPARE_CALLBACK_OBJ TC0_CallbackObject;

// *****************************************************************************
// *****************************************************************************
// Section: TC0 Implementation
// *****************************************************************************
// *****************************************************************************

/* Initialize TC module in Compare Mode */
void TC0_CompareInitialize( void )
{
    /* Reset TC */
    TC0_REGS->COUNT16.TC_CTRLA = TC_CTRLA_SWRST_Msk;

    while((TC0_REGS->COUNT16.TC_SYNCBUSY & TC_SYNCBUSY_SWRST_Msk) == TC_SYNCBUSY_SWRST_Msk)
    {
        /* Wait for Write Synchronization */
    }

    /* Configure counter mode & prescaler */
    TC0_REGS->COUNT16.TC_CTRLA = TC_CTRLA_MODE_COUNT16 | TC_CTRLA_PRESCALER_DIV2 | TC_CTRLA_PRESCSYNC_PRESC ;

    /* Configure waveform generation mode */
    TC0_REGS->COUNT16.TC_WAVE = (uint8_t)TC_WAVE_WAVEGEN_MFRQ;

    /* Configure timer one shot mode & direction */
    TC0_REGS->COUNT16.TC_CTRLBSET = (uint8_t)(TC_CTRLBSET_DIR_Msk);

    TC0_REGS->COUNT16.TC_CC[0] = 3000U;

    /* Clear all interrupt flags */
    TC0_REGS->COUNT16.TC_INTFLAG = (uint8_t)TC_INTFLAG_Msk;

    /* Enable period Interrupt */
    TC0_CallbackObject.callback = NULL;
    TC0_REGS->COUNT16.TC_INTENSET = (uint8_t)(TC_INTENSET_MC0_Msk);

    TC0_REGS->COUNT16.TC_EVCTRL = (uint16_t)(TC_EVCTRL_OVFEO_Msk);
    while((TC0_REGS->COUNT16.TC_SYNCBUSY) != 0U)
    {
        /* Wait for Write Synchronization */
    }
}

/* Enable the counter */
void TC0_CompareStart( void )
{
    TC0_REGS->COUNT16.TC_CTRLA |= TC_CTRLA_ENABLE_Msk;
    while((TC0_REGS->COUNT16.TC_SYNCBUSY & TC_SYNCBUSY_ENABLE_Msk) == TC_SYNCBUSY_ENABLE_Msk)
    {
        /* Wait for Write Synchronization */
    }
}

/* Disable the counter */
void TC0_CompareStop( void )
{
    TC0_REGS->COUNT16.TC_CTRLA &= ~TC_CTRLA_ENABLE_Msk;
    while((TC0_REGS->COUNT16.TC_SYNCBUSY & TC_SYNCBUSY_ENABLE_Msk) == TC_SYNCBUSY_ENABLE_Msk)
    {
        /* Wait for Write Synchronization */
    }
}

uint32_t TC0_CompareFrequencyGet( void )
{
    return (uint32_t)(30000000UL);
}

void TC0_CompareCommandSet(TC_COMMAND command)
{
    TC0_REGS->COUNT16.TC_CTRLBSET = (uint8_t)((uint32_t)command << TC_CTRLBSET_CMD_Pos);
    while((TC0_REGS->COUNT16.TC_SYNCBUSY) != 0U)
    {
        /* Wait for Write Synchronization */
    }    
}

/* Get the current counter value */
uint16_t TC0_Compare16bitCounterGet( void )
{
    /* Write command to force COUNT register read synchronization */
    TC0_REGS->COUNT16.TC_CTRLBSET |= (uint8_t)TC_CTRLBSET_CMD_READSYNC;

    while((TC0_REGS->COUNT16.TC_SYNCBUSY & TC_SYNCBUSY_CTRLB_Msk) == TC_SYNCBUSY_CTRLB_Msk)
    {
        /* Wait for Write Synchronization */
    }

    while((TC0_REGS->COUNT16.TC_CTRLBSET & TC_CTRLBSET_CMD_Msk) != 0U)
    {
        /* Wait for CMD to become zero */
    }

    /* Read current count value */
    return (uint16_t)TC0_REGS->COUNT16.TC_COUNT;
}

/* Configure counter value */
void TC0_Compare16bitCounterSet( uint16_t count )
{
    TC0_REGS->COUNT16.TC_COUNT = count;

    while((TC0_REGS->COUNT16.TC_SYNCBUSY & TC_SYNCBUSY_COUNT_Msk) == TC_SYNCBUSY_COUNT_Msk)
    {
        /* Wait for Write Synchronization */
    }
}

/* Configure period value */
bool TC0_Compare16bitPeriodSet( uint16_t period )
{
    bool status = false;
    if((TC0_REGS->COUNT16.TC_STATUS & TC_STATUS_CCBUFV0_Msk) == 0U)
    {
        /* Configure period value */
        TC0_REGS->COUNT16.TC_CCBUF[0] = period;
        status = true;
    }
    return status;
}

/* Read period value */
uint16_t TC0_Compare16bitPeriodGet( void )
{
    /* Get period value */
    return (uint16_t)TC0_REGS->COUNT16.TC_CC[0];
}

/* Configure duty cycle value */
bool TC0_Compare16bitMatch0Set( uint16_t compareValue )
{
    bool status = false;
    if((TC0_REGS->COUNT16.TC_STATUS & TC_STATUS_CCBUFV0_Msk) == 0U)
    {
        /* Set new compare value for compare channel 0 */
        TC0_REGS->COUNT16.TC_CCBUF[0] = compareValue;
        status = true;
    }
    return status;
}

/* Configure duty cycle value */
bool TC0_Compare16bitMatch1Set( uint16_t compareValue )
{
    bool status = false;
    if((TC0_REGS->COUNT16.TC_STATUS & TC_STATUS_CCBUFV1_Msk) == 0U)
    {
        /* Set new compare value for compare channel 1 */
        TC0_REGS->COUNT16.TC_CCBUF[1] = compareValue;
        status = true;
    }
    return status;
}





/* Register callback function */
void TC0_CompareCallbackRegister( TC_COMPARE_CALLBACK callback, uintptr_t context )
{
    TC0_CallbackObject.callback = callback;

    TC0_CallbackObject.context = context;
}

/* Compare match interrupt handler */
void TC0_CompareInterruptHandler( void )
{
    if (TC0_REGS->COUNT16.TC_INTENSET != 0U)
    {
        TC_COMPARE_STATUS status;
        status = TC0_REGS->COUNT16.TC_INTFLAG;
        /* clear interrupt flag */
        TC0_REGS->COUNT16.TC_INTFLAG = (uint8_t)TC_INTFLAG_Msk;
        if((status != TC_COMPARE_STATUS_NONE) && (TC0_CallbackObject.callback != NULL))
        {
            TC0_CallbackObject.callback(status, TC0_CallbackObject.context);
        }
    }
}

