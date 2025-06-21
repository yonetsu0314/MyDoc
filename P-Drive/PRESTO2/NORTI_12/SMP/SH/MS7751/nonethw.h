/******************************************************************************
* NORTi Network User's Hardware Definition for MS7751SE01 board.              *
*                                                                             *
* File name         :  nonethw.h                                              *
*                                                                             *
* 05/Sept/2001      :           SP                                            *
* 20/Oct/2002  Board specific header file (ms7751.h) added      [SP]          *
******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

#include "sh7751.h"
#include "ms7751.h"
#include "pci7751.h"

#ifndef IP
    #define IP  IP_SIO          /* Interrupr priority declaration */
#endif


#ifdef __cplusplus
}
#endif

/* end */
