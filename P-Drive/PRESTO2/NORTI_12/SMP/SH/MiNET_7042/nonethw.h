/******************************************************************************
* NORTi Network User's Hardware Definition                                    *
*                                                                             *
*                                                                             *
* 17/Dec/1998                                                                 *
******************************************************************************/

/* LAN �R���g���[���̃x�[�X�A�h���X */

#define LANC_BASE   0x00cc0000L     /* MiNET-MB86964 MB86964 DLCR0-DLCR15 */

/* LAN �R���g���[���̓��o�� */

#define lan_in(n)       (*(volatile UB *)(LANC_BASE+(n)))
#define lan_out(n,c)    (*(volatile UB *)(LANC_BASE+(n)) = (c))
#define lan_inw(n)      (*(volatile UH *)(LANC_BASE+(n)))
#define lan_outw(n,c)   (*(volatile UH *)(LANC_BASE+(n)) = (c))

/* end */
