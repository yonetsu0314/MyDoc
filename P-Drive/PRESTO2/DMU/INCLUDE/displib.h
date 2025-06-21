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
    int     x,y;		/* 表示座標				*/
    int     attr;		/* アトリビュート			*/
    char    *str;		/* 文字列				*/
} ELS_STR;

typedef struct {
    int     xs,ys;		/* 部品の大きさ				*/
    int     *graph;		/* 部品の実体				*/
} ELS_PARTS;

typedef struct {
    int       maxty;		/* 行数 (20 or 25)			*/
    int       maxpalet;		/* グラフィックの最大パレット色 (8 or 16) */
    int       maxgcolor;	/* グラフィックの最大表示色 (8 or 4096)	*/
    int       *ppal;		/* カラーパレットテーブル		*/
    char      *tile;		/* タイルパターン群			*/
    char      **pptn;		/* パターン群				*/
    char      **pstr;		/* グラフィック文字列群			*/
    ELS_PARTS *pparts;		/* 部 品 群				*/
    ELS_STR   *str;		/* 文字列群				*/
    int       *graph;		/* グラフィックス群			*/
} ELS_FRAME;

typedef struct {
    int       maxty;		/* 行数 (20 or 25)			*/
    int       maxpalet;		/* グラフィックの最大パレット色 (8 or 16) */
    int       maxgcolor;	/* グラフィックの最大表示色 (8 or 4096)	*/
    int       *ppal;		/* カラーパレットテーブル		*/
    char      *tile;		/* タイルパターン群			*/
    char      **pptn;		/* パターン群				*/
    char      **pstr;		/* グラフィック文字列群			*/
    ELS_PARTS *pparts;		/* 部 品 群				*/
/* >>>>> MULT_LING */
    ELS_STR   **str;		/* 文字列群				*/
/* <<<<< */
    int       *graph;		/* グラフィックス群			*/
} ELS_FRAMES;

typedef struct {		/* アイコン個別の定義体			*/
    int   x1,y1,x2,y2;		/* アイコンのグラフィック範囲		*/
    char  *key;			/* 返るキーコード			*/
    int   (*prg)();		/* 実行するルーチン			*/
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

