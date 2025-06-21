/*----------------------------------------------------------------------*
 * File name	: setarg.c		/					 					*
 *----------------------------------------------------------------------*
 *$Header: p:/h8lib/custom/src/rcs/setarg.c 1.1 1996/01/19 10:51:55 kabumoto Exp $
 *$Log: setarg.c $
 * ���r�W���� 1.1  1996/01/19  10:51:55  kabumoto
 * �������r�W����
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 1993, ISHIDA  CO., LTD.					*/
/*				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					*/
/*				SHIGA JAPAN												*/
/*				(0775) 53-4141											*/
/*----------------------------------------------------------------------*/
/* ����                                             		     		*/
/*		buf�Ŏ������o�b�t�@���e�̃X�g�����O�X���A�������̃Z�p���[		*/
/*		�^�̃R�[�h" "�ŕ��������̃X�g�����O�X�ɕϊ�����A���̌��ʂ�		*/
/*		���ăX�g�����O�X�ւ̃|�C���^(*argv)��ݒ肵�A����������(arg		*/
/*		c)��Ԃ��B														*/
/*----------------------------------------------------------------------*/
/* ������																*/
/*		*buf		: ���ݸ޽��ޯ̧�ւ��߲��							*/
/*		max			: argc�̍ő�l										*/
/*		*argv[]		: �߲���̔z��ւ��߲��								*/
/*																		*/
/* �߂�l																*/
/*		int			: argc	��� �������̐�(>=1)							*/
/*					  -1	��� �װ										*/
/*----------------------------------------------------------------------*/

#include	<stddef.h>
#include	<string.h>

static const char	*s = " \t";					/* terminate string		*/

int
setarg(
	char		*buf,							/* pointer to sirng buf	*/
	int			max,							/* maximun argc			*/
	char		*argv[]							/* pointer to argv[]	*/
	)
{
	int			argc =0;						/* no of argument		*/
	char		*cp, *cp1;						/* current string		*/
	char		*pp;							/* privious string		*/
	int			len;

	if((len = strlen( buf)) <= 0)					return(-1);
	if(max == 0)  max = (len >> 1);
	pp = cp = buf;
	while((cp = strpbrk( cp, s))!= NULL) {
		*(cp++) = '\0';
		if((cp - pp) <= 1)  {
			cp1 = cp;
			do {
				cp = cp1;
				if((cp1 = strpbrk( cp1, s))== NULL) {
					argv[++argc] = cp;	goto end;
				}
				*(cp1++) = '\0';
			} while((cp1 - cp) <= 1);
			pp = cp;
			cp = cp1;
		}
		argv[++argc] = pp;
		if(argc >= max)					goto end;
		pp = cp;
	}
	argv[++argc] = pp;
end:
	if(*argv[argc]== '\0' || strchr(s, *argv[argc])!= NULL)  --argc;
	if(argc == 0)  return(-1);
	return(argc);
}
