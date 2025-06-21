#*======================================================================
#* File name	: prcsh3b_.mk
#*----------------------------------------------------------------------
#* Function	:
#*	PRESTOコマンド&テキストに関するライブラリーを作成する。
#*	SH3 ビッグエンディアン用
#*----------------------------------------------------------------------
#*$Header: p:/presto2/shlib_v7/prcmd/src/RCS/prcsh3b_.mk 1.2 1970/01/01 00:00:00 kawamura Exp $
#*$Log: prcsh3b_.mk $
#* リビジョン 1.2  1970/01/01  00:00:00  kawamura
#* 2004/05/08  15:30
#* 新ＡＦＤに関するコマンドを追加する。
#* 
#* リビジョン 1.1  1970/01/01  00:00:00  kawamura
#* 初期リビジョン
#* 
#*----------------------------------------------------------------------
#*		(c) Copyright 2003, ISHIDA CO., LTD.
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
#CPUTYPE = 2
CPUTYPE = 3
#
ifeq ($(ENDIAN),1)
ETYPE	= big
else
ETYPE	= little
endif

ifeq ($(CPUTYPE),3)
CTYPE	= sh3
ifeq ($(ENDIAN),1)
TARGET		=prcsh3b_
else
TARGET		=prcsh3l_
endif
else
CTYPE	= sh2
TARGET		=prcsh2b_
endif
#
#	<<< ファイルネーム定義 >>>
#
ID	=prcmdid
#
PRI0001	=pri0001
PRI0002	=pri0002
PRI0003	=pri0003
PRI0004	=pri0004
PRI0005	=pri0005
PRI0006	=pri0006
PRI0007	=pri0007
PRI0008	=pri0008
PRI0009	=pri0009
PRI0010	=pri0010
PRI0011	=pri0011
PRI0012	=pri0012
PRI0013	=pri0013
PRI0014	=pri0014
PRI0015	=pri0015
PRI0016	=pri0016
PRI0017	=pri0017
PRI0018	=pri0018
PRI0019	=pri0019
PRI0020	=pri0020
PRI0021	=pri0021
PRI0022	=pri0022
PRI0023	=pri0023
PRI0024	=pri0024
PRI0025	=pri0025
PRI0026	=pri0026
PRI0027	=pri0027
PRI0028	=pri0028
PRI0029	=pri0029
PRI0030	=pri0030
PRI0031	=pri0031
PRI0032	=pri0032
PRI0033	=pri0033
PRI0034	=pri0034
PRI0035	=pri0035
PRI0036	=pri0036
PRI0043	=pri0043
PRI0044	=pri0044
PRI0047	=pri0047
PRI0048	=pri0048
PRI0049	=pri0049
PRI0050	=pri0050
PRI0051	=pri0051
PRI0052	=pri0052
PRI0053	=pri0053
PRI0054	=pri0054
PRI0055	=pri0055
PRI0056	=pri0056
PRI0057	=pri0057
PRI0058	=pri0058
PRR0003	=prr0003
PRR0004	=prr0004
PRR0005	=prr0005
PRR0006	=prr0006
PRR0007	=prr0007
PRR0008	=prr0008
PRR0009	=prr0009
PRR0010	=prr0010
PRR0011	=prr0011
PRR0012	=prr0012
PRR0013	=prr0013
PRR0014	=prr0014
PRR0015	=prr0015
PRR0016	=prr0016
PRR0017	=prr0017
PRR0018	=prr0018
PRR0019	=prr0019
PRR0020	=prr0020
PRR0021	=prr0021
PRR0022	=prr0022
PRR0023	=prr0023
PRR0024	=prr0024
PRR0025	=prr0025
PRR0026	=prr0026
PRR0027	=prr0027
PRR0028	=prr0028
PRR0029	=prr0029
PRR0030	=prr0030
PRR0031	=prr0031
PRR0032	=prr0032
PRR0033	=prr0033
PRR0034	=prr0034
PRR0035	=prr0035
PRR0036	=prr0036
PRR0037	=prr0037
PRR0038	=prr0038
PRR0039	=prr0039
PRR0040	=prr0040
PRR0041	=prr0041
PRR0042	=prr0042
PRR0043	=prr0043
PRR0044	=prr0044
PRR0045	=prr0045
PRR0046	=prr0046
PRR0047	=prr0047
PRR0048	=prr0048
PRR0049	=prr0049
PRR0050	=prr0050
PRR0051	=prr0051
PRR0052	=prr0052
PRR0053	=prr0053
PRR0054	=prr0054
PRR0055	=prr0055
PRR0056	=prr0056
PRR0057	=prr0057
PRR0058	=prr0058
PRR0059	=prr0059
PRR0060	=prr0060
PRR0061	=prr0061
PRR0062	=prr0062
PRR0063	=prr0063
PRR0064	=prr0064
PRR0067	=prr0067
PRR0068	=prr0068
PRR0069	=prr0069
PRR0070	=prr0070
PRR0071	=prr0071
PRR0072	=prr0072
PRR0085	=prr0085
PRR0086	=prr0086
PRR0101	=prr0101
PRR0102	=prr0102
PRR0103	=prr0103
PRR0104	=prr0104
PRR0105	=prr0105
PRR0106	=prr0106
PRR0107	=prr0107
PRR0108	=prr0108
PRR0109	=prr0109
PRR0110	=prr0110
PRR0111	=prr0111
PRR0112	=prr0112
PRR0113	=prr0113
PRR0114	=prr0114
PRR0115	=prr0115
PRR0116	=prr0116
PRR0119	=prr0119
PRR0120	=prr0120
PRR0121	=prr0121
PRR0122	=prr0122
PRR0123	=prr0123
PRR0124	=prr0124
PRR0125	=prr0125
PRR0126	=prr0126
PRR0127	=prr0127
PRR0128	=prr0128
PRR0129	=prr0129
PRR0130	=prr0130
PRR0131	=prr0131
PRR0132	=prr0132
PRR0133	=prr0133
PRR0134	=prr0134

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
OFILE000  =$(ID).obj
OFILE001	=$(PRI0001).obj $(PRI0002).obj $(PRI0003).obj $(PRI0004).obj
OFILE002	=$(PRI0005).obj $(PRI0006).obj $(PRI0007).obj $(PRI0008).obj
OFILE003	=$(PRI0009).obj $(PRI0010).obj $(PRI0011).obj $(PRI0012).obj
OFILE004	=$(PRI0013).obj $(PRI0014).obj $(PRI0015).obj $(PRI0016).obj
OFILE005	=$(PRI0017).obj $(PRI0018).obj $(PRI0019).obj $(PRI0020).obj
OFILE006	=$(PRI0021).obj $(PRI0022).obj $(PRI0023).obj $(PRI0024).obj
OFILE007	=$(PRI0025).obj $(PRI0026).obj $(PRI0027).obj $(PRI0028).obj
OFILE008	=$(PRI0029).obj $(PRI0030).obj $(PRI0031).obj $(PRI0032).obj
OFILE009	=$(PRI0033).obj $(PRI0034).obj $(PRI0035).obj $(PRI0036).obj
OFILE010	=$(PRI0043).obj $(PRI0044).obj $(PRI0047).obj $(PRI0048).obj
OFILE011	=$(PRI0049).obj $(PRI0050).obj $(PRI0051).obj $(PRI0052).obj
OFILE012	=$(PRI0053).obj $(PRI0054).obj $(PRI0055).obj $(PRI0056).obj
OFILE013	=$(PRI0057).obj $(PRI0058).obj

OFILE101	=$(PRR0003).obj $(PRR0004).obj $(PRR0005).obj $(PRR0006).obj
OFILE102	=$(PRR0007).obj $(PRR0008).obj $(PRR0009).obj $(PRR0010).obj
OFILE103	=$(PRR0011).obj $(PRR0012).obj $(PRR0013).obj $(PRR0014).obj
OFILE104	=$(PRR0015).obj $(PRR0016).obj $(PRR0017).obj $(PRR0018).obj
OFILE105	=$(PRR0019).obj $(PRR0020).obj $(PRR0021).obj $(PRR0022).obj
OFILE106	=$(PRR0023).obj $(PRR0024).obj $(PRR0025).obj $(PRR0026).obj
OFILE107	=$(PRR0027).obj $(PRR0028).obj $(PRR0029).obj $(PRR0030).obj
OFILE108	=$(PRR0031).obj $(PRR0032).obj $(PRR0033).obj $(PRR0034).obj
OFILE109	=$(PRR0035).obj $(PRR0036).obj $(PRR0037).obj $(PRR0038).obj
OFILE110	=$(PRR0039).obj $(PRR0040).obj $(PRR0041).obj $(PRR0042).obj
OFILE111	=$(PRR0043).obj $(PRR0044).obj $(PRR0045).obj $(PRR0046).obj
OFILE112	=$(PRR0047).obj $(PRR0048).obj $(PRR0049).obj $(PRR0050).obj
OFILE113	=$(PRR0051).obj $(PRR0052).obj $(PRR0053).obj $(PRR0054).obj
OFILE114	=$(PRR0055).obj $(PRR0056).obj $(PRR0057).obj $(PRR0058).obj
OFILE115	=$(PRR0059).obj $(PRR0060).obj $(PRR0061).obj $(PRR0062).obj
OFILE116	=$(PRR0063).obj $(PRR0064).obj $(PRR0067).obj $(PRR0068).obj
OFILE117	=$(PRR0069).obj $(PRR0070).obj $(PRR0071).obj $(PRR0072).obj
OFILE118	=$(PRR0085).obj $(PRR0086).obj $(PRR0101).obj $(PRR0102).obj
OFILE119	=$(PRR0103).obj $(PRR0104).obj $(PRR0105).obj $(PRR0106).obj
OFILE120	=$(PRR0107).obj $(PRR0108).obj $(PRR0109).obj $(PRR0110).obj
OFILE121	=$(PRR0111).obj $(PRR0112).obj $(PRR0113).obj $(PRR0114).obj
OFILE122	=$(PRR0115).obj $(PRR0116).obj $(PRR0119).obj $(PRR0120).obj
OFILE123	=$(PRR0121).obj $(PRR0122).obj $(PRR0123).obj $(PRR0124).obj
OFILE124	=$(PRR0125).obj $(PRR0126).obj $(PRR0127).obj $(PRR0128).obj
OFILE125	=$(PRR0129).obj $(PRR0130).obj $(PRR0131).obj $(PRR0132).obj
OFILE126	=$(PRR0133).obj $(PRR0134).obj


OFILES	=$(OFILE000) $(OFILE001) $(OFILE002) $(OFILE003) $(OFILE004) $(OFILE005)\
	 $(OFILE006) $(OFILE007) $(OFILE008) $(OFILE009) $(OFILE010) $(OFILE011)\
	 $(OFILE012) $(OFILE013) \
	 $(OFILE101) $(OFILE102) $(OFILE103) $(OFILE104) $(OFILE105) $(OFILE106)\
	 $(OFILE107) $(OFILE108) $(OFILE109) $(OFILE110) $(OFILE111) $(OFILE112)\
	 $(OFILE113) $(OFILE114) $(OFILE115) $(OFILE116) $(OFILE117) $(OFILE118)\
	 $(OFILE119) $(OFILE120) $(OFILE121) $(OFILE122) $(OFILE123) $(OFILE124)\
	 $(OFILE125) $(OFILE126)
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
$(PRI0001).obj   : $(PRI0001).c
	$(CC) $(PRI0001).c			>>  $(ERRFILE)
$(PRI0002).obj   : $(PRI0002).c
	$(CC) $(PRI0002).c			>>  $(ERRFILE)
$(PRI0003).obj   : $(PRI0003).c
	$(CC) $(PRI0003).c			>>  $(ERRFILE)
$(PRI0004).obj   : $(PRI0004).c
	$(CC) $(PRI0004).c			>>  $(ERRFILE)
$(PRI0005).obj   : $(PRI0005).c
	$(CC) $(PRI0005).c			>>  $(ERRFILE)
$(PRI0006).obj   : $(PRI0006).c
	$(CC) $(PRI0006).c			>>  $(ERRFILE)
$(PRI0007).obj   : $(PRI0007).c
	$(CC) $(PRI0007).c			>>  $(ERRFILE)
$(PRI0008).obj   : $(PRI0008).c
	$(CC) $(PRI0008).c			>>  $(ERRFILE)
$(PRI0009).obj   : $(PRI0009).c
	$(CC) $(PRI0009).c			>>  $(ERRFILE)
$(PRI0010).obj   : $(PRI0010).c
	$(CC) $(PRI0010).c			>>  $(ERRFILE)
$(PRI0011).obj   : $(PRI0011).c
	$(CC) $(PRI0011).c			>>  $(ERRFILE)
$(PRI0012).obj   : $(PRI0012).c
	$(CC) $(PRI0012).c			>>  $(ERRFILE)
$(PRI0013).obj   : $(PRI0013).c
	$(CC) $(PRI0013).c			>>  $(ERRFILE)
$(PRI0014).obj   : $(PRI0014).c
	$(CC) $(PRI0014).c			>>  $(ERRFILE)
$(PRI0015).obj   : $(PRI0015).c
	$(CC) $(PRI0015).c			>>  $(ERRFILE)
$(PRI0016).obj   : $(PRI0016).c
	$(CC) $(PRI0016).c			>>  $(ERRFILE)
$(PRI0017).obj   : $(PRI0017).c
	$(CC) $(PRI0017).c			>>  $(ERRFILE)
$(PRI0018).obj   : $(PRI0018).c
	$(CC) $(PRI0018).c			>>  $(ERRFILE)
$(PRI0019).obj   : $(PRI0019).c
	$(CC) $(PRI0019).c			>>  $(ERRFILE)
$(PRI0020).obj   : $(PRI0020).c
	$(CC) $(PRI0020).c			>>  $(ERRFILE)
$(PRI0021).obj   : $(PRI0021).c
	$(CC) $(PRI0021).c			>>  $(ERRFILE)
$(PRI0022).obj   : $(PRI0022).c
	$(CC) $(PRI0022).c			>>  $(ERRFILE)
$(PRI0023).obj   : $(PRI0023).c
	$(CC) $(PRI0023).c			>>  $(ERRFILE)
$(PRI0024).obj   : $(PRI0024).c
	$(CC) $(PRI0024).c			>>  $(ERRFILE)
$(PRI0025).obj   : $(PRI0025).c
	$(CC) $(PRI0025).c			>>  $(ERRFILE)
$(PRI0026).obj   : $(PRI0026).c
	$(CC) $(PRI0026).c			>>  $(ERRFILE)
$(PRI0027).obj   : $(PRI0027).c
	$(CC) $(PRI0027).c			>>  $(ERRFILE)
$(PRI0028).obj   : $(PRI0028).c
	$(CC) $(PRI0028).c			>>  $(ERRFILE)
$(PRI0029).obj   : $(PRI0029).c
	$(CC) $(PRI0029).c			>>  $(ERRFILE)
$(PRI0030).obj   : $(PRI0030).c
	$(CC) $(PRI0030).c			>>  $(ERRFILE)
$(PRI0031).obj   : $(PRI0031).c
	$(CC) $(PRI0031).c			>>  $(ERRFILE)
$(PRI0032).obj   : $(PRI0032).c
	$(CC) $(PRI0032).c			>>  $(ERRFILE)
$(PRI0033).obj   : $(PRI0033).c
	$(CC) $(PRI0033).c			>>  $(ERRFILE)
$(PRI0034).obj   : $(PRI0034).c
	$(CC) $(PRI0034).c			>>  $(ERRFILE)
$(PRI0035).obj   : $(PRI0035).c
	$(CC) $(PRI0035).c			>>  $(ERRFILE)
$(PRI0036).obj   : $(PRI0036).c
	$(CC) $(PRI0036).c			>>  $(ERRFILE)
$(PRI0043).obj   : $(PRI0043).c
	$(CC) $(PRI0043).c			>>  $(ERRFILE)
$(PRI0044).obj   : $(PRI0044).c
	$(CC) $(PRI0044).c			>>  $(ERRFILE)
$(PRI0047).obj   : $(PRI0047).c
	$(CC) $(PRI0047).c			>>  $(ERRFILE)
$(PRI0048).obj   : $(PRI0048).c
	$(CC) $(PRI0048).c			>>  $(ERRFILE)
$(PRI0049).obj   : $(PRI0049).c
	$(CC) $(PRI0049).c			>>  $(ERRFILE)
$(PRI0050).obj   : $(PRI0050).c
	$(CC) $(PRI0050).c			>>  $(ERRFILE)
$(PRI0051).obj   : $(PRI0051).c
	$(CC) $(PRI0051).c			>>  $(ERRFILE)
$(PRI0052).obj   : $(PRI0052).c
	$(CC) $(PRI0052).c			>>  $(ERRFILE)
$(PRI0053).obj   : $(PRI0053).c
	$(CC) $(PRI0053).c			>>  $(ERRFILE)
$(PRI0054).obj   : $(PRI0054).c
	$(CC) $(PRI0054).c			>>  $(ERRFILE)
$(PRI0055).obj   : $(PRI0055).c
	$(CC) $(PRI0055).c			>>  $(ERRFILE)
$(PRI0056).obj   : $(PRI0056).c
	$(CC) $(PRI0056).c			>>  $(ERRFILE)
$(PRI0057).obj   : $(PRI0057).c
	$(CC) $(PRI0057).c			>>  $(ERRFILE)
$(PRI0058).obj   : $(PRI0058).c
	$(CC) $(PRI0058).c			>>  $(ERRFILE)
$(PRR0003).obj   : $(PRR0003).c
	$(CC) $(PRR0003).c			>>  $(ERRFILE)
$(PRR0004).obj   : $(PRR0004).c
	$(CC) $(PRR0004).c			>>  $(ERRFILE)
$(PRR0005).obj   : $(PRR0005).c
	$(CC) $(PRR0005).c			>>  $(ERRFILE)
$(PRR0006).obj   : $(PRR0006).c
	$(CC) $(PRR0006).c			>>  $(ERRFILE)
$(PRR0007).obj   : $(PRR0007).c
	$(CC) $(PRR0007).c			>>  $(ERRFILE)
$(PRR0008).obj   : $(PRR0008).c
	$(CC) $(PRR0008).c			>>  $(ERRFILE)
$(PRR0009).obj   : $(PRR0009).c
	$(CC) $(PRR0009).c			>>  $(ERRFILE)
$(PRR0010).obj   : $(PRR0010).c
	$(CC) $(PRR0010).c			>>  $(ERRFILE)
$(PRR0011).obj   : $(PRR0011).c
	$(CC) $(PRR0011).c			>>  $(ERRFILE)
$(PRR0012).obj   : $(PRR0012).c
	$(CC) $(PRR0012).c			>>  $(ERRFILE)
$(PRR0013).obj   : $(PRR0013).c
	$(CC) $(PRR0013).c			>>  $(ERRFILE)
$(PRR0014).obj   : $(PRR0014).c
	$(CC) $(PRR0014).c			>>  $(ERRFILE)
$(PRR0015).obj   : $(PRR0015).c
	$(CC) $(PRR0015).c			>>  $(ERRFILE)
$(PRR0016).obj   : $(PRR0016).c
	$(CC) $(PRR0016).c			>>  $(ERRFILE)
$(PRR0017).obj   : $(PRR0017).c
	$(CC) $(PRR0017).c			>>  $(ERRFILE)
$(PRR0018).obj   : $(PRR0018).c
	$(CC) $(PRR0018).c			>>  $(ERRFILE)
$(PRR0019).obj   : $(PRR0019).c
	$(CC) $(PRR0019).c			>>  $(ERRFILE)
$(PRR0020).obj   : $(PRR0020).c
	$(CC) $(PRR0020).c			>>  $(ERRFILE)
$(PRR0021).obj   : $(PRR0021).c
	$(CC) $(PRR0021).c			>>  $(ERRFILE)
$(PRR0022).obj   : $(PRR0022).c
	$(CC) $(PRR0022).c			>>  $(ERRFILE)
$(PRR0023).obj   : $(PRR0023).c
	$(CC) $(PRR0023).c			>>  $(ERRFILE)
$(PRR0024).obj   : $(PRR0024).c
	$(CC) $(PRR0024).c			>>  $(ERRFILE)
$(PRR0025).obj   : $(PRR0025).c
	$(CC) $(PRR0025).c			>>  $(ERRFILE)
$(PRR0026).obj   : $(PRR0026).c
	$(CC) $(PRR0026).c			>>  $(ERRFILE)
$(PRR0027).obj   : $(PRR0027).c
	$(CC) $(PRR0027).c			>>  $(ERRFILE)
$(PRR0028).obj   : $(PRR0028).c
	$(CC) $(PRR0028).c			>>  $(ERRFILE)
$(PRR0029).obj   : $(PRR0029).c
	$(CC) $(PRR0029).c			>>  $(ERRFILE)
$(PRR0030).obj   : $(PRR0030).c
	$(CC) $(PRR0030).c			>>  $(ERRFILE)
$(PRR0031).obj   : $(PRR0031).c
	$(CC) $(PRR0031).c			>>  $(ERRFILE)
$(PRR0032).obj   : $(PRR0032).c
	$(CC) $(PRR0032).c			>>  $(ERRFILE)
$(PRR0033).obj   : $(PRR0033).c
	$(CC) $(PRR0033).c			>>  $(ERRFILE)
$(PRR0034).obj   : $(PRR0034).c
	$(CC) $(PRR0034).c			>>  $(ERRFILE)
$(PRR0035).obj   : $(PRR0035).c
	$(CC) $(PRR0035).c			>>  $(ERRFILE)
$(PRR0036).obj   : $(PRR0036).c
	$(CC) $(PRR0036).c			>>  $(ERRFILE)
$(PRR0037).obj   : $(PRR0037).c
	$(CC) $(PRR0037).c			>>  $(ERRFILE)
$(PRR0038).obj   : $(PRR0038).c
	$(CC) $(PRR0038).c			>>  $(ERRFILE)
$(PRR0039).obj   : $(PRR0039).c
	$(CC) $(PRR0039).c			>>  $(ERRFILE)
$(PRR0040).obj   : $(PRR0040).c
	$(CC) $(PRR0040).c			>>  $(ERRFILE)
$(PRR0041).obj   : $(PRR0041).c
	$(CC) $(PRR0041).c			>>  $(ERRFILE)
$(PRR0042).obj   : $(PRR0042).c
	$(CC) $(PRR0042).c			>>  $(ERRFILE)
$(PRR0043).obj   : $(PRR0043).c
	$(CC) $(PRR0043).c			>>  $(ERRFILE)
$(PRR0044).obj   : $(PRR0044).c
	$(CC) $(PRR0044).c			>>  $(ERRFILE)
$(PRR0045).obj   : $(PRR0045).c
	$(CC) $(PRR0045).c			>>  $(ERRFILE)
$(PRR0046).obj   : $(PRR0046).c
	$(CC) $(PRR0046).c			>>  $(ERRFILE)
$(PRR0047).obj   : $(PRR0047).c
	$(CC) $(PRR0047).c			>>  $(ERRFILE)
$(PRR0048).obj   : $(PRR0048).c
	$(CC) $(PRR0048).c			>>  $(ERRFILE)
$(PRR0049).obj   : $(PRR0049).c
	$(CC) $(PRR0049).c			>>  $(ERRFILE)
$(PRR0050).obj   : $(PRR0050).c
	$(CC) $(PRR0050).c			>>  $(ERRFILE)
$(PRR0051).obj   : $(PRR0051).c
	$(CC) $(PRR0051).c			>>  $(ERRFILE)
$(PRR0052).obj   : $(PRR0052).c
	$(CC) $(PRR0052).c			>>  $(ERRFILE)
$(PRR0053).obj   : $(PRR0053).c
	$(CC) $(PRR0053).c			>>  $(ERRFILE)
$(PRR0054).obj   : $(PRR0054).c
	$(CC) $(PRR0054).c			>>  $(ERRFILE)
$(PRR0055).obj   : $(PRR0055).c
	$(CC) $(PRR0055).c			>>  $(ERRFILE)
$(PRR0056).obj   : $(PRR0056).c
	$(CC) $(PRR0056).c			>>  $(ERRFILE)
$(PRR0057).obj   : $(PRR0057).c
	$(CC) $(PRR0057).c			>>  $(ERRFILE)
$(PRR0058).obj   : $(PRR0058).c
	$(CC) $(PRR0058).c			>>  $(ERRFILE)
$(PRR0059).obj   : $(PRR0059).c
	$(CC) $(PRR0059).c			>>  $(ERRFILE)
$(PRR0060).obj   : $(PRR0060).c
	$(CC) $(PRR0060).c			>>  $(ERRFILE)
$(PRR0061).obj   : $(PRR0061).c
	$(CC) $(PRR0061).c			>>  $(ERRFILE)
$(PRR0062).obj   : $(PRR0062).c
	$(CC) $(PRR0062).c			>>  $(ERRFILE)
$(PRR0063).obj   : $(PRR0063).c
	$(CC) $(PRR0063).c			>>  $(ERRFILE)
$(PRR0064).obj   : $(PRR0064).c
	$(CC) $(PRR0064).c			>>  $(ERRFILE)
$(PRR0067).obj   : $(PRR0067).c
	$(CC) $(PRR0067).c			>>  $(ERRFILE)
$(PRR0068).obj   : $(PRR0068).c
	$(CC) $(PRR0068).c			>>  $(ERRFILE)
$(PRR0069).obj   : $(PRR0069).c
	$(CC) $(PRR0069).c			>>  $(ERRFILE)
$(PRR0070).obj   : $(PRR0070).c
	$(CC) $(PRR0070).c			>>  $(ERRFILE)
$(PRR0071).obj   : $(PRR0071).c
	$(CC) $(PRR0071).c			>>  $(ERRFILE)
$(PRR0072).obj   : $(PRR0072).c
	$(CC) $(PRR0072).c			>>  $(ERRFILE)
$(PRR0085).obj   : $(PRR0085).c
	$(CC) $(PRR0085).c			>>  $(ERRFILE)
$(PRR0086).obj   : $(PRR0086).c
	$(CC) $(PRR0086).c			>>  $(ERRFILE)
$(PRR0101).obj   : $(PRR0101).c
	$(CC) $(PRR0101).c			>>  $(ERRFILE)
$(PRR0102).obj   : $(PRR0102).c
	$(CC) $(PRR0102).c			>>  $(ERRFILE)
$(PRR0103).obj   : $(PRR0103).c
	$(CC) $(PRR0103).c			>>  $(ERRFILE)
$(PRR0104).obj   : $(PRR0104).c
	$(CC) $(PRR0104).c			>>  $(ERRFILE)
$(PRR0105).obj   : $(PRR0105).c
	$(CC) $(PRR0105).c			>>  $(ERRFILE)
$(PRR0106).obj   : $(PRR0106).c
	$(CC) $(PRR0106).c			>>  $(ERRFILE)
$(PRR0107).obj   : $(PRR0107).c
	$(CC) $(PRR0107).c			>>  $(ERRFILE)
$(PRR0108).obj   : $(PRR0108).c
	$(CC) $(PRR0108).c			>>  $(ERRFILE)
$(PRR0109).obj   : $(PRR0109).c
	$(CC) $(PRR0109).c			>>  $(ERRFILE)
$(PRR0110).obj   : $(PRR0110).c
	$(CC) $(PRR0110).c			>>  $(ERRFILE)
$(PRR0111).obj   : $(PRR0111).c
	$(CC) $(PRR0111).c			>>  $(ERRFILE)
$(PRR0112).obj   : $(PRR0112).c
	$(CC) $(PRR0112).c			>>  $(ERRFILE)
$(PRR0113).obj   : $(PRR0113).c
	$(CC) $(PRR0113).c			>>  $(ERRFILE)
$(PRR0114).obj   : $(PRR0114).c
	$(CC) $(PRR0114).c			>>  $(ERRFILE)
$(PRR0115).obj   : $(PRR0115).c
	$(CC) $(PRR0115).c			>>  $(ERRFILE)
$(PRR0116).obj   : $(PRR0116).c
	$(CC) $(PRR0116).c			>>  $(ERRFILE)
$(PRR0119).obj   : $(PRR0119).c
	$(CC) $(PRR0119).c			>>  $(ERRFILE)
$(PRR0120).obj   : $(PRR0120).c
	$(CC) $(PRR0120).c			>>  $(ERRFILE)
$(PRR0121).obj   : $(PRR0121).c
	$(CC) $(PRR0121).c			>>  $(ERRFILE)
$(PRR0122).obj   : $(PRR0122).c
	$(CC) $(PRR0122).c			>>  $(ERRFILE)
$(PRR0123).obj   : $(PRR0123).c
	$(CC) $(PRR0123).c			>>  $(ERRFILE)
$(PRR0124).obj   : $(PRR0124).c
	$(CC) $(PRR0124).c			>>  $(ERRFILE)
$(PRR0125).obj   : $(PRR0125).c
	$(CC) $(PRR0125).c			>>  $(ERRFILE)
$(PRR0126).obj   : $(PRR0126).c
	$(CC) $(PRR0126).c			>>  $(ERRFILE)
$(PRR0127).obj   : $(PRR0127).c
	$(CC) $(PRR0127).c			>>  $(ERRFILE)
$(PRR0128).obj   : $(PRR0128).c
	$(CC) $(PRR0128).c			>>  $(ERRFILE)
$(PRR0129).obj   : $(PRR0129).c
	$(CC) $(PRR0129).c			>>  $(ERRFILE)
$(PRR0130).obj   : $(PRR0130).c
	$(CC) $(PRR0130).c			>>  $(ERRFILE)
$(PRR0131).obj   : $(PRR0131).c
	$(CC) $(PRR0131).c			>>  $(ERRFILE)
$(PRR0132).obj   : $(PRR0132).c
	$(CC) $(PRR0132).c			>>  $(ERRFILE)
$(PRR0133).obj   : $(PRR0133).c
	$(CC) $(PRR0133).c			>>  $(ERRFILE)
$(PRR0134).obj   : $(PRR0134).c
	$(CC) $(PRR0134).c			>>  $(ERRFILE)

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
