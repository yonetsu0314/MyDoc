/****************************************************************************
* NORTi TCP/IP �T���v�����C���v���O����                                     *
*   MS7615SE, SH7615EV, SH7616EV,                                           *
*   MS7709SE, MS7709ASE, MS7727SE, MS7729RSE,                               *
*   MS7750SE, MS7750SSE  ���p                                               *
*                                                                           *
*  Copyright (c) 1998-2003, MiSPO Co., Ltd.                                 *
*  All rights reserved.                                                     *
*                                                                           *
* 23/Apr/2000 Created                                                       *
* 06/Apr/2000 Supported MS7615SE01 board                                    *
* 25/Feb/2003 char/unsigned char �̃L���X�g��ǉ�(���[�j���O�̍폜)      KA *
* 20/Apr/2003 MS7750SE01�{MSWCEDB02�ֈڐA, POL, DUMP�@�\�̒ǉ�           SZ *
* 21/Apr/2003 MiNET-7042�����O, �e�{�[�h�p�̋N�����b�Z�[�W(title)�ǉ�    MI *
* 22/Apr/2003 MAC�A�h���X�ݒ�R�}���h mac_command �ǉ�                   MI *
* 07/May/2003 dhcp_command���s��ɃR���\�[���̓��͂������ɂȂ�����C�� YS *
* 07/May/2003 ftp_passwd_check��ǉ�                                     YS *
* 12/May/2003 ��l�̏C���𑼂�net7xxx.c�ɓK�p                          KA *
* 18/Jun/2003 sio�̐ݒ��"38400 B8 PN S1"�ɓ���Adns_resolver�v���g�^�C�v   *
*             CH_DUMP�̒ǉ�                                              KA *
****************************************************************************/

#include <stdio.h>
#include <string.h>
#include "kernel.h"
#include "nosio.h"

#if defined(SH7615EV)
const char title[] = "\r\n*** NORTi TCP/IP Sample Program for SH7615EV ***";
#include "nos7615.h"
#include "sh7615.h"

#elif defined(SH7616EV)
const char title[] = "\r\n*** NORTi TCP/IP Sample Program for SH7616EV ***";
#include "nos7616.h"
#include "sh7616.h"
#endif

#include "nonet.h"
#include "nonecho.h"
#include "nontftp.h"
#include "nonteln.h"
#include "noncons.h"
#include "nonfile.h"
#include "nonftp.h"
#include "nonitod.h"
#include "nondhcp.h"

extern ER dns_resolver(ID cepid, UW dns_ip, B *name, UW* ipaddr);

/* �f�o�b�O�p */

#ifndef POL
#define POL         0   /* LAN�h���C�o�Ŋ����݂��g��(0), �|�[�����O�Ńe�X�g(1) */
#endif
#if POL
void lan_intr(void);
#endif

#ifndef CH_DUMP
#define CH_DUMP     1   /* LAN�p�P�b�g�_���v�p�̃V���A���`���l���ԍ� */
#endif

/* ����u���b�N */

T_TELNETD telnetd;      /* TELNETD ����u���b�N */
T_CONSOLE console;      /* Console ����u���b�N */
T_FTP ftp;              /* FTP Server ����u���b�N */
T_FTP ftpc;             /* FTP Client ����u���b�N */
#define NFILE       8   /* �����I�[�v���\�ȃt�@�C���� */
T_FILE file[NFILE];     /* �t�@�C������u���b�N */
T_DISK disk[1];         /* �f�B�X�N�h���C�u����u���b�N */

/* �I�u�W�F�N�g������� */

TASK MainTask(void);
const T_CTSK cMainTask = { TA_HLNG, NULL, MainTask, 7, 1024, NULL, (const B *)"MainTask" };

/*****************************************************************************
* FTPd �̃p�X���[�h�`�F�b�N
*
******************************************************************************/

B *ftp_passwd_check(T_FTP *ftp, const B *user, const B *pass)
{
  #if 0
    if ((strcmp(user, "mispo") == 0)
     && (strcmp(pass, "12345") == 0))
        return "230 Logged in\r\n";
    else
        return NULL;
  #else
    return "230 Logged in\r\n";
  #endif
}

/*****************************************************************************
* TELNET �̃p�X���[�h�`�F�b�N
*
******************************************************************************/

B *telnet_passwd_check(T_TELNETD *t, const B *name, const B *passwd)
{
  #if 0
    if (strcmp(name, "mispo") == 0
     && strcmp(passwd, "12345") == 0)
        return (B *)"mispo>";
    else
        return (B *)NULL;
  #else
    return (B *)">";
  #endif
}

/*****************************************************************************
* TELNET �̃R�}���h����
*
******************************************************************************/

int ping_command(int argc, char *argv[]);
int ftp_command(int argc, char *argv[]);
int dial_command(int argc, char *argv[]);
int telnet_command(int argc, char *argv[]);
int dns_command(int argc, char *argv[]);
int mac_command(int argc, char *argv[]);
int disp_param(void);
ER dhcp_command(void);

BOOL telnetd_callback(T_TERMINAL *t, B *s)
{
    char *argv[16];
    int argc, r;

    for (argc = 0; argc < sizeof argv / sizeof (B *); argc++)
    {   while (*s == ' ')
            s++;
        if (*s == '\0')
            break;
        argv[argc] = (char *)s;
        s = (B *)strchr((char *)s, ' ');
        if (s == NULL)
            break;
        *s++ = '\0';
    }
    argc++;

    if (stricmp(argv[0], (char *)"?") == 0 || stricmp(argv[0], (char *)"help") == 0)
        print((B *)"ip  ping  ftp  telnet  techo  uecho  dns  dhcp");
    else if (stricmp(argv[0], (char *)"ip") == 0)
        disp_param();
    else if (stricmp(argv[0], (char *)"ping") == 0)
        r = ping_command(argc, argv);
    else if (stricmp(argv[0], (char *)"ftp") == 0)
        r = ftp_command(argc, argv);
    else if (stricmp(argv[0], (char *)"telnet") == 0)
        r = telnet_command(argc, argv);
    else if (stricmp(argv[0], (char *)"dns") == 0)
        dns_command(argc, argv);
    else if (stricmp(argv[0], (char *)"dhcp") == 0)
        dhcp_command();
    else if (stricmp(argv[0], (char *)"techo") == 0)
        tcp_echo_client(0);
    else if (stricmp(argv[0], (char *)"uecho") == 0)
        udp_echo_client(0);
    else
        ;   /* �ȉ��A�R�}���h�͓K���ɍ쐬���Ă������� */

    return TRUE;
}

/*****************************************************************************
* �\���⏕�֐�
*
******************************************************************************/

static void print_ip(UB *ipaddr)
{
    B  buf[10];
    INT i;

    for (i = 0 ; ; i++) {
        itou((char *)buf, (UH)ipaddr[i], itoulen(ipaddr[i]));
        print(buf);
        if (i >= 3)
            break;
        print((B *)".");
    }
}

static void print_mac(UB *macaddr)
{
    B  buf[10];
    INT i;

    for (i = 0 ; ; i++) {
        ito0X((char *)buf, (UH)macaddr[i], 2);
        print(buf);
        if (i >= 5)
            break;
        print((B *)"-");
    }
}

void error(char *err_str, ER ercd)
{
    char str[10];

    ito_d(str, (short)ercd, 3);
    print((B *)err_str);
    print((B *)":(");
    print((B *)str);
    print((B *)")\r\n");
}

/*****************************************************************************
* �ݒ�p�����[�^�̕\��
*
******************************************************************************/

int disp_param(void)
{
    print((B *)"[Ethernet Address   ] : [");
    print_mac(ethernet_addr);
    print((B *)"]\r\n");
    print((B *)"[Default IP Address ] : [");
    print_ip(default_ipaddr);
    print((B *)"]\r\n");
    print((B *)"[Default Gateway    ] : [");
    print_ip(default_gateway);
    print((B *)"]\r\n");
    print((B *)"[Subnet Mask        ] : [");
    print_ip(subnet_mask);
    print((B *)"]\r\n");
    return 1;
}

/*****************************************************************************
* DHCP�̃e�X�g
*
******************************************************************************/

extern T_TERMINAL *current_terminal;

ER dhcp_command(void)
{
    static int dhcp=FALSE;
    ER ercd;
    T_TERMINAL *ct;

    ct = current_terminal;

    if (ct == NULL)
        current_terminal = (T_TERMINAL *)&console;

    if (dhcp)
        ercd = dhcp_reb_data(0);
    else
        ercd = dhcp_get_data(0);

    if (ercd != E_OK)
        error((char *)"\r\n\r\nError get data from DHCP server.", ercd);
    else {
        print((B *)"\r\nSuccess get data from DHCP server.\r\n");
        disp_param();
        print((B *)"[DNS IP Address     ] : [");
        print_ip(dns_ipaddr);
        print((B *)"]\r\n");
        print((B *)"[DHCP IP Address    ] : [");
        print_ip(dhcp_ipaddr);
        print((B *)"]\r\n");
        if (!dhcp) {
            ercd = dhcp_sta_tsk(0, 0, 0, NULL);
            if (ercd != E_OK)
                error((char *)"\r\n\r\nError dhcp_sta_tsk", ercd);
            dhcp = TRUE;
        }
    }
    current_terminal = ct;

    return ercd;
}

/*****************************************************************************
* DNS�̃e�X�g
*
******************************************************************************/

int dns_command(int argc, char *argv[])
{
    ER ercd;
    UW dns_ip, ipaddr;
    B buf[20];

    if (argc < 2) {
        print((B *)"Usage: dns domainname [dns server address]\r\n");
        return 0;
    }

    dns_ip = byte4_to_long(dns_ipaddr);
    if (dns_ip == 0)
        dns_ip = ascii_to_ipaddr((B *)argv[2]);

    if (dns_ip == 0){
        print((B *)"DNS server address isn't specified\r\n");
        print((B *)"Usage: dns domainname dns-server-address\r\n");
        return 0;
    }

    ercd = dns_resolver(0, dns_ip, argv[1], &ipaddr);
    if (ercd == E_OK){
        print((B *)"[");
        print((B *)argv[1]);
        print((B *)"] : [");
        long_to_byte4((UB *)buf, ipaddr);
        print_ip((UB *)buf);
        print((B *)"]\r\n");
    }
    else{
        error((char *)"Error dns_resolver", ercd);
        return 0;
    }
    return 1;
}

/*****************************************************************************
* MAC�A�h���X�̓ǂݎ��
*
* Flash��EEPROM�ɕۑ��̏ꍇ�A�����ŕϐ��ɑ�����Ă��������B
* LAN�h���C�o���Ή����Ă���ALAN�R���g���[����EEPROM����擾�̏ꍇ�́AALL 0
* �Ƃ��Ă��������B
******************************************************************************/

BOOL read_ethernet_addr(void)
{
  #ifdef MSWCEDB02
    ethernet_addr[0] = 0x00;    /* 0�Ȃ�LAN�R���g���[����EEPROM����擾 */
    ethernet_addr[1] = 0x00;
    ethernet_addr[2] = 0x00;
    ethernet_addr[3] = 0x00;
    ethernet_addr[4] = 0x00;
    ethernet_addr[5] = 0x00;
  #else
    ethernet_addr[0] = 0x12;    /* �b��I�ɑ��l(���[�J����MAC�A�h���X) */
    ethernet_addr[1] = 0x34;
    ethernet_addr[2] = 0x56;
    ethernet_addr[3] = 0x78;
    ethernet_addr[4] = 0x9A;
    ethernet_addr[5] = 0x96;
  #endif
    return TRUE;
}

/*****************************************************************************
* IP�A�h���X���̓ǂݎ��
*
* Flash��EEPROM�ɕۑ��̏ꍇ�A�����ŕϐ��ɑ�����Ă��������B
* ���[�^�����݂���ꍇ�Adefault_gateway��subnet_mask���ݒ肵�Ă��������B
* DHCP�Ŏ擾����ꍇ�́A�����̕ϐ��ɏ㏑������܂��B
******************************************************************************/

BOOL read_ip_addr(void)
{
    default_ipaddr[0] = 192;    /* �b��I�ɑ��l!! */
    default_ipaddr[1] = 168;
    default_ipaddr[2] = 0;
    default_ipaddr[3] = 96;

    default_gateway[0] = 192;   /* �b��I�ɑ��l!! */
    default_gateway[1] = 168;
    default_gateway[2] = 0;
    default_gateway[3] = 1;

    subnet_mask[0] = 255;       /* �b��I�ɑ��l!! */
    subnet_mask[1] = 255;
    subnet_mask[2] = 255;
    subnet_mask[3] = 0;
    return TRUE;
}

/*****************************************************************************
* ���C���^�X�N
*
******************************************************************************/

TASK MainTask(void)
{
    ER ercd;

    /* �V���A���� LAN �p�P�b�g���e���o�� */
  #ifdef DUMP
    ercd = ini_sio(CH_DUMP, "38400 B8 PN S1");
    if (ercd != E_OK)
        goto END;
    ercd = ctl_sio(CH_DUMP, TSIO_RXE|TSIO_TXE|TSIO_RTSON);
    if (ercd != E_OK)
        goto END;
    ercd = landump_ini(0, 0, 0);
    if (ercd != E_OK)
        goto END;
  #endif

    /* IP & Ethernet Address �̓ǂݏo�� */
  #ifndef DHCP
    read_ip_addr();
  #endif
    read_ethernet_addr();

    /* �v���g�R���X�^�b�N������ */
    ercd = tcp_ini();
    if (ercd != E_OK)
        goto END;

    /* �R���\�[�������� */
  #if !(defined(DUMP) && CH_DUMP==0)
    ercd = console_ini(&console, 0, 0, 0, (B *)"38400 B8 PN S1");
    if (ercd != E_OK)
        goto END;
    console_print(&console, (B *)title);
  #endif

    /* DHCP�T�[�o�[����IP�A�h���X���擾 */
  #ifdef DHCP
    dly_tsk(2200/MSEC); /* �n�[�h�ɂ���Ă͂����Ə����Ȓl����(10�ȏ�) */
    ercd = dhcp_command();
    if (ercd != E_OK)
        read_ip_addr();
  #endif

    ercd = shell_ini(&console, 0, 0, telnet_passwd_check);
    if (ercd != E_OK)
        goto END;

    /* �t�@�C���V�X�e���� RAM �f�B�X�N�̏����� */
    ercd = file_ini(file, NFILE, 0, 0);
    if (ercd != E_OK)
        goto END;

    /* (��) SDRAM �̌㔼 1MB (0x06F00000�`0x06FFFFFF) ���g�p���܂�.  */
    /* ���̗̈�ɕϐ���X�^�b�N�����蓖�ĂȂ��悤���ӂ��Ă�������. */
    ercd = disk_ini(disk, (char *)"A:", ramdisk, 0x06F00000, 0x100000);
    if (ercd != E_OK)
        goto END;

    /* UDP �G�R�[�T�[�o�[������ */
    ercd = udpecho_ini(0, 0);
    if (ercd != E_OK)
        goto END;

    /* TFTP �T�[�o�[������ */
    ercd = tftpsrv_ini(0, 0);
    if (ercd != E_OK)
        goto END;

    /* TCP �G�R�[�T�[�o�[������ */
    ercd = tcpecho_ini(0, 0, 0);
    if (ercd != E_OK)
        goto END;

    /* TELNET �f�[���������� */
    ercd = telnetd_ini(&telnetd, 0, 0, 0, 0);
    if (ercd != E_OK)
        goto END;
    ercd = shell_ini(&telnetd, 0, 0, telnet_passwd_check);
    if (ercd != E_OK)
        goto END;

    /* FTP �T�[�o�[������ */
    ercd = ftp_cre_srv(&ftp, 0, 0, 0, 0, ftp_passwd_check);
    if (ercd != E_OK)
        goto END;
    ercd = ftp_sta_srv(&ftp);
    if (ercd != E_OK)
        goto END;

END:
    for (;;) {
      #if (POL) /* �|�[�����O�Ńf�o�b�O�̏ꍇ */
        chg_pri(TSK_SELF, 1);
        lan_intr();
        chg_pri(TSK_SELF, TPRI_INI);
      #else
        slp_tsk();
      #endif
    }
}

/*****************************************************************************
* peripheral �������̗�
*
******************************************************************************/

void ini_pio(void)
{
    sfr_setw(PFC_PBCR2, PBCR_RXD2);  /* to RxD2 */
    sfr_setw(PFC_PBCR2, PBCR_TXD2);  /* to TxD2 */
}

/*****************************************************************************
*  main�֐�
*
* �R���t�B�O���[�^���g�p����ꍇ�́A�R���t�B�O���[�^���쐬�����t�@�C����
* �����N���Ă��������B
******************************************************************************/
#ifndef USE_CONFIG
int main(void)
{
    ID id;

    /* �V�X�e�������� */
    ini_pio();                  /* peripheral ������ */
    sysini();

    /* �^�X�N���� */

    id = acre_tsk(&cMainTask);

    /* �^�X�N�N�� */

    sta_tsk(id, 0);

    /* �V�X�e���N�� */

    intsta();                   /* �����^�C�}�����݂��N�� */
    syssta();                   /* �}���`�^�X�N�ֈڍs */
}
#endif

/* end */
