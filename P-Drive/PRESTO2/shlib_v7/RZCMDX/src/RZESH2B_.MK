#*======================================================================
#* File name    : rzesh2b_.mk
#* Original	: 
#*----------------------------------------------------------------------
#* Function	:
#*	RZコマンド&テキストに関する追加機能ライブラリーを作成する。
#*	SH2 ビッグエンディアン用
#*----------------------------------------------------------------------
#*$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzesh2b_.mk 1.2 1970/01/01 00:00:00 chimura Exp $
#*$Log: rzesh2b_.mk $
#* リビジョン 1.2  1970/01/01  00:00:00  chimura
#* 2003/08/05 compiler ver7 vup
#* 
#* リビジョン 1.1  1970/01/01  00:00:00  chimura
#* 初期リビジョン
#* 
#*----------------------------------------------------------------------
#*Header: p:/presto/shlib/rzcmdx/src/RCS/rzex.mk 1.2 1970/01/01 00:00:00
#*Log: rzex.mk
#*----------------------------------------------------------------------
#*		(c) Copyright 2002, ISHIDA CO., LTD.
#*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
#*		(077) 553-4141
#*======================================================================
#
#	<<< デバッグ情報の定義 >>>
#
#DEBUG 	= 0
DEBUG	= 1
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
TARGET		=rzesh3b_
else
TARGET		=rzesh3l_
endif
else
CTYPE	= sh2
TARGET		=rzesh2b_
endif
#
#	<<< ファイルネーム定義 >>>
#
ID		=rzexid
#
RZR203		=rzr203
RZR204		=rzr204
RZR420		=rzr420
RZR419		=rzr419
RZR194		=rzr194
RZR193		=rzr193
RZR104		=rzr104
RZR103		=rzr103
RZR210		=rzr210
RZR209		=rzr209
RZI112		=rzi112
RZI111		=rzi111
RZR152		=rzr152
RZR151		=rzr151
RZR524		=rzr524
RZR523		=rzr523
RZR424		=rzr424
RZR423		=rzr423
RZR412		=rzr412
RZR411		=rzr411
RZR324		=rzr324
RZR323		=rzr323
RZR320		=rzr320
RZR319		=rzr319
RZR318		=rzr318
RZR317		=rzr317
RZR312		=rzr312
RZR311		=rzr311
RZR260		=rzr260
RZR259		=rzr259
RZR224		=rzr224
RZR223		=rzr223
RZR220		=rzr220
RZR219		=rzr219
RZR218		=rzr218
RZR217		=rzr217
RZR212		=rzr212
RZR211		=rzr211
RZR160		=rzr160
RZR159		=rzr159
RZR136		=rzr136
RZR135		=rzr135
RZR134		=rzr134
RZR133		=rzr133
RZR124		=rzr124
RZR123		=rzr123
RZR120		=rzr120
RZR119		=rzr119
RZR118		=rzr118
RZR117		=rzr117
RZR112		=rzr112
RZR111		=rzr111
RZR110		=rzr110
RZR109		=rzr109
RZR100		=rzr100
RZR099		=rzr099
RZR098		=rzr098
RZR097		=rzr097
RZR096		=rzr096
RZR095		=rzr095
RZR094		=rzr094
RZR093		=rzr093
RZR092		=rzr092
RZR091		=rzr091
RZR090		=rzr090
RZR089		=rzr089
RZR088		=rzr088
RZR087		=rzr087
RZR086		=rzr086
RZR085		=rzr085
RZR084		=rzr084
RZR083		=rzr083
RZR082		=rzr082
RZR081		=rzr081
RZR080		=rzr080
RZR079		=rzr079
RZR078		=rzr078
RZR077		=rzr077
RZR076		=rzr076
RZR075		=rzr075
RZR074		=rzr074
RZR073		=rzr073
RZR070		=rzr070
RZR069		=rzr069
RZR068		=rzr068
RZR067		=rzr067
RZR066		=rzr066
RZR065		=rzr065
RZR064		=rzr064
RZR063		=rzr063
RZI240		=rzi240
RZI239		=rzi239
RZI140		=rzi140
RZI139		=rzi139
RZI118		=rzi118
RZI117		=rzi117
RZI114A		=rzi114a
#RZI114		=rzi114
RZI113A		=rzi113a
#RZI113		=rzi113
RZI104		=rzi104
RZI103		=rzi103
RZI046		=rzi046
RZI045		=rzi045
RZI044		=rzi044
RZI043		=rzi043
RZI042		=rzi042
RZI041		=rzi041
RZI040		=rzi040
RZI039		=rzi039
RZI038		=rzi038
RZI037		=rzi037
RZI034		=rzi034
RZI033		=rzi033

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
OFILE1=$(ID).obj
OFILE2=$(RZR203).obj $(RZR204).obj $(RZR420).obj $(RZR419).obj $(RZR194).obj $(RZR193).obj
OFILE3=$(RZR104).obj $(RZR103).obj $(RZR210).obj $(RZR209).obj $(RZR152).obj $(RZR151).obj
OFILE4=$(RZR524).obj $(RZR523).obj $(RZR424).obj $(RZR423).obj $(RZR412).obj $(RZR411).obj
OFILE5=$(RZR324).obj $(RZR323).obj $(RZR320).obj $(RZR319).obj $(RZR318).obj $(RZR317).obj
OFILE6=$(RZR312).obj $(RZR311).obj $(RZR260).obj $(RZR259).obj $(RZR224).obj $(RZR223).obj
OFILE7=$(RZR220).obj $(RZR219).obj $(RZR218).obj $(RZR217).obj $(RZR212).obj $(RZR211).obj
OFILE8=$(RZR160).obj $(RZR159).obj $(RZR136).obj $(RZR135).obj $(RZR134).obj $(RZR133).obj
OFILE9=$(RZR124).obj $(RZR123).obj $(RZR120).obj $(RZR119).obj $(RZR118).obj $(RZR117).obj
OFILE10=$(RZR112).obj $(RZR111).obj $(RZR110).obj $(RZR109).obj $(RZR100).obj $(RZR099).obj
OFILE11=$(RZR098).obj $(RZR097).obj $(RZR096).obj $(RZR095).obj $(RZR094).obj $(RZR093).obj
OFILE12=$(RZR092).obj $(RZR091).obj $(RZR090).obj $(RZR089).obj $(RZR088).obj $(RZR087).obj
OFILE13=$(RZR086).obj $(RZR085).obj $(RZR084).obj $(RZR083).obj $(RZR082).obj $(RZR081).obj
OFILE14=$(RZR080).obj $(RZR079).obj $(RZR078).obj $(RZR077).obj $(RZR076).obj $(RZR075).obj
OFILE15=$(RZR074).obj $(RZR073).obj $(RZR070).obj $(RZR069).obj $(RZR068).obj $(RZR067).obj
OFILE16=$(RZR066).obj $(RZR065).obj $(RZR064).obj $(RZR063).obj

OFILE17=$(RZI112).obj $(RZI111).obj $(RZI240).obj $(RZI239).obj $(RZI140).obj $(RZI139).obj
OFILE18=$(RZI118).obj $(RZI117).obj $(RZI114A).obj $(RZI113A).obj $(RZI104).obj $(RZI103).obj
OFILE19=$(RZI046).obj $(RZI045).obj $(RZI044).obj $(RZI043).obj $(RZI042).obj $(RZI041).obj
OFILE20=$(RZI040).obj $(RZI039).obj $(RZI038).obj $(RZI037).obj $(RZI034).obj $(RZI033).obj


OFILES	=$(OFILE1) $(OFILE2) $(OFILE3) $(OFILE4) $(OFILE5) $(OFILE6)\
	 $(OFILE7) $(OFILE8) $(OFILE9) $(OFILE10) $(OFILE11) $(OFILE12)\
	 $(OFILE13) $(OFILE14) $(OFILE15) $(OFILE16) $(OFILE17) $(OFILE18)\
	 $(OFILE19) $(OFILE20)
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
$(RZR203).obj : $(RZR203).c
	$(CC) $(RZR203).c			>> $(ERRFILE)
$(RZR204).obj : $(RZR204).c
	$(CC) $(RZR204).c			>> $(ERRFILE)
$(RZR420).obj : $(RZR420).c
	$(CC) $(RZR420).c			>> $(ERRFILE)
$(RZR419).obj : $(RZR419).c
	$(CC) $(RZR419).c			>> $(ERRFILE)
$(RZR194).obj : $(RZR194).c
	$(CC) $(RZR194).c			>> $(ERRFILE)
$(RZR193).obj : $(RZR193).c
	$(CC) $(RZR193).c			>> $(ERRFILE)
$(RZR104).obj : $(RZR104).c
	$(CC) $(RZR104).c			>> $(ERRFILE)
$(RZR103).obj : $(RZR103).c
	$(CC) $(RZR103).c			>> $(ERRFILE)
$(RZR210).obj : $(RZR210).c
	$(CC) $(RZR210).c			>> $(ERRFILE)
$(RZR209).obj : $(RZR209).c
	$(CC) $(RZR209).c			>> $(ERRFILE)
$(RZI112).obj : $(RZI112).c
	$(CC) $(RZI112).c			>> $(ERRFILE)
$(RZI111).obj : $(RZI111).c
	$(CC) $(RZI111).c			>> $(ERRFILE)
$(RZR152).obj : $(RZR152).c
	$(CC) $(RZR152).c			>> $(ERRFILE)
$(RZR151).obj : $(RZR151).c
	$(CC) $(RZR151).c			>> $(ERRFILE)
$(RZR524).obj : $(RZR524).c
	$(CC) $(RZR524).c			>> $(ERRFILE)
$(RZR523).obj : $(RZR523).c
	$(CC) $(RZR523).c			>> $(ERRFILE)
$(RZR424).obj : $(RZR424).c
	$(CC) $(RZR424).c			>> $(ERRFILE)
$(RZR423).obj : $(RZR423).c
	$(CC) $(RZR423).c			>> $(ERRFILE)
$(RZR412).obj : $(RZR412).c
	$(CC) $(RZR412).c			>> $(ERRFILE)
$(RZR411).obj : $(RZR411).c
	$(CC) $(RZR411).c			>> $(ERRFILE)
$(RZR324).obj : $(RZR324).c
	$(CC) $(RZR324).c			>> $(ERRFILE)
$(RZR323).obj : $(RZR323).c
	$(CC) $(RZR323).c			>> $(ERRFILE)
$(RZR320).obj : $(RZR320).c
	$(CC) $(RZR320).c			>> $(ERRFILE)
$(RZR319).obj : $(RZR319).c
	$(CC) $(RZR319).c			>> $(ERRFILE)
$(RZR318).obj : $(RZR318).c
	$(CC) $(RZR318).c			>> $(ERRFILE)
$(RZR317).obj : $(RZR317).c
	$(CC) $(RZR317).c			>> $(ERRFILE)
$(RZR312).obj : $(RZR312).c
	$(CC) $(RZR312).c			>> $(ERRFILE)
$(RZR311).obj : $(RZR311).c
	$(CC) $(RZR311).c			>> $(ERRFILE)
$(RZR260).obj : $(RZR260).c
	$(CC) $(RZR260).c			>> $(ERRFILE)
$(RZR259).obj : $(RZR259).c
	$(CC) $(RZR259).c			>> $(ERRFILE)
$(RZR224).obj : $(RZR224).c
	$(CC) $(RZR224).c			>> $(ERRFILE)
$(RZR223).obj : $(RZR223).c
	$(CC) $(RZR223).c			>> $(ERRFILE)
$(RZR220).obj : $(RZR220).c
	$(CC) $(RZR220).c			>> $(ERRFILE)
$(RZR219).obj : $(RZR219).c
	$(CC) $(RZR219).c			>> $(ERRFILE)
$(RZR218).obj : $(RZR218).c
	$(CC) $(RZR218).c			>> $(ERRFILE)
$(RZR217).obj : $(RZR217).c
	$(CC) $(RZR217).c			>> $(ERRFILE)
$(RZR212).obj : $(RZR212).c
	$(CC) $(RZR212).c			>> $(ERRFILE)
$(RZR211).obj : $(RZR211).c
	$(CC) $(RZR211).c			>> $(ERRFILE)
$(RZR160).obj : $(RZR160).c
	$(CC) $(RZR160).c			>> $(ERRFILE)
$(RZR159).obj : $(RZR159).c
	$(CC) $(RZR159).c			>> $(ERRFILE)
$(RZR136).obj : $(RZR136).c
	$(CC) $(RZR136).c			>> $(ERRFILE)
$(RZR135).obj : $(RZR135).c
	$(CC) $(RZR135).c			>> $(ERRFILE)
$(RZR134).obj : $(RZR134).c
	$(CC) $(RZR134).c			>> $(ERRFILE)
$(RZR133).obj : $(RZR133).c
	$(CC) $(RZR133).c			>> $(ERRFILE)
$(RZR124).obj : $(RZR124).c
	$(CC) $(RZR124).c			>> $(ERRFILE)
$(RZR123).obj : $(RZR123).c
	$(CC) $(RZR123).c			>> $(ERRFILE)
$(RZR120).obj : $(RZR120).c
	$(CC) $(RZR120).c			>> $(ERRFILE)
$(RZR119).obj : $(RZR119).c
	$(CC) $(RZR119).c			>> $(ERRFILE)
$(RZR118).obj : $(RZR118).c
	$(CC) $(RZR118).c			>> $(ERRFILE)
$(RZR117).obj : $(RZR117).c
	$(CC) $(RZR117).c			>> $(ERRFILE)
$(RZR112).obj : $(RZR112).c
	$(CC) $(RZR112).c			>> $(ERRFILE)
$(RZR111).obj : $(RZR111).c
	$(CC) $(RZR111).c			>> $(ERRFILE)
$(RZR110).obj : $(RZR110).c
	$(CC) $(RZR110).c			>> $(ERRFILE)
$(RZR109).obj : $(RZR109).c
	$(CC) $(RZR109).c			>> $(ERRFILE)
$(RZR100).obj : $(RZR100).c
	$(CC) $(RZR100).c			>> $(ERRFILE)
$(RZR099).obj : $(RZR099).c
	$(CC) $(RZR099).c			>> $(ERRFILE)
$(RZR098).obj : $(RZR098).c
	$(CC) $(RZR098).c			>> $(ERRFILE)
$(RZR097).obj : $(RZR097).c
	$(CC) $(RZR097).c			>> $(ERRFILE)
$(RZR096).obj : $(RZR096).c
	$(CC) $(RZR096).c			>> $(ERRFILE)
$(RZR095).obj : $(RZR095).c
	$(CC) $(RZR095).c			>> $(ERRFILE)
$(RZR094).obj : $(RZR094).c
	$(CC) $(RZR094).c			>> $(ERRFILE)
$(RZR093).obj : $(RZR093).c
	$(CC) $(RZR093).c			>> $(ERRFILE)
$(RZR092).obj : $(RZR092).c
	$(CC) $(RZR092).c			>> $(ERRFILE)
$(RZR091).obj : $(RZR091).c
	$(CC) $(RZR091).c			>> $(ERRFILE)
$(RZR090).obj : $(RZR090).c
	$(CC) $(RZR090).c			>> $(ERRFILE)
$(RZR089).obj : $(RZR089).c
	$(CC) $(RZR089).c			>> $(ERRFILE)
$(RZR088).obj : $(RZR088).c
	$(CC) $(RZR088).c			>> $(ERRFILE)
$(RZR087).obj : $(RZR087).c
	$(CC) $(RZR087).c			>> $(ERRFILE)
$(RZR086).obj : $(RZR086).c
	$(CC) $(RZR086).c			>> $(ERRFILE)
$(RZR085).obj : $(RZR085).c
	$(CC) $(RZR085).c			>> $(ERRFILE)
$(RZR084).obj : $(RZR084).c
	$(CC) $(RZR084).c			>> $(ERRFILE)
$(RZR083).obj : $(RZR083).c
	$(CC) $(RZR083).c			>> $(ERRFILE)
$(RZR082).obj : $(RZR082).c
	$(CC) $(RZR082).c			>> $(ERRFILE)
$(RZR081).obj : $(RZR081).c
	$(CC) $(RZR081).c			>> $(ERRFILE)
$(RZR080).obj : $(RZR080).c
	$(CC) $(RZR080).c			>> $(ERRFILE)
$(RZR079).obj : $(RZR079).c
	$(CC) $(RZR079).c			>> $(ERRFILE)
$(RZR078).obj : $(RZR078).c
	$(CC) $(RZR078).c			>> $(ERRFILE)
$(RZR077).obj : $(RZR077).c
	$(CC) $(RZR077).c			>> $(ERRFILE)
$(RZR076).obj : $(RZR076).c
	$(CC) $(RZR076).c			>> $(ERRFILE)
$(RZR075).obj : $(RZR075).c
	$(CC) $(RZR075).c			>> $(ERRFILE)
$(RZR074).obj : $(RZR074).c
	$(CC) $(RZR074).c			>> $(ERRFILE)
$(RZR073).obj : $(RZR073).c
	$(CC) $(RZR073).c			>> $(ERRFILE)
$(RZR070).obj : $(RZR070).c
	$(CC) $(RZR070).c			>> $(ERRFILE)
$(RZR069).obj : $(RZR069).c
	$(CC) $(RZR069).c			>> $(ERRFILE)
$(RZR068).obj : $(RZR068).c
	$(CC) $(RZR068).c			>> $(ERRFILE)
$(RZR067).obj : $(RZR067).c
	$(CC) $(RZR067).c			>> $(ERRFILE)
$(RZR066).obj : $(RZR066).c
	$(CC) $(RZR066).c			>> $(ERRFILE)
$(RZR065).obj : $(RZR065).c
	$(CC) $(RZR065).c			>> $(ERRFILE)
$(RZR064).obj : $(RZR064).c
	$(CC) $(RZR064).c			>> $(ERRFILE)
$(RZR063).obj : $(RZR063).c
	$(CC) $(RZR063).c			>> $(ERRFILE)
$(RZI240).obj : $(RZI240).c
	$(CC) $(RZI240).c			>> $(ERRFILE)
$(RZI239).obj : $(RZI239).c
	$(CC) $(RZI239).c			>> $(ERRFILE)
$(RZI140).obj : $(RZI140).c
	$(CC) $(RZI140).c			>> $(ERRFILE)
$(RZI139).obj : $(RZI139).c
	$(CC) $(RZI139).c			>> $(ERRFILE)
$(RZI118).obj : $(RZI118).c
	$(CC) $(RZI118).c			>> $(ERRFILE)
$(RZI117).obj : $(RZI117).c
	$(CC) $(RZI117).c			>> $(ERRFILE)
$(RZI114A).obj : $(RZI114A).c
	$(CC) $(RZI114A).c			>> $(ERRFILE)
$(RZI113A).obj : $(RZI113A).c
	$(CC) $(RZI113A).c			>> $(ERRFILE)
$(RZI104).obj : $(RZI104).c
	$(CC) $(RZI104).c			>> $(ERRFILE)
$(RZI103).obj : $(RZI103).c
	$(CC) $(RZI103).c			>> $(ERRFILE)
$(RZI046).obj : $(RZI046).c
	$(CC) $(RZI046).c			>> $(ERRFILE)
$(RZI045).obj : $(RZI045).c
	$(CC) $(RZI045).c			>> $(ERRFILE)
$(RZI044).obj : $(RZI044).c
	$(CC) $(RZI044).c			>> $(ERRFILE)
$(RZI043).obj : $(RZI043).c
	$(CC) $(RZI043).c			>> $(ERRFILE)
$(RZI042).obj : $(RZI042).c
	$(CC) $(RZI042).c			>> $(ERRFILE)
$(RZI041).obj : $(RZI041).c
	$(CC) $(RZI041).c			>> $(ERRFILE)
$(RZI040).obj : $(RZI040).c
	$(CC) $(RZI040).c			>> $(ERRFILE)
$(RZI039).obj : $(RZI039).c
	$(CC) $(RZI039).c			>> $(ERRFILE)
$(RZI038).obj : $(RZI038).c
	$(CC) $(RZI038).c			>> $(ERRFILE)
$(RZI037).obj : $(RZI037).c
	$(CC) $(RZI037).c			>> $(ERRFILE)
$(RZI034).obj : $(RZI034).c
	$(CC) $(RZI034).c			>> $(ERRFILE)
$(RZI033).obj : $(RZI033).c
	$(CC) $(RZI033).c			>> $(ERRFILE)

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

