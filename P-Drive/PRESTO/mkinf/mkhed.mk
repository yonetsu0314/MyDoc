#���̃t�@�C���́A�D�����t�@�C���ɒǉ����镔��������������̂ł��B
#���������̂P�s��ǉ�����݂̂ł��B

#DMU�̏ꍇ

PROG	=	t0013# ROM �ԍ��Ɠ����ɂ���

ifndef	PERL5
PERL5	=p:\tool\perl\ver5win3\Perl
endif

$(PROG).abs : spec.def option.def $(OBJS_DMU) $(OBJS_PLCIF) $(MAKEFILE) $(PROG).sub
	$(LINK) $(LDFLAGS)
	$(PERL5)\perl $(PERL5)\mkhed.pl $(PROG).c > $(PROG).hed�@�������ǉ�����



#WCU�̏ꍇ

TARGET		=r6510

DRV	=p:

$(TARGET).abs : $(OBJS) $(TARGET).mk $(TARGET).sub
	$(LNK) -sub=$(TARGET).sub
	$(DRV)\tool\perl\perl $(DRV)\tool\perl\mkhed.pl $(TARGET).c > $(TARGET).hed
	���@�����ǉ�����

