/*!**********************************************************************************************************************
@file leds.c                                                                
@brief LED driver and API

This driver provides on, off, toggle, blink and PWM functionality.
The basic on/off/toggle functionality is applied directly to the LEDs.
Blinking and PWMing of LEDs rely on the EIE operating system to provide timing at
regular 1ms calls to LedUpdate().

------------------------------------------------------------------------------------------------------------------------
GLOBALS
- NONE

CONSTANTS
- NONE

TYPES
- LedNameType

(from eief1-pcb-01):
 {WHITE, PURPLE, BLUE, CYAN, 
  GREEN, YELLOW, ORANGE, RED, 
  LCD_RED, LCD_GREEN, LCD_BLUE}


- LedRateType:
  {LED_0HZ = 0, LED_0_5HZ = 1000, LED_1HZ = 500, LED_2HZ = 250, 
   LED_4HZ = 125, LED_8HZ = 63, LED_PWM_100 = 20}

PUBLIC FUNCTIONS
- void LedOn(LedNameType eLED_)
- void LedOff(LedNameType eLED_)
- void LedToggle(LedNameType eLED_)
- void LedBlink(LedNameType eLED_, LedRateType eBlinkRate_)

PROTECTED FUNCTIONS
- void LedInitialize(void)
- void LedRunActiveState(void)

***********************************************************************************************************************/

#include "configuration.h"


/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_<type>Led"
***********************************************************************************************************************/
/*--------------------------------------------------------------------------------------------------------------------*/
/* New variables (all shall start with G_xxLed*/


/*--------------------------------------------------------------------------------------------------------------------*/
/* External global variables defined in other files (must indicate which file they are defined in) */
extern volatile u32 G_u32SystemTime1ms;                /*!< @brief From main.c */
extern volatile u32 G_u32SystemTime1s;                 /*!< @brief From main.c */
extern volatile u32 G_u32SystemFlags;                  /*!< @brief From main.c */
extern volatile u32 G_u32ApplicationFlags;             /*!< @brief From main.c */

extern const LedConfigurationType G_asBspLedConfigurations[U8_TOTAL_LEDS]; /*!< @brief from board-specific file */

/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "Led_<type>" and be declared as static.
***********************************************************************************************************************/
static fnCode_type Led_StateMachine;                   /*!< @brief The state machine function pointer */
//static u32 Led_u32Timeout;                           /*!< @brief Timeout counter used across states */

static LedControlType Led_asControl[U8_TOTAL_LEDS];
/*!< @brief Holds individual control parameters for LEDs */

 

/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/*! @publicsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*!-------------------------------------------------------------------------------------------------------------------
@fn void LedOn(LedNameType eLED_)

@brief Turns the specified LED on.

This function automatically takes care of the active low vs. active high LEDs. The
function works immediately (it does not require the main application loop to be
running).

Requires:
- Definitions in G_asBspLedConfigurations[eLED_] and Led_asControl[eLED_] are correct

@param eLED_ is a valid LED index

Promises:
- eLED_ is turned on
- eLED_ is set to LED_NORMAL_MODE mode

*/
void LedOn(LedNameType eLED_)
{
  u32 *pu32OnAddress;
  
  /* Configure set and clear addresses */
  if(G_asBspLedConfigurations[(u8)eLED_].eActiveState == LED_ACTIVE_HIGH)
  {
    /* Active high LEDs use SODR to turn on */
    pu32OnAddress = (u32*)(&(AT91C_BASE_PIOA->PIO_SODR) +
                           G_asBspLedConfigurations[(u8)eLED_].ePort);
  }
  else
  {
    /* Active low LEDs use CODR tu turn on */
    pu32OnAddress = (u32*)(&(AT91C_BASE_PIOA->PIO_CODR) +
                           G_asBspLedConfigurations[(u8)eLED_].ePort);
  }
  
  /* Turn on the LED */
  *pu32OnAddress = G_asBspLedConfigurations[(u8)eLED_].u32BitPosition;
  
  /* Always set the LED back to LED_NORMAL_MODE */
  Led_asControl[(u8)eLED_].eMode = LED_NORMAL_MODE;
} /* end LedOn() */
  

/*--------------------------------------------------------------------------------------------------------------------*/
/*! @protectedsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*!----------------------------------------------------------------------------------------------------------------------
@fn void LedInitialize(void)

@brief Initialization of LED system parameters and visual LED check.


Requires:
- 

Promises:
- 

*/
void LedInitialize(void)
{
  /* Initialize the LED control array */
  for(u8 i = 0; i < U8_TOTAL_LEDS; i++)
  {
    Led_asControl[i].eMode = LED_NORMAL_MODE;
    Led_asControl[i].eRate = LED_0HZ;
    
    Led_asControl[i].u16Count = 0;
  }
  
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    Led_StateMachine = LedSM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    Led_StateMachine = LedSM_Error;
  }
  
} /* end LedInitialize() */


/*!----------------------------------------------------------------------------------------------------------------------
@fn void LedRunActiveState(void)

@brief Selects and runs one iteration of the current state in the state machine.

All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
- State machine function pointer points at current state

Promises:
- Calls the function to pointed by the state machine function pointer

*/
void LedRunActiveState(void)
{
  Led_StateMachine();

} /* end LedRunActiveState */



/*------------------------------------------------------------------------------------------------------------------*/
/*! @privatesection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/


/***********************************************************************************************************************
State Machine Declarations
***********************************************************************************************************************/

/*!-------------------------------------------------------------------------------------------------------------------
@fn static void LedSM_Idle(void)

@brief What does this function do?
*/
static void LedSM_Idle(void)
{
 
} /* end LedSM_Idle() */


/*!-------------------------------------------------------------------------------------------------------------------
@fn static void LedSM_Error(void)

@brief Handle an error here.  For now, the task is just held in this state. 
*/
static void LedSM_Error(void)          
{
  
} /* end LedSM_Error() */

/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File */
/*--------------------------------------------------------------------------------------------------------------------*/


