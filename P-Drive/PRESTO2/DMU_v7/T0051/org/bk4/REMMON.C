/*======================================================================
 * File name    : remmon.c
 * Original		: p:/presto/dmu_v7/bootload/rcs/init.c 1.11
 *----------------------------------------------------------------------
 * Function 	:
 *		初期設定ルーチン（FOR SH7615）
 *----------------------------------------------------------------------
 *$Header: p:/presto/dmu_v7/bootload/rcs/remmon.c 2.4 1970/01/01 00:00:00 kagatume Exp $
 *$Log: remmon.c $
 * リビジョン 2.4  1970/01/01  00:00:00  kagatume
 * 2004/03/24 15:00
 * コマンド一覧ヘルプ機能を整理する
 * 
 * リビジョン 2.3  1970/01/01  00:00:00  kagatume
 * 2004/03/19 17:00
 * ソフトの簡易ダウンロード・シーケンスcomEの作成に対応する
 * 
 * リビジョン 2.2  1970/01/01  00:00:00  kagatume
 * 2004/03/16 12:00
 * 不要なコマンド処理関数を削除する。
 * 
 * リビジョン 2.1  1970/01/01  00:00:00  kagatume
 * 2004/03/08 19:30
 * 起動時のモード判別処理をinit.cに移動する
 * 
 * リビジョン 1.9  1970/01/01  00:00:00  kagatume
 * 2004/02/13 12:30
 * WCUブートローダー移行コマンド送信処理をdownload.cに移動する
 * 
 * リビジョン 1.8  1970/01/01  00:00:00  kagatume
 * 2004/02/06 17:10
 * check_wcu_boot_cmdsend によりチェック後、ブートローダー移行
 * コマンドを送信する処理を追加
 * 
 * リビジョン 1.7  1970/01/01  00:00:00  kagatume
 * 2004/02/04 12:20
 * DMUアプリからＷＤＴによるリセットがかかった場合は、
 * ＷＣＵをブートローダに移行させるようにする（暫定）
 * 
 * リビジョン 1.6  1970/01/01  00:00:00  kagatume
 * 2004/01/28 20:00
 * jump_to_resetをdmu_jump_to_resetに変更する
 * 
 * リビジョン 1.5  1970/01/01  00:00:00  kagatume
 * 2004/01/27 18:00
 * remmon 関数内のＷＣＵへのシリアル送信は不要なのでコメントにする
 * 
 * リビジョン 1.4  1970/01/01  00:00:00  kagatume
 * 2004/01/27 14:00
 * アプリでＷＤＴによるリセットをかけた時には、
 * ブートローダーを実行するようにremmon を変更する
 * 
 * リビジョン 1.3  1970/01/01  00:00:00  kagatume
 * 2004/01/14 13:45
 * ファイル名とコメントをＣＣＷ用に変更する
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * ｲﾝｸﾙｰﾄﾞ･ﾌｧｲﾙを整理
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * ソースをわかりやすく整理
 * 
 *----------------------------------------------------------------------
 *			(c) Copyright 1993, ISHIDA CO., LTD.
 *			959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *			SHIGA JAPAN
 *			(0775) 53-4141
 *======================================================================
 */
#include <stddef.h>
#include <string.h>
#include <kernel.h>
#include <drive\atadrv.h>
#include <sh7615\bootpara.h>
#include <sh7615\dmupara.h>
#include <drive\flash.h>
#include <sh7615\7615.h>
#include "cmfhdl.h"
#include "remmon.h"
#include "rsinout.h"
#include "bootcmds.h"
#include "ledset.h"

/*---------------------------------------------------------------
				コマンド解析関数
----------------------------------------------------------------*/
void remmon(void)
{
	char	c,*adr,p[3];
	int	i,j;
	unsigned short	*adri;

	esc=rep=0;
	adr=adrp=(char *)0;

	rsstr("\nBootLoader on DMU/SH2");

	for(;;){
		rsstr("\n");
		rsstr("DMUAPL>");
		c = rsin();
		if(c!=(char)0x1b) rsout(c);
		switch(c){
		    case 'A':
				comA();
				break;
		    case 'B':
				comB();
				break;
		    case 'C':
				comC();
				break;
		    case 'D':		/* メモリ・ダンプ */
				rsout((char)'-');
				adr=(char *)get8hex();
				if(chkesc()) break;
				if(chkrep()) adr=adrp;
				for(i=0;i<8;++i){
					rslf();
					put8hex((int)adr);
					rsout((char)' ');
					for(j=0;j<16;++j){
						rsout((char)' ');
						put2hex((int)(*(adr++)));
					}
				}
				rslf();
				adrp = adr;
				break;
		    case 'E':
				if (comE()) set_ledmode(FUNC_ERROR);
				break;
		    case 'F':
				comF();
				break;
		    case 'G':
				comG();
				break;
		    case 'H':
				comH();
				break;
		    case 'J':
				comJ();
				break;
		    case 'K':
				comK();
				break;
		    case 'L':
				comL();
				break;
		    case 'N':
				comN();
				break;
		    case 'O':
				comO();
				break;
		    case 'P':
				comP();
				break;
		    case 'Q':
				comQ();
				break;
		    case 'R':
				comR();
				break;
		    case 'S':		/* メモリの代入 */
				p[1]=(char)0;
				rsout((char)'-');
				p[0] = rsin();
				rsout(p[0]);
				if(strcomp(p,"W")==0){		/* １６ｂｉｔワード */
					rsout((char)'-');
					adri=(unsigned short *)get8hex();
					if(chkout()) break;
					for(;;){
						rslf();
						put8hex((int)adri);
						rsout((char)'=');put4hex((int)*adri);
						rsout((char)'-');
						i=get4hex();
						if(chkout()) break;
						*adri=(unsigned short)i;
						++adri;
					}
					break;
				}
				if(strcomp(p,"B")==0){		/* ８ｂｉｔバイト */
					rsout((char)'-');
					adr=(char *)get8hex();
					if(chkout()) break;
					for(;;){
						rslf();
						put8hex((int)adr);
						rsout((char)'=');put2hex((int)*adr);
						rsout((char)'-');
						i=get2hex();
						if(chkout()) break;
						*adr=(char)i;
						++adr;
					}
					break;
				}
				p[2]=(char)0;
				p[1] = rsin();
				rsout(p[1]);
				if(strcomp(p,"PC")==0){		/* プログラム・カウンタ（予備） */
					rsout((char)'=');put4hex(gpc);
					rsout((char)'-');
					i=get8hex();
					if(chkout()) break;
					gpc=i;
				}
				break;
		    case 'T':
				comT();
				break;
		    case '?':
				comQuest();
				break;
		    default:
				break;
		}
	}
}


/*---------------------------------------------------------------
				ヘルプ機能
----------------------------------------------------------------*/
void comQuest(void)
{
	rsstr("【コマンド一覧】\n");
	rsstr("Ａ　ＤＭＵメモリ・テスト（パターン書込み可能）\n");
	rsstr("Ｂ　ＴＣＰ／ＩＰアドレスの設定\n");
	rsstr("Ｃ　ＤＭＵメモリ比較\n");
	rsstr("Ｄ　ＤＭＵメモリ・ダンプ\n");
	rsstr("Ｅ　簡易ダウンロード・シーケンスの実行\n");
	rsstr("Ｆ　ＣＦから読出し\n");
	rsstr("Ｈ　Ｓファイルのセーブ\n");
	rsstr("Ｊ　ＤＭＵのＦＬＡＳＨブロック書き込み\n");
	rsstr("Ｋ　ＤＭＵアプリ実行（アドレス指定）\n");
	rsstr("Ｌ　Ｓファイルのロード\n");
	rsstr("Ｎ　ＤＭＵブートローダ　バージョン表示\n");
	rsstr("Ｏ　ＤＭＵＦＬＡＳＨワード書き込み\n");
	rsstr("Ｐ　ＤＭＵブートローダ再起動\n");
	rsstr("Ｑ　ＤＭＵＦＬＡＳＨセクタ消去\n");
	rsstr("Ｔ　ＣＦに書込み\n");
	rsstr("\n");
	dly_tsk((RELTIM)10);
	rsstr("Ｓ　Ｂ　バイト単位でＤＭＵメモリ読出し、代入（Ｉ／Ｏ操作も可能）\n");
	rsstr("Ｓ　Ｗ　ワード単位でＤＭＵメモリ読出し、代入（Ｉ／Ｏ操作も可能）\n");
	rsstr("\n");
	dly_tsk((RELTIM)10);
	rsstr("Ｇ　Ｆ　ファイルの作成\n");
	rsstr("Ｇ　Ｃ　ファイルの削除\n");
	rsstr("Ｇ　Ｍ　ディレクトリの作成\n");
	rsstr("Ｇ　Ｅ　ディレクトリの削除\n");
	rsstr("Ｇ　Ｏ　ディレクトリの移動、オープン\n");
	rsstr("Ｇ　Ａ　ディレクトリ名の確認\n");
	rsstr("Ｇ　Ｒ　ディレクトリ・リストの表示\n");
	rsstr("Ｇ　Ｈ　ディレクトリのクローズ\n");
	rsstr("Ｇ　Ｉ　ドライブＣ（ＳＲＡＭ）の残量表示\n");
	rsstr("Ｇ　Ｊ　ドライブＥ（ＣＦ）の残量表示\n");
	rsstr("\n");
	dly_tsk((RELTIM)10);
	rsstr("Ｒ　Ｃ　ＲＴＣリセット\n");
	rsstr("Ｒ　Ｂ　ＲＴＣ年月日週セット\n");
	rsstr("Ｒ　Ｅ　ＲＴＣ時分秒セット\n");
	rsstr("Ｒ　Ｆ　ＲＴＣ年月日週表示\n");
	rsstr("Ｒ　Ｈ　ＲＴＣ時分秒表示\n");
	rsstr("Ｒ　Ｓ　ＭＡＣアドレス設定\n");
	rsstr("Ｒ　Ｔ　１ｍＳタイマ・テスト\n");
	rsstr("Ｒ　Ｕ　０．１ｍＳタイマ・テスト\n");
	rsstr("\n");
	dly_tsk((RELTIM)10);
	rsstr("Ｇ　Ｄ　ＣＦへダウンロード（Ｗｅｂリモコン対応）\n");
	rsstr("Ｇ　Ｕ　ＣＦからアップロード（Ｗｅｂリモコン対応）\n");
	rsstr("Ｇ　Ｂ　ＣＦからＣＰＵへダウンロード\n");
	rsstr("Ｇ　２　ＷＣＵのブートローダ書換え\n");
	rsstr("Ｇ　３　ＤＭＵのブートローダ書換え\n");
	rsstr("\n");
}


