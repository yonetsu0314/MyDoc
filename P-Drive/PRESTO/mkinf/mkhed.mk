#このファイルは、．ｍｋファイルに追加する部分を解説したものです。
#ｐｅｒｌの１行を追加するのみです。

#DMUの場合

PROG	=	t0013# ROM 番号と同じにする

ifndef	PERL5
PERL5	=p:\tool\perl\ver5win3\Perl
endif

$(PROG).abs : spec.def option.def $(OBJS_DMU) $(OBJS_PLCIF) $(MAKEFILE) $(PROG).sub
	$(LINK) $(LDFLAGS)
	$(PERL5)\perl $(PERL5)\mkhed.pl $(PROG).c > $(PROG).hed　←これを追加する



#WCUの場合

TARGET		=r6510

DRV	=p:

$(TARGET).abs : $(OBJS) $(TARGET).mk $(TARGET).sub
	$(LNK) -sub=$(TARGET).sub
	$(DRV)\tool\perl\perl $(DRV)\tool\perl\mkhed.pl $(TARGET).c > $(TARGET).hed
	↑　これを追加する

