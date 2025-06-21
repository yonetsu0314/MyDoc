/*======================================================================
 * File name    : pcmtpl.h
 * Original		: r:/c/astra-s/exp/rcs/pcmtpl.h 1.1
 *----------------------------------------------------------------------
 * Function 	:
 *----------------------------------------------------------------------
 * $Header: p:/presto2/shinc/drive/RCS/pcmtpl.h 1.1 1970/01/01 00:00:00 sakaguti Exp $
 * $Log: pcmtpl.h $
 * ÉäÉrÉWÉáÉì 1.1  1970/01/01  00:00:00  sakaguti
 * êVãKÉäÉrÉWÉáÉì
 * 
 *----------------------------------------------------------------------
 *		(C) Copyright 2002, ISHIDA CO., LTD.                            
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                        
 *		PHONE (077) 553-4141                                            
 *----------------------------------------------------------------------*/
/************************************************************************/
/*																		*/
/*		ÉtÉ@ÉCÉãñº		: PCMTPL.H										*/
/* ==================================================================== */
/*		ã@î\ï™óﬁ		: ÇoÇbÇlÇbÇhÇ`É^ÉvÉãèàóù(CardInfoStructure)		*/
/*		çÏê¨ì˙ït		: 1997.12.9 < Tsu >								*/
/*		çÏê¨é“ñº		:												*/
/*																		*/
/*	óöóä«óù															*/
/* ------------+----------+-------------------------------------------- */
/*	ì˙ ït	   | ä«óùî‘çÜ | íSìñé“ñºãyÇ—ïœçXì‡óe						*/
/* ------------+----------+-------------------------------------------- */
/*			   |		  | 											*/
/* ------------+----------+-------------------------------------------- */
/*			   |		  | 											*/
/* ------------+----------+-------------------------------------------- */
/*																		*/
/************************************************************************/
#ifndef __PCMTPL__
#define __PCMTPL__

#define CISTPL_NULL				0x00
#define CISTPL_DEVICE			0x01
#define CISTPL_LONGLINK_CB		0x02
#define CISTPL_CONFIG_CB		0x04
#define CISTPL_CFTABLE_ENTRY_CB 0x05
#define CISTPL_LONGLINK_MFC		0x06
#define CISTPL_BAR				0x07
#define CISTPL_CHECKSUM 		0x10
#define CISTPL_LONGLINK_A		0x11
#define CISTPL_LONGLINK_C		0x12
#define CISTPL_LINKTARGET		0x13
#define CISTPL_NO_LINK			0x14
#define CISTPL_VERS_1			0x15
#define CISTPL_ALTSTR			0x16
#define CISTPL_DEVICE_A 		0x17
#define CISTPL_JEDEC_C			0x18
#define CISTPL_JEDEC_A			0x19
#define CISTPL_CONFIG			0x1a
#define CISTPL_CFTABLE_ENTRY	0x1b
#define CISTPL_DEVICE_OC		0x1c
#define CISTPL_DEVICE_OA		0x1d
#define CISTPL_DEVICE_GEO		0x1e
#define CISTPL_DEVICE_GEO_A		0x1f
#define CISTPL_MANFID			0x20
#define CISTPL_FUNCID			0x21
#define CISTPL_FUNCE			0x22
#define CISTPL_SWIL				0x23
#define CISTPL_END				0xff
/* Layer 2 tuples */
#define CISTPL_VERS_2			0x40
#define CISTPL_FORMAT			0x41
#define CISTPL_GEOMETRY 		0x42
#define CISTPL_BYTEORDER		0x43
#define CISTPL_DATE				0x44
#define CISTPL_BATTERY			0x45
/* Layer 3 tuples */
#define CISTPL_ORG				0x46

typedef struct cistpl_longlink_t {
    unsigned long	addr;
} cistpl_longlink_t;

typedef struct cistpl_checksum_t {
    unsigned short	addr;
    unsigned short	len;
    unsigned char	sum;
} cistpl_checksum_t;

#define CISTPL_MAX_FUNCTIONS	8
#define CISTPL_MFC_ATTR 		0x00
#define CISTPL_MFC_COMMON		0x01

typedef struct cistpl_longlink_mfc_t {
    unsigned char	nfn;
    struct {
		unsigned char	space;
		unsigned long	addr;
    } fn[CISTPL_MAX_FUNCTIONS];
} cistpl_longlink_mfc_t;

#define CISTPL_MAX_ALTSTR_STRINGS	4

typedef struct cistpl_altstr_t {
    int 	ns;
    int 	ofs[CISTPL_MAX_ALTSTR_STRINGS];
    char	str[254];
} cistpl_altstr_t;

#define CISTPL_DTYPE_NULL		0x00
#define CISTPL_DTYPE_ROM		0x01
#define CISTPL_DTYPE_OTPROM		0x02
#define CISTPL_DTYPE_EPROM		0x03
#define CISTPL_DTYPE_EEPROM		0x04
#define CISTPL_DTYPE_FLASH		0x05
#define CISTPL_DTYPE_SRAM		0x06
#define CISTPL_DTYPE_DRAM		0x07
#define CISTPL_DTYPE_FUNCSPEC	0x0d
#define CISTPL_DTYPE_EXTEND		0x0e

#define CISTPL_MAX_DEVICES	4

typedef struct cistpl_device_t {
    int 	ndev;
    struct device_info {
		unsigned char	type;
		unsigned char	wp;
		unsigned long	speed;
		unsigned long	size;
    } dev[CISTPL_MAX_DEVICES];
} cistpl_device_t;

#define CISTPL_DEVICE_MWAIT	0x01
#define CISTPL_DEVICE_3VCC	0x02

typedef struct cistpl_device_o_t {
    unsigned char		flags;
    cistpl_device_t	device;
} cistpl_device_o_t;

#define CISTPL_VERS_1_MAX_PROD_STRINGS	4

typedef struct cistpl_vers_1_t {
    unsigned char	major;
    unsigned char	minor;
    int 	ns;
    int 	ofs[CISTPL_VERS_1_MAX_PROD_STRINGS];
    char	str[254];
} cistpl_vers_1_t;

typedef struct cistpl_jedec_t {
    int 	nid;
    struct jedec_id {
		unsigned char	mfr;
		unsigned char	info;
    } id[CISTPL_MAX_DEVICES];
} cistpl_jedec_t;

typedef struct cistpl_manfid_t {
    unsigned short	manf;
    unsigned short	card;
} cistpl_manfid_t;

#define CISTPL_FUNCID_MULTI		0x00
#define CISTPL_FUNCID_MEMORY	0x01
#define CISTPL_FUNCID_SERIAL	0x02
#define CISTPL_FUNCID_PARALLEL	0x03
#define CISTPL_FUNCID_FIXED		0x04
#define CISTPL_FUNCID_VIDEO		0x05
#define CISTPL_FUNCID_NETWORK	0x06
#define CISTPL_FUNCID_AIMS		0x07
#define CISTPL_FUNCID_SCSI		0x08

#define CISTPL_SYSINIT_POST		0x01
#define CISTPL_SYSINIT_ROM		0x02

typedef struct cistpl_funcid_t {
    unsigned char	func;
    unsigned char	sysinit;
} cistpl_funcid_t;

typedef struct cistpl_funce_t {
    unsigned char	type;
    unsigned char	data[1];
} cistpl_funce_t;

/*======================================================================

    Modem Function Extension Tuples

======================================================================*/

#define CISTPL_FUNCE_SERIAL			0x00
#define CISTPL_FUNCE_SERIAL_DATA	0x08
#define CISTPL_FUNCE_SERIAL_FAX 	0x09
#define CISTPL_FUNCE_SERIAL_VOICE	0x0a
#define CISTPL_FUNCE_CAP			0x01
#define CISTPL_FUNCE_CAP_DATA		0x05
#define CISTPL_FUNCE_CAP_FAX		0x06
#define CISTPL_FUNCE_CAP_VOICE		0x07
#define CISTPL_FUNCE_SERV_DATA		0x02
#define CISTPL_FUNCE_SERV_FAX_1 	0x13
#define CISTPL_FUNCE_SERV_FAX_2 	0x23
#define CISTPL_FUNCE_SERV_FAX_3 	0x33
#define CISTPL_FUNCE_SERV_VOICE 	0x84

/* UART identification */
#define CISTPL_SERIAL_UART_8250 	0x00
#define CISTPL_SERIAL_UART_16450	0x01
#define CISTPL_SERIAL_UART_16550	0x02

/* UART capabilities */
#define CISTPL_SERIAL_UART_SPACE	0x01
#define CISTPL_SERIAL_UART_MARK 	0x02
#define CISTPL_SERIAL_UART_ODD		0x04
#define CISTPL_SERIAL_UART_EVEN 	0x08
#define CISTPL_SERIAL_UART_5BIT 	0x01
#define CISTPL_SERIAL_UART_6BIT 	0x02
#define CISTPL_SERIAL_UART_7BIT 	0x04
#define CISTPL_SERIAL_UART_8BIT 	0x08
#define CISTPL_SERIAL_UART_1STOP	0x10
#define CISTPL_SERIAL_UART_MSTOP	0x20
#define CISTPL_SERIAL_UART_2STOP	0x40

typedef struct cistpl_serial_t {
    unsigned char	uart_type;
    unsigned char	uart_cap_0;
    unsigned char	uart_cap_1;
} cistpl_serial_t;

typedef struct cistpl_modem_cap_t {
    unsigned char	flow;
    unsigned char	cmd_buf;
    unsigned int	rcv_buf:24;
    unsigned int	xmit_buf:24;
} cistpl_modem_cap_t;

typedef struct cistpl_data_serv_t {
    unsigned char	max_data_0;
    unsigned char	max_data_1;
    unsigned char	modulation_0;
    unsigned char	modulation_1;
    unsigned char	error_control;
    unsigned char	compression;
    unsigned char	cmd_protocol;
    unsigned char	escape;
    unsigned char	encrypt;
    unsigned char	misc_features;
    unsigned char	ccitt_code[1];
} cistpl_data_serv_t;

typedef struct cistpl_fax_serv_t {
    unsigned char	max_data_0;
    unsigned char	max_data_1;
    unsigned char	modulation;
    unsigned char	encrypt;
    unsigned char	features_0;
    unsigned char	features_1;
    unsigned char	ccitt_code[1];
} cistpl_fax_serv_t;

typedef struct cistpl_voice_serv_t {
    unsigned char	max_data_0;
    unsigned char	max_data_1;
} cistpl_voice_serv_t;

/*======================================================================

    LAN Function Extension Tuples

======================================================================*/

#define CISTPL_FUNCE_LAN_TECH		0x01
#define CISTPL_FUNCE_LAN_SPEED		0x02
#define CISTPL_FUNCE_LAN_MEDIA		0x03
#define CISTPL_FUNCE_LAN_NODE_ID	0x04
#define CISTPL_FUNCE_LAN_CONNECTOR	0x05

/* LAN technologies */
#define CISTPL_LAN_TECH_ARCNET		0x01
#define CISTPL_LAN_TECH_ETHERNET	0x02
#define CISTPL_LAN_TECH_TOKENRING	0x03
#define CISTPL_LAN_TECH_LOCALTALK	0x04
#define CISTPL_LAN_TECH_FDDI		0x05
#define CISTPL_LAN_TECH_ATM			0x06
#define CISTPL_LAN_TECH_WIRELESS	0x07

typedef struct cistpl_lan_tech_t {
    unsigned char	tech;
} cistpl_lan_tech_t;

typedef struct cistpl_lan_speed_t {
    unsigned long	speed;
} cistpl_lan_speed_t;

/* LAN media definitions */
#define CISTPL_LAN_MEDIA_UTP		0x01
#define CISTPL_LAN_MEDIA_STP		0x02
#define CISTPL_LAN_MEDIA_THIN_COAX	0x03
#define CISTPL_LAN_MEDIA_THICK_COAX	0x04
#define CISTPL_LAN_MEDIA_FIBER		0x05
#define CISTPL_LAN_MEDIA_900MHZ 	0x06
#define CISTPL_LAN_MEDIA_2GHZ		0x07
#define CISTPL_LAN_MEDIA_5GHZ		0x08
#define CISTPL_LAN_MEDIA_DIFF_IR	0x09
#define CISTPL_LAN_MEDIA_PTP_IR 	0x0a

typedef struct cistpl_lan_media_t {
    unsigned char	media;
} cistpl_lan_media_t;

typedef struct cistpl_lan_node_id_t {
    unsigned char	nb;
    unsigned char	id[16];
} cistpl_lan_node_id_t;

typedef struct cistpl_lan_connector_t {
    unsigned char	code;
} cistpl_lan_connector_t;

/*======================================================================

    IDE Function Extension Tuples

======================================================================*/

#define CISTPL_IDE_INTERFACE		0x01

typedef struct cistpl_ide_interface_t {
    unsigned char	interface;
} cistpl_ide_interface_t;

/* First feature byte */
#define CISTPL_IDE_SILICON		0x04
#define CISTPL_IDE_UNIQUE		0x08
#define CISTPL_IDE_DUAL 		0x10

/* Second feature byte */
#define CISTPL_IDE_HAS_SLEEP		0x01
#define CISTPL_IDE_HAS_STANDBY		0x02
#define CISTPL_IDE_HAS_IDLE		0x04
#define CISTPL_IDE_LOW_POWER		0x08
#define CISTPL_IDE_REG_INHIBIT		0x10
#define CISTPL_IDE_HAS_INDEX		0x20
#define CISTPL_IDE_IOIS16		0x40

typedef struct cistpl_ide_feature_t {
    unsigned char	feature1;
    unsigned char	feature2;
} cistpl_ide_feature_t;

#define CISTPL_FUNCE_IDE_IFACE		0x01
#define CISTPL_FUNCE_IDE_MASTER 	0x02
#define CISTPL_FUNCE_IDE_SLAVE		0x03

/*======================================================================

    Configuration Table Entries

======================================================================*/

typedef struct cistpl_config_t {
    unsigned char	last_idx;
    unsigned long	base;
    unsigned long	rmask[4];
    unsigned char	subtuples;
} cistpl_config_t;

/* These are bits in the 'present' field, and indices in 'param' */
#define CISTPL_POWER_VNOM		0
#define CISTPL_POWER_VMIN		1
#define CISTPL_POWER_VMAX		2
#define CISTPL_POWER_ISTATIC	3
#define CISTPL_POWER_IAVG		4
#define CISTPL_POWER_IPEAK		5
#define CISTPL_POWER_IDOWN		6

#define CISTPL_POWER_HIGHZ_OK	0x01
#define CISTPL_POWER_HIGHZ_REQ	0x02

typedef struct cistpl_power_t {
    unsigned char	present;
    unsigned char	flags;
    unsigned long	param[7];
} cistpl_power_t;

typedef struct cistpl_timing_t {
    unsigned long	wait, waitscale;
    unsigned long	ready, rdyscale;
    unsigned long	reserved, rsvscale;
} cistpl_timing_t;

#define CISTPL_IO_LINES_MASK	0x1f
#define CISTPL_IO_8BIT			0x20
#define CISTPL_IO_16BIT 		0x40
#define CISTPL_IO_RANGE 		0x80

#define CISTPL_IO_MAX_WIN	16

typedef struct cistpl_io_t {
    unsigned char	flags;
    int 	nwin;
    struct {
		unsigned long	base;
		unsigned long	len;
    } win[CISTPL_IO_MAX_WIN];
} cistpl_io_t;

typedef struct cistpl_irq_t {
    unsigned long	IRQInfo1;
    unsigned long	IRQInfo2;
} cistpl_irq_t;

#define CISTPL_MEM_MAX_WIN	8

typedef struct cistpl_mem_t {
    unsigned char	flags;
    unsigned char	nwin;
    struct {
		unsigned long	len;
		unsigned long	card_addr;
		char*			host_addr;
    } win[CISTPL_MEM_MAX_WIN];
} cistpl_mem_t;

#define CISTPL_CFTABLE_DEFAULT		0x01
#define CISTPL_CFTABLE_BVDS			0x02
#define CISTPL_CFTABLE_WP			0x04
#define CISTPL_CFTABLE_RDYBSY		0x08
#define CISTPL_CFTABLE_MWAIT		0x10
#define CISTPL_CFTABLE_AUDIO		0x20
#define CISTPL_CFTABLE_READONLY 	0x40
#define CISTPL_CFTABLE_PWRDOWN		0x80

typedef struct cistpl_cftable_entry_t {
    unsigned char		index;
    unsigned char		flags;
    unsigned char		interface;
    cistpl_power_t	vcc, vpp1, vpp2;
    cistpl_timing_t	timing;
    cistpl_io_t 	io;
    cistpl_irq_t	irq;
    cistpl_mem_t	mem;
    unsigned char		subtuples;
} cistpl_cftable_entry_t;

typedef struct cistpl_device_geo_t {
    int 	ngeo;
    struct geo {
		unsigned char		buswidth;
		unsigned long		erase_block;
		unsigned long		read_block;
		unsigned long		write_block;
		unsigned long		partition;
		unsigned long		interleave;
    } geo[CISTPL_MAX_DEVICES];
} cistpl_device_geo_t;

typedef struct cistpl_vers_2_t {
    unsigned char	vers;
    unsigned char	comply;
    unsigned short	dindex;
    unsigned char	vspec8, vspec9;
    unsigned char	nhdr;
    int 	vendor, info;
    char	str[244];
} cistpl_vers_2_t;

typedef struct cistpl_org_t {
    unsigned char	data_org;
    char	desc[30];
} cistpl_org_t;

#define CISTPL_ORG_FS		0x00
#define CISTPL_ORG_APPSPEC	0x01
#define CISTPL_ORG_XIP		0x02

typedef union cisparse_t {
    cistpl_device_t			device;
    cistpl_checksum_t		checksum;
    cistpl_longlink_t		longlink;
    cistpl_longlink_mfc_t	longlink_mfc;
    cistpl_vers_1_t			version_1;
    cistpl_altstr_t			altstr;
    cistpl_jedec_t			jedec;
    cistpl_manfid_t			manfid;
    cistpl_funcid_t			funcid;
    cistpl_funce_t			funce;
    cistpl_config_t			config;
    cistpl_cftable_entry_t	cftable_entry;
    cistpl_device_geo_t 	device_geo;
    cistpl_vers_2_t			vers_2;
    cistpl_org_t			org;
} cisparse_t;

typedef struct tuple_t {
    unsigned long	Attributes;
    unsigned char	DesiredTuple;
    unsigned long	Flags;			/* internal use */
    unsigned long	LinkOffset;		/* internal use */
    long			CISOffset;		/* internal use */
    unsigned char	TupleCode;
    unsigned char	TupleLink;
    unsigned char	TupleOffset;
    unsigned char	TupleDataMax;
    unsigned char	TupleDataLen;
    unsigned char	*TupleData;
} tuple_t;

/* Special cisdata_t(unsigned char) value */
#define RETURN_FIRST_TUPLE	0xff

/* Attributes for tuple calls */
#define TUPLE_RETURN_LINK	0x01
#define TUPLE_RETURN_COMMON	0x02

/* For ValidateCIS */
typedef struct cisinfo_t {
    unsigned long	Chains;
} cisinfo_t;

/* Manufacturer and Product ID codes */

#define MANFID_3COM			0x0101
#define PRODID_3COM_3C562		0x0562

#define MANFID_ADAPTEC			0x012f
#define PRODID_ADAPTEC_SCSI		0x0001

#define MANFID_ATT			0xffff
#define PRODID_ATT_KIT			0x0100

#define MANFID_IBM			0x00a4
#define PRODID_IBM_HOME_AND_AWAY	0x002e

#define MANFID_INTEL			0x0089
#define PRODID_INTEL_2PLUS		0x8422

#define MANFID_LINKSYS			0x0143
#define PRODID_LINKSYS_PCMLM28		0xc0ab

#define MANFID_MEGAHERTZ		0x0102
#define PRODID_MEGAHERTZ_VARIOUS	0x0000
#define PRODID_MEGAHERTZ_EM3288		0x0006

#define MANFID_MACNICA			0xc00b

#define MANFID_OSITECH			0x0140
#define PRODID_OSITECH_JACK_144		0x0001
#define PRODID_OSITECH_JACK_288		0x0002

#define MANFID_QUATECH			0x0137
#define PRODID_QUATECH_DUAL_RS232	0x0012

#define MANFID_SMC			0x0108
#define PRODID_SMC_ETHER		0x0105

#define MANFID_SUNDISK			0x0045

#define MANFID_TDK				0x0105

/* Offsets from ConfigBase for CIS registers */
#define CISREG_COR		0x00
#define CISREG_CCSR		0x02
#define CISREG_PRR		0x04
#define CISREG_SCR		0x06
#define CISREG_ESR		0x08
#define CISREG_IOBASE_0		0x0a
#define CISREG_IOBASE_1		0x0c
#define CISREG_IOBASE_2		0x0e
#define CISREG_IOBASE_3		0x10
#define CISREG_IOSIZE		0x12

/*
 * Configuration Option Register
 */
#define COR_CONFIG_MASK		0x3f
#define COR_MFC_CONFIG_MASK	0x38
#define COR_FUNC_ENA		0x01
#define COR_ADDR_DECODE		0x02
#define COR_IREQ_ENA		0x04
#define COR_LEVEL_REQ		0x40
#define COR_SOFT_RESET		0x80

/*
 * Card Configuration and Status Register
 */
#define CCSR_INTR_ACK		0x01
#define CCSR_INTR_PENDING	0x02
#define CCSR_POWER_DOWN		0x04
#define CCSR_AUDIO_ENA		0x08
#define CCSR_IOIS8		0x20
#define CCSR_SIGCHG_ENA		0x40
#define CCSR_CHANGED		0x80

/*
 * Pin Replacement Register
 */
#define PRR_WP_STATUS		0x01
#define PRR_READY_STATUS	0x02
#define PRR_BVD2_STATUS		0x04
#define PRR_BVD1_STATUS		0x08
#define PRR_WP_EVENT		0x10
#define PRR_READY_EVENT		0x20
#define PRR_BVD2_EVENT		0x40
#define PRR_BVD1_EVENT		0x80

/*
 * Socket and Copy Register
 */
#define SCR_SOCKET_NUM		0x0f
#define SCR_COPY_NUM		0x70

/*
 * Extended Status Register
 */
#define ESR_REQ_ATTN_ENA	0x01
#define ESR_REQ_ATTN		0x10



int GetNextTuple( int sock, tuple_t *tuple );
int GetFirstTuple( int sock, tuple_t *tuple );
int ParseTuple( tuple_t *tuple, cisparse_t *parse );
int ReadFirstTuple( int sock, tuple_t *tuple, cisparse_t *parse );


#endif
