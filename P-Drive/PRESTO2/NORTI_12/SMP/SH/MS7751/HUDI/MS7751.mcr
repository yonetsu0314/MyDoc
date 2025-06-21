//****************************************************************************//
//             Board Initialization Module for MS7751SE01 board.              //
//                                                                            //
//             18/Oct/2001                        [MiSPO - SP]                //
//****************************************************************************//
Outport(2, ffc00000, 0e13)          // FRQCR register.
                    // PLL1 Enabled
                    // PLL2 Enabled
                    // IFC = 000 CPU clk division ratio =1
                    // BFC = 010 Bus clk division ratio =1/3
                    // PFC = 011 Peripheral clk division ratio = 1/6.

Outport(3, ff800000, 00080008)      // BCR1 register.
                    // bit31    : ENDIAN    0   MD5 level monitor(Read only)
                    // bit30    : MASTER    0   MD7 level monitor(Read only)
                    // bit29    : A0MPX     0   MD6 level monitor(Read only)
                    // bit28-27 : Reserved  00
                    // bit26    : DPUP      0   Data pull-up ON
                    // bit25    : IPUP      0   Control input pull-up ON
                    // bit24    : OPUP      0   Control output pull-up ON
                    // bit23-22 : Reserved  00
                    // bit21    : A1MBC     0   CS1 area ordinary SRAM
                    // bit20    : A4MBC     0   CS4 area ordinary SRAM
                    // bit19    : BREQEN    1   BREQ enable
                    // bit18    : Reserved  0
                    // bit17    : MEMMPX    0   CS1 to CS6 normal interface
                    // bit16    : DMABST    0   DMAC burst transfer OFF
                    // bit15    : HIZMEM    0   Control line High-Z
                    // bit14    : HIZCNT    0   RAS,CAS High-Z
                    // bit13-11 : A0BST2-0  000 CS0 area ordinary memory
                    // bit10- 8 : A5BST2-0  000 CS5 area ordinary memory
                    // bit 7- 5 : A6BST2-0  000 CS6 area ordinary memory
                    // bit 4- 2 : DRAMTP2-0 010 CS2 area ordinary memory
                    //                          CS3 area SDRAM
                    // bit 1    : Reserved  0
                    // bit 0    : A56PCM    0   CS5,CS6 area normal memory

Outport(2, ff800004, 2FFC)      // BCR2 register.
                    // bit15-14 : A0SZ1-0   00  MD3,MD4 level monitor(Read only)
                    // bit13-12 : A6SZ1-0   10  CS6 area 16bit bus size
                    // bit11-10 : A5SZ1-0   11  CS5 area 32bit bus size
                    // bit 9- 8 : A4SZ1-0   11  CS4 area 32bit bus size
                    // bit 7- 6 : A3SZ1-0   11  CS3 area 32bit bus size
                    // bit 5- 4 : A2SZ1-0   11  CS2 area 32bit bus size
                    // bit 3- 2 : A1SZ1-0   11  CS1 area 32bit bus size
                    // bit 1    : Reserved  0
                    // bit 0    : PORTEN    0   Port not used

Outport(3, ff800008, 02770771)      // WCR1 register.
                    // bit31    : Reserved  0
                    // bit30-28 : DMAIW2-0  000 DMA cycle No idle cycles
                    // bit27    : Reserved  0
                    // bit26-24 : A6IW2-0   010 CS6 area  2 idle cycle inserted
                    // bit23    : Reserved  0
                    // bit22-20 : A5IW2-0   111 CS5 area 15 idle cycle inserted
                    // bit19    : Reserved  0
                    // bit18-16 : A4IW2-0   111 CS4 area 15 idle cycle inserted
                    // bit15    : Reserved  0
                    // bit14-12 : A3IW2-0   000 CS3 area No idle cycles
                    // bit11    : Reserved  0
                    // bit10- 8 : A2IW2-0   111 CS2 area 15 idle cycle inserted
                    // bit 7    : Reserved  0
                    // bit 6- 4 : A1IW2-0   111 CS1 area 15 idle cycle inserted
                    // bit 3    : Reserved  0
                    // bit 2- 0 : A0IW2-0   001 CS0 area  1 idle cycle inserted

Outport(3, ff80000c, 7ffe4fe7)      // WCR2 register.
                    // bit31-29 : A6W2-0    011 CS6 area  3 wait cycle inserted
                    // bit28-26 : A6B2-0    111 CS6 area burst pitch 7
                    // bit25-23 : A5W2-0    111 CS5 area 15 wait cycle inserted
                    // bit22-20 : A5B2-0    111 CS5 area burst pitch 7
                    // bit19-17 : A4W2-0    111 CS4 area 15 wait cycle inserted
                    // bit16    : Reserved  0
                    // bit15-13 : A3W2-0    010 CS3 area CAS latency 2-cycle
                    // bit12    : Reserved  0
                    // bit11- 9 : A2W2-0    111 CS2 area 15 wait cycle inserted
                    // bit 8- 6 : A1W2-0    111 CS1 area 15 wait cycle inserted
                    // bit 5- 3 : A0W2-0    100 CS0 area  6 wait cycle inserted
                    // bit 2- 0 : A0B2-0    111 CS0 area burst pitch 7

Outport(3, ff800010, 0)             // WCR3 register.
                    // No wait cycles for setup and Hold timings for all area.

Outport(2, ff800018, 0000)          // PCR register.
                    // bit15-14 : A5PCW1-0  00  CS5 area PCMCIA wait  : 0-cycle
                    // bit13-12 : A6PCW1-0  00  CS6 area PCMCIA wait  : 0-cycle
                    // bit11- 9 : A5TED2-0  000
                    //   CS5 area ADDRESS-OE/WE latency time : 0-cycle
                    // bit 8- 6 : A6TED2-0  000
                    //   CS6 area ADDRESS-OE/WE latency time : 0-cycle
                    // bit 5- 3 : A5TEH2-0  000
                    //   CS5 area OE/WE-ADDRESS hold time    : 0-cycle
                    // bit 2- 0 : A6TEH2-0  000
                    //   CS6 area OE/WE-ADDRESS hold time    : 0-cycle

Outport(2, ff800020, a500)          // RTCNT register.
                    // RTCNT clear data

Outport(2, ff800024, a561)          // RTCOR register.
                    // Refresh cycle setting.

Outport(3, ff800014, 10110194)      // MCR register.
                    // bit31    : RASD      0   通常ﾓｰﾄﾞ
                    // bit30    : MRSET     0   全ﾊﾞﾝｸﾌﾟﾘﾁｬｰｼﾞ
                    // bit29-27 : TRC2-0    010
                    //       Ref/Active to Ref/Active command period : 6-cycle(108nS)
                    // bit26-24 : Reserved  000
                    // bit23    : TCAS      0   SDRAM時は未使用
                    // bit22    : Reserved  0
                    // bit21-19 : TPC2-0    010
                    //       Precharge to active command period : 3-cycle(54nS)
                    // bit18    : Reserved  0
                    // bit17-16 : RCD1-0    01
                    //       Active command to column command(same bank) : 2-cycle(36nS)
                    // bit15-13 : TWRL2-0   000
                    //   Write recovery or data-in to precharge lead time : 1-cycle(18nS)
                    // bit12-10 : TRAS2-0   000
                    //   Ref/Active to Ref/Active command period : 10-cycle(180nS)
                    // bit 9    : BE        0   SDRAM時は未使用
                    // bit 8-7  : SZ1-0     11  Memory data size : 32bit
                    // bit 6    : AMXEXT    0   SDRAM interface
                    // bit 5- 3 : AMX2-0    010 (64M * 2)
                    // bit 2    : RFSH      1   Refresh enable
                    // bit 1    : RMODE     0   Refresh mode         : Auto refresh
                    // bit 0    : EDOMODE   0   SDRAM時は未使用

Outport(1, ff940088, aa)            // SDMR register.
Outport(2, ff80001c, a508)          // RTCSR register.
                    // bit15- 8 : Reserved  00000000 Write code h'a5
                    // bit 7    : CMF  0   Compare match flug                   : Clear
                    // bit 6    : CMIE 0   Compare match interrupt              : Disable
                    // bit 5- 3 : CKS2-0   001 Count clock                      : CKIO/4
                    // bit 2    : OVF  0   Refresh count over flow flag         : Clear
                    // bit 1    : OVIE 0   Refresh count over flow interrupt    : Disable
                    // bit 0    : LMTS 0   Refresh count over flow limit select : 1024

Outport(2, ff800028, a400)          // RFCR register.
                    // RFCR clear data

Wait(4)
Outport(3, ff800014, 50110194)      // MCR register.
                    // bit31    : RASD      0   通常ﾓｰﾄﾞ
                    // bit30    : MRSET     1   ﾓｰﾄﾞﾚｼﾞｽﾀ設定
                    // bit29-27 : TRC2-0    010
                    //       Ref/Active to Ref/Active command period : 6-cycle(108nS)
                    // bit26-24 : Reserved  000
                    // bit23    : TCAS      0   SDRAM時は未使用
                    // bit22    : Reserved  0
                    // bit21-19 : TPC2-0    010
                    //       Precharge to active command period : 3-cycle(54nS)
                    // bit18    : Reserved  0
                    // bit17-16 : RCD1-0    01
                    //       Active command to column command(same bank) : 2-cycle(36nS)
                    // bit15-13 : TWRL2-0   000
                    //   Write recovery or data-in to precharge lead time : 1-cycle(18nS)
                    // bit12-10 : TRAS2-0   000
                    //   Ref/Active to Ref/Active command period : 10-cycle(180nS)
                    // bit 9    : BE        0   SDRAM時は未使用
                    // bit 8-7  : SZ1-0     11  Memory data size : 32bit
                    // bit 6    : AMXEXT    0   SDRAM interface
                    // bit 5- 3 : AMX2-0    010 (64M * 2)
                    // bit 2    : RFSH      1   Refresh enable
                    // bit 1    : RMODE     0   Refresh mode         : Auto refresh
                    // bit 0    : EDOMODE   0   SDRAM時は未使用

Outport(1, ff940088, aa)            // SDMR register.
Outport(2, ff80001c, a518)          // RTCSR register.
                    // bit15- 8 : Reserved  00000000 Write code h'a5
                    // bit 7    : CMF  0   Compare match flug                   : Clear
                    // bit 6    : CMIE 0   Compare match interrupt              : Disable
                    // bit 5- 3 : CKS2-0   011 Count clock                      : CKIO/64
                    // bit 2    : OVF  0   Refresh count over flow flag         : Clear
                    // bit 1    : OVIE 0   Refresh count over flow interrupt    : Disable
                    // bit 0    : LMTS 0   Refresh count over flow limit select : 1024

Outport(3, ff80002c, 00000000)      // PCTRA register.
Outport(3, ff800040, 00000000)      // PCTRB register.
                    // PORT all pins configured as input with internal pull-up

Outport(2, ff800048, 0000)          // GPIOIC register.
                    // GPIO PORT all pins configured for I/O mode

//*************************************************************************************//



