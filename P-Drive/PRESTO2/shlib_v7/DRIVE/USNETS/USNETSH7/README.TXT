
FILE:  readme.txt

General information about USNET

Last Updated:  31-JAN-2000

INSTALLATION
============

USNET is delivered on two or three 3-1/2 inch diskettes
depending on the requested cpu and compiler support.  The first
diskette contains the source code common to all cpus, compilers,
and protocols.  The second diskette contains the drivers and
support code explicitly for x86 Real Mode.  A third diskette
will be deliver if the the specified target cpu is other than
x86 Real Mode.  The third diskette contains only support code
for the target cpu and compiler.

To install to DOS, insert diskette in drive x, then type

        x:
        install <destination directory>

This install overrides the install defined in the manual.
Repeat this process for each of the disks in the delivery.

For UNIX, see DOC\UNIXBLD.TXT.
For SH7000 Unix support, mount the disk on a directory, set it as the 
current directory, and type install.  Some machines may require root 
privileges for this. Under Solaris:

        volcheck
        cd /floppy/usn1
        csh install.csh <destination directory> <processor> <compiler> [rtos]

When disk 1 is complete, eject it and repeat the install with the
remaining disks.  Also note the floppy label name "usn1" will
vary from disk to disk.  The first disk will have a label "usn1",
the second disk containing the x86 real mode support will have
a label "usn2", and the third disk containing the target CPU
support will have a label of "usn<cpu>" where <cpu> is the
target CPU name.  For SH support the label will be "usnsh7000".
 

******* IMPORTANT ********

Once USNET is installed read all *.txt files especially those found in
the DOC directory.  Many of the files contain information about known
problems, building instructions for specific platforms, or corrections
to the manual.  Reading these files can save you weeks of debug
time.

TARGET SUPPORT
==============
USNET currently supports the following processors and compilers:

    Processor       Compilers
    =========       ========
    
    ARM             ARM 
    
    CF              DiabData

    I386            Microsoft, Watcom, Borland, MetaWare

    I8086           Borland, Microsoft, CAD-UL

    I960            Gnu, Intel IC960

    M68K            Aztec, Crosscode, DiabData, Microtec

    MIPS            EPI (MetaWare)

    MPPC            DiabData, MetaWare, Green Hills

    PPC             DiabData, MetaWare
    
    S16X            Keil

    SH7000          Gnu, Hitachi SHC

    V850            Greenhills

    Z180            IAR (Archimedes)

Only I8086 support and the target support for the specified
processor are included on the distribution disks.

DISTRIBUTION
============
USNET's root directory is specified by the user during
installation.  The install.bat file will create the following
subdirectories depending on the packages the user has purchased:

    Directory    Description
    =========    ===========
    NETSRC      Contains the main stack and netconf.c files
    DRVSRC      Contains the device drivers and subdirectories
                    for dependencies on target cpus
    INCLUDE     Contains the included header files and 
                    subdirectories for supported RTOSes
    CONFIG      Contains the subdirectories for cpu/compiler
                    specific makefiles
    APPSRC      Contains USNET example test programs
    LIB         The libraries are stored here after they are made
    DOC         Added documentation
    UNSUPP      Unsupported utilities and programs
    IAPSRC      Internet Access Package containing WEB server, SMTP, DNS
    SNMPSRC     Contains the SNMP package
    PPPSRC      Contains the PPP specific source including dialing support
    
The IAPSRC, SNMPSRC, and the PPPSRC directories are not included 
with the basic package of USNET.  These packages must be purchased
separately.

MAKE UTILITIES
==============
USNET is built with OPUS make utility.  US Software refers to the
make program as 'omake' in order to prevent conflicts with other
installed make utilities.   Consequently, the user may need to rename
OPUS make.exe to omake.exe or create a batch file with the name of
omake.bat to avoid conflicts with other make utilities.  The batch
file should then call OPUS make.  Keep in mind OPUS make may need
parameters passed to it.

Executing 'omake' at the command prompt will cause the USNET libraries
to be built.  To automatically build the test programs execute
'omake all' at the command prompt.  To build a specific target, specify
the target name, e.g. 'omake ltest' where ltest is the test program
to build.

The directory structure may be cleared of all libraries, executables,
and object code by executing 'omake clean' at the command prompt.

CONFIGURATION
=============
The following steps only pertain to DOS development environment.
For UNIX information see the section labeled MISCELLANEOUS near
the end of this document.

Once the software is installed, there are several steps
that must be taken in order to compile the USNET library
and its test programs on the development system:

    1.  In the install root directory edit the file config.mak to specify
        the parameters for the application.

        a.  Uncomment the line specifying the target cpu.

            If the target cpu is an x86, then the line

                #CPU = i8086  # Intel real mode, COMPILERS: borland, msoft

            should have the initial '#' removed so it reads

                CPU = i8086  # Intel real mode, COMPILERS: borland, msoft

            All other target CPUs should be commented out.

        b.  Uncomment the line specifying the development compiler.

            If the target compile is an Watcom, then the line

                #COMPILER = watcom

            should have the initial '#' removed so it reads

                COMPILER = watcom

            All other target COMPILERS should be commented out.

        c.  Edit the line specifying the RTOS USNET will
            be running under.  The default RTOS is set to
            none.  All RTOSes do not support all target cpus.
            For instance, HI-SH77 will only run on an SH3.  Keep
            this in mind when selecting an RTOS.

            IMPORTANT NOTE:  Because the RTOSes do not
            support all CPUs, the engineer may need to add
            makefile support inside USNET's release structure
            for the project at hand.  This means modifying
            the compiler.mak, see 2. below, and possibly
            other makefiles.  See file release.txt in the
            DOC directory for an explanation on the release
            structure.

            If the the target RTOS is US Software's MultiTask!,
            then the line reading

                RTOS = none

            should be changed to

                RTOS = MT

            where MT specifies which include\rtos sub-directory to pull
            support files from.  USNET supports the following RTOS:

                HI-SH7      Hitachi uITron; SH1 and SH2 only
                HI-SH77     Hitachi uITron; SH3 only
                MT          US Software's MultiTask!
                NONE        No RTOS
                PPSM        PPSM for DragonBall (M68K)
                RX850       NEC RX850 uITRON
                TT          US Software's TronTask! 2.0x (uITRON 2.01)
                TT3         US Software's TronTask! 3.0x (uITRON 3.0)
                TX          ThreadX from Express Logic

            Define where the RTOS include files are located by
            setting RTOS_INCLUDES.  If the include files are
            located in c:\rtos\includes, then:

                RTOS_INCLUDES = c:\rtos\includes

            Set the names of the RTOS libraries in RTOS_LIBS.  These
            libraries should be copied to the $(USROOTDIR)\lib directory.
            If the libraries are rtos.lib, kernel.lib, and console.lib, then

                RTOS_LIBS = rtos kernal console

            No extensions are necessary.

            Further information concerning integrating USNET with an
            RTOS may be found in file rtos_int.txt in the
            $(USROOTDIR)\doc directory.

        d.  Define the path to the default root
            directory where USNET is installed.  If USNET
            is installed in  C:\USSW, then the symbol USROOTDIR
            should be defined as:

                USROOTDIR = c:\ussw

        e.  Define which US Software products are installed.
            At the end of each product type enter the name of the
            libraries installed.

            Three entries are setup as examples: the IAP package, PPP
            package and the SNMP package.  To include these libraries
            during linking, uncomment the line defining them.  For
            example, if the IAP is installed change the following
            lines:

                #%set USIAP=usiap uscgi
                %set USIAP=

            to

                %set USIAP=usiap uscgi

            and this will link in the usiap.lib and uscgi.lib
            libraries.

        f.  Another macro is defined to allow the user to define
            other libraries.  At the line

                %set USER_LIBS =

            specify any other libraries that should be linked in.
            The specified libraries should be placed in
            $(USROOTDIR)\lib directory.

        g.  Currently, USNET does not support the BOARD option.

    2.  In the directory $(USROOTDIR)\CONFIG\$(CPU)\$(COMPILER), where

            $(USROOTDIR) = Path specified by user
            $(CPU)       = Target cpu
            $(COMPILER)  = Target compiler

        edit the compiler.mak file to specify the tool chain path.

        a.  Change the definition of where the compiler is install
            by changing the PTH symbol. If the development tools
            are located in D:\WATCOM, then PTH should be
            set to:

                PTH = d:\watcom

        b.  There may be other PTH like symbols depending on the compiler.
            Some represent DOS extenders, Assemblers, or other utilities.
            These will also have to be changed.

        c.  The debugging trace level is set by changing the symbol
            NTRACE to a new value.  The default value is 3.  Valid
            values range from 0 to 9 where 9 has the highest level
            of detail.

        d.  Other tool chain specifics are changed in this file as well.
            Such options include:

                Memory Model
                Compiler Flags
                Assembler Flags
                Link Flags
                Librarian
                Tool Chain Environment Variables

            See the compiler.mak file for more details.

    3.  File local.h located in the $(USROOTDIR)\include directory
        may need to be changed.   This file defines how USNET is
        configured for the application and contains some platform
        specific information.

        Applications on an embedded target processor will need
        a new version of local.h.

        a.  Check the $(USROOTDIR)\drvsrc\$(CPU) directory for
            a copy of local.h specific to that target.

        b.  Copy the processor specific local.h to the
            $(USROOTDIR)\include directory.

        The main differences between the orignal local.h and
        target specific local.h are the macro definitions do
        not call functions such as getenv() to find the host
        name.

        Also, some protocols can be disabled in this file by using
        "#undef <protocol>".  For example, PPP is not included with
        our base USNET so "#undef PPP" is already set in local.h.  If
        the PPPSRC directory is included, you will need to remove that
        line from local.h to enable PPP in the build process.

    4.  Other files may need to be configured to conform to the
        the needs of the application.  The following files
        are possible candidates:

            netconf.c       Network configuration table
            local.h         USNET configuration
            drvconf.h       *** NO LONGER USED ***

        See the manual for more details.

DRIVER SUPPORT
==============
Drivers are configured in INCLUDE\NET.H by user

        Driver                Comments
        ======                ========

        3C503                 3Com EtherLink II (NS 8390)
        3C509                 3Com EtherLink III (3C509, 3C595)
        3C9xx                 3Com 3c900 and 3c905 driver
        AMD961                AMD's AM79C961A PCnet-ISA II Ethernet
          amd961.h              defs. for AMD's Net186 Ethernet
        AM186ES               AMD's Net186 serial 
          am186msr.h            defs. for AMD's Net186 serial
        AM7990                Embedded AMD 7990 family chip set
          am7990.h              definitions for AMD 7990 based Ethernet
        CS8900                Crystal Ethernet driver
        DC21040               DEC 21040 Ethernet controller
        DC21140               DEC 21140 Fast Ethernet controller
        EN360                 Motorola 68360 Ethernet; NOTE: add 4 to MAXBUF
          m360.h                68360 hardware definitions
        EN302                 Motorola 68EN302 Ethernet
          men302.h              see notes.txt for 68000 change
        EXP16                 Intel EtherExpress 16 (old) (Intel 82586)
          i82586.h              definitions for Intel 82586 based Ethernet
        I8250                 PC serial port
        I82557                Intel EtherExpress PR/100 (Intel 82557, 82558, 82559)
        I82595                Intel EtherExpress PRO/10 (Intel I82595)
        I82596                Intel I82596 Ethernet controller
        MB86960               Fujitsu MB86960, MB86965 Ethernet
        NE1000                Novell NE1000 (8-bit) standard Ethernet (NS 8390)
        NE2000                Novell NE2000 (16bit) standard Ethernet (NS 8390)
        NE2100                Novell NE2100 standard Ethernet (AMD 7990)
        NS16550               PC serial port with FIFO
        NS8390                National Semiconductor 8390x (16-bit) Ethernet
          ns8390.h              definitions for NS 8390 based Ethernet
        RTELAN32              Midas RTE-LAN-BOARD-32 (DP 83902)
        SH7604                Hitachi SH7000 serial port
        SM83C790              SMC 83c790 10Mbit ethernet driver
        SMC91C92              SMC 91C92 (also 91C90, 91C94) Ethernet controller
          smc91c92.h
        WD8003                Western Digital Ethernet, SMC Ultra (NS 8390)
        PPSMUART              UART driver for DragonBall PPSM
        NDIS                  Interface to NDIS drivers for DOS
          ndisa.asm             assembly subroutines
          ndistsr.asm           loaded at bootload time
        ODI                   Interface to Novell ODI drivers for DOS
                                NOTE: add 56 to MAXBUF
          odia.asm              assembly subroutines
        wrap                  Wraparound for testing, used in LTEST
        swrap                 Serial wraparound for test, used in ltest

Adapters are also configured in INCLUDE\NET.H by the user under
x86 real mode support only:

        Adapter               Comments
        =======               ========

        PCMCIA1               PCMCIA using card services
        PCMCIA2               PCMCIA using socket services
          PCMCIA.H              definitions for PCMCIA
        PCI                   PCI bus, configure as driver
         PCIA.ASM               assembly subroutines

MISCELLANEOUS
============
SH7000 Support

    USNET support for the Hitachi SH series processors exists for
    UNIX and DOS.  SH1 and SH2 are supported on DOS with hooks for
    SH3 support.  SH1, SH2, and SH3 are supported on UNIX.
    When installing USNET on a UNIX machine, use the
    install.csh file.  USNET will be installed in a single directory
    as opposed to the the structure described above.  The makefiles
    are designed to work under Solaris make.  The paths, PTH, defined
    in the makefile will have to be changed in order to support
    the development environment.

    At the top of file local.h, the user will have to change the
    include statement for config.h.  See local.h for further details.

    Tool chains are defined in the file makefile.

File config.h

    File config.h is included to supply a place for the designer to
    include any global definitions needed by an application.

Release Structure

    For further information about USNET's release structure and
    makefile structure, see file release.txt in the DOC directory.

