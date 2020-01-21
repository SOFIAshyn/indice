/*******************************************************************************
* File Name: Counter_Motor_Steps.c
* Version 2.10
*
* Description:
*  This file provides the source code to the API for the Counter_Motor_Steps
*  component
*
* Note:
*  None
*
********************************************************************************
* Copyright 2013-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "Counter_Motor_Steps.h"

uint8 Counter_Motor_Steps_initVar = 0u;


/*******************************************************************************
* Function Name: Counter_Motor_Steps_Init
********************************************************************************
*
* Summary:
*  Initialize/Restore default Counter_Motor_Steps configuration.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Counter_Motor_Steps_Init(void)
{

    /* Set values from customizer to CTRL */
    #if (Counter_Motor_Steps__QUAD == Counter_Motor_Steps_CONFIG)
        Counter_Motor_Steps_CONTROL_REG = Counter_Motor_Steps_CTRL_QUAD_BASE_CONFIG;
        
        /* Set values from customizer to CTRL1 */
        Counter_Motor_Steps_TRIG_CONTROL1_REG  = Counter_Motor_Steps_QUAD_SIGNALS_MODES;

        /* Set values from customizer to INTR */
        Counter_Motor_Steps_SetInterruptMode(Counter_Motor_Steps_QUAD_INTERRUPT_MASK);
        
         /* Set other values */
        Counter_Motor_Steps_SetCounterMode(Counter_Motor_Steps_COUNT_DOWN);
        Counter_Motor_Steps_WritePeriod(Counter_Motor_Steps_QUAD_PERIOD_INIT_VALUE);
        Counter_Motor_Steps_WriteCounter(Counter_Motor_Steps_QUAD_PERIOD_INIT_VALUE);
    #endif  /* (Counter_Motor_Steps__QUAD == Counter_Motor_Steps_CONFIG) */

    #if (Counter_Motor_Steps__TIMER == Counter_Motor_Steps_CONFIG)
        Counter_Motor_Steps_CONTROL_REG = Counter_Motor_Steps_CTRL_TIMER_BASE_CONFIG;
        
        /* Set values from customizer to CTRL1 */
        Counter_Motor_Steps_TRIG_CONTROL1_REG  = Counter_Motor_Steps_TIMER_SIGNALS_MODES;
    
        /* Set values from customizer to INTR */
        Counter_Motor_Steps_SetInterruptMode(Counter_Motor_Steps_TC_INTERRUPT_MASK);
        
        /* Set other values from customizer */
        Counter_Motor_Steps_WritePeriod(Counter_Motor_Steps_TC_PERIOD_VALUE );

        #if (Counter_Motor_Steps__COMPARE == Counter_Motor_Steps_TC_COMP_CAP_MODE)
            Counter_Motor_Steps_WriteCompare(Counter_Motor_Steps_TC_COMPARE_VALUE);

            #if (1u == Counter_Motor_Steps_TC_COMPARE_SWAP)
                Counter_Motor_Steps_SetCompareSwap(1u);
                Counter_Motor_Steps_WriteCompareBuf(Counter_Motor_Steps_TC_COMPARE_BUF_VALUE);
            #endif  /* (1u == Counter_Motor_Steps_TC_COMPARE_SWAP) */
        #endif  /* (Counter_Motor_Steps__COMPARE == Counter_Motor_Steps_TC_COMP_CAP_MODE) */

        /* Initialize counter value */
        #if (Counter_Motor_Steps_CY_TCPWM_V2 && Counter_Motor_Steps_TIMER_UPDOWN_CNT_USED && !Counter_Motor_Steps_CY_TCPWM_4000)
            Counter_Motor_Steps_WriteCounter(1u);
        #elif(Counter_Motor_Steps__COUNT_DOWN == Counter_Motor_Steps_TC_COUNTER_MODE)
            Counter_Motor_Steps_WriteCounter(Counter_Motor_Steps_TC_PERIOD_VALUE);
        #else
            Counter_Motor_Steps_WriteCounter(0u);
        #endif /* (Counter_Motor_Steps_CY_TCPWM_V2 && Counter_Motor_Steps_TIMER_UPDOWN_CNT_USED && !Counter_Motor_Steps_CY_TCPWM_4000) */
    #endif  /* (Counter_Motor_Steps__TIMER == Counter_Motor_Steps_CONFIG) */

    #if (Counter_Motor_Steps__PWM_SEL == Counter_Motor_Steps_CONFIG)
        Counter_Motor_Steps_CONTROL_REG = Counter_Motor_Steps_CTRL_PWM_BASE_CONFIG;

        #if (Counter_Motor_Steps__PWM_PR == Counter_Motor_Steps_PWM_MODE)
            Counter_Motor_Steps_CONTROL_REG |= Counter_Motor_Steps_CTRL_PWM_RUN_MODE;
            Counter_Motor_Steps_WriteCounter(Counter_Motor_Steps_PWM_PR_INIT_VALUE);
        #else
            Counter_Motor_Steps_CONTROL_REG |= Counter_Motor_Steps_CTRL_PWM_ALIGN | Counter_Motor_Steps_CTRL_PWM_KILL_EVENT;
            
            /* Initialize counter value */
            #if (Counter_Motor_Steps_CY_TCPWM_V2 && Counter_Motor_Steps_PWM_UPDOWN_CNT_USED && !Counter_Motor_Steps_CY_TCPWM_4000)
                Counter_Motor_Steps_WriteCounter(1u);
            #elif (Counter_Motor_Steps__RIGHT == Counter_Motor_Steps_PWM_ALIGN)
                Counter_Motor_Steps_WriteCounter(Counter_Motor_Steps_PWM_PERIOD_VALUE);
            #else 
                Counter_Motor_Steps_WriteCounter(0u);
            #endif  /* (Counter_Motor_Steps_CY_TCPWM_V2 && Counter_Motor_Steps_PWM_UPDOWN_CNT_USED && !Counter_Motor_Steps_CY_TCPWM_4000) */
        #endif  /* (Counter_Motor_Steps__PWM_PR == Counter_Motor_Steps_PWM_MODE) */

        #if (Counter_Motor_Steps__PWM_DT == Counter_Motor_Steps_PWM_MODE)
            Counter_Motor_Steps_CONTROL_REG |= Counter_Motor_Steps_CTRL_PWM_DEAD_TIME_CYCLE;
        #endif  /* (Counter_Motor_Steps__PWM_DT == Counter_Motor_Steps_PWM_MODE) */

        #if (Counter_Motor_Steps__PWM == Counter_Motor_Steps_PWM_MODE)
            Counter_Motor_Steps_CONTROL_REG |= Counter_Motor_Steps_CTRL_PWM_PRESCALER;
        #endif  /* (Counter_Motor_Steps__PWM == Counter_Motor_Steps_PWM_MODE) */

        /* Set values from customizer to CTRL1 */
        Counter_Motor_Steps_TRIG_CONTROL1_REG  = Counter_Motor_Steps_PWM_SIGNALS_MODES;
    
        /* Set values from customizer to INTR */
        Counter_Motor_Steps_SetInterruptMode(Counter_Motor_Steps_PWM_INTERRUPT_MASK);

        /* Set values from customizer to CTRL2 */
        #if (Counter_Motor_Steps__PWM_PR == Counter_Motor_Steps_PWM_MODE)
            Counter_Motor_Steps_TRIG_CONTROL2_REG =
                    (Counter_Motor_Steps_CC_MATCH_NO_CHANGE    |
                    Counter_Motor_Steps_OVERLOW_NO_CHANGE      |
                    Counter_Motor_Steps_UNDERFLOW_NO_CHANGE);
        #else
            #if (Counter_Motor_Steps__LEFT == Counter_Motor_Steps_PWM_ALIGN)
                Counter_Motor_Steps_TRIG_CONTROL2_REG = Counter_Motor_Steps_PWM_MODE_LEFT;
            #endif  /* ( Counter_Motor_Steps_PWM_LEFT == Counter_Motor_Steps_PWM_ALIGN) */

            #if (Counter_Motor_Steps__RIGHT == Counter_Motor_Steps_PWM_ALIGN)
                Counter_Motor_Steps_TRIG_CONTROL2_REG = Counter_Motor_Steps_PWM_MODE_RIGHT;
            #endif  /* ( Counter_Motor_Steps_PWM_RIGHT == Counter_Motor_Steps_PWM_ALIGN) */

            #if (Counter_Motor_Steps__CENTER == Counter_Motor_Steps_PWM_ALIGN)
                Counter_Motor_Steps_TRIG_CONTROL2_REG = Counter_Motor_Steps_PWM_MODE_CENTER;
            #endif  /* ( Counter_Motor_Steps_PWM_CENTER == Counter_Motor_Steps_PWM_ALIGN) */

            #if (Counter_Motor_Steps__ASYMMETRIC == Counter_Motor_Steps_PWM_ALIGN)
                Counter_Motor_Steps_TRIG_CONTROL2_REG = Counter_Motor_Steps_PWM_MODE_ASYM;
            #endif  /* (Counter_Motor_Steps__ASYMMETRIC == Counter_Motor_Steps_PWM_ALIGN) */
        #endif  /* (Counter_Motor_Steps__PWM_PR == Counter_Motor_Steps_PWM_MODE) */

        /* Set other values from customizer */
        Counter_Motor_Steps_WritePeriod(Counter_Motor_Steps_PWM_PERIOD_VALUE );
        Counter_Motor_Steps_WriteCompare(Counter_Motor_Steps_PWM_COMPARE_VALUE);

        #if (1u == Counter_Motor_Steps_PWM_COMPARE_SWAP)
            Counter_Motor_Steps_SetCompareSwap(1u);
            Counter_Motor_Steps_WriteCompareBuf(Counter_Motor_Steps_PWM_COMPARE_BUF_VALUE);
        #endif  /* (1u == Counter_Motor_Steps_PWM_COMPARE_SWAP) */

        #if (1u == Counter_Motor_Steps_PWM_PERIOD_SWAP)
            Counter_Motor_Steps_SetPeriodSwap(1u);
            Counter_Motor_Steps_WritePeriodBuf(Counter_Motor_Steps_PWM_PERIOD_BUF_VALUE);
        #endif  /* (1u == Counter_Motor_Steps_PWM_PERIOD_SWAP) */
    #endif  /* (Counter_Motor_Steps__PWM_SEL == Counter_Motor_Steps_CONFIG) */
    
}


/*******************************************************************************
* Function Name: Counter_Motor_Steps_Enable
********************************************************************************
*
* Summary:
*  Enables the Counter_Motor_Steps.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Counter_Motor_Steps_Enable(void)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();
    Counter_Motor_Steps_BLOCK_CONTROL_REG |= Counter_Motor_Steps_MASK;
    CyExitCriticalSection(enableInterrupts);

    /* Start Timer or PWM if start input is absent */
    #if (Counter_Motor_Steps__PWM_SEL == Counter_Motor_Steps_CONFIG)
        #if (0u == Counter_Motor_Steps_PWM_START_SIGNAL_PRESENT)
            Counter_Motor_Steps_TriggerCommand(Counter_Motor_Steps_MASK, Counter_Motor_Steps_CMD_START);
        #endif /* (0u == Counter_Motor_Steps_PWM_START_SIGNAL_PRESENT) */
    #endif /* (Counter_Motor_Steps__PWM_SEL == Counter_Motor_Steps_CONFIG) */

    #if (Counter_Motor_Steps__TIMER == Counter_Motor_Steps_CONFIG)
        #if (0u == Counter_Motor_Steps_TC_START_SIGNAL_PRESENT)
            Counter_Motor_Steps_TriggerCommand(Counter_Motor_Steps_MASK, Counter_Motor_Steps_CMD_START);
        #endif /* (0u == Counter_Motor_Steps_TC_START_SIGNAL_PRESENT) */
    #endif /* (Counter_Motor_Steps__TIMER == Counter_Motor_Steps_CONFIG) */
    
    #if (Counter_Motor_Steps__QUAD == Counter_Motor_Steps_CONFIG)
        #if (0u != Counter_Motor_Steps_QUAD_AUTO_START)
            Counter_Motor_Steps_TriggerCommand(Counter_Motor_Steps_MASK, Counter_Motor_Steps_CMD_RELOAD);
        #endif /* (0u != Counter_Motor_Steps_QUAD_AUTO_START) */
    #endif  /* (Counter_Motor_Steps__QUAD == Counter_Motor_Steps_CONFIG) */
}


/*******************************************************************************
* Function Name: Counter_Motor_Steps_Start
********************************************************************************
*
* Summary:
*  Initializes the Counter_Motor_Steps with default customizer
*  values when called the first time and enables the Counter_Motor_Steps.
*  For subsequent calls the configuration is left unchanged and the component is
*  just enabled.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  Counter_Motor_Steps_initVar: global variable is used to indicate initial
*  configuration of this component.  The variable is initialized to zero and set
*  to 1 the first time Counter_Motor_Steps_Start() is called. This allows
*  enabling/disabling a component without re-initialization in all subsequent
*  calls to the Counter_Motor_Steps_Start() routine.
*
*******************************************************************************/
void Counter_Motor_Steps_Start(void)
{
    if (0u == Counter_Motor_Steps_initVar)
    {
        Counter_Motor_Steps_Init();
        Counter_Motor_Steps_initVar = 1u;
    }

    Counter_Motor_Steps_Enable();
}


/*******************************************************************************
* Function Name: Counter_Motor_Steps_Stop
********************************************************************************
*
* Summary:
*  Disables the Counter_Motor_Steps.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Counter_Motor_Steps_Stop(void)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Counter_Motor_Steps_BLOCK_CONTROL_REG &= (uint32)~Counter_Motor_Steps_MASK;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Counter_Motor_Steps_SetMode
********************************************************************************
*
* Summary:
*  Sets the operation mode of the Counter_Motor_Steps. This function is used when
*  configured as a generic Counter_Motor_Steps and the actual mode of operation is
*  set at runtime. The mode must be set while the component is disabled.
*
* Parameters:
*  mode: Mode for the Counter_Motor_Steps to operate in
*   Values:
*   - Counter_Motor_Steps_MODE_TIMER_COMPARE - Timer / Counter with
*                                                 compare capability
*         - Counter_Motor_Steps_MODE_TIMER_CAPTURE - Timer / Counter with
*                                                 capture capability
*         - Counter_Motor_Steps_MODE_QUAD - Quadrature decoder
*         - Counter_Motor_Steps_MODE_PWM - PWM
*         - Counter_Motor_Steps_MODE_PWM_DT - PWM with dead time
*         - Counter_Motor_Steps_MODE_PWM_PR - PWM with pseudo random capability
*
* Return:
*  None
*
*******************************************************************************/
void Counter_Motor_Steps_SetMode(uint32 mode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Counter_Motor_Steps_CONTROL_REG &= (uint32)~Counter_Motor_Steps_MODE_MASK;
    Counter_Motor_Steps_CONTROL_REG |= mode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Counter_Motor_Steps_SetQDMode
********************************************************************************
*
* Summary:
*  Sets the the Quadrature Decoder to one of the 3 supported modes.
*  Its functionality is only applicable to Quadrature Decoder operation.
*
* Parameters:
*  qdMode: Quadrature Decoder mode
*   Values:
*         - Counter_Motor_Steps_MODE_X1 - Counts on phi 1 rising
*         - Counter_Motor_Steps_MODE_X2 - Counts on both edges of phi1 (2x faster)
*         - Counter_Motor_Steps_MODE_X4 - Counts on both edges of phi1 and phi2
*                                        (4x faster)
*
* Return:
*  None
*
*******************************************************************************/
void Counter_Motor_Steps_SetQDMode(uint32 qdMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Counter_Motor_Steps_CONTROL_REG &= (uint32)~Counter_Motor_Steps_QUAD_MODE_MASK;
    Counter_Motor_Steps_CONTROL_REG |= qdMode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Counter_Motor_Steps_SetPrescaler
********************************************************************************
*
* Summary:
*  Sets the prescaler value that is applied to the clock input.  Not applicable
*  to a PWM with the dead time mode or Quadrature Decoder mode.
*
* Parameters:
*  prescaler: Prescaler divider value
*   Values:
*         - Counter_Motor_Steps_PRESCALE_DIVBY1    - Divide by 1 (no prescaling)
*         - Counter_Motor_Steps_PRESCALE_DIVBY2    - Divide by 2
*         - Counter_Motor_Steps_PRESCALE_DIVBY4    - Divide by 4
*         - Counter_Motor_Steps_PRESCALE_DIVBY8    - Divide by 8
*         - Counter_Motor_Steps_PRESCALE_DIVBY16   - Divide by 16
*         - Counter_Motor_Steps_PRESCALE_DIVBY32   - Divide by 32
*         - Counter_Motor_Steps_PRESCALE_DIVBY64   - Divide by 64
*         - Counter_Motor_Steps_PRESCALE_DIVBY128  - Divide by 128
*
* Return:
*  None
*
*******************************************************************************/
void Counter_Motor_Steps_SetPrescaler(uint32 prescaler)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Counter_Motor_Steps_CONTROL_REG &= (uint32)~Counter_Motor_Steps_PRESCALER_MASK;
    Counter_Motor_Steps_CONTROL_REG |= prescaler;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Counter_Motor_Steps_SetOneShot
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the Counter_Motor_Steps runs
*  continuously or stops when terminal count is reached.  By default the
*  Counter_Motor_Steps operates in the continuous mode.
*
* Parameters:
*  oneShotEnable
*   Values:
*     - 0 - Continuous
*     - 1 - Enable One Shot
*
* Return:
*  None
*
*******************************************************************************/
void Counter_Motor_Steps_SetOneShot(uint32 oneShotEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Counter_Motor_Steps_CONTROL_REG &= (uint32)~Counter_Motor_Steps_ONESHOT_MASK;
    Counter_Motor_Steps_CONTROL_REG |= ((uint32)((oneShotEnable & Counter_Motor_Steps_1BIT_MASK) <<
                                                               Counter_Motor_Steps_ONESHOT_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Counter_Motor_Steps_SetPWMMode
********************************************************************************
*
* Summary:
*  Writes the control register that determines what mode of operation the PWM
*  output lines are driven in.  There is a setting for what to do on a
*  comparison match (CC_MATCH), on an overflow (OVERFLOW) and on an underflow
*  (UNDERFLOW).  The value for each of the three must be ORed together to form
*  the mode.
*
* Parameters:
*  modeMask: A combination of three mode settings.  Mask must include a value
*  for each of the three or use one of the preconfigured PWM settings.
*   Values:
*     - CC_MATCH_SET        - Set on comparison match
*     - CC_MATCH_CLEAR      - Clear on comparison match
*     - CC_MATCH_INVERT     - Invert on comparison match
*     - CC_MATCH_NO_CHANGE  - No change on comparison match
*     - OVERLOW_SET         - Set on overflow
*     - OVERLOW_CLEAR       - Clear on  overflow
*     - OVERLOW_INVERT      - Invert on overflow
*     - OVERLOW_NO_CHANGE   - No change on overflow
*     - UNDERFLOW_SET       - Set on underflow
*     - UNDERFLOW_CLEAR     - Clear on underflow
*     - UNDERFLOW_INVERT    - Invert on underflow
*     - UNDERFLOW_NO_CHANGE - No change on underflow
*     - PWM_MODE_LEFT       - Setting for left aligned PWM.  Should be combined
*                             with up counting mode
*     - PWM_MODE_RIGHT      - Setting for right aligned PWM.  Should be combined
*                             with down counting mode
*     - PWM_MODE_CENTER     - Setting for center aligned PWM.  Should be
*                             combined with up/down 0 mode
*     - PWM_MODE_ASYM       - Setting for asymmetric PWM.  Should be combined
*                             with up/down 1 mode
*
* Return:
*  None
*
*******************************************************************************/
void Counter_Motor_Steps_SetPWMMode(uint32 modeMask)
{
    Counter_Motor_Steps_TRIG_CONTROL2_REG = (modeMask & Counter_Motor_Steps_6BIT_MASK);
}



/*******************************************************************************
* Function Name: Counter_Motor_Steps_SetPWMSyncKill
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the PWM kill signal (stop input)
*  causes asynchronous or synchronous kill operation.  By default the kill
*  operation is asynchronous.  This functionality is only applicable to the PWM
*  and PWM with dead time modes.
*
*  For Synchronous mode the kill signal disables both the line and line_n
*  signals until the next terminal count.
*
*  For Asynchronous mode the kill signal disables both the line and line_n
*  signals when the kill signal is present.  This mode should only be used
*  when the kill signal (stop input) is configured in the pass through mode
*  (Level sensitive signal).

*
* Parameters:
*  syncKillEnable
*   Values:
*     - 0 - Asynchronous
*     - 1 - Synchronous
*
* Return:
*  None
*
*******************************************************************************/
void Counter_Motor_Steps_SetPWMSyncKill(uint32 syncKillEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Counter_Motor_Steps_CONTROL_REG &= (uint32)~Counter_Motor_Steps_PWM_SYNC_KILL_MASK;
    Counter_Motor_Steps_CONTROL_REG |= ((uint32)((syncKillEnable & Counter_Motor_Steps_1BIT_MASK)  <<
                                               Counter_Motor_Steps_PWM_SYNC_KILL_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Counter_Motor_Steps_SetPWMStopOnKill
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the PWM kill signal (stop input)
*  causes the PWM counter to stop.  By default the kill operation does not stop
*  the counter.  This functionality is only applicable to the three PWM modes.
*
*
* Parameters:
*  stopOnKillEnable
*   Values:
*     - 0 - Don't stop
*     - 1 - Stop
*
* Return:
*  None
*
*******************************************************************************/
void Counter_Motor_Steps_SetPWMStopOnKill(uint32 stopOnKillEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Counter_Motor_Steps_CONTROL_REG &= (uint32)~Counter_Motor_Steps_PWM_STOP_KILL_MASK;
    Counter_Motor_Steps_CONTROL_REG |= ((uint32)((stopOnKillEnable & Counter_Motor_Steps_1BIT_MASK)  <<
                                                         Counter_Motor_Steps_PWM_STOP_KILL_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Counter_Motor_Steps_SetPWMDeadTime
********************************************************************************
*
* Summary:
*  Writes the dead time control value.  This value delays the rising edge of
*  both the line and line_n signals the designated number of cycles resulting
*  in both signals being inactive for that many cycles.  This functionality is
*  only applicable to the PWM in the dead time mode.

*
* Parameters:
*  Dead time to insert
*   Values: 0 to 255
*
* Return:
*  None
*
*******************************************************************************/
void Counter_Motor_Steps_SetPWMDeadTime(uint32 deadTime)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Counter_Motor_Steps_CONTROL_REG &= (uint32)~Counter_Motor_Steps_PRESCALER_MASK;
    Counter_Motor_Steps_CONTROL_REG |= ((uint32)((deadTime & Counter_Motor_Steps_8BIT_MASK) <<
                                                          Counter_Motor_Steps_PRESCALER_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Counter_Motor_Steps_SetPWMInvert
********************************************************************************
*
* Summary:
*  Writes the bits that control whether the line and line_n outputs are
*  inverted from their normal output values.  This functionality is only
*  applicable to the three PWM modes.
*
* Parameters:
*  mask: Mask of outputs to invert.
*   Values:
*         - Counter_Motor_Steps_INVERT_LINE   - Inverts the line output
*         - Counter_Motor_Steps_INVERT_LINE_N - Inverts the line_n output
*
* Return:
*  None
*
*******************************************************************************/
void Counter_Motor_Steps_SetPWMInvert(uint32 mask)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Counter_Motor_Steps_CONTROL_REG &= (uint32)~Counter_Motor_Steps_INV_OUT_MASK;
    Counter_Motor_Steps_CONTROL_REG |= mask;

    CyExitCriticalSection(enableInterrupts);
}



/*******************************************************************************
* Function Name: Counter_Motor_Steps_WriteCounter
********************************************************************************
*
* Summary:
*  Writes a new 16bit counter value directly into the counter register, thus
*  setting the counter (not the period) to the value written. It is not
*  advised to write to this field when the counter is running.
*
* Parameters:
*  count: value to write
*
* Return:
*  None
*
*******************************************************************************/
void Counter_Motor_Steps_WriteCounter(uint32 count)
{
    Counter_Motor_Steps_COUNTER_REG = (count & Counter_Motor_Steps_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Counter_Motor_Steps_ReadCounter
********************************************************************************
*
* Summary:
*  Reads the current counter value.
*
* Parameters:
*  None
*
* Return:
*  Current counter value
*
*******************************************************************************/
uint32 Counter_Motor_Steps_ReadCounter(void)
{
    return (Counter_Motor_Steps_COUNTER_REG & Counter_Motor_Steps_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Counter_Motor_Steps_SetCounterMode
********************************************************************************
*
* Summary:
*  Sets the counter mode.  Applicable to all modes except Quadrature Decoder
*  and the PWM with a pseudo random output.
*
* Parameters:
*  counterMode: Enumerated counter type values
*   Values:
*     - Counter_Motor_Steps_COUNT_UP       - Counts up
*     - Counter_Motor_Steps_COUNT_DOWN     - Counts down
*     - Counter_Motor_Steps_COUNT_UPDOWN0  - Counts up and down. Terminal count
*                                         generated when counter reaches 0
*     - Counter_Motor_Steps_COUNT_UPDOWN1  - Counts up and down. Terminal count
*                                         generated both when counter reaches 0
*                                         and period
*
* Return:
*  None
*
*******************************************************************************/
void Counter_Motor_Steps_SetCounterMode(uint32 counterMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Counter_Motor_Steps_CONTROL_REG &= (uint32)~Counter_Motor_Steps_UPDOWN_MASK;
    Counter_Motor_Steps_CONTROL_REG |= counterMode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Counter_Motor_Steps_WritePeriod
********************************************************************************
*
* Summary:
*  Writes the 16 bit period register with the new period value.
*  To cause the counter to count for N cycles this register should be written
*  with N-1 (counts from 0 to period inclusive).
*
* Parameters:
*  period: Period value
*
* Return:
*  None
*
*******************************************************************************/
void Counter_Motor_Steps_WritePeriod(uint32 period)
{
    Counter_Motor_Steps_PERIOD_REG = (period & Counter_Motor_Steps_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Counter_Motor_Steps_ReadPeriod
********************************************************************************
*
* Summary:
*  Reads the 16 bit period register.
*
* Parameters:
*  None
*
* Return:
*  Period value
*
*******************************************************************************/
uint32 Counter_Motor_Steps_ReadPeriod(void)
{
    return (Counter_Motor_Steps_PERIOD_REG & Counter_Motor_Steps_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Counter_Motor_Steps_SetCompareSwap
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the compare registers are
*  swapped. When enabled in the Timer/Counter mode(without capture) the swap
*  occurs at a TC event. In the PWM mode the swap occurs at the next TC event
*  following a hardware switch event.
*
* Parameters:
*  swapEnable
*   Values:
*     - 0 - Disable swap
*     - 1 - Enable swap
*
* Return:
*  None
*
*******************************************************************************/
void Counter_Motor_Steps_SetCompareSwap(uint32 swapEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Counter_Motor_Steps_CONTROL_REG &= (uint32)~Counter_Motor_Steps_RELOAD_CC_MASK;
    Counter_Motor_Steps_CONTROL_REG |= (swapEnable & Counter_Motor_Steps_1BIT_MASK);

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Counter_Motor_Steps_WritePeriodBuf
********************************************************************************
*
* Summary:
*  Writes the 16 bit period buf register with the new period value.
*
* Parameters:
*  periodBuf: Period value
*
* Return:
*  None
*
*******************************************************************************/
void Counter_Motor_Steps_WritePeriodBuf(uint32 periodBuf)
{
    Counter_Motor_Steps_PERIOD_BUF_REG = (periodBuf & Counter_Motor_Steps_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Counter_Motor_Steps_ReadPeriodBuf
********************************************************************************
*
* Summary:
*  Reads the 16 bit period buf register.
*
* Parameters:
*  None
*
* Return:
*  Period value
*
*******************************************************************************/
uint32 Counter_Motor_Steps_ReadPeriodBuf(void)
{
    return (Counter_Motor_Steps_PERIOD_BUF_REG & Counter_Motor_Steps_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Counter_Motor_Steps_SetPeriodSwap
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the period registers are
*  swapped. When enabled in Timer/Counter mode the swap occurs at a TC event.
*  In the PWM mode the swap occurs at the next TC event following a hardware
*  switch event.
*
* Parameters:
*  swapEnable
*   Values:
*     - 0 - Disable swap
*     - 1 - Enable swap
*
* Return:
*  None
*
*******************************************************************************/
void Counter_Motor_Steps_SetPeriodSwap(uint32 swapEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Counter_Motor_Steps_CONTROL_REG &= (uint32)~Counter_Motor_Steps_RELOAD_PERIOD_MASK;
    Counter_Motor_Steps_CONTROL_REG |= ((uint32)((swapEnable & Counter_Motor_Steps_1BIT_MASK) <<
                                                            Counter_Motor_Steps_RELOAD_PERIOD_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Counter_Motor_Steps_WriteCompare
********************************************************************************
*
* Summary:
*  Writes the 16 bit compare register with the new compare value. Not
*  applicable for Timer/Counter with Capture or in Quadrature Decoder modes.
*
* Parameters:
*  compare: Compare value
*
* Return:
*  None
*
* Note:
*  It is not recommended to use the value equal to "0" or equal to 
*  "period value" in Center or Asymmetric align PWM modes on the 
*  PSoC 4100/PSoC 4200 devices.
*  PSoC 4000 devices write the 16 bit compare register with the decremented 
*  compare value in the Up counting mode (except 0x0u), and the incremented 
*  compare value in the Down counting mode (except 0xFFFFu).
*
*******************************************************************************/
void Counter_Motor_Steps_WriteCompare(uint32 compare)
{
    #if (Counter_Motor_Steps_CY_TCPWM_4000)
        uint32 currentMode;
    #endif /* (Counter_Motor_Steps_CY_TCPWM_4000) */

    #if (Counter_Motor_Steps_CY_TCPWM_4000)
        currentMode = ((Counter_Motor_Steps_CONTROL_REG & Counter_Motor_Steps_UPDOWN_MASK) >> Counter_Motor_Steps_UPDOWN_SHIFT);

        if (((uint32)Counter_Motor_Steps__COUNT_DOWN == currentMode) && (0xFFFFu != compare))
        {
            compare++;
        }
        else if (((uint32)Counter_Motor_Steps__COUNT_UP == currentMode) && (0u != compare))
        {
            compare--;
        }
        else
        {
        }
        
    
    #endif /* (Counter_Motor_Steps_CY_TCPWM_4000) */
    
    Counter_Motor_Steps_COMP_CAP_REG = (compare & Counter_Motor_Steps_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Counter_Motor_Steps_ReadCompare
********************************************************************************
*
* Summary:
*  Reads the compare register. Not applicable for Timer/Counter with Capture
*  or in Quadrature Decoder modes.
*  PSoC 4000 devices read the incremented compare register value in the 
*  Up counting mode (except 0xFFFFu), and the decremented value in the 
*  Down counting mode (except 0x0u).
*
* Parameters:
*  None
*
* Return:
*  Compare value
*
* Note:
*  PSoC 4000 devices read the incremented compare register value in the 
*  Up counting mode (except 0xFFFFu), and the decremented value in the 
*  Down counting mode (except 0x0u).
*
*******************************************************************************/
uint32 Counter_Motor_Steps_ReadCompare(void)
{
    #if (Counter_Motor_Steps_CY_TCPWM_4000)
        uint32 currentMode;
        uint32 regVal;
    #endif /* (Counter_Motor_Steps_CY_TCPWM_4000) */

    #if (Counter_Motor_Steps_CY_TCPWM_4000)
        currentMode = ((Counter_Motor_Steps_CONTROL_REG & Counter_Motor_Steps_UPDOWN_MASK) >> Counter_Motor_Steps_UPDOWN_SHIFT);
        
        regVal = Counter_Motor_Steps_COMP_CAP_REG;
        
        if (((uint32)Counter_Motor_Steps__COUNT_DOWN == currentMode) && (0u != regVal))
        {
            regVal--;
        }
        else if (((uint32)Counter_Motor_Steps__COUNT_UP == currentMode) && (0xFFFFu != regVal))
        {
            regVal++;
        }
        else
        {
        }

        return (regVal & Counter_Motor_Steps_16BIT_MASK);
    #else
        return (Counter_Motor_Steps_COMP_CAP_REG & Counter_Motor_Steps_16BIT_MASK);
    #endif /* (Counter_Motor_Steps_CY_TCPWM_4000) */
}


/*******************************************************************************
* Function Name: Counter_Motor_Steps_WriteCompareBuf
********************************************************************************
*
* Summary:
*  Writes the 16 bit compare buffer register with the new compare value. Not
*  applicable for Timer/Counter with Capture or in Quadrature Decoder modes.
*
* Parameters:
*  compareBuf: Compare value
*
* Return:
*  None
*
* Note:
*  It is not recommended to use the value equal to "0" or equal to 
*  "period value" in Center or Asymmetric align PWM modes on the 
*  PSoC 4100/PSoC 4200 devices.
*  PSoC 4000 devices write the 16 bit compare register with the decremented 
*  compare value in the Up counting mode (except 0x0u), and the incremented 
*  compare value in the Down counting mode (except 0xFFFFu).
*
*******************************************************************************/
void Counter_Motor_Steps_WriteCompareBuf(uint32 compareBuf)
{
    #if (Counter_Motor_Steps_CY_TCPWM_4000)
        uint32 currentMode;
    #endif /* (Counter_Motor_Steps_CY_TCPWM_4000) */

    #if (Counter_Motor_Steps_CY_TCPWM_4000)
        currentMode = ((Counter_Motor_Steps_CONTROL_REG & Counter_Motor_Steps_UPDOWN_MASK) >> Counter_Motor_Steps_UPDOWN_SHIFT);

        if (((uint32)Counter_Motor_Steps__COUNT_DOWN == currentMode) && (0xFFFFu != compareBuf))
        {
            compareBuf++;
        }
        else if (((uint32)Counter_Motor_Steps__COUNT_UP == currentMode) && (0u != compareBuf))
        {
            compareBuf --;
        }
        else
        {
        }
    #endif /* (Counter_Motor_Steps_CY_TCPWM_4000) */
    
    Counter_Motor_Steps_COMP_CAP_BUF_REG = (compareBuf & Counter_Motor_Steps_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Counter_Motor_Steps_ReadCompareBuf
********************************************************************************
*
* Summary:
*  Reads the compare buffer register. Not applicable for Timer/Counter with
*  Capture or in Quadrature Decoder modes.
*
* Parameters:
*  None
*
* Return:
*  Compare buffer value
*
* Note:
*  PSoC 4000 devices read the incremented compare register value in the 
*  Up counting mode (except 0xFFFFu), and the decremented value in the 
*  Down counting mode (except 0x0u).
*
*******************************************************************************/
uint32 Counter_Motor_Steps_ReadCompareBuf(void)
{
    #if (Counter_Motor_Steps_CY_TCPWM_4000)
        uint32 currentMode;
        uint32 regVal;
    #endif /* (Counter_Motor_Steps_CY_TCPWM_4000) */

    #if (Counter_Motor_Steps_CY_TCPWM_4000)
        currentMode = ((Counter_Motor_Steps_CONTROL_REG & Counter_Motor_Steps_UPDOWN_MASK) >> Counter_Motor_Steps_UPDOWN_SHIFT);

        regVal = Counter_Motor_Steps_COMP_CAP_BUF_REG;
        
        if (((uint32)Counter_Motor_Steps__COUNT_DOWN == currentMode) && (0u != regVal))
        {
            regVal--;
        }
        else if (((uint32)Counter_Motor_Steps__COUNT_UP == currentMode) && (0xFFFFu != regVal))
        {
            regVal++;
        }
        else
        {
        }

        return (regVal & Counter_Motor_Steps_16BIT_MASK);
    #else
        return (Counter_Motor_Steps_COMP_CAP_BUF_REG & Counter_Motor_Steps_16BIT_MASK);
    #endif /* (Counter_Motor_Steps_CY_TCPWM_4000) */
}


/*******************************************************************************
* Function Name: Counter_Motor_Steps_ReadCapture
********************************************************************************
*
* Summary:
*  Reads the captured counter value. This API is applicable only for
*  Timer/Counter with the capture mode and Quadrature Decoder modes.
*
* Parameters:
*  None
*
* Return:
*  Capture value
*
*******************************************************************************/
uint32 Counter_Motor_Steps_ReadCapture(void)
{
    return (Counter_Motor_Steps_COMP_CAP_REG & Counter_Motor_Steps_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Counter_Motor_Steps_ReadCaptureBuf
********************************************************************************
*
* Summary:
*  Reads the capture buffer register. This API is applicable only for
*  Timer/Counter with the capture mode and Quadrature Decoder modes.
*
* Parameters:
*  None
*
* Return:
*  Capture buffer value
*
*******************************************************************************/
uint32 Counter_Motor_Steps_ReadCaptureBuf(void)
{
    return (Counter_Motor_Steps_COMP_CAP_BUF_REG & Counter_Motor_Steps_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Counter_Motor_Steps_SetCaptureMode
********************************************************************************
*
* Summary:
*  Sets the capture trigger mode. For PWM mode this is the switch input.
*  This input is not applicable to the Timer/Counter without Capture and
*  Quadrature Decoder modes.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - Counter_Motor_Steps_TRIG_LEVEL     - Level
*     - Counter_Motor_Steps_TRIG_RISING    - Rising edge
*     - Counter_Motor_Steps_TRIG_FALLING   - Falling edge
*     - Counter_Motor_Steps_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void Counter_Motor_Steps_SetCaptureMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Counter_Motor_Steps_TRIG_CONTROL1_REG &= (uint32)~Counter_Motor_Steps_CAPTURE_MASK;
    Counter_Motor_Steps_TRIG_CONTROL1_REG |= triggerMode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Counter_Motor_Steps_SetReloadMode
********************************************************************************
*
* Summary:
*  Sets the reload trigger mode. For Quadrature Decoder mode this is the index
*  input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - Counter_Motor_Steps_TRIG_LEVEL     - Level
*     - Counter_Motor_Steps_TRIG_RISING    - Rising edge
*     - Counter_Motor_Steps_TRIG_FALLING   - Falling edge
*     - Counter_Motor_Steps_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void Counter_Motor_Steps_SetReloadMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Counter_Motor_Steps_TRIG_CONTROL1_REG &= (uint32)~Counter_Motor_Steps_RELOAD_MASK;
    Counter_Motor_Steps_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << Counter_Motor_Steps_RELOAD_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Counter_Motor_Steps_SetStartMode
********************************************************************************
*
* Summary:
*  Sets the start trigger mode. For Quadrature Decoder mode this is the
*  phiB input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - Counter_Motor_Steps_TRIG_LEVEL     - Level
*     - Counter_Motor_Steps_TRIG_RISING    - Rising edge
*     - Counter_Motor_Steps_TRIG_FALLING   - Falling edge
*     - Counter_Motor_Steps_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void Counter_Motor_Steps_SetStartMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Counter_Motor_Steps_TRIG_CONTROL1_REG &= (uint32)~Counter_Motor_Steps_START_MASK;
    Counter_Motor_Steps_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << Counter_Motor_Steps_START_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Counter_Motor_Steps_SetStopMode
********************************************************************************
*
* Summary:
*  Sets the stop trigger mode. For PWM mode this is the kill input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - Counter_Motor_Steps_TRIG_LEVEL     - Level
*     - Counter_Motor_Steps_TRIG_RISING    - Rising edge
*     - Counter_Motor_Steps_TRIG_FALLING   - Falling edge
*     - Counter_Motor_Steps_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void Counter_Motor_Steps_SetStopMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Counter_Motor_Steps_TRIG_CONTROL1_REG &= (uint32)~Counter_Motor_Steps_STOP_MASK;
    Counter_Motor_Steps_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << Counter_Motor_Steps_STOP_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Counter_Motor_Steps_SetCountMode
********************************************************************************
*
* Summary:
*  Sets the count trigger mode. For Quadrature Decoder mode this is the phiA
*  input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - Counter_Motor_Steps_TRIG_LEVEL     - Level
*     - Counter_Motor_Steps_TRIG_RISING    - Rising edge
*     - Counter_Motor_Steps_TRIG_FALLING   - Falling edge
*     - Counter_Motor_Steps_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void Counter_Motor_Steps_SetCountMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Counter_Motor_Steps_TRIG_CONTROL1_REG &= (uint32)~Counter_Motor_Steps_COUNT_MASK;
    Counter_Motor_Steps_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << Counter_Motor_Steps_COUNT_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Counter_Motor_Steps_TriggerCommand
********************************************************************************
*
* Summary:
*  Triggers the designated command to occur on the designated TCPWM instances.
*  The mask can be used to apply this command simultaneously to more than one
*  instance.  This allows multiple TCPWM instances to be synchronized.
*
* Parameters:
*  mask: A combination of mask bits for each instance of the TCPWM that the
*        command should apply to.  This function from one instance can be used
*        to apply the command to any of the instances in the design.
*        The mask value for a specific instance is available with the MASK
*        define.
*  command: Enumerated command values. Capture command only applicable for
*           Timer/Counter with Capture and PWM modes.
*   Values:
*     - Counter_Motor_Steps_CMD_CAPTURE    - Trigger Capture/Switch command
*     - Counter_Motor_Steps_CMD_RELOAD     - Trigger Reload/Index command
*     - Counter_Motor_Steps_CMD_STOP       - Trigger Stop/Kill command
*     - Counter_Motor_Steps_CMD_START      - Trigger Start/phiB command
*
* Return:
*  None
*
*******************************************************************************/
void Counter_Motor_Steps_TriggerCommand(uint32 mask, uint32 command)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Counter_Motor_Steps_COMMAND_REG = ((uint32)(mask << command));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Counter_Motor_Steps_ReadStatus
********************************************************************************
*
* Summary:
*  Reads the status of the Counter_Motor_Steps.
*
* Parameters:
*  None
*
* Return:
*  Status
*   Values:
*     - Counter_Motor_Steps_STATUS_DOWN    - Set if counting down
*     - Counter_Motor_Steps_STATUS_RUNNING - Set if counter is running
*
*******************************************************************************/
uint32 Counter_Motor_Steps_ReadStatus(void)
{
    return ((Counter_Motor_Steps_STATUS_REG >> Counter_Motor_Steps_RUNNING_STATUS_SHIFT) |
            (Counter_Motor_Steps_STATUS_REG & Counter_Motor_Steps_STATUS_DOWN));
}


/*******************************************************************************
* Function Name: Counter_Motor_Steps_SetInterruptMode
********************************************************************************
*
* Summary:
*  Sets the interrupt mask to control which interrupt
*  requests generate the interrupt signal.
*
* Parameters:
*   interruptMask: Mask of bits to be enabled
*   Values:
*     - Counter_Motor_Steps_INTR_MASK_TC       - Terminal count mask
*     - Counter_Motor_Steps_INTR_MASK_CC_MATCH - Compare count / capture mask
*
* Return:
*  None
*
*******************************************************************************/
void Counter_Motor_Steps_SetInterruptMode(uint32 interruptMask)
{
    Counter_Motor_Steps_INTERRUPT_MASK_REG =  interruptMask;
}


/*******************************************************************************
* Function Name: Counter_Motor_Steps_GetInterruptSourceMasked
********************************************************************************
*
* Summary:
*  Gets the interrupt requests masked by the interrupt mask.
*
* Parameters:
*   None
*
* Return:
*  Masked interrupt source
*   Values:
*     - Counter_Motor_Steps_INTR_MASK_TC       - Terminal count mask
*     - Counter_Motor_Steps_INTR_MASK_CC_MATCH - Compare count / capture mask
*
*******************************************************************************/
uint32 Counter_Motor_Steps_GetInterruptSourceMasked(void)
{
    return (Counter_Motor_Steps_INTERRUPT_MASKED_REG);
}


/*******************************************************************************
* Function Name: Counter_Motor_Steps_GetInterruptSource
********************************************************************************
*
* Summary:
*  Gets the interrupt requests (without masking).
*
* Parameters:
*  None
*
* Return:
*  Interrupt request value
*   Values:
*     - Counter_Motor_Steps_INTR_MASK_TC       - Terminal count mask
*     - Counter_Motor_Steps_INTR_MASK_CC_MATCH - Compare count / capture mask
*
*******************************************************************************/
uint32 Counter_Motor_Steps_GetInterruptSource(void)
{
    return (Counter_Motor_Steps_INTERRUPT_REQ_REG);
}


/*******************************************************************************
* Function Name: Counter_Motor_Steps_ClearInterrupt
********************************************************************************
*
* Summary:
*  Clears the interrupt request.
*
* Parameters:
*   interruptMask: Mask of interrupts to clear
*   Values:
*     - Counter_Motor_Steps_INTR_MASK_TC       - Terminal count mask
*     - Counter_Motor_Steps_INTR_MASK_CC_MATCH - Compare count / capture mask
*
* Return:
*  None
*
*******************************************************************************/
void Counter_Motor_Steps_ClearInterrupt(uint32 interruptMask)
{
    Counter_Motor_Steps_INTERRUPT_REQ_REG = interruptMask;
}


/*******************************************************************************
* Function Name: Counter_Motor_Steps_SetInterrupt
********************************************************************************
*
* Summary:
*  Sets a software interrupt request.
*
* Parameters:
*   interruptMask: Mask of interrupts to set
*   Values:
*     - Counter_Motor_Steps_INTR_MASK_TC       - Terminal count mask
*     - Counter_Motor_Steps_INTR_MASK_CC_MATCH - Compare count / capture mask
*
* Return:
*  None
*
*******************************************************************************/
void Counter_Motor_Steps_SetInterrupt(uint32 interruptMask)
{
    Counter_Motor_Steps_INTERRUPT_SET_REG = interruptMask;
}


/* [] END OF FILE */
