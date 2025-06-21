#----------------------------------------------------------------------*
# File name	:                                                          *
#----------------------------------------------------------------------*
#$Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/RCS/validate.mak 1.1 1970/01/01 00:00:00 masahiro Exp $
#$Log: validate.mak $
# リビジョン 1.1  1970/01/01  00:00:00  masahiro
# 初期リビジョン
# 
#----------------------------------------------------------------------*
#		(C) Copyright 2002, ISHIDA CO., LTD.                           *
#		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                       *
#		PHONE (077) 553-4141                                           *
#----------------------------------------------------------------------*/
#
# +++=>>> * %n  Edition: %v  Last changed: %f  By: %w */
# MAKEFILE  Edition: 0  Last changed: 17-aug-98, 3:43:56  By: BILLK */
# +++=>>> '%l' */
# Currently extracted for edit by: '***_NOBODY_***' */
#
#   Copyright (C) 1998 By
#   United States Software Corporation
#   7175 NW Evergreen Parkway
#   Hillsboro, OR 97124
#   503-844-6614  www.ussw.com
#
#   VALIDATE.MAK  Validation of config.mak makefile
#
#   This software is furnished under a license and may be used
#   and copied only in accordance with the terms of such license
#   and with the inclusion of the above copyright notice. This
#   software or any other copies thereof may not be provided or
#   otherwise made available to any other person.  No title to
#   and ownership of the software is hereby transferred.
#
#   The information in this software is subject to change
#   without notice and should not be construed as a commitment
#   by United States Software Corporation.
#
# # # # # # # # # # # # # # # # # # # # # # # # # #
#
#  MODIFICATION HISTORY:
#
#  20-OCT-1998  BSK   Chg: Modified for simplicity
#
# # # # # # # # # # # # # # # # # # # # # # # # # #
#

#
#  Compare options selected in config.mak to those supported.
#

%if "$(VALIDATED)" != "1"
%echo -n Verifying cpu/compiler . . . 

#
#  Check if CPU has been defined
#
%ifndef CPU
%abort 0 CPU not selected in config.mak!
%endif

#
#  Check if COMPILER has been defined
#
%ifndef COMPILER
%abort 0 COMPILER not selected in config.mak!
%endif

%if !(-d $(USROOTDIR))
%abort 0 $(USROOTDIR,LC) does not exist!
%endif

%if !(-d $(USROOTDIR)\config\$(CPU))
%abort 0 $(CPU) is not a valid CPU in config.mak!
%endif

%if !(-d $(USROOTDIR)\config\$(CPU)\$(COMPILER))
%abort 0 $(COMPILER) is not a valid COMPILER in config.mak!
%endif

%if -e $(BLDCFG)
%echo $(CPU)/$(COMPILER) is acceptable.
%setenv VALIDATED=1
%else
%abort 0 $(BLDCFG,LC) does not exist!  Consult USSW support.
%endif

%endif
