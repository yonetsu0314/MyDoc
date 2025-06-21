#----------------------------------------------------------------------*
# File name	: depends.mak                                              *
#----------------------------------------------------------------------*
#$Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/config/sh7615/hitachi/rcs/depends.mak 1.2 1970/01/01 00:00:00 sakaguti Exp $
#$Log: depends.mak $
# リビジョン 1.2  1970/01/01  00:00:00  sakaguti
# ver7 vup
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
# +++=>>> * %n  Edition: %v  Last changed: %f  By: %w */
# MAKEFILE  Edition: 0  Last changed: 17-aug-98, 3:43:56  By: BILLK */
# +++=>>> '%l' */
# Currently extracted for edit by: '***_NOBODY_***' */
#
#Create library:============================================================
$(CURLIB).$L : $(USLIBOBJ) $(MORE_DEPENDS)
    -del $(CURLIB).$(L)
    %set LIB_FIRST = YES
    %foreach OBJECT $(USLIBOBJ)
      %if "$(LIB_FIRST)" == "YES" 
        %echo form   = library         > $(CURLIB).sub
        %echo output = $(CURLIB).$(L) >> $(CURLIB).sub
        %echo input  = $(OBJECT)      >> $(CURLIB).sub
        %set LIB_FIRST = NO
      %else
        %echo input  = $(OBJECT)      >> $(CURLIB).sub
      %endif
    %end
    %echo list  = $(CURLIB) >> $(CURLIB).sub
    %echo exit              >> $(CURLIB).sub
    $(LIBR) -SUBCOMMAND=$(CURLIB).sub        
