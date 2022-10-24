/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _UART_H    /* Guard against multiple inclusion */
#define _UART_H
#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include "definitions.h"                // SYS function prototypes
#include "delay.h"
#include "../../../../../src/Arducam/ArducamCamera.h"
/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif


    /* ************************************************************************** */
    /* ************************************************************************** */
    /* Section: Constants                                                         */
    /* ************************************************************************** */
    /* ************************************************************************** */

    /*  A brief description of a section can be given directly below the section
        banner.
     */


    /* ************************************************************************** */
    /** Descriptive Constant Name

      @Summary
        Brief one-line summary of the constant.
    
      @Description
        Full description, explaining the purpose and usage of the constant.
        <p>
        Additional description in consecutive paragraphs separated by HTML 
        paragraph breaks, as necessary.
        <p>
        Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.
    
      @Remarks
        Any additional remarks
     */
#define EXAMPLE_CONSTANT 0
#define SET_PICTURE_RESOLUTION   0X01
#define SET_VIDEO_RESOLUTION     0X02
#define SET_BRIGHTNESS			 0X03
#define SET_CONTRAST             0X04
#define SET_SATURATION           0X05
#define SET_EV					 0X06
#define SET_WHITEBALANCE		 0X07
#define SET_SPECIAL_EFFECTS      0X08
#define SET_FOCUS_CONTROL        0X09
#define SET_EXPOSURE_CONTROL     0X0A
#define SET_GAIN_CONTROL         0X0B
#define SET_WHILEBALANCE_CONTROL 0X0C
#define SET_MANUAL_GAIN          0X0D
#define SET_MANUAL_EXPOSURE      0X0E
#define GET_CAMERA_INFO          0X0F
#define TAKE_PICTURE             0X10
#define SET_SHARPNESS            0X11
#define DEBUG_WRITE_REGISTER     0X12
#define READ_IMAGE_LENGTH        255

    
extern uint8_t UartCommBuff[20];
extern uint8_t UartCommLength;
extern uint8_t ReadBuffLength;
 uint8_t commandProcessing(ArducamCamera*,uint8_t*,uint8_t);
void reportCameraInfo(ArducamCamera*);
void cameraGetPicture(ArducamCamera*);   
void uart_init();
void arducamUartWrite(uint8_t data);
void uartWriteBuffer(uint8_t* buff,uint32_t length);
uint32_t arducamUartAvailable(void);
uint8_t arducamUartRead(void);
void write_string(char* buff);
void uart_read_callback ();
    // *****************************************************************************
    // *****************************************************************************
    // Section: Data Types
    // *****************************************************************************
    // *****************************************************************************

    /*  A brief description of a section can be given directly below the section
        banner.
     */


    // *****************************************************************************

    /** Descriptive Data Type Name

      @Summary
        Brief one-line summary of the data type.
    
      @Description
        Full description, explaining the purpose and usage of the data type.
        <p>
        Additional description in consecutive paragraphs separated by HTML 
        paragraph breaks, as necessary.
        <p>
        Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.

      @Remarks
        Any additional remarks
        <p>
        Describe enumeration elements and structure and union members above each 
        element or member.
     */
    typedef struct _example_struct_t {
        /* Describe structure member. */
        int some_number;

        /* Describe structure member. */
        bool some_flag;

    } example_struct_t;


    // *****************************************************************************
    // *****************************************************************************
    // Section: Interface Functions
    // *****************************************************************************
    // *****************************************************************************

    /*  A brief description of a section can be given directly below the section
        banner.
     */

    // *****************************************************************************
    /**
      @Function
        int ExampleFunctionName ( int param1, int param2 ) 

      @Summary
        Brief one-line description of the function.

      @Description
        Full description, explaining the purpose and usage of the function.
        <p>
        Additional description in consecutive paragraphs separated by HTML 
        paragraph breaks, as necessary.
        <p>
        Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.

      @Precondition
        List and describe any required preconditions. If there are no preconditions,
        enter "None."

      @Parameters
        @param param1 Describe the first parameter to the function.
    
        @param param2 Describe the second parameter to the function.

      @Returns
        List (if feasible) and describe the return values of the function.
        <ul>
          <li>1   Indicates an error occurred
          <li>0   Indicates an error did not occur
        </ul>

      @Remarks
        Describe any special behavior not described above.
        <p>
        Any additional remarks.

      @Example
        @code
        if(ExampleFunctionName(1, 2) == 0)
        {
            return 3;
        }
     */
    int ExampleFunction(int param1, int param2);


    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
