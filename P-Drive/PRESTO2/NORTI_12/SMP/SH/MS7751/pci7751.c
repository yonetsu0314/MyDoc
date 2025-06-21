/******************************************************************************
* Driver file for PCIC (SH7751）                                              *
*                                                                             *
*  File name : pci7751.c                                                      *
*  Copyright (c)  1995-2002, MiSPO Co., Ltd.                                  *
*  All rights reserved.                                                       *
*                                                                             *
*                                                                             *
* 26/June/2001     ; file created by                                    [SP]  *
* 08/Apr/2002      ; Comments and Beautification                        [SP]  *
* 14/Aug/2002      ; NC_RAM memory pool creation added                  [SP]  *
* 15/Aug/2002      ; Hardware init delay added in pcic_init function    [SP]  *
* 14/Dec/2002      ; function names corrected                           [SP]  *
* 27/Dec/2002 V4マクロの不要化、 esle を V4 と見なすよう順序を入れ替え     KA *
******************************************************************************/

#ifdef V2
 #include "norti.h"
#elif defined(V3)
 #include "norti3.h"
#else /* V4 */
 #ifndef V4
 #define V4     1
 #endif
 #include "kernel.h"
#endif

#include "sh7751.h"
#include "ms7751.h"
#include "pci7751.h"
#include "pci7751i.h"

void pcic_init(void);
void pci_clear_error_reg(void);

void pci_iowrite_long(UW, UW, UW);      /* PCI 32-bit IO write */
void pci_iowrite_word(UW, UH, UW);      /* PCI 16-bit IO write */
void pci_iowrite_byte(UW, UB, UW);      /* PCI 8-bit IO write */

void pci_ioread_long(UW, UW *, UW);     /* PCI 32-bit IO read */
void pci_ioread_word(UW, UH *, UW);     /* PCI 16-bit IO read */
void pci_ioread_byte(UW, UB *, UW);     /* PCI 8-bit IO read */

void pci_memwrite_long(UW, UW, UW);     /* PCI 32-bit MEM write */
void pci_memwrite_word(UW, UH, UW);     /* PCI 16-bit MEM write */
void pci_memwrite_byte(UW, UB, UW);     /* PCI 8-bit MEM write */

void pci_memread_long(UW, UW *, UW);    /* PCI 32-bit MEM read */
void pci_memread_word(UW, UH *, UW);    /* PCI 16-bit MEM read */
void pci_memread_byte(UW, UB *, UW);    /* PCI 8-bit MEM read */


void pci_write_config(UB, UB, UB, UB, UW);  /* PCI config reg write */
void pci_read_config(UB, UB, UB, UB, UW *); /* PCI config reg read */

static void set_config_addr(UB, UB, UB, UB);    /* PCI config address set */
void pci_check(void);
void init_lan_device(void);
void init_pmu_device(void);
void init_usb_device(void);
void init_sio_device(void);
UW htopci(UW);

UW addr_cpu_to_pci(UW);                  /* Convert CPU address to PCI address */
UW addr_pci_to_cpu(UW);                  /* Convert PCI address to CPU address */

PCI_device lan_device;              /* define for LAN controller 79C973 */
static PCI_device usb_device;       /* define for USB controller M5237 */
static PCI_device pmu_device;       /* define for power management unit M7101 */
PCI_device sio_device;              /* define for super I/O controller M1543 */
static PCI_device external_card[2]; /* additional cards attached */

    /************************************************************************
        Non-Cacheable SRAM space is reserved for external IO device interface.
        A variable length memory pool is created for this purpose.
        All IO devices should allocate non-cacheable area from this
        space.
    *************************************************************************/

const T_CMPL cmpl_ncram = { TA_TPRI, NCMEM_SZ, (VP)NCMEM_BASE, NULL};
ID mplid_ncram;                 /* Memory pool ID for NON Cache-RAM space */
ER init_ncram(void);

/*****************************************************************************/
/* Function to initialize Hitachi PCI controller.                            */
/*                                                                           */
/*****************************************************************************/
void pcic_init(void)
{
    UW data;

/* PCI BUS window is set to -
    (1)I/O :0xab000000-0xab0fffff(1MB)
    (2)MEM0:0xb0000000-0xb03fffff(4MB)
    (3)MEM1:0xd0000000-0xd03fffff(4MB)

   SH4 local bus window is set to -
    (1)MEM0:0x0c000000-0x0c3fffff(4MB)
    (2)MEM1:0x0d000000-0x0d3fffff(4MB) */

    for (data=0; data <65536; data++);  /* Short delay for hardware initialization */

    efr_outl(PCIC_PCICR, efr_inl(PCIC_PCICR) & (~0x01));
                /* clear CFINIT bit */

    efr_outl(PCIC_PCIBCR1, sfr_inl(BSC_BCR1)); /* set PCIC BCR1 0x0000008*/
    efr_outl(PCIC_PCIBCR2, (UW)sfr_inw(BSC_BCR2)); /* set PCIC BCR2 */
    efr_outl(PCIC_PCIWCR1, sfr_inl(BSC_WCR1)); /* set PCIC WCR1 */
    efr_outl(PCIC_PCIWCR2, sfr_inl(BSC_WCR2)); /* set PCIC WCR2 */
    efr_outl(PCIC_PCIWCR3, sfr_inl(BSC_WCR3)); /* set PCIC WCR3 */
    efr_outl(PCIC_PCIMCR, sfr_inl(BSC_MCR));  /* set PCIC MCR */
    efr_outl(PCIC_PCICONF1, 0x02900007); /* set config1 register */
    efr_outl(PCIC_PCICONF2, 0x06000000); /* set config2 register */
    efr_outl(PCIC_PCICONF4, 0xFFF00001); /* set PCI IO area 0xab000001*/

    efr_outl(PCIC_PCICONF5, PCI_MEM0_BASE); /* set PCI MEM0 area 0xb0000000*/
    efr_outl(PCIC_PCICONF6, PCI_MEM1_BASE); /* set PCI MEM1 area 0xd0000000*/
    efr_outl(PCIC_PCICONF11, 0x35051054);
                /* PCI Sub system ID & Sub system vendor ID */
    efr_outl(PCIC_PCILSR0, 0x00F00000);  /* PCI MEM0 size = 16MB */

    efr_outl(PCIC_PCILSR1, 0x00F00000);  /* PCI MEM1 size = 16MB */

    efr_outl(PCIC_PCILAR0, SH4_MEM0_BASE);  /* MEM0 local addr = 0xAC000000 */
    efr_outl(PCIC_PCILAR1, SH4_MEM1_BASE);  /* MEM1 local addr = 0xAD000000 */
    efr_outl(PCIC_PCIINTM, 0x0000c3ff);  /* enable all PCI interrupts */
    efr_outl(PCIC_PCIAINTM, 0x0000980f); /* enable PCI arbiter interrupts */

    efr_outl(PCIC_PCICR, efr_inl(PCIC_PCICR) | 0xa5000001);
                /* Start the PCI arbiter and control */
    efr_outl(PCIC_PCIMBR, 0);
    efr_outl(PCIC_PCIIOBR, 0);

    init_ncram();   /* Creat Fix size memory pool for non-Cache applications */
}

/*****************************************************************************/
/* Function to clear all PCIC error status registers                         */
/*                                                                           */
/*****************************************************************************/
void pci_clear_error_reg(void)
{
    efr_outl(PCIC_PCICONF1, efr_inl(PCIC_PCICONF1) | 0xF9000000);
    efr_outl(PCIC_PCIINT, efr_inl(PCIC_PCIINT) | 0x0000C3FF);
    efr_outl(PCIC_PCIAINT, efr_inl(PCIC_PCIAINT) | 0x0000380F);
    efr_outl(PCIC_PCIPINT, efr_inl(PCIC_PCIPINT) | 0x00000003);
}

/*****************************************************************************/
/* Function to write long word data to specified PCI IO address.             */
/*                                                                           */
/* Input  -                                                                  */
/*    pci_io_addr  :PCI I/O address                                          */
/*    pci_io_data  :PCI I/O data                                             */
/*    io_lock      :lock option                                              */
/*****************************************************************************/
void pci_iowrite_long(UW pci_io_addr,
                             UW pci_io_data,
                             UW io_lock)
{
    UW pci_io_brset;
    UW* pci_io_area;

    pci_io_brset = pci_io_addr & 0xFFFC0000;
    io_lock &= 0x1;
    pci_io_brset |= io_lock;
    pci_io_area   = (UW*)(PCI_IO_AREA + (pci_io_addr & 0x0003FFFF));
    *pci_io_area = pci_io_data;                 /* store data in PCI area */
}

/*****************************************************************************/
/* Function to write word size data to specified PCI IO address.             */
/*                                                                           */
/* Input  -                                                                  */
/*    pci_io_addr  :PCI I/O address                                          */
/*    pci_io_data  :PCI I/O data                                             */
/*    io_lock      :lock option                                              */
/*****************************************************************************/
void pci_iowrite_word(UW pci_io_addr,
                             UH pci_io_data,
                             UW io_lock)
{
    UW pci_io_brset;
    UH* pci_io_area;

    pci_io_brset = pci_io_addr & 0xFFFC0000;
    io_lock &= 0x1;
    pci_io_brset |= io_lock;                    /* set PCI IO base address */
    pci_io_area   = (UH*)(PCI_IO_AREA + (pci_io_addr & 0x0003FFFF));
    *pci_io_area = pci_io_data;                 /* store data in PCI area */
}

/*****************************************************************************/
/* Function to write byte size data to specified PCI IO address.             */
/*                                                                           */
/* Input  -                                                                  */
/*    pci_io_addr  :PCI I/O address                                          */
/*    pci_io_data  :PCI I/O data                                             */
/*    io_lock      :lock option                                              */
/*****************************************************************************/
void pci_iowrite_byte(UW pci_io_addr,
                             UB pci_io_data,
                             UW io_lock)
{
    UW pci_io_brset;
    UB* pci_io_area;

    pci_io_brset = pci_io_addr & 0xFFFC0000;
    io_lock &= 0x1;
    pci_io_brset |= io_lock;                    /* set PCI IO base address */
    pci_io_area = (UB*)(PCI_IO_AREA + (pci_io_addr & 0x0003FFFF));
    *pci_io_area = pci_io_data;                 /* store data in PCI area */
}

/*****************************************************************************/
/* Function to read long-word size data from specified PCI IO address.       */
/*                                                                           */
/* Input  -                                                                  */
/*    pci_io_addr  :PCI I/O address                                          */
/*    pci_io_data  :PCI I/O data pointer where read data will be stored      */
/*    io_lock      :lock option                                              */
/*****************************************************************************/
void pci_ioread_long(UW pci_io_addr,
                            UW *pci_io_data,
                            UW io_lock)
{
    UW pci_io_brset;
    UW* pci_io_area;

    pci_io_brset = pci_io_addr & 0xFFFC0000;
    io_lock &= 0x1;
    pci_io_brset |= io_lock;                    /* set PCI IO base address */

    pci_io_area   = (UW*)(PCI_IO_AREA + (pci_io_addr & 0x0003FFFF));
    *pci_io_data = *pci_io_area;                /* read data from PCI area */
}

/*****************************************************************************/
/* Function to read word size data from specified PCI IO address.            */
/*                                                                           */
/* Input  -                                                                  */
/*    pci_io_addr  :PCI I/O address                                          */
/*    pci_io_data  :PCI I/O data pointer where read data will be stored      */
/*    io_lock      :lock option                                              */
/*****************************************************************************/
void pci_ioread_word(UW pci_io_addr,
                            UH *pci_io_data,
                            UW io_lock)
{
    UW pci_io_brset;
    UH* pci_io_area;

    pci_io_brset = pci_io_addr & 0xFFFC0000;
    io_lock &= 0x1;
    pci_io_brset |= io_lock;                    /* set PCI IO base address */

    pci_io_area   = (UH*)(PCI_IO_AREA + (pci_io_addr & 0x0003FFFF));
    *pci_io_data = *pci_io_area;    /* read data from PCI area */
}

/*****************************************************************************/
/* Function to read byte size data from specified PCI IO address.            */
/*                                                                           */
/* Input  -                                                                  */
/*    pci_io_addr  :PCI I/O address                                          */
/*    pci_io_data  :PCI I/O data pointer where read data will be stored      */
/*    io_lock      :lock option                                              */
/*****************************************************************************/
void pci_ioread_byte(UW pci_io_addr,
                            UB *pci_io_data,
                            UW io_lock)
{
    UW pci_io_brset;
    UB* pci_io_area;

    pci_io_brset = pci_io_addr & 0xFFFC0000;
    io_lock &= 0x1;
    pci_io_brset |= io_lock;                    /* set PCI IO base address */

    pci_io_area   = (UB *)(PCI_IO_AREA + (pci_io_addr & 0x0003FFFF));
    *pci_io_data = *pci_io_area;                /* read data from PCI area */
}

/*****************************************************************************/
/* Function to write long word data to specified PCI MEM address.            */
/*                                                                           */
/* Input  -                                                                  */
/*    pci_mem_addr  :PCI MEM address                                         */
/*    pci_mem_data  :PCI MEM data                                            */
/*    mem_lock      :lock option                                             */
/*****************************************************************************/
void pci_memwrite_long(UW pci_mem_addr,
                              UW pci_mem_data,
                              UW mem_lock)
{
    UW pci_mem_brset, pci_mem_area;

    pci_mem_brset = pci_mem_addr & 0xFF000000;
    mem_lock &= 0x1;
    pci_mem_brset |= mem_lock;
    efr_outl(PCIC_PCIMBR, pci_mem_brset);
    pci_mem_area   = PCI_MEM_AREA + (pci_mem_addr & 0x00FFFFFF);
    *((UW *)pci_mem_area) = pci_mem_data; /* store data in PCI area */
}

/*****************************************************************************/
/* Function to write word size data to specified PCI MEM address.            */
/*                                                                           */
/* Input  -                                                                  */
/*    pci_mem_addr  :PCI MEM address                                         */
/*    pci_mem_data  :PCI MEM data                                            */
/*    mem_lock      :lock option                                             */
/*****************************************************************************/
void pci_memwrite_word(UW pci_mem_addr,
                              UH pci_mem_data,
                              UW mem_lock)
{
    UW pci_mem_brset, pci_mem_area;

    pci_mem_brset = pci_mem_addr & 0xFF000000;
    mem_lock &= 0x1;
    pci_mem_brset |= mem_lock;
    efr_outl(PCIC_PCIMBR, pci_mem_brset);
    pci_mem_area   = PCI_MEM_AREA + (pci_mem_addr & 0x00FFFFFF);
    *((UH *)pci_mem_area) = pci_mem_data; /* store data in PCI area */
}

/*****************************************************************************/
/* Function to write byte size data to specified PCI MEM address.            */
/*                                                                           */
/* Input  -                                                                  */
/*    pci_mem_addr  :PCI MEM address                                         */
/*    pci_mem_data  :PCI MEM data                                            */
/*    mem_lock      :lock option                                             */
/*****************************************************************************/
void pci_memwrite_byte(UW pci_mem_addr,
                              UB pci_mem_data,
                              UW mem_lock)
{
    UW pci_mem_brset, pci_mem_area;

    pci_mem_brset = pci_mem_addr & 0xFF000000;
    mem_lock &= 0x1;
    pci_mem_brset |= mem_lock;
    efr_outl(PCIC_PCIMBR, pci_mem_brset);
    pci_mem_area   = PCI_MEM_AREA + (pci_mem_addr & 0x00FFFFFF);
    *((UB *)pci_mem_area) = pci_mem_data; /* store data in PCI area */
}

/*****************************************************************************/
/* Function to read long word data from specified PCI MEM address.           */
/*                                                                           */
/* Input  -                                                                  */
/*    pci_mem_addr  :PCI MEM address                                         */
/*    pci_mem_data  :PCI MEM data pointer where read data will be stored     */
/*    mem_lock      :lock option                                             */
/*****************************************************************************/
void pci_memread_long(UW pci_mem_addr,
                             UW *pci_mem_data,
                             UW mem_lock)
{
    UW pci_mem_brset, pci_mem_area;

    pci_mem_brset = pci_mem_addr & 0xFF000000;
    mem_lock &= 0x1;
    pci_mem_brset |= mem_lock;
    efr_outl(PCIC_PCIMBR, pci_mem_brset);
    pci_mem_area   = PCI_MEM_AREA + (pci_mem_addr & 0x00FFFFFF);
    *pci_mem_data = *((UW *)pci_mem_area); /* read data from PCI area */
}

/*****************************************************************************/
/* Function to read word size data from specified PCI MEM address.           */
/*                                                                           */
/* Input  -                                                                  */
/*    pci_mem_addr  :PCI MEM address                                         */
/*    pci_mem_data  :PCI MEM data pointer where read data will be stored     */
/*    mem_lock      :lock option                                             */
/*****************************************************************************/
void pci_memread_word(UW pci_mem_addr,
                             UH *pci_mem_data,
                             UW mem_lock)
{
    UW pci_mem_brset;
    UW pci_mem_area;

    pci_mem_brset = pci_mem_addr & 0xFF000000;
    mem_lock &= 0x1;
    pci_mem_brset |= mem_lock;
    efr_outl(PCIC_PCIMBR, pci_mem_brset);
    pci_mem_area   = PCI_MEM_AREA + (pci_mem_addr & 0x00FFFFFF);
    *pci_mem_data = *((UH *)pci_mem_area); /* read data from PCI area */
}

/*****************************************************************************/
/* Function to read byte size data from specified PCI MEM address.           */
/*                                                                           */
/* Input  -                                                                  */
/*    pci_mem_addr  :PCI MEM address                                         */
/*    pci_mem_data  :PCI MEM data pointer where read data will be stored     */
/*    mem_lock      :lock option                                             */
/*****************************************************************************/
void pci_memread_byte(UW pci_mem_addr,
                             UB *pci_mem_data,
                             UW mem_lock)
{
    UW pci_mem_brset, pci_mem_area;

    pci_mem_brset = pci_mem_addr & 0xFF000000;
    mem_lock &= 0x1;
    pci_mem_brset |= mem_lock;
    efr_outl(PCIC_PCIMBR, pci_mem_brset);
    pci_mem_area   = PCI_MEM_AREA + (pci_mem_addr & 0x00FFFFFF);
    *pci_mem_data = *((UB *)pci_mem_area); /* read data from PCI area */
}

/*****************************************************************************/
/* Function to write data to specified PCIC config space address.            */
/*                                                                           */
/* Input  -                                                                  */
/*    bus_no        :PCI bus no (0-255) (0 for primary bus)                  */
/*    device_no     :device number on PCI bus (0-31)                         */
/*    func_no       :functional number (0-7 usually 0)                       */
/*    config_addr   :Configuration register address                          */
/*    config_data   :Data to be written in Config register                   */
/*****************************************************************************/
void pci_write_config(  UB bus_no,
                        UB device_no,
                        UB func_no,
                        UB config_addr,
                        UW config_data)
{
    set_config_addr(bus_no, device_no, func_no, config_addr);
                /* set PIO address register */
    efr_outl(PCIC_PCIPDR, config_data);
                /* load PIO data register */
}

/*****************************************************************************/
/* Function to read data from specified PCIC config space address.           */
/*                                                                           */
/* Input  -                                                                  */
/*    bus_no        :PCI bus no (0-255) (0 for primary bus)                  */
/*    device_no     :device number on PCI bus (0-31)                         */
/*    func_no       :functional number (0-7 usually 0)                       */
/*    config_addr   :Configuration register address                          */
/*    config_data   :Pointer to data to be written in Config register        */
/*****************************************************************************/
void pci_read_config(   UB bus_no,
                        UB device_no,
                        UB func_no,
                        UB config_addr,
                        UW *config_data)
{
    set_config_addr(bus_no, device_no, func_no, config_addr);
                /* set PIO address register */
    *config_data = efr_inl(PCIC_PCIPDR);
                /* read PIO data register */
}

/*****************************************************************************/
/* Function to set config address for PCIC                                   */
/*                                                                           */
/* Input  -                                                                  */
/*    bus_no        :PCI bus no (0-255) (0 for primary bus)                  */
/*    device_no     :device number on PCI bus (0-31)                         */
/*    func_no       :functional number (0-7 usually 0)                       */
/*    config_addr   :Configuration register address                          */
/*****************************************************************************/
static void set_config_addr(UB bus_no,
                            UB device_no,
                            UB func_no,
                            UB config_addr)
{
    UW config_addr_data;
    config_addr_data = 0x80000000;          /* CFGEN enable */
    bus_no &= 0xFF;
    config_addr_data |= (bus_no<<16);       /* set bus number */
    device_no &= 0x1F;
    config_addr_data |= (device_no<<11);        /* set device number */
    func_no &= 0x7;
    config_addr_data |= (func_no<<8);       /* set function type */
    config_addr &= 0xFC;
    config_addr_data |= config_addr;
    efr_outl(PCIC_PCIPAR, config_addr_data);    /* set PCIPAR register */
}

/*****************************************************************************/
/* Function to check PCI device                                              */
/*                                                                           */
/*****************************************************************************/
void pci_check(void)
{
UB device_no, add_on;
UW data, tmp;
UH vendor_id, device_id;

    add_on = 0;         /* temperory counter for external PCI devices */
    pcic_init();
    pci_clear_error_reg();
    for (device_no = 0; device_no <= 0x10; device_no++) {
        pci_read_config(0, device_no, 0, PCI_ID, &data);
        if (data != 0xffffffff) {
            vendor_id = (UH)(data & 0x0000ffff);
            device_id = (UH)((data >> 16) & 0x0000ffff);

            switch (vendor_id){
                case PCI_VENDOR_ID_AMD:     /* check for LANC device */
                    lan_device.vendor_id = vendor_id;
                    lan_device.device_id = device_id;
                    lan_device.bus_no = 0;
                    lan_device.slot_no = device_no;
                    pci_read_config(0, device_no, 0, PCI_ILR, &tmp);
                    lan_device.int_no = (UB)(tmp & 0xff);
                    pci_read_config(0, device_no, 0, PCI_IOBASE, &tmp);
                    lan_device.io_addr = tmp;
                    pci_read_config(0, device_no, 0, PCI_MEMBASE, &tmp);
                    lan_device.mem_addr = tmp;
                    break;

                case PCI_VENDOR_ID_AL:      /* check for ALi device */
                    switch (device_id) {
                        case PCI_DEVICE_ID_AL_M1533:
                            sio_device.vendor_id = vendor_id;
                            sio_device.device_id = device_id;
                            sio_device.bus_no = 0;
                            sio_device.slot_no = device_no;
                            pci_read_config(0, device_no, 0, PCI_ILR, &tmp);
                            sio_device.int_no = (UB)(tmp & 0xff);
                            pci_read_config(0, device_no, 0, PCI_IOBASE, &tmp);
                            sio_device.io_addr = tmp;
                            pci_read_config(0, device_no, 0, PCI_MEMBASE, &tmp);
                            sio_device.mem_addr = tmp;
                            break;

                        case PCI_DEVICE_ID_AL_M5237:
                            usb_device.vendor_id = vendor_id;
                            usb_device.device_id = device_id;
                            usb_device.bus_no = 0;
                            usb_device.slot_no = device_no;
                            pci_read_config(0, device_no, 0, PCI_ILR, &tmp);
                            usb_device.int_no = (UB)(tmp & 0xff);
                            pci_read_config(0, device_no, 0, PCI_IOBASE, &tmp);
                            usb_device.io_addr = tmp;
                            pci_read_config(0, device_no, 0, PCI_MEMBASE, &tmp);
                            usb_device.mem_addr = tmp;
                            break;

                        case PCI_DEVICE_ID_AL_M7101:
                            pmu_device.vendor_id = vendor_id;
                            pmu_device.device_id = device_id;
                            pmu_device.bus_no = 0;
                            pmu_device.slot_no = device_no;
                            pci_read_config(0, device_no, 0, PCI_ILR, &tmp);
                            pmu_device.int_no = (UB)(tmp & 0xff);
                            pci_read_config(0, device_no, 0, PCI_IOBASE, &tmp);
                            pmu_device.io_addr = tmp;
                            pci_read_config(0, device_no, 0, PCI_MEMBASE, &tmp);
                            pmu_device.mem_addr = tmp;
                            break;

                        default:
                            break;
                    }
                    break;

                default:                    /* other device on PCI slot */
                    external_card[add_on].vendor_id = vendor_id;
                    external_card[add_on].device_id = device_id;
                    external_card[add_on].bus_no = 0;
                    external_card[add_on].slot_no = device_no;
                    pci_read_config(0, device_no, 0, PCI_ILR, &tmp);
                    external_card[add_on].int_no = (UB)(tmp & 0xff);
                    pci_read_config(0, device_no, 0, PCI_IOBASE, &tmp);
                    external_card[add_on].io_addr = tmp;
                    pci_read_config(0, device_no, 0, PCI_MEMBASE, &tmp);
                    external_card[add_on].mem_addr = tmp;

                    if (add_on < PCI_MAX_SLOT) {
                        add_on++;       /* increment add_on for next device */
                    }
                    break;
            }
        }
    }
}
/******************************************************************************
* Function to initialize LAN controller                                       *
*                                                                             *
******************************************************************************/
void init_lan_device(void)
{
UW tmp;
    /* set memory base address for */
    pci_read_config(lan_device.bus_no, lan_device.slot_no, 0, PCI_MEMBASE, &tmp);
    tmp = LANC_MEM_BASE;
    pci_write_config(lan_device.bus_no, lan_device.slot_no, 0, PCI_MEMBASE, tmp);
    tmp = LANC_IO_BASE;
    pci_write_config(lan_device.bus_no, lan_device.slot_no, 0, PCI_IOBASE, tmp);

    pci_read_config(lan_device.bus_no, lan_device.slot_no, 0, 0x30, &tmp);
    pci_read_config(lan_device.bus_no, lan_device.slot_no, 0, 0x3C, &tmp);

    /* set the PCI command register */
    pci_read_config(lan_device.bus_no, lan_device.slot_no, 0, PCI_CMD, &tmp);
    tmp = tmp | 0x07;
            /* set PCI command register for IO and MEM space access. */
            /* set PCI bus master mode */
    pci_write_config(lan_device.bus_no, lan_device.slot_no, 0, PCI_CMD, tmp);
    pci_read_config(lan_device.bus_no, lan_device.slot_no, 0, 0x48, &tmp);
    pci_read_config(lan_device.bus_no, lan_device.slot_no, 0, 0x4C, &tmp);

}

/******************************************************************************
* Function to initialize PMU controller                                       *
*                                                                             *
******************************************************************************/
void init_pmu_device(void)
{
UW tmp;
    /* set memory base address for */
    pci_read_config(pmu_device.bus_no, pmu_device.slot_no, 0, PCI_MEMBASE, &tmp);
    tmp = PMUC_MEM_BASE;
    pci_write_config(pmu_device.bus_no, pmu_device.slot_no, 0, PCI_MEMBASE, tmp);
    tmp = PMUC_IO_BASE;
    pci_write_config(pmu_device.bus_no, pmu_device.slot_no, 0, PCI_IOBASE, tmp);

    pci_read_config(pmu_device.bus_no, pmu_device.slot_no, 0, 0x30, &tmp);
    pci_read_config(pmu_device.bus_no, pmu_device.slot_no, 0, 0x3C, &tmp);

    /* set the PCI command register */
    pci_read_config(pmu_device.bus_no, pmu_device.slot_no, 0, PCI_CMD, &tmp);
    tmp = tmp | 0x07;
            /* set PCI command register for IO and MEM space access. */
            /* set PCI bus master mode */
    pci_write_config(pmu_device.bus_no, pmu_device.slot_no, 0, PCI_CMD, tmp);

}

/******************************************************************************
* Function to initialize USB controller                                       *
*                                                                             *
******************************************************************************/
void init_usb_device(void)
{
UW tmp;
    /* set memory base address for */
    pci_read_config(usb_device.bus_no, usb_device.slot_no, 0, PCI_MEMBASE, &tmp);
    tmp = USBC_MEM_BASE;
    pci_write_config(usb_device.bus_no, usb_device.slot_no, 0, PCI_MEMBASE, tmp);
    tmp = USBC_IO_BASE;
    pci_write_config(usb_device.bus_no, usb_device.slot_no, 0, PCI_IOBASE, tmp);

    pci_read_config(usb_device.bus_no, usb_device.slot_no, 0, 0x30, &tmp);
    pci_read_config(usb_device.bus_no, usb_device.slot_no, 0, 0x3C, &tmp);

    /* set the PCI command register */
    pci_read_config(usb_device.bus_no, usb_device.slot_no, 0, PCI_CMD, &tmp);
    tmp = tmp | 0x07;
            /* set PCI command register for IO and MEM space access. */
            /* set PCI bus master mode */
    pci_write_config(usb_device.bus_no, usb_device.slot_no, 0, PCI_CMD, tmp);

}

/******************************************************************************
* Function to initialize SIO controller                                       *
*                                                                             *
******************************************************************************/
void init_sio_device(void)
{
UW tmp;
    /* set memory base address for */
    pci_read_config(sio_device.bus_no, sio_device.slot_no, 0, PCI_MEMBASE, &tmp);
    tmp = SIOC_MEM_BASE;
    pci_write_config(sio_device.bus_no, sio_device.slot_no, 0, PCI_MEMBASE, tmp);
    tmp = SIOC_IO_BASE;
    pci_write_config(sio_device.bus_no, sio_device.slot_no, 0, PCI_IOBASE, tmp);

    pci_read_config(sio_device.bus_no, sio_device.slot_no, 0, 0x30, &tmp);
    pci_read_config(sio_device.bus_no, sio_device.slot_no, 0, 0x3C, &tmp);

    /* set the PCI command register */
    pci_read_config(sio_device.bus_no, sio_device.slot_no, 0, PCI_CMD, &tmp);
    tmp = tmp | 0x0f;
            /* set PCI command register for IO and MEM space access. */
            /* set PCI bus master mode */
    pci_write_config(sio_device.bus_no, sio_device.slot_no, 0, PCI_CMD, tmp);

}
/*****************************************************************************/
/* Function to update the PCI data as per system byte order.                 */
/*                                                                           */
/* Input  -                                                                  */
/*    src_data  : source data.                                               */
/*****************************************************************************/
UW htopci(UW src_data)
{
    UW data = 0;

    #if defined (BIG_ENDIAN)
        UW tmp;
        for (tmp = 0; tmp <4; tmp++) {
            data = (data << 8);
            data |= (src_data & 0xff);
            src_data = (src_data >> 8);
        }
    #else
        data = src_data;
    #endif

    return data;
}
/*****************************************************************************/
/* Function to convert CPU system address to PCI address.                    */
/*                                                                           */
/* Input  -                                                                  */
/*    cpu_addr  : CPU system memory address.                                 */
/* Output -                                                                  */
/*    pci_addr  : corresponding address in PCI memory space.                 */
/*****************************************************************************/
UW addr_cpu_to_pci(UW cpu_addr)
{
    UW pci_addr;
    if ((cpu_addr & 0x1f000000) == (SH4_MEM0_BASE & 0x1f000000)) {
        pci_addr = ((cpu_addr & 0x00ffffff) | PCI_MEM0_BASE);
    }
    else if ((cpu_addr & 0x1f000000) == (SH4_MEM1_BASE & 0x1f000000)) {
        pci_addr = ((cpu_addr & 0x00ffffff) | PCI_MEM1_BASE);
    }
    else pci_addr = -10;

    return pci_addr;
}
/*****************************************************************************/
/* Function to convert PCI address to SH4 system address.                    */
/*                                                                           */
/* Input  -                                                                  */
/*    pci_addr  : PCI memory address.                                        */
/* Output -                                                                  */
/*    cpu_addr  : corresponding address in System memory space.              */
/*****************************************************************************/
UW addr_pci_to_cpu(UW pci_addr)
{
    UW cpu_addr;
    if ((pci_addr & 0xff000000) == PCI_MEM0_BASE) {
        cpu_addr = ((pci_addr & 0x00ffffff) | SH4_MEM0_BASE);
    }
    else if ((pci_addr & 0xff000000) == PCI_MEM1_BASE) {
        cpu_addr = ((pci_addr & 0x00ffffff) | SH4_MEM1_BASE);
    }
    else cpu_addr = -10;

    return cpu_addr;
}
/*****************************************************************************
* Memory pool creation for IO devices access in SRAM. (no data-cache area)
*
******************************************************************************/
ER init_ncram(void)
{
    ER ercd;

    mplid_ncram = 0;

    ercd = acre_mpl(&cmpl_ncram);
    if (ercd <= 0) {
        return ercd;    /* Return Error type if Memory pool creation problem */
    }
    mplid_ncram = ercd;
    return E_OK;

}


/************************* End of file pci7751.C *********************************/
