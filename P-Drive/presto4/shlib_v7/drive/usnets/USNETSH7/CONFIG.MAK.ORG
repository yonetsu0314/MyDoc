#----------------------------------------------------------------------*
# File name	:                                                          *
#----------------------------------------------------------------------*
#$Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/rcs/config.mak 1.2 1970/01/01 00:00:00 sakaguti Exp $
#$Log: config.mak $
# リビジョン 1.2  1970/01/01  00:00:00  sakaguti
# ﾃﾞｨﾚｸﾄﾘ変更
# 
# リビジョン 1.1  1970/01/01  00:00:00  masahiro
# 初期リビジョン
# 
#----------------------------------------------------------------------*
#		(C) Copyright 2002, ISHIDA CO., LTD.                           *
#		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                       *
#		PHONE (077) 553-4141                                           *
#----------------------------------------------------------------------*/
#

CPU = sh7615  # Hitachi SH7615 , COMPILERS: hitachi


COMPILER = hitachi  # Hitachi SHC




#RTOS = none
#RTOS_INCLUDE =

RTOS = norti4
#RTOS_INCLUDE = c:\tst\usnets\usnetsh7\include\(RTOS)
RTOS_INCLUDE = p:\presto\shlib_v7\drive\usnets\usnetsh7\include\rtos\norti4

ZZZ_INCLUDE = p:\presto\shinc


#RTOS = hi77xx                         # Hitachi HI7750
#RTOS_INCLUDE = c:\hi7750\hihead       # Include files

#RTOS = hi77xx                         # Hitachi HI7700
#RTOS_INCLUDE = c:\hi7700\hihead       # Include files

#RTOS = hi77xx                         # Hitachi HI7000
#RTOS_INCLUDE = c:\hi7000\hihead       # Include files


# Define the root directory where USNET is installed
#
USROOTDIR = p:\presto\shlib_v7\drive\usnets\usnetsh7


# BOARD is the specialized board that is include is some makes
#
BOARD = unk


# INSTALLED PRODUCTS **************
#
#%set USNET=                     #  USNET libraries
%set USNET=usnet usdrv uscpu

%set USIAP=                      #  USIAP libraries
#%set USIAP=usiap uscgi

%set USSNMP=                     #  SNMP libraries
#%set USSNMP=snmp

%set USPPP=                      #  PPP libraries
#%set USPPP=usppp

%if -e $(USROOTDIR)\npcfg.mak     #  Check to see if NetPeer is installed
  %include $(USROOTDIR)\npcfg.mak
%endif

%set USFILES=                    #  USFiles libraries
%set GOFAST=                     #  GOFast libraries

# USER LIBRARIES TO LINK IN **********
#
#  Define any user libaries to be linked in.  These should
#  be placed in the $(USROOTDIR)\lib directory with the same
#  extentions as the usnet libraries.  In the below list do
#  not add the extension.  For example, add the libraries
#  important.lib, app.lib, scooby.lib:
#
#     %set USER_LIB = important app scooby
#
%set USER_LIBS =


# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#
# Do not edit below this line
#
#

#
# Define other macros used by the makefiles
#
DEL = del
CP  = copy

#
# Define configuration flag.  All makefiles check USCONF
#   to see if configuration parameters are defined.
#
USCONF = US_Software

#
# Where all net source include files are
#
INCPTH = $(USROOTDIR)\include

#
# Path to where the compiled librarys go and where to pick
#   up libraries for linking
#
LIBPTH = $(USROOTDIR)\lib

#
# Path where all tool chain support is
#
CFGPTH = $(USROOTDIR)\config

#
# Path to the source code for drivers
#
DRVPTH = $(USROOTDIR)\drvsrc

#
# Path to specified tool chain support is.
#
BLDPTH = $(USROOTDIR)\config\$(CPU)\$(COMPILER)

#
# Makefile containing the actuall tool chain definitions.
#
BLDCFG   = $(CFGPTH)\$(CPU)\$(COMPILER)\compiler.mak

#
# Makefile containing build information when an RTOS is being
#    used.
#
RTOSCFG  = $(CFGPTH)\$(RTOS).mak

#
# List of all possible include paths.  Two version exist because some
#   compilers like an ";" delimited list.
#
INCPTHS = . $(INCPTH) $(BLDPTH) $(DRVPTH)\$(CPU) $(INCPTH)\rtos\$(RTOS) $(DRVPTH)\$(CPU)\$(COMPILER) $(RTOS_INCLUDE) $(ZZZ_INCLUDE)
INCPTHS2 = $(INCPTH);$(DRVPTH)\$(CPU);$(CFGPTH)\$(CPU)\$(COMPILER);$(INCPTH)\rtos\$(RTOS);$(DRVPTH)\$(CPU)\$(COMPILER);$(RTOS_INCLUDE)
#INCPTHS2 = $(INCPTH);$(BLDPTH);$(DRVPTH)\$(CPU);$(INCPTH)\rtos\$(RTOS);$(RTOS_INCLUDE)

#
# Define the important dependencies.
#
DEPENDS = $(USROOTDIR)\config.mak $(USROOTDIR)\config.h

#
# Setup macros defining various libraries
#
NETLIB = $(USNET)
CFGLIB = $(CONFIG)
IAPLIB = $(USIAP)
SNMPLIB = $(USSNMP)
PPPLIB = $(USPPP)
NPLIB = $(USNP)
FILELIB = $(USFILES)
RTOSLIB = $(RTOS_LIBS)
GOFSTLIB = $(GOFAST)

#
# A list of all libaries to link in.  This list is used by the compiler.mak
#   when linking an application.  The order matters for some linkers, so
#   the libraries are listed in a general order of dependance from top to
#   bottom:
#       NetPeer, IAP and SNMP depend on USNET
#       USNET depends on PPP and an RTOS
#       The RTOS or the application may depend on USFILES
#       The application or other libraries may depend on GOFAST for F.P.
#       Any of the above may depend on other user libraries
#
# Also note that some of these library macros may be empty.
#
STDLIBS =
STDLIBS += $(NPLIB)
STDLIBS += $(IAPLIB)
STDLIBS += $(SNMPLIB)
STDLIBS += $(NETLIB)
STDLIBS += $(PPPLIB)
STDLIBS += $(RTOSLIB)
STDLIBS += $(FILELIB)
STDLIBS += $(GOFSTLIB)
STDLIBS += $(USER_LIBS)

#
# Used by I386
#
LIBRARIES_TO_LINK = $(STDLIBS)

#
# NT/OS2 compatability
#
%if $(OS) == OS2 || $(OS) == NT
; = &
%else
; = ;
%endif

#
# Include the compiler dependent makefile, compiler.mak, if valid.
#
%include $(USROOTDIR)\validate.mak
%include $(BLDCFG)

