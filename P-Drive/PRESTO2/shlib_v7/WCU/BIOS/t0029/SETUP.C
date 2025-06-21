/*======================================================================
 * File name    : setup.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	:
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/wcu/bios/src/rcs/setup.c 1.1.29.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: setup.c $
 * リビジョン 1.1.29.1  1970/01/01  00:00:00  chimura
 * 2004/06/19 16:30
 * 57Kでシリアルポートを初期化する。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2002, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */
/*#define	APL_SECT	(0xc010000)*/
#define	BOTT_SECT	

#define RQ15_P		(16-1)
int	scu_io_baud(int,int);
int	*scu_io_init(int, int, int,int);
/*======================================================================
 *      セクション初期化
 *----------------------------------------------------------------------
*/
static void INITSCT(void)
{
	extern int *_D_ROM, *_B_BGN, *_B_END, *_D_BGN, *_D_END;
	int	*p, *q;

	for(p=_D_BGN, q=_D_ROM; p<_D_END; p++, q++) *p=*q;	/* SECTION (D)  */
	for(p=_B_BGN; p<_B_END; p++) *p=0;			/* SECTION (B)  */
}
typedef void	(*V_FUNC)();
typedef struct	apl_info {
	void	*apl_start;
	void	*vect_no;
	V_FUNC	*vecter_adr;
};
void
init_setup(void)
{
	extern void (*hard_int_tbl[])();
	extern void init_registers(void);
	extern void init_fpga(void);
	struct	apl_info	*p;
	V_FUNC			*vec;
	int			i;
	void			(*apl)();
	init_registers();
	INITSCT();
	scu_io_baud(0,6);
	scu_io_init(0,1,0,0);
	init_fpga();
	p = (struct apl_info *)APL_SECT;
	vec = p->vecter_adr;
	for(i = *(int *)p->vect_no; i; i--) {
		hard_int_tbl[i+RQ15_P] = *vec++;
	}
	apl = p->apl_start;
	apl();
	for(;;);
}

void
halt(void)
{
}
