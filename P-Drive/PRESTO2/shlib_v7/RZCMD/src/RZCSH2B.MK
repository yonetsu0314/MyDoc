#*======================================================================
#* File name    : rzcsh2b.mk
#* Original	: 
#*----------------------------------------------------------------------
#* Function 	:
#*	RZコマンド&テキストに関するライブラリーを作成する。		
#*	SH2 ビッグエンディアン用
#*----------------------------------------------------------------------
#*$Header: p:/presto2/shlib_v7/rzcmd/src/RCS/rzcsh2b.mk 1.3 1970/01/01 00:00:00 chimura Exp $
#*$Log: rzcsh2b.mk $
#* リビジョン 1.3  1970/01/01  00:00:00  chimura
#* 2003/08/05 compiler ver7 vup
#* 
#* リビジョン 1.2  1970/01/01  00:00:00  kawamura
#* 2003/05/30  14:51
#* *.lctファイルのコピー条件を修正
#* 
#* リビジョン 1.1  1970/01/01  00:00:00  chimura
#* 初期リビジョン
#* 
#*----------------------------------------------------------------------
#*Header: p:/presto/shlib/rzcmd/src/RCS/rzcmd.mk 1.2 1970/01/01 00:00:00
#*Log: rzcmd.mk
#*----------------------------------------------------------------------
#*		(c) Copyright 2002, ISHIDA CO., LTD.
#*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
#*		(077) 553-4141
#*======================================================================
#
#	<<< デバッグ情報の定義 >>>
#
DEBUG 	= 0
#DEBUG	= 1
#
#	<<< エンディアンの定義 >>>
#
#ENDIAN	= 0
ENDIAN	= 1
#
#	<<< CPUの定義 >>>
#
CPUTYPE = 2
#CPUTYPE = 3
#
ifeq ($(ENDIAN),1)
ETYPE	= big
else
ETYPE	= little
endif

ifeq ($(CPUTYPE),3)
CTYPE	= sh3
ifeq ($(ENDIAN),1)
TARGET		=rzcsh3b
else
TARGET		=rzcsh3l
endif
else
CTYPE	= sh2
TARGET		=rzcsh2b
endif
#
#	<<< ファイルネーム定義 >>>
#
ID		=rzcmdid
#
RZI003		=rzi003
RZI004		=rzi004
RZI005		=rzi005
RZI006		=rzi006
RZI007		=rzi007
RZI008		=rzi008
RZI009		=rzi009
RZI010		=rzi010
RZI011		=rzi011
RZI012		=rzi012
RZI013		=rzi013
RZI014		=rzi014
RZI015		=rzi015
RZI016		=rzi016
RZI017		=rzi017
RZI018		=rzi018
RZI019		=rzi019
RZI020		=rzi020
RZI021		=rzi021
RZI022		=rzi022
RZI023		=rzi023
RZI024		=rzi024
RZI025		=rzi025
RZI026		=rzi026
RZI027		=rzi027
RZI028		=rzi028
RZI029		=rzi029
RZI030		=rzi030
RZI031		=rzi031
RZI032		=rzi032
RZI035		=rzi035
RZI036		=rzi036
RZR003		=rzr003
RZR004		=rzr004
RZR005		=rzr005
RZR006		=rzr006
RZR007		=rzr007
RZR008		=rzr008
RZR009		=rzr009
RZR010		=rzr010
RZR011		=rzr011
RZR012		=rzr012
RZR013		=rzr013
RZR014		=rzr014
RZR015		=rzr015
RZR016		=rzr016
RZR017		=rzr017
RZR018		=rzr018
RZR019		=rzr019
RZR020		=rzr020
RZR021		=rzr021
RZR022		=rzr022
RZR023		=rzr023
RZR024		=rzr024
RZR025		=rzr025
RZR026		=rzr026
RZR027		=rzr027
RZR028		=rzr028
RZR033		=rzr033
RZR034		=rzr034
RZR035		=rzr035
RZR036		=rzr036
RZR037		=rzr037
RZR038		=rzr038
RZR039		=rzr039
RZR040		=rzr040
RZR041		=rzr041
RZR042		=rzr042
RZR043		=rzr043
RZR044		=rzr044
RZR045		=rzr045
RZR046		=rzr046
RZR047		=rzr047
RZR048		=rzr048
RZR049		=rzr049
RZR050		=rzr050
RZR051		=rzr051
RZR052		=rzr052
RZR053		=rzr053
RZR054		=rzr054
RZR055		=rzr055
RZR056		=rzr056
RZR057		=rzr057
RZR058		=rzr058
RZR059		=rzr059
RZR060		=rzr060
RZR071		=rzr071
RZR072		=rzr072
RZI009A		=rzi009a
RZI010A		=rzi010a
RZI013A		=rzi013a
RZI014A		=rzi014a
RZI017A		=rzi017a
RZI018A		=rzi018a
RZR025A		=rzr025a
RZR026A		=rzr026a

#===========================================================
#
ifeq ($(DEBUG),1)
CCOPT	= -cpu=$(CTYPE) -op=0 -en=$(ETYPE) -me -debug -comment=nest -l -def=DEBUG
else
CCOPT	= -cpu=$(CTYPE) -op=1 -en=$(ETYPE) -me -nodebug -comment=nest -l -def=NDEBUG
endif

INCLIST := $(INC_BASE)
INCLIST := $(INCLIST),$(SHC_INC)
INCLIST := $(INCLIST),$(NORTI_BASE)\inc

CFLAGS	:= -i=$(INCLIST)

CC	:=shc $(CCOPT) $(CFLAGS)
OPTLNK	=optlnk

#===========================================================
#
#	<<< ライブラリーの作成 >>>
#
OFILE0  	=$(ID).obj
OFILE1		=$(RZI003).obj $(RZI004).obj $(RZI005).obj $(RZI006).obj
OFILE2		=$(RZI007).obj $(RZI008).obj
OFILE3		=$(RZI011).obj $(RZI012).obj
OFILE4		=$(RZI015).obj $(RZI016).obj
OFILE5		=$(RZI019).obj $(RZI020).obj $(RZI021).obj $(RZI022).obj
OFILE6		=$(RZI023).obj $(RZI024).obj $(RZI025).obj $(RZI026).obj
OFILE7		=$(RZI027).obj $(RZI028).obj $(RZI029).obj $(RZI030).obj
OFILE8		=$(RZI031).obj $(RZI032).obj
OFILE9		=$(RZI035).obj $(RZI036).obj
#OFILE10	=$(RZI009).obj  $(RZI010).obj  $(RZI013).obj  $(RZI014).obj  $(RZI017).obj  $(RZI018).obj
OFILE10		=$(RZI009A).obj $(RZI010A).obj $(RZI013A).obj $(RZI014A).obj $(RZI017A).obj $(RZI018A).obj
OFILE11		=$(RZR003).obj $(RZR004).obj $(RZR005).obj $(RZR006).obj
OFILE12		=$(RZR007).obj $(RZR008).obj $(RZR009).obj $(RZR010).obj
OFILE13		=$(RZR011).obj $(RZR012).obj $(RZR013).obj $(RZR014).obj
OFILE14		=$(RZR015).obj $(RZR016).obj $(RZR017).obj $(RZR018).obj
OFILE15		=$(RZR019).obj $(RZR020).obj $(RZR021).obj $(RZR022).obj
OFILE16		=$(RZR023).obj $(RZR024).obj
OFILE17		=$(RZR027).obj $(RZR028).obj $(RZR033).obj $(RZR034).obj
OFILE18		=$(RZR035).obj $(RZR036).obj $(RZR037).obj $(RZR038).obj
OFILE19		=$(RZR039).obj $(RZR040).obj $(RZR041).obj $(RZR042).obj
OFILE20		=$(RZR043).obj $(RZR044).obj $(RZR045).obj $(RZR046).obj
OFILE21		=$(RZR047).obj $(RZR048).obj $(RZR049).obj $(RZR050).obj
OFILE22		=$(RZR051).obj $(RZR052).obj $(RZR053).obj $(RZR054).obj
OFILE23		=$(RZR055).obj $(RZR056).obj $(RZR057).obj $(RZR058).obj
OFILE24		=$(RZR059).obj $(RZR060).obj $(RZR071).obj $(RZR072).obj
#OFILE25	=$(RZR025).obj  $(RZR026).obj
OFILE25		=$(RZR025A).obj $(RZR026A).obj

OFILES		=$(OFILE0) $(OFILE1) $(OFILE2) $(OFILE3) $(OFILE4) $(OFILE5) $(OFILE6)\
		 $(OFILE7) $(OFILE8) $(OFILE9) $(OFILE10)\
		 $(OFILE11) $(OFILE12) $(OFILE13) $(OFILE14) $(OFILE15) $(OFILE16) $(OFILE17)\
		 $(OFILE18) $(OFILE19) $(OFILE20) $(OFILE21) $(OFILE22) $(OFILE23)\
		 $(OFILE24) $(OFILE25)
#
#
#
all : clean $(TARGET).lib
#
#	<<< オブジェクト･ファイルの生成 >>>
#
ERRFILE =err.txt

$(ID).obj : $(ID).c
	$(CC) $(ID).c				>  $(ERRFILE)
$(RZI003).obj   : $(RZI003).c
	$(CC) $(RZI003).c			>>  $(ERRFILE)
$(RZI004).obj   : $(RZI004).c
	$(CC) $(RZI004).c			>>  $(ERRFILE)
$(RZI005).obj   : $(RZI005).c
	$(CC) $(RZI005).c			>>  $(ERRFILE)
$(RZI006).obj   : $(RZI006).c
	$(CC) $(RZI006).c			>>  $(ERRFILE)
$(RZI007).obj   : $(RZI007).c
	$(CC) $(RZI007).c			>>  $(ERRFILE)
$(RZI008).obj   : $(RZI008).c
	$(CC) $(RZI008).c			>>  $(ERRFILE)
$(RZI009).obj   : $(RZI009).c
	$(CC) $(RZI009).c			>>  $(ERRFILE)
$(RZI010).obj   : $(RZI010).c
	$(CC) $(RZI010).c			>>  $(ERRFILE)
$(RZI011).obj   : $(RZI011).c
	$(CC) $(RZI011).c			>>  $(ERRFILE)
$(RZI012).obj   : $(RZI012).c
	$(CC) $(RZI012).c			>>  $(ERRFILE)
$(RZI013).obj   : $(RZI013).c
	$(CC) $(RZI013).c			>>  $(ERRFILE)
$(RZI014).obj   : $(RZI014).c
	$(CC) $(RZI014).c			>>  $(ERRFILE)
$(RZI015).obj   : $(RZI015).c
	$(CC) $(RZI015).c			>>  $(ERRFILE)
$(RZI016).obj   : $(RZI016).c
	$(CC) $(RZI016).c			>>  $(ERRFILE)
$(RZI017).obj   : $(RZI017).c
	$(CC) $(RZI017).c			>>  $(ERRFILE)
$(RZI018).obj   : $(RZI018).c
	$(CC) $(RZI018).c			>>  $(ERRFILE)
$(RZI019).obj   : $(RZI019).c
	$(CC) $(RZI019).c			>>  $(ERRFILE)
$(RZI020).obj   : $(RZI020).c
	$(CC) $(RZI020).c			>>  $(ERRFILE)
$(RZI021).obj   : $(RZI021).c
	$(CC) $(RZI021).c			>>  $(ERRFILE)
$(RZI022).obj   : $(RZI022).c
	$(CC) $(RZI022).c			>>  $(ERRFILE)
$(RZI023).obj   : $(RZI023).c
	$(CC) $(RZI023).c			>>  $(ERRFILE)
$(RZI024).obj   : $(RZI024).c
	$(CC) $(RZI024).c			>>  $(ERRFILE)
$(RZI025).obj   : $(RZI025).c
	$(CC) $(RZI025).c			>>  $(ERRFILE)
$(RZI026).obj   : $(RZI026).c
	$(CC) $(RZI026).c			>>  $(ERRFILE)
$(RZI027).obj   : $(RZI027).c
	$(CC) $(RZI027).c			>>  $(ERRFILE)
$(RZI028).obj   : $(RZI028).c
	$(CC) $(RZI028).c			>>  $(ERRFILE)
$(RZI029).obj   : $(RZI029).c
	$(CC) $(RZI029).c			>>  $(ERRFILE)
$(RZI030).obj   : $(RZI030).c
	$(CC) $(RZI030).c			>>  $(ERRFILE)
$(RZI031).obj   : $(RZI031).c
	$(CC) $(RZI031).c			>>  $(ERRFILE)
$(RZI032).obj   : $(RZI032).c
	$(CC) $(RZI032).c			>>  $(ERRFILE)
$(RZI035).obj   : $(RZI035).c
	$(CC) $(RZI035).c			>>  $(ERRFILE)
$(RZI036).obj   : $(RZI036).c
	$(CC) $(RZI036).c			>>  $(ERRFILE)
$(RZR003).obj   : $(RZR003).c
	$(CC) $(RZR003).c			>>  $(ERRFILE)
$(RZR004).obj   : $(RZR004).c
	$(CC) $(RZR004).c			>>  $(ERRFILE)
$(RZR005).obj   : $(RZR005).c
	$(CC) $(RZR005).c			>>  $(ERRFILE)
$(RZR006).obj   : $(RZR006).c
	$(CC) $(RZR006).c			>>  $(ERRFILE)
$(RZR007).obj   : $(RZR007).c
	$(CC) $(RZR007).c			>>  $(ERRFILE)
$(RZR008).obj   : $(RZR008).c
	$(CC) $(RZR008).c			>>  $(ERRFILE)
$(RZR009).obj   : $(RZR009).c
	$(CC) $(RZR009).c			>>  $(ERRFILE)
$(RZR010).obj   : $(RZR010).c
	$(CC) $(RZR010).c			>>  $(ERRFILE)
$(RZR011).obj   : $(RZR011).c
	$(CC) $(RZR011).c			>>  $(ERRFILE)
$(RZR012).obj   : $(RZR012).c
	$(CC) $(RZR012).c			>>  $(ERRFILE)
$(RZR013).obj   : $(RZR013).c
	$(CC) $(RZR013).c			>>  $(ERRFILE)
$(RZR014).obj   : $(RZR014).c
	$(CC) $(RZR014).c			>>  $(ERRFILE)
$(RZR015).obj   : $(RZR015).c
	$(CC) $(RZR015).c			>>  $(ERRFILE)
$(RZR016).obj   : $(RZR016).c
	$(CC) $(RZR016).c			>>  $(ERRFILE)
$(RZR017).obj   : $(RZR017).c
	$(CC) $(RZR017).c			>>  $(ERRFILE)
$(RZR018).obj   : $(RZR018).c
	$(CC) $(RZR018).c			>>  $(ERRFILE)
$(RZR019).obj   : $(RZR019).c
	$(CC) $(RZR019).c			>>  $(ERRFILE)
$(RZR020).obj   : $(RZR020).c
	$(CC) $(RZR020).c			>>  $(ERRFILE)
$(RZR021).obj   : $(RZR021).c
	$(CC) $(RZR021).c			>>  $(ERRFILE)
$(RZR022).obj   : $(RZR022).c
	$(CC) $(RZR022).c			>>  $(ERRFILE)
$(RZR023).obj   : $(RZR023).c
	$(CC) $(RZR023).c			>>  $(ERRFILE)
$(RZR024).obj   : $(RZR024).c
	$(CC) $(RZR024).c			>>  $(ERRFILE)
$(RZR025).obj   : $(RZR025).c
	$(CC) $(RZR025).c			>>  $(ERRFILE)
$(RZR026).obj   : $(RZR026).c
	$(CC) $(RZR026).c			>>  $(ERRFILE)
$(RZR027).obj   : $(RZR027).c
	$(CC) $(RZR027).c			>>  $(ERRFILE)
$(RZR028).obj   : $(RZR028).c
	$(CC) $(RZR028).c			>>  $(ERRFILE)
$(RZR033).obj   : $(RZR033).c
	$(CC) $(RZR033).c			>>  $(ERRFILE)
$(RZR034).obj   : $(RZR034).c
	$(CC) $(RZR034).c			>>  $(ERRFILE)
$(RZR035).obj   : $(RZR035).c
	$(CC) $(RZR035).c			>>  $(ERRFILE)
$(RZR036).obj   : $(RZR036).c
	$(CC) $(RZR036).c			>>  $(ERRFILE)
$(RZR037).obj   : $(RZR037).c
	$(CC) $(RZR037).c			>>  $(ERRFILE)
$(RZR038).obj   : $(RZR038).c
	$(CC) $(RZR038).c			>>  $(ERRFILE)
$(RZR039).obj   : $(RZR039).c
	$(CC) $(RZR039).c			>>  $(ERRFILE)
$(RZR040).obj   : $(RZR040).c
	$(CC) $(RZR040).c			>>  $(ERRFILE)
$(RZR041).obj   : $(RZR041).c
	$(CC) $(RZR041).c			>>  $(ERRFILE)
$(RZR042).obj   : $(RZR042).c
	$(CC) $(RZR042).c			>>  $(ERRFILE)
$(RZR043).obj   : $(RZR043).c
	$(CC) $(RZR043).c			>>  $(ERRFILE)
$(RZR044).obj   : $(RZR044).c
	$(CC) $(RZR044).c			>>  $(ERRFILE)
$(RZR045).obj   : $(RZR045).c
	$(CC) $(RZR045).c			>>  $(ERRFILE)
$(RZR046).obj   : $(RZR046).c
	$(CC) $(RZR046).c			>>  $(ERRFILE)
$(RZR047).obj   : $(RZR047).c
	$(CC) $(RZR047).c			>>  $(ERRFILE)
$(RZR048).obj   : $(RZR048).c
	$(CC) $(RZR048).c			>>  $(ERRFILE)
$(RZR049).obj   : $(RZR049).c
	$(CC) $(RZR049).c			>>  $(ERRFILE)
$(RZR050).obj   : $(RZR050).c
	$(CC) $(RZR050).c			>>  $(ERRFILE)
$(RZR051).obj   : $(RZR051).c
	$(CC) $(RZR051).c			>>  $(ERRFILE)
$(RZR052).obj   : $(RZR052).c
	$(CC) $(RZR052).c			>>  $(ERRFILE)
$(RZR053).obj   : $(RZR053).c
	$(CC) $(RZR053).c			>>  $(ERRFILE)
$(RZR054).obj   : $(RZR054).c
	$(CC) $(RZR054).c			>>  $(ERRFILE)
$(RZR055).obj   : $(RZR055).c
	$(CC) $(RZR055).c			>>  $(ERRFILE)
$(RZR056).obj   : $(RZR056).c
	$(CC) $(RZR056).c			>>  $(ERRFILE)
$(RZR057).obj   : $(RZR057).c
	$(CC) $(RZR057).c			>>  $(ERRFILE)
$(RZR058).obj   : $(RZR058).c
	$(CC) $(RZR058).c			>>  $(ERRFILE)
$(RZR059).obj   : $(RZR059).c
	$(CC) $(RZR059).c			>>  $(ERRFILE)
$(RZR060).obj   : $(RZR060).c
	$(CC) $(RZR060).c			>>  $(ERRFILE)
$(RZR071).obj   : $(RZR071).c
	$(CC) $(RZR071).c			>>  $(ERRFILE)
$(RZR072).obj   : $(RZR072).c
	$(CC) $(RZR072).c			>>  $(ERRFILE)
$(RZI009A).obj   : $(RZI009A).c
	$(CC) $(RZI009A).c			>>  $(ERRFILE)
$(RZI010A).obj   : $(RZI010A).c
	$(CC) $(RZI010A).c			>>  $(ERRFILE)
$(RZI013A).obj   : $(RZI013A).c
	$(CC) $(RZI013A).c			>>  $(ERRFILE)
$(RZI014A).obj   : $(RZI014A).c
	$(CC) $(RZI014A).c			>>  $(ERRFILE)
$(RZI017A).obj   : $(RZI017A).c
	$(CC) $(RZI017A).c			>>  $(ERRFILE)
$(RZI018A).obj   : $(RZI018A).c
	$(CC) $(RZI018A).c			>>  $(ERRFILE)
$(RZR025A).obj   : $(RZR025A).c
	$(CC) $(RZR025A).c			>>  $(ERRFILE)
$(RZR026A).obj   : $(RZR026A).c
	$(CC) $(RZR026A).c			>>  $(ERRFILE)

$(TARGET).lib : $(OFILES) $(TARGET).sub
	if exist $(TARGET).lib del $(TARGET).lib
	$(OPTLNK) -SUBCOMMAND=$(TARGET).sub
	copy $(TARGET).lib ..\*.*

$(TARGET).sub :
	echo	FORM	= library		>$(TARGET).sub
	echo	OUTPUT	= $(TARGET).lib		>>$(TARGET).sub
	echo	INPUT	= $(OFILES)		>>$(TARGET).sub
	echo	LIST	= $(TARGET)		>>$(TARGET).sub
	echo	EXIT				>>$(TARGET).sub

#-----------------------------------------------------------------------#
#	ＭＡＫＥコマンドの定義						#
#-----------------------------------------------------------------------#
DRV 	=p:

checkin :
	perl $(DRV)\tool\perl\checkin.pl *.mk	>  $(TARGET).bat
	perl $(DRV)\tool\perl\checkin.pl *.h	>> $(TARGET).bat
	perl $(DRV)\tool\perl\checkin.pl *.c	>> $(TARGET).bat
	perl $(DRV)\tool\perl\checkin.pl *.asm	>> $(TARGET).bat

clean :
	if exist *.obj del *.obj
	if exist *.lbp del *.lbp
	if exist *.lst del *.lst
	if exist *.sub del *.sub
	if exist *.lib del *.lib

release :
	if exist *.mk	ident *.mk	>  release.id
	if exist *.h	ident *.h	>> release.id
	if exist *.c	ident *.c	>> release.id
	if exist *.asm	ident *.asm	>> release.id

