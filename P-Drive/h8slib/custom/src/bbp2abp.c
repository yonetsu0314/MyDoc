/*----------------------------------------------------------------------*
 * File name	: bbp2abp.c		/ binary ptn to ascii ptn convert		*
 *----------------------------------------------------------------------*
 *$Header: p:/h8slib/custom/src/RCS/bbp2abp.c 1.1 1970/01/01 00:00:00 yonetsu Exp $
 *$Log: bbp2abp.c $
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
/*		�o�C�i���[�̃r�b�g�p�^�[��bbd��LSB���4bit�j�u���P�ʂ�			*/
/*		ASCII(0x30:'0'-0x3f:'?')�ɕϊ����Aabp[]�Ɏw��o�C�g���X�g		*/
/*		�A�[����B														*/
/*																		*/
/*      ���j ���g���[���n�̃������z�u�ɑΉ��B							*/
/*----------------------------------------------------------------------*/
/* ������																*/
/*		abp[]	: ASCII�ޯ�����ݏo�͐�ւ��߲��							*/
/*		bbd		: ���͂����޲�ذ��ޯ������								*/
/*		len		: �ϊ�����ASCII�ޯ�����݂��޲Đ�						*/
/*																		*/
/* �߂�l																*/
/*		int		: 0		��� successful complete							*/
/*				  -1	��� error code									*/
/*----------------------------------------------------------------------*/
/*
#include	<custom\custom.h>
*/
int
bbp2abp(
	unsigned char	*abp,
	unsigned long	bbd,
	int				len
	)
{
	register int	i= 0;
	unsigned char	 *bbp;

	if( len == 0)								return(-1);
	if( len > (sizeof(long)*2))					return(-1);

	bbp = (unsigned char *)&bbd;
	bbp += sizeof(long);
	
	do {
		if(i & 0x1)  *abp = ((*(bbp--) & 0xf0)>>4)|'0';
		else	     *abp = ((*bbp & 0x0f))|'0';
		abp++;
	} while(++i < len);
	return(0);
}
