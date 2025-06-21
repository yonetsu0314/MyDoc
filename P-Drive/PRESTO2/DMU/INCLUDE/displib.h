/*----------------------------------------------------------------------*
 * File name	: displib.h						*
 *----------------------------------------------------------------------*
 *$Header$
 *$Log$
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CITY				*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#if	!defined(DISPLIB_H)
#define	DISPLIB_H

typedef struct {
    int     x,y;		/* �\�����W				*/
    int     attr;		/* �A�g���r���[�g			*/
    char    *str;		/* ������				*/
} ELS_STR;

typedef struct {
    int     xs,ys;		/* ���i�̑傫��				*/
    int     *graph;		/* ���i�̎���				*/
} ELS_PARTS;

typedef struct {
    int       maxty;		/* �s�� (20 or 25)			*/
    int       maxpalet;		/* �O���t�B�b�N�̍ő�p���b�g�F (8 or 16) */
    int       maxgcolor;	/* �O���t�B�b�N�̍ő�\���F (8 or 4096)	*/
    int       *ppal;		/* �J���[�p���b�g�e�[�u��		*/
    char      *tile;		/* �^�C���p�^�[���Q			*/
    char      **pptn;		/* �p�^�[���Q				*/
    char      **pstr;		/* �O���t�B�b�N������Q			*/
    ELS_PARTS *pparts;		/* �� �i �Q				*/
    ELS_STR   *str;		/* ������Q				*/
    int       *graph;		/* �O���t�B�b�N�X�Q			*/
} ELS_FRAME;

typedef struct {
    int       maxty;		/* �s�� (20 or 25)			*/
    int       maxpalet;		/* �O���t�B�b�N�̍ő�p���b�g�F (8 or 16) */
    int       maxgcolor;	/* �O���t�B�b�N�̍ő�\���F (8 or 4096)	*/
    int       *ppal;		/* �J���[�p���b�g�e�[�u��		*/
    char      *tile;		/* �^�C���p�^�[���Q			*/
    char      **pptn;		/* �p�^�[���Q				*/
    char      **pstr;		/* �O���t�B�b�N������Q			*/
    ELS_PARTS *pparts;		/* �� �i �Q				*/
/* >>>>> MULT_LING */
    ELS_STR   **str;		/* ������Q				*/
/* <<<<< */
    int       *graph;		/* �O���t�B�b�N�X�Q			*/
} ELS_FRAMES;

typedef struct {		/* �A�C�R���ʂ̒�`��			*/
    int   x1,y1,x2,y2;		/* �A�C�R���̃O���t�B�b�N�͈�		*/
    char  *key;			/* �Ԃ�L�[�R�[�h			*/
    int   (*prg)();		/* ���s���郋�[�`��			*/
} SICON;

int	append_icon_tbl(SICON *icon, int num);
int	check_mouse_icon_tbl(void);
int	delete_icon_tbl(int num);
int	Els_draw(ELS_FRAME *p);
int	_Els_draw(ELS_FRAMES *frame);
int	__Els_draw(ELS_FRAME *frame);
void	Els_setframe(ELS_FRAME *frame);
void	_Els_setframe(ELS_FRAMES *frame);
void	__Els_setframe(ELS_FRAME *frame);
void	Els_init(ELS_FRAME *p);
void	_Els_init(ELS_FRAMES *frame);
void	__Els_init(ELS_FRAME *frame);
int	els_draw_str(ELS_FRAME *fr);
int	_els_draw_str(ELS_FRAMES *frame);
int	__els_draw_str(ELS_FRAME *frame);
void	g_page(int active, int disp);
int	init_icon_tbl(void);
int	Set_Palette(ELS_FRAME *frame);
int	Init_Palette(void);
int	_Set_Palette(ELS_FRAMES *frame);
int	put_tnrb48(int c, int x, int y, int col);

#endif	/* DISPLIB_H */

