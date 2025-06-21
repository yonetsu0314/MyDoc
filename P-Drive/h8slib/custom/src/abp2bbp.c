/*----------------------------------------------------------------------*
 * File name	: abp2bbp.c		/ ascii ptn to binary ptn convert		*
 *----------------------------------------------------------------------*
 *$Header: p:/h8slib/custom/src/RCS/abp2bbp.c 1.1 1970/01/01 00:00:00 yonetsu Exp $
 *$Log: abp2bbp.c $
 * ���r�W���� 1.1  1970/01/01  00:00:00  yonetsu
 * �������r�W����
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 1993-2000, ISHIDA  CO., LTD.				*/
/*				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					*/
/*				SHIGA JAPAN												*/
/*				(077) 553-4141											*/
/*----------------------------------------------------------------------*/
/* ����                                                  				*/
/*		abp�Ŏ������w��o�C�g����ASCII�r�b�g�p�^�[��(abp[0]��bit0		*/
/*		��LSB)���o�C�i���[�̃r�b�g�p�^�[���ɕϊ���32bit�Ō��ʂ�Ԃ��B	*/
/*																		*/
/*      ���j ���g���[���n�̃������z�u�ɑΉ��B							*/
/*----------------------------------------------------------------------*/
/* ������																*/
/*		abp[]		: ASCII�ޯ�����݂ւ��߲��							*/
/*		len			: ASCII�ޯ�����݂��޲Đ�(MAX= 8byte)				*/
/*																		*/
/* �߂�l																*/
/*		unsigned long	: �޲�ذ��ޯ������								*/
/*----------------------------------------------------------------------*/
/*
#include	<custom\custom.h>
*/
union bp {
	unsigned long	lp;
	unsigned char	cp[sizeof(long)];
};

unsigned long
abp2bbp(
	unsigned char	*abp,
	int				len
	)
{
	union bp		bbp;
	unsigned char	*p= &bbp.cp[sizeof(long)-1];
	register int	i= 0;

	if((len == 0) || (len > (sizeof(long)*2)))		return(0);
	bbp.lp = 0;
	do {
		if(i & 0x1)  *(p--) |= (*abp & 0xf)<<4;
		else	     *p |= (*abp & 0xf);
		abp++;
	} while(++i < len);
	return((unsigned long)bbp.lp);
}
