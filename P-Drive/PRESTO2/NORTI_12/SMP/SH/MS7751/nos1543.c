/******************************************************************************
* Serial Com (RS232) Driver file for Super IO controller MC1543               *
*                                                                             *
*  File name : nos1543.c                                                      *
*  Copyright (c)  1995-2003, MiSPO Co., Ltd.                                  *
*  All rights reserved.                                                       *
*                                                                             *
*                                                                             *
* 18/Sept/2001     ; file created                                       [SP]  *
* 04/Dec/2001      ; Multi-channel transmission problem corrected       [SP]  *
* 08/Apr/2002      ; Comments added                                     [SP]  *
* 12/Aug/2002      ; set_param() function corrected                     [SP]  *
* 12/Aug/2002      ; Hardware flow control corrected                    [SP]  *
* 20/Oct/2002  Board specific header file (ms7751.h) added      [SP]          *
* 18/Dec/2002      chg_ims() updated                                    [SP]  *
* 27/Dec/2002 V4マクロの不要化、 esle を V4 と見なすよう順序を入れ替え     KA *
* 25/Feb/2003 char/unsigned char のキャストを追加(ワーニングの削除)        KA *
* 31/Aug/2003 ヘッダーのinclude順バグを修正                                KA *
******************************************************************************/

/******************************************************************************
* Compilation option for Renesas C compiler -

  ------- Big Endian, debug mode for UART1 --
    shc -cp=sh4 -op=0 -deb -en=b -def= BIG_ENDIAN,V4,CH=?,CH2 nos1543.c
  ------- Big Endian, debug mode for UART3 --
    shc -cp=sh4 -op=0 -deb -en=b -def= BIG_ENDIAN,V4,CH=?,CH3 nos1543.c

  ------- Little Endian, debug mode for UART1 --
    shc -cp=sh4 -op=0 -deb -en=l -def= LITTLE_ENDIAN,V4,CH=?,CH2 nos1543.c
  ------- Little Endian, debug mode for UART3 --
    shc -cp=sh4 -op=0 -deb -en=l -def= LITTLE_ENDIAN,V4,CH=?,CH3 nos1543.c

    Please specify serial channel number using macro definition of "CH".
    Valid serial channel numbers for this Serial Driver are {0,1,2,3}

    This Driver is programmed to configure UART1 and UART3 serial controllers,
    encapsulated within Super IO controller IC MC1543. UART1 is selected if "CH2"
    is defined during compilation. UART3 is selected if "CH3" is defined as the
    compilation option. If non is defined, UART1 is selected by default.

    As the Clock for serial controller is independent of Board frequency, the CLK
    defination is not required.

* User Definations and Specifications -

    -- Transmitter Data Buffer size (default value), TXBUFSZ = 1024 bytes.
    -- Receiver Data Buffer size (default value), 2 x BUFSZ = 2048.
        Tx buffer size an Rx data buffer size can be different. Serial Data flow
        may become slow if Data Buffer size is low. Please select the optimum values
        for Rx/Tx buffer size, depending on application requirement.

    -- Serial Driver Configuration
        Baud rates = {4800/9600/19200/38400/57600/115200bps}
                        (Default Baud rate = 9600 bps)
        parity control = {"PN" / "PO" / "PE"}
                {"PN"} = No parity (default)
                {"PE"} = Odd Parity
                {"PO"} = Even Parity
        Stop bits = {NULL / "S2"}
                {NULL} = 1 stop bit (default)
                {"S2"} = 2 stop bits
        Data length = {NULL / "B7" / "B6" / "B5" }
                {NULL} = 8-bit Data Frame length (Default)
                {"B7"} = 7-bit Data Frame length
                {"B6"} = 6-bit Data Frame length
                {"B5"} = 5-bit Data Frame length
        Flow Control = {NULL / "XON" / "RTS" / DTR"}
                {NULL} = NO flow control. (Default)
                {"XON"}= Xon/Xoff flow control. (Software control)
                {"RTS"}= Hardware flow control using RTS signal.
                {"DTR"}= Hardware flow control using DTR signal.

    -- Xon/Xoff flow control can be Suppressed with defination of "NOXON".
       Xon/Xoff control code will be disabled even when it is used as Driver
       configuration setting. User may specify their own control/debug code.

    -- Hardware flow control (using RTS / DTR) can be Suppressed with the
       defination of "NOFLOW". Hardware flow control code will be disabled even
       when it is used as Driver configuration setting. User may specify their
       own control/debug code.

******************************************************************************/

#include <stdlib.h>
#include <string.h>

#ifdef V2
 #include "norti.h"
 #define PSW         UW
#elif defined(V3)
 #include "norti3.h"
 #define wai_tsk(t)  tslp_tsk(t)
 #ifndef iwup_tsk
  #define iwup_tsk(i) wup_tsk(i)
 #endif
 #define PSW         UINT
#else /* V4 */
 #ifndef V4
 #define V4     1
 #endif
 #include "kernel.h"
 #define wai_tsk(t)  tslp_tsk(t)
 #define PSW         UINT
#endif

#include "nosio.h"
#include "sh7751.h"
#include "ms7751.h"
#include "nos1543.h"


#if (!defined CH2 && !defined CH3)
#define CH2             /* Default serial channel is UART1 */
#endif

#ifdef CH2

#define UART_RBR    UART1_RBR
#define UART_THR    UART1_THR
#define UART_DLL    UART1_DLL
#define UART_DLM    UART1_DLM
#define UART_IER    UART1_IER
#define UART_IIR    UART1_IIR
#define UART_FCR    UART1_FCR
#define UART_LCR    UART1_LCR
#define UART_MCR    UART1_MCR
#define UART_LSR    UART1_LSR
#define UART_MSR    UART1_MSR
#define UART_SCR    UART1_SCR

#else

#define UART_RBR    UART3_RBR
#define UART_THR    UART3_THR
#define UART_DLL    UART3_DLL
#define UART_DLM    UART3_DLM
#define UART_IER    UART3_IER
#define UART_IIR    UART3_IIR
#define UART_FCR    UART3_FCR
#define UART_LCR    UART3_LCR
#define UART_MCR    UART3_MCR
#define UART_LSR    UART3_LSR
#define UART_MSR    UART3_MSR
#define UART_SCR    UART3_SCR

#endif

#ifndef BUFSZ
 #define BUFSZ       1024        /* Rx Buffer size (Default Value) */
#endif

#ifndef TXBUFSZ
 #define TXBUFSZ     BUFSZ       /* Tx Buffer size (Default Value) */
#endif

#define XOFF_LEN    (BUFSZ*3/4) /* XOFF Length for Tx Buffer */
#define XON_LEN     (BUFSZ*1/4) /* XON Length for Tx Buffer */
#define RTSOFF_LEN  (BUFSZ-32)  /* RTS Off legth */
#define RTSON_LEN   (BUFSZ-80)  /* RTS ON length */

#define XOFF        0x13        /* Transmit Stop character code（CTRL-S）*/
#define XON         0x11        /* Transmit Start character code（CTRL-Q）*/

static UB txbuf[TXBUFSZ];       /* Transmit data buffer */
static UB rxbuf[BUFSZ*2];       /* Receive Data and Status buffer */

extern T_SIO SIO[];

static BOOL set_param(const B*, UB*, UW*);
static void init_buf(void);     /* Buffer Initialization Program */
static void clr_rxbuf(void);    /* Rx Buffer and Data pointer Initialization */
static void clr_txbuf(void);    /* Tx Buffer and Data pointer Initialization */
static BOOL put_txbuf(UB);
static BOOL tx_chr(UB);
static void tx_int(void);       /* Tx Interrupt handler function */

static void rx_int(void);

static TASK SerTask(const B *);
static const T_CTSK ctsk_sertest = { TA_HLNG, NULL, SerTask, 3, 1024, NULL, (B*)"SerTask"};

#ifndef IP
    #define IP  IP_SIO          /* Interrupr priority declaration */
#endif
/******************************************************************************
*   Routine to Initialize serial controller for specified mode and Baud Rate.
*
* Input -
*         baud_rate  - Baud rate.
*         mode       - Operation mode word.
* Output -
*         E_OK       - if initialization successful.
*******************************************************************************/
static ER serial_uart_init(const B *param)
{
    static UB   baud_lsb, baud_msb, mode;
    static UW  baud_rate, tmp;

    init_buf();             /* Initialize Rx and Tx data and Status buffer */

    /* Update the Numerical values for Serial controller Mode and Baud Rate data */
    if (!set_param(param, &mode, &baud_rate))
        return E_PAR;

    if (baud_rate == 115200) {
        baud_lsb = UART_DLL_115200;     /* select Baud Rate = 115200 */
        baud_msb = UART_DLM_115200;
    } else if (baud_rate == 57600) {
        baud_lsb = UART_DLL_57600;      /* select Baud Rate = 57600 */
        baud_msb = UART_DLM_57600;
    } else if (baud_rate == 38400) {
        baud_lsb = UART_DLL_38400;      /* select Baud Rate = 38400 */
        baud_msb = UART_DLM_38400;
    } else if (baud_rate == 19200) {
        baud_lsb = UART_DLL_19200;      /* select Baud Rate = 19200 */
        baud_msb = UART_DLM_19200;
    } else if (baud_rate == 4800) {
        baud_lsb = UART_DLL_4800;       /* select Baud Rate = 4800 */
        baud_msb = UART_DLM_4800;
    } else {
        baud_lsb = UART_DLL_9600;       /* default Baud Rate = 9600 */
        baud_msb = UART_DLM_9600;
    }
    pci_iowrite_byte(UART_LCR, UART_LCR_DLAB | mode, 0);
    pci_iowrite_byte(UART_DLL, baud_lsb, 0);
    pci_iowrite_byte(UART_DLM, baud_msb, 0);
    pci_iowrite_byte(UART_LCR, SIO[CH].cmd[1] = mode, 0);   /* Clear DLAB bit */
    SIO[CH].cmd[2] = UART_MCR_RTS | UART_MCR_DTR | UART_MCR_OUT1;
    pci_iowrite_byte(UART_MCR, SIO[CH].cmd[2], 0);
    SIO[CH].cmd[0] = 0;
    pci_iowrite_byte(UART_MCR, SIO[CH].cmd[0], 0);

    pci_iowrite_byte(UART_FCR, UART_FCR_CLEAR_XMIT | UART_FCR_CLEAR_RCVR, 0);
                    /* Rx & Tx FIFO Reset */
    for (tmp =0; tmp<40000; tmp++);           /* small delay */
    pci_iowrite_byte(UART_FCR, UART_FCR_ENABLE_FIFO | UART_FCR_TRIGGER_8, 0);
                    /* FIFO enable */

    SIO[CH].flag |= TSF_INIT;           /* Set Initialize Ok flag */


    clr_rxbuf();
    clr_txbuf();
    return E_OK;
}
/******************************************************************************
*   Routine to decode mode and Baud Rate from ascii to numeric value.
*
* Input -
*         *s          - Character string specifying mode and Baud Rate
* Output -
*         mode        - UART control mode word in numeric format.
*         baud_rate   -
*******************************************************************************/
static BOOL set_param(const B *s, UB* mode, UW* baud_rate)
{
    static B c;
    static UW b;
    static UB md;

    md = 0;     /* Update Configuration word */
    if (strstr((const char *)s, (char *)"PE") != NULL)
        md |= UART_LCR_PEN | UART_LCR_EPS;
    else if (strstr((const char *)s, (char *)"PO") != NULL)
        md &= ~UART_LCR_EPS;
    if (strstr((const char *)s, (char *)"S2") != NULL)
        md |= UART_LCR_STB;
    if (strstr((const char *)s, (char *)"B5") != NULL)
        md |= UART_LCR_WLEN5;
    else if (strstr((const char *)s, (char *)"B6") != NULL)
        md |= UART_LCR_WLEN6;
    else if (strstr((const char *)s, (char *)"B7") != NULL)
        md |= UART_LCR_WLEN7;
    else
        md |= UART_LCR_WLEN8;

    *mode = md;

    if (strstr((const char *)s, (char *)"XON") != NULL)   SIO[CH].flag |= TSF_XON;
    if (strstr((const char *)s, (char *)"DTR") != NULL)   SIO[CH].flag |= TSF_DTR;
    if (strstr((const char *)s, (char *)"RTS") != NULL)   SIO[CH].flag |= TSF_RTS;

    if (strstr((const char *)s, (char *)"ETX") != NULL)   SIO[CH].eot = 0x03;
    if (strstr((const char *)s, (char *)"EOT") != NULL)   SIO[CH].eot = 0x04;
    if (strstr((const char *)s, (char *)"CR" ) != NULL)   SIO[CH].eot = 0x0d;
    if (strstr((const char *)s, (char *)"LF" ) != NULL)   SIO[CH].eot = 0x0a;

    for (;;) {      /* Collect the Baud Rate */
        c = *s++;
        if (c == '\0') {
            *baud_rate = 9600L;     /* Default Baud Rate = 9600 bps */
            break;
        }

        if (c < '0' || c > '9')
            continue;
        c = *s++;
        if (c < '0' || c > '9')
            continue;
        c = *s++;
        if (c < '0' || c > '9')
            continue;

        s -= 3;
        b = 0L;
        for (;;) {      /* Get the numerical value for Baud Rate */
            c = *s++;
            if (c < '0' || c > '9')
                break;
            b = b * 10 + (c - '0');
        }
        *baud_rate = b;
        break;
    }
    return TRUE;
}
/******************************************************************************
*   Routine to Control RTS ON/OFF.
*
*******************************************************************************/
static void rts_on(void)
{
    static PSW psw;

    psw = vdis_psw();       /* Set Ready To Send Status */
    pci_iowrite_byte(UART_MCR, SIO[CH].cmd[2] |= UART_MCR_RTS, 0);
    vset_psw(psw);
}

static void rts_off(void)
{
    static PSW psw;

    psw = vdis_psw();       /* Clear Ready To Send Status */
    pci_iowrite_byte(UART_MCR, SIO[CH].cmd[2] &= ~UART_MCR_RTS, 0);
    vset_psw(psw);
}
/******************************************************************************
*   Routine to Control DTR ON/OFF.
*
*******************************************************************************/
static void dtr_on(void)
{
    static PSW psw;

    psw = vdis_psw();   /* Set Data Transmit Ready status */
    pci_iowrite_byte(UART_MCR, SIO[CH].cmd[2] |= UART_MCR_DTR, 0);
    vset_psw(psw);

}

static void dtr_off(void)
{
    static PSW psw;

    psw = vdis_psw();   /* Clear Data Transmit Ready status */
    pci_iowrite_byte(UART_MCR, SIO[CH].cmd[2] &= ~UART_MCR_DTR, 0);
    vset_psw(psw);

}

/******************************************************************************
*   SIO interrupt Enable/Disable Control functions.
*
*******************************************************************************/
static void ena_int_sio(void)
{
    static PSW psw;

    psw = vdis_psw();       /* Enable Serial Controller Interrupts */
    SIO[CH].cmd[0] |= UART_IER_ELSI;
    if (SIO[CH].flag & (TSF_RTS | TSF_DTR))
        SIO[CH].cmd[0] |= UART_IER_EMSI;
    pci_iowrite_byte(UART_MCR, SIO[CH].cmd[2] |= UART_MCR_ENINT, 0);
    pci_iowrite_byte(UART_IER, SIO[CH].cmd[0], 0);
    vset_psw(psw);
}

static void dis_int_sio(void)
{
    static PSW psw;

    psw = vdis_psw();       /* Disable Serial Controller Interrupts */
    pci_iowrite_byte(UART_IER, SIO[CH].cmd[0] = 0, 0);
    pci_iowrite_byte(UART_MCR, SIO[CH].cmd[2] = 0, 0);
    vset_psw(psw);
}
/******************************************************************************
*   Routine to Initialize Serial IO Control/Data buffer.
*
*******************************************************************************/
static void init_buf(void)
{
    static int init;

    /* Initialize Control Block */

    memset(&SIO[CH], 0, sizeof SIO[CH]);
    SIO[CH].flag = 0;
    SIO[CH].ch = CH;

    /* Initialize Data pointers */

    SIO[CH].txbuf = txbuf;          /* Tx Buffer */
    SIO[CH].rxbuf = rxbuf;          /* Rx Buffer */
    SIO[CH].txputp = SIO[CH].txbuf; /* Tx Buffer write pointer */
    SIO[CH].txgetp = SIO[CH].txbuf; /* Tx Buffer read pointer */
    SIO[CH].rxputp = SIO[CH].rxbuf; /* Rx buffer write pointer */
    SIO[CH].rxgetp = SIO[CH].rxbuf; /* Rx buffer read pointer */
}
/******************************************************************************
*   Routine to clear the Tx Buffer and Initiailze pointers
*
*******************************************************************************/
static void clr_txbuf(void)
{
    SIO[CH].txcnt = 0;
    SIO[CH].txgetp = SIO[CH].txputp;

}
/******************************************************************************
*   Routine to clear the Rx Buffer and Initiailze pointers
*
*******************************************************************************/
static void clr_rxbuf(void)
{

    SIO[CH].rxsts = 0;              /* Clear Rx status word */
    SIO[CH].oldsts = 0;
    SIO[CH].eotcnt = 0;             /* Clear Rx count control registers */
    SIO[CH].rxcnt = 0;
    SIO[CH].rxgetp = SIO[CH].rxputp;    /* Update read pointer = write pointer */

#ifndef NOFLOW
    if (SIO[CH].flag & TSF_RTS)
        rts_on();                   /* Ready To Send after Buffer Initialization */

    if (SIO[CH].flag & TSF_DTR)
        dtr_on();                   /* Data Transmit Ready after Buffer Init */
#endif

#ifndef NOXON
    if (SIO[CH].flag & TSF_TXOFF) {
        if (tx_chr(XON))
            SIO[CH].flag &= ~TSF_TXOFF;
        else
            SIO[CH].flag |= TSF_TXREQ;
    }
#endif
}

/******************************************************************************
*   Routine to collect serial controller status.
*
*******************************************************************************/
static int get_stat(void)
{
    static int stat;
    static UB tmp;
    PSW psw;

    stat = 0;

#ifndef NOFLOW
    psw = vdis_psw();
    pci_ioread_byte( UART_MSR, &tmp, 0);
    vset_psw(psw);

    SIO[CH].cmd[4] = tmp;

    if (!(tmp & UART_MSR_CTS))
        stat |= TSIO_CTS;
    if (!(tmp & UART_MSR_DSR))
        stat |= TSIO_DSR;
    if ((tmp & UART_MSR_DCD))
        stat |= TSIO_CD;
#endif

    psw = vdis_psw();
    pci_ioread_byte(UART_LSR, &tmp, 0);
    vset_psw(psw);


    tmp |= (SIO[CH].rxsts & (UART_LSR_FE | UART_LSR_OE | UART_LSR_PE));

    if (tmp & UART_LSR_PE)          /* Check if Parity error */
        stat |= TSIO_PE;
    if (tmp & UART_LSR_OE)          /* Check if Overflow error */
        stat |= UART_LSR_OE;
    if (tmp & UART_LSR_FE)          /* Check if Framing error */
        stat |= UART_LSR_FE;
    if (tmp & UART_LSR_BI)          /* Check if break detect error */
        stat |= TSIO_BD;
    if (tmp & UART_LSR_TEMT)        /* Check if Tx Empty */
        stat |= TSIO_TXEMP;

    return stat;
}
/******************************************************************************
*   Routine to transmit character (low level function).
*
*******************************************************************************/
static BOOL tx_chr(UB c)
{
    static UB tmp;
    static PSW  psw;

    psw = vdis_psw();
    pci_ioread_byte(UART_LSR, &tmp, 0);     /* Read Line Status */
    vset_psw(psw);

    if (!(tmp & (UART_LSR_THRE | UART_LSR_TEMT))) {       /* If Tx Hold Reg not empty */
        psw = vdis_psw();               /* Enable Tx Hold Reg Empty Interrupt */
        pci_iowrite_byte(UART_IER, SIO[CH].cmd[0] |= UART_IER_ETHREI, 0);
        vset_psw(psw);
        return FALSE;
    }

#ifndef NOFLOW
    if (SIO[CH].flag & (TSF_RTS|TSF_DTR)) {
        psw = vdis_psw();
        pci_ioread_byte(UART_MSR, &tmp, 0);     /* Read Modem Status */
        vset_psw(psw);

        SIO[CH].cmd[4] = tmp;

        if (!(tmp & UART_MSR_CTS)) {    /* If not ready to send */
            psw = vdis_psw();           /* Enable Modem Status Interrupt */
            pci_iowrite_byte(UART_IER, SIO[CH].cmd[0] |= UART_IER_EMSI, 0);
            vset_psw(psw);
            return FALSE;
        }
    }
#endif

    /* If Tx hold Reg Empty and clear to send then, transmit 8-bit data */
    psw = vdis_psw();
    pci_iowrite_byte(UART_THR, SIO[CH].txchr = c, 0);
    vset_psw(psw);
    return TRUE;
}

/******************************************************************************
*   Routine to store character in the Tx Buffer.
*
* Output -
*           Returns FALSE if Tx Buffer is full and no free space.
*******************************************************************************/
static BOOL put_txbuf(UB c)
{
    static UB* p;
    static UB   tmp;
    static PSW psw;
    static int tid, cnt;

    if ((SIO[CH].txcnt == 0) && (!(SIO[CH].flag & TSF_RXOFF))) {
        if (tx_chr(c))
            return TRUE;
    }

    if (SIO[CH].txcnt >= TXBUFSZ) {
        return FALSE;       /* Return error if no free space */
    }

    SIO[CH].txcnt++;

    *SIO[CH].txputp = c;    /* If else store character in Buffer */

    if (++SIO[CH].txputp >= (SIO[CH].txbuf + TXBUFSZ))
        SIO[CH].txputp = SIO[CH].txbuf;

    psw = vdis_psw();
    pci_iowrite_byte(UART_IER, SIO[CH].cmd[0] |= UART_IER_ETHREI, 0); /* added */
    vset_psw(psw);

    return TRUE;
}
/******************************************************************************
*   Routine to store received character in the Rx Buffer.
*
*******************************************************************************/
static void put_rxbuf(void)
{
    int cnt;
    UB  *p;

    /* If Data Break Error then not valid data to store */
    if ((SIO[CH].rxsts & UART_LSR_BI) && (SIO[CH].oldsts & UART_LSR_BI))
        return;

    cnt = SIO[CH].rxcnt;

    if (cnt == BUFSZ) {
        return;
    }

    if (cnt == BUFSZ - 1)
        SIO[CH].rxsts |= UART_LSR_DR;   /* Set Rx Buffer overflow */

    if ((*SIO[CH].rxputp = SIO[CH].rxchr) == SIO[CH].eot)
        SIO[CH].eotcnt++;
    *(SIO[CH].rxputp + BUFSZ) = SIO[CH].rxsts;

    if (++SIO[CH].rxputp >= (SIO[CH].rxbuf + BUFSZ))
        SIO[CH].rxputp = SIO[CH].rxbuf;

    SIO[CH].rxcnt = ++cnt;

#ifndef NOFLOW
    if ((cnt == RTSOFF_LEN) && (SIO[CH].flag & TSF_RTS))
        rts_off();

    if ((cnt == RTSOFF_LEN) && (SIO[CH].flag & TSF_DTR))
        dtr_off();
#endif

#ifndef NOXON
    if ((SIO[CH].flag & (TSF_XON|TSF_TXOFF)) == TSF_XON) {
        if (cnt == XOFF_LEN)
            SIO[CH].flag |= TSF_TXREQ;

    }
#endif
}

/******************************************************************************
*   Routine to get character from the Rx Buffer.
*
* Output -
*           Returns (-1) if no character in Rx Buffer.
*******************************************************************************/
static int get_rxbuf(UB *c)
{
    int cnt;
    int sts;
    UB  *p;

    cnt = SIO[CH].rxcnt;

    if ((SIO[CH].rxgetp == SIO[CH].rxputp) && (cnt == 0)) {
        return (-1);
    }

    if (cnt != 0) {
        SIO[CH].rxcnt = (UH)(--cnt);
    }

#ifndef NOXON
    if ((cnt == XON_LEN) && (SIO[CH].flag & TSF_TXOFF)) {

        SIO[CH].flag |= TSF_TXREQ;
    }
#endif

#ifndef NOFLOW
    if ((cnt == RTSON_LEN) && (SIO[CH].flag & TSF_RTS))
        rts_on();

    if ((cnt == RTSON_LEN) && (SIO[CH].flag & TSF_DTR))
        dtr_on();
#endif

    p = SIO[CH].rxgetp;
    if ((*c = *p) == SIO[CH].eot)
        SIO[CH].eotcnt--;
    sts = *(p + BUFSZ);

    if (++p >= SIO[CH].rxbuf + BUFSZ)
        p = SIO[CH].rxbuf;
    SIO[CH].rxgetp = p;

    return sts;
}
/******************************************************************************
*   Routine to handle Serial Reception Interrupt.
*
*******************************************************************************/
static void rx_int(void)
{
    static UB sts, chr;
    static int tid;
    static PSW  psw;

    tid = 0;
    for (;;) {
        psw = vdis_psw();
        pci_ioread_byte(UART_LSR, &sts, 0);
        vset_psw(psw);

        if (!(sts & UART_LSR_DR)) {               /* Check if Rx Data ready */
            break;
        }

        psw = vdis_psw();
        pci_ioread_byte(UART_RBR, &chr, 0); /* Get the Received Character */
        vset_psw(psw);

#ifndef NOXON
        if (SIO[CH].flag & TSF_XON) {
            if (chr == XOFF) {
                SIO[CH].flag |= TSF_RXOFF;
                 continue;
            }

            if (chr == XON) {
                SIO[CH].flag &= ~TSF_RXOFF;
                if (tid = SIO[CH].txtid)
                    iwup_tsk(tid);
                continue;
            }
        }
#endif

        SIO[CH].oldsts = SIO[CH].rxsts;
        SIO[CH].rxsts =
        (sts & (UART_LSR_BI | UART_LSR_FE | UART_LSR_OE | UART_LSR_PE | UART_LSR_FIFO));
        SIO[CH].rxchr = chr;

        put_rxbuf();

        tid = SIO[CH].rxtid;

    }
    if (tid)
       iwup_tsk(tid);

}

/******************************************************************************
*   Routine to handle Serial Transmission Interrupt.
*
*******************************************************************************/
static void tx_int(void)
{
    static UB *p;
    static int cnt, tid, flag;
    static UB tmp;
    static PSW  psw;


#ifndef NOFLOW
    if (SIO[CH].flag & (TSF_RTS|TSF_DTR)) {
        psw = vdis_psw();
        pci_iowrite_byte(UART_IER, SIO[CH].cmd[0] &= ~UART_IER_EMSI, 0);
        pci_ioread_byte(UART_MSR, &tmp, 0);
        vset_psw(psw);

        SIO[CH].cmd[4] = tmp;
    }
#endif

    psw = vdis_psw();
    pci_ioread_byte(UART_LSR, &tmp, 0);     /* Get Line Status */
    vset_psw(psw);

    if (tmp & (UART_LSR_THRE | UART_LSR_TEMT)) {    /* If Tx Hold Reg Empty */
        cnt = 0;
        psw = vdis_psw();
        pci_iowrite_byte(UART_IER, SIO[CH].cmd[0] &= ~UART_IER_ETHREI, 0);
        vset_psw(psw);

#ifndef NOXON
        if (SIO[CH].flag & TSF_RXOFF) {
            return;
        }

        if (SIO[CH].flag & TSF_TXREQ) {
            if (SIO[CH].flag & TSF_TXOFF) {
                psw = vdis_psw();
                pci_iowrite_byte(UART_THR, SIO[CH].txchr = XON, 0);
                vset_psw(psw);
                SIO[CH].flag &= ~TSF_TXOFF;
            }
            else {
                psw = vdis_psw();
                pci_iowrite_byte(UART_THR, SIO[CH].txchr = XOFF, 0);
                vset_psw(psw);
                SIO[CH].flag |= TSF_TXOFF;

            }
            SIO[CH].flag &= ~TSF_TXREQ;
             cnt++;
        }

#endif

        for(; cnt < 16; cnt++) {    /* Transmit the data Buffer */

#ifndef NOFLOW
            if (SIO[CH].flag & (TSF_RTS|TSF_DTR)) {
                psw = vdis_psw();
                pci_ioread_byte(UART_MSR, &tmp, 0);
                vset_psw(psw);
                if (!(tmp & UART_MSR_CTS)) {
                    psw = vdis_psw();
                    pci_iowrite_byte(UART_IER, SIO[CH].cmd[0] |= UART_IER_EMSI, 0);
                    vset_psw(psw);
                    return;
                }
            }
#endif

            if (--SIO[CH].txcnt == (UH)-1) {
                SIO[CH].txcnt = 0;
                if (tid = SIO[CH].tetid)
                    iwup_tsk(tid);
                if (tid = SIO[CH].txtid)
                    iwup_tsk(tid);
                return;
            }

            p = SIO[CH].txgetp;
            psw = vdis_psw();
            pci_iowrite_byte(UART_THR, *p, 0);
            vset_psw(psw);

            if (++p >= (SIO[CH].txbuf + TXBUFSZ))
                p = SIO[CH].txbuf;
            SIO[CH].txgetp = p;
            if (tid = SIO[CH].txtid)
                iwup_tsk(tid);
        }

        if (tid = SIO[CH].txtid)
            iwup_tsk(tid);

        psw = vdis_psw();
        pci_iowrite_byte(UART_IER, SIO[CH].cmd[0] |= UART_IER_ETHREI, 0);
        vset_psw(psw);
     }
}


/******************************************************************************
*   Interrupt Handler routine for Serial Rx and Tx Interrupts.
*
*******************************************************************************/

#ifdef CH2
void uart1_intr(void)
#else
void uart3_intr(void)
#endif
{
    UB tmp;
    PSW psw;

    tx_int();
    rx_int();
}
/******************************************************************************
*   Routine to Configure and Initialize Serial Controller Channel.
*
* Input  -
*        param   -  Configuration string.
*******************************************************************************/
#if (CH==0)
ER ini_sio0(const B *param)
#elif (CH==1)
ER ini_sio1(const B *param)
#elif (CH==2)
ER ini_sio2(const B *param)
#elif (CH==3)
ER ini_sio3(const B *param)
#endif
{
    UB mode;
    UW baud;

    if (serial_uart_init(param) != E_OK)    /* Initialize The Serial Control */
        return E_PAR;

    ena_int_sio();                      /* Enable Serial control Interrupts */

    return E_OK;
}
/******************************************************************************
*   Routine to Terminate Serial Controller Channel.
*
*******************************************************************************/
#if (CH==0)
void ext_sio0(void)
#elif (CH==1)
void ext_sio1(void)
#elif (CH==2)
void ext_sio2(void)
#elif (CH==3)
void ext_sio3(void)
#endif
{
    if (!(SIO[CH].flag & TSF_INIT))
        return;     /* Nothing to do if Serial Channel is not initialized */
    dis_int_sio();                      /* Disable Serial Interrupt */

#ifndef NOFLOW
    rts_off();                          /* RTS OFF */
#endif

    SIO[CH].flag &= ~TSF_INIT;          /* Clear Initialization status */
}
/******************************************************************************
*   Routine to get single character from specified Rx Channel Buffer.
*
*******************************************************************************/
#if (CH==0)
ER get_sio0(UB *c, TMO tmout)
#elif (CH==1)
ER get_sio1(UB *c, TMO tmout)
#elif (CH==2)
ER get_sio2(UB *c, TMO tmout)
#elif (CH==3)
ER get_sio3(UB *c, TMO tmout)
#endif
{
    ER err;
    INT sts;
    PSW psw;
    UINT imask;

    for (;;) {
#ifdef V4
        get_ims(&imask);
#else
        ref_ims(&imask);
#endif
        chg_ims(IP);      /* Disable Rx Interrupts */
        sts = get_rxbuf(c);

        if (sts != -1) {    /* Check if valid data received */
            chg_ims(imask);

            if (sts & UART_LSR_DR)
                err = EV_SIOOVF;        /* If Rx Buffer overflow */
            else if (sts & UART_LSR_OE)
                err = EV_SIOORN;        /* If Rx Overflow Status */
            else if (sts & UART_LSR_BI)
                err = EV_SIOBRK;        /* If Break in Line is detected */
            else if (sts & UART_LSR_FE)
                err = EV_SIOFRM;        /* If Framing error */
            else if (sts & UART_LSR_PE)
                err = EV_SIOPTY;        /* If Data Parity error */
            else
                err = E_OK;             /* Data Received without error */

            return err;
        }

        SIO[CH].rxtid = vget_tid();
        chg_ims(imask);

        err = wai_tsk(tmout);
        SIO[CH].rxtid = 0;
        vcan_wup();
        if (err)
            return err;
    }
}
/******************************************************************************
*   Routine to put single character to specified Tx Channel Buffer.
*
*******************************************************************************/
#if (CH==0)
ER put_sio0(UB c, TMO tmout)
#elif (CH==1)
ER put_sio1(UB c, TMO tmout)
#elif (CH==2)
ER put_sio2(UB c, TMO tmout)
#elif (CH==3)
ER put_sio3(UB c, TMO tmout)
#endif
{
    ER err;
    PSW psw;
    UINT imask;

    for (;;) {
#ifdef V4
        get_ims(&imask);
#else
        ref_ims(&imask);
#endif
        chg_ims(IP);

        if (put_txbuf(c)) {         /* Send character to Tx Buffer */
            chg_ims(imask);
            return E_OK;
        }

        SIO[CH].txtid = vget_tid();
        chg_ims(imask);
        err = wai_tsk(tmout);
        SIO[CH].txtid = 0;
        vcan_wup();
        if (err)
            return err;
    }
}
/******************************************************************************
*   Routine to Control Serial I/O.
*
*******************************************************************************/
#if (CH==0)
ER ctl_sio0(UH fncd)
#elif (CH==1)
ER ctl_sio1(UH fncd)
#elif (CH==2)
ER ctl_sio2(UH fncd)
#elif (CH==3)
ER ctl_sio3(UH fncd)
#endif
{
    UB c;
    PSW psw;
    UINT imask;

#ifdef V4
    get_ims(&imask);
#else
    ref_ims(&imask);
#endif
    chg_ims(IP);

    if (fncd & TSIO_RXCLR)
        clr_rxbuf();        /* Clear Rx Buffer if specified by Control Word */
    if (fncd & TSIO_TXCLR)
        clr_txbuf();        /* Clear Tx Buffer if specified by Control Word */

    if (fncd & (TSIO_RXE|TSIO_RXD)) {
        c = SIO[CH].cmd[0];
        if (fncd & TSIO_RXE)
            c |= (UART_IER_ERDAI);
                    /* Enable Rx if specified by Control Word */
        if (fncd & TSIO_RXD)
            c &= ~(UART_IER_ERDAI);
                    /* Disable Rx if specified by Control Word */
        SIO[CH].cmd[0] = c;
    }

    if (fncd & (TSIO_TXE|TSIO_TXD)) {
        c = SIO[CH].cmd[0];
        if (fncd & TSIO_TXE)
            c |= UART_IER_ETHREI;
                    /* EnableTRx if specified by Control Word */
        if (fncd & TSIO_TXD)
            c &= ~UART_IER_ETHREI;
                    /* Disable Tx if specified by Control Word */
        SIO[CH].cmd[0] = c;
    }
    psw = vdis_psw();
    pci_iowrite_byte(UART_IER, SIO[CH].cmd[0], 0); /* added */
    vset_psw(psw);

    if (fncd & (TSIO_SBON|TSIO_SBOFF)) {
        c = SIO[CH].cmd[1];
        if (fncd & TSIO_SBON)
            c |= UART_LCR_SBC;  /* Break Control if specified by Control Word */
        else
            c &= ~UART_LCR_SBC;
        psw = vdis_psw();
        pci_iowrite_byte(UART_LCR, SIO[CH].cmd[1] = c, 0);
        vset_psw(psw);
    }

#ifndef NOFLOW
    if (fncd & (TSIO_RTSON|TSIO_RTSOFF|TSIO_DTRON|TSIO_DTROFF)) {
        c = SIO[CH].cmd[2];
        if (fncd & TSIO_RTSON)
            c |=  UART_MCR_RTS; /* Set RTS if specified by Control Word */
        if (fncd & TSIO_RTSOFF)
            c &= ~UART_MCR_RTS; /* Clear RTS if specified by Control Word */
        if (fncd & TSIO_DTRON)
            c |=  UART_MCR_DTR; /* Set DTR if specified by Control Word */
        if (fncd & TSIO_DTROFF)
            c &= ~UART_MCR_DTR; /* Clear DTR if specified by Control Word */
        psw = vdis_psw();
        pci_iowrite_byte(UART_MCR, SIO[CH].cmd[2] = c, 0);
        vset_psw(psw);
    }
#endif

    chg_ims(imask);
    return E_OK;
}
/******************************************************************************
*   Routine to Refer to Serial I/O state.
*
*******************************************************************************/
#if (CH==0)
ER ref_sio0(T_SIOS *pk_sios)
#elif (CH==1)
ER ref_sio1(T_SIOS *pk_sios)
#elif (CH==2)
ER ref_sio2(T_SIOS *pk_sios)
#elif (CH==3)
ER ref_sio3(T_SIOS *pk_sios)
#endif
{
    UB stat;
    UINT imask;

#ifdef V4
    get_ims(&imask);
#else
    ref_ims(&imask);
#endif
    chg_ims(IP);

    stat = get_stat();          /* Get current line status */
    if (SIO[CH].txcnt != 0)
        stat &= ~TSIO_TXEMP;

    pk_sios->siostat = stat;
    pk_sios->rxlen   = SIO[CH].rxcnt;
    pk_sios->frbufsz = (UH)(TXBUFSZ - SIO[CH].txcnt);
    pk_sios->eotcnt  = SIO[CH].eotcnt;

    chg_ims(imask);

    return E_OK;
}
/******************************************************************************
*   Routine to Flush Serial Transmitting Buffer.
*
*******************************************************************************/
#if (CH==0)
ER fls_sio0(TMO tmout)
#elif (CH==1)
ER fls_sio1(TMO tmout)
#elif (CH==2)
ER fls_sio2(TMO tmout)
#elif (CH==3)
ER fls_sio3(TMO tmout)
#endif
{
    ER err;
    UINT imask;

    for (;;) {
#ifdef V4
        get_ims(&imask);
#else
        ref_ims(&imask);
#endif
        chg_ims(IP);
        if (SIO[CH].txcnt == 0) {   /* Check if Tx Buffer Empty */
            chg_ims(imask);
            return E_OK;
        }

        SIO[CH].tetid = vget_tid();
        chg_ims(imask);
        err = wai_tsk(tmout);
        SIO[CH].tetid = 0;
        vcan_wup();

        if (err)
            return err;
    }
}


/******************************************************************************
*   Sample Program for Serial Rx and Tx operation check..
*
*******************************************************************************/
static TASK SerTask(const  B* param)
{
    static UB c,chr;
    static UINT tmp;
    static T_SIOS sios;
    static ER ercd;
    UB title[] = "\n\r********** SERIAL DRIVER TEST PROGRAM *************\n\n\r\n";

#define ESC_KEY 0x1B

/* Numerical characters '0' to '9' are transmitted to serial console.
   The routine will also check for serial Reception. If any key is hit
   on console, it will be transmitted back. It will appear like print
   echo on serial console  */

    ercd = serial_uart_init(param);
    ctl_sio(CH, TSIO_RXE|TSIO_TXE|TSIO_RTSON |TSIO_DTRON);
    ena_int_sio();

    tmp =0;
    while(title[tmp])
        put_sio(CH, title[tmp++], 10/MSEC);

    for (;;) {
        c ='0';
        for (;;) {  /* Send Numerical char '0' to '9' in a continuous stream */
            dly_tsk(200/MSEC);
            put_sio(CH, c, 10/MSEC);
            if (++c > '9') {
                c = '0';
            }
            ref_sio(CH, &sios);
            if (sios.rxlen != 0) {
                sios.rxlen = 0;
                break;
            }
        }
        put_sio(CH, '\n', 10/MSEC);
        put_sio(CH, '\r', 10/MSEC);

        for(;;) {
            ercd = get_sio(CH, &c, 500/MSEC);
            if (ercd != E_OK) {
                dly_tsk(1);
                continue;
            }
            if (c == ESC_KEY)
                break;
            put_sio(CH, c, 10/MSEC);
            if (c == '\r')
                put_sio(CH, '\n', 10/MSEC);
        }
    }
}
/******************************************************************************
*   Serial Controller self check program.
*
*******************************************************************************/
#if (CH==0)
void serial_test0(const B *param)
#elif (CH==1)
void serial_test1(const B *param)
#elif (CH==2)
void serial_test2(const B *param)
#elif (CH==3)
void serial_test3(const B *param)
#endif

{
    ID id;

    id = acre_tsk(&ctsk_sertest);
    sta_tsk(id, param);
}
/************************* End of file nos1543.C *******************************/

