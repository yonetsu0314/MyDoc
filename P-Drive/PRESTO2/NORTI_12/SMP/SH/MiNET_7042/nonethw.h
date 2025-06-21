/******************************************************************************
* NORTi Network User's Hardware Definition                                    *
*                                                                             *
*                                                                             *
* 17/Dec/1998                                                                 *
******************************************************************************/

/* LAN コントローラのベースアドレス */

#define LANC_BASE   0x00cc0000L     /* MiNET-MB86964 MB86964 DLCR0-DLCR15 */

/* LAN コントローラの入出力 */

#define lan_in(n)       (*(volatile UB *)(LANC_BASE+(n)))
#define lan_out(n,c)    (*(volatile UB *)(LANC_BASE+(n)) = (c))
#define lan_inw(n)      (*(volatile UH *)(LANC_BASE+(n)))
#define lan_outw(n,c)   (*(volatile UH *)(LANC_BASE+(n)) = (c))

/* end */
