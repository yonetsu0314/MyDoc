/*****************************************************************************
* TCP/IP コンフィグレーション
*
******************************************************************************/

#include "kernel.h"
#include "nonet.h"

#define TCP_REPID_MAX   6       /* TCP受付口最大個数 */
#define TCP_CEPID_MAX   8       /* TCP通信端点最大個数 */
#define UDP_CEPID_MAX   3       /* UDP通信端点最大個数 */

#define TCP_TSKID_TOP   0       /* TCP/IP用タスクID先頭 */
#define TCP_SEMID_TOP   0       /* TCP/IP用セマフォID先頭 */
#define TCP_MBXID_TOP   0       /* TCP/IP用メイルボックスID先頭 */
#define TCP_MPFID_TOP   0       /* TCP/IP用固定長メモリプールID先頭 */

UB ethernet_addr[]   = { 0, 0, 0, 0, 0, 0 }; /* MACアドレス */
UB default_ipaddr[]  = { 0, 0, 0, 0 };       /* ローカルIPアドレス */
UB subnet_mask[]     = { 255, 255, 255, 0 }; /* サブネットマスク */
UB default_gateway[] = { 0, 0, 0, 0 };       /* デフォルトゲートウェイ */


#include "nonetc.h"

/*****************************************************************************
* カーネルコンフィグレーション
*
******************************************************************************/

#define TSKID_MAX   9+TCP_TSKID_CNT /* タスクID上限 */
#define SEMID_MAX   1+TCP_SEMID_CNT /* セマフォID上限 */
#define FLGID_MAX   1               /* イベントフラグID上限 */
#define MBXID_MAX   1+TCP_MBXID_CNT /* メイルボックスID上限 */
#define MBFID_MAX   4               /* メッセージバッファID上限 */
#define PORID_MAX   1               /* ランデブ用ポートID上限 */
#define MPLID_MAX   1               /* 可変長メモリプールID上限 */
#define MPFID_MAX   1+TCP_MPFID_CNT /* 固定長メモリプールID上限★訂正 */
#define CYCNO_MAX   1               /* 周期起動ハンドラ番号上限 */
#define ALMNO_MAX   1               /* アラームハンドラ番号上限 */

#include "nocfg4.h"

