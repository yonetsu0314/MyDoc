/*****************************************************************************
* TCP/IP �R���t�B�O���[�V����
*
******************************************************************************/

#include "kernel.h"
#include "nonet.h"

#define TCP_REPID_MAX   6       /* TCP��t���ő�� */
#define TCP_CEPID_MAX   8       /* TCP�ʐM�[�_�ő�� */
#define UDP_CEPID_MAX   3       /* UDP�ʐM�[�_�ő�� */

#define TCP_TSKID_TOP   0       /* TCP/IP�p�^�X�NID�擪 */
#define TCP_SEMID_TOP   0       /* TCP/IP�p�Z�}�t�HID�擪 */
#define TCP_MBXID_TOP   0       /* TCP/IP�p���C���{�b�N�XID�擪 */
#define TCP_MPFID_TOP   0       /* TCP/IP�p�Œ蒷�������v�[��ID�擪 */

UB ethernet_addr[]   = { 0, 0, 0, 0, 0, 0 }; /* MAC�A�h���X */
UB default_ipaddr[]  = { 0, 0, 0, 0 };       /* ���[�J��IP�A�h���X */
UB subnet_mask[]     = { 255, 255, 255, 0 }; /* �T�u�l�b�g�}�X�N */
UB default_gateway[] = { 0, 0, 0, 0 };       /* �f�t�H���g�Q�[�g�E�F�C */


#include "nonetc.h"

/*****************************************************************************
* �J�[�l���R���t�B�O���[�V����
*
******************************************************************************/

#define TSKID_MAX   9+TCP_TSKID_CNT /* �^�X�NID��� */
#define SEMID_MAX   1+TCP_SEMID_CNT /* �Z�}�t�HID��� */
#define FLGID_MAX   1               /* �C�x���g�t���OID��� */
#define MBXID_MAX   1+TCP_MBXID_CNT /* ���C���{�b�N�XID��� */
#define MBFID_MAX   4               /* ���b�Z�[�W�o�b�t�@ID��� */
#define PORID_MAX   1               /* �����f�u�p�|�[�gID��� */
#define MPLID_MAX   1               /* �ϒ��������v�[��ID��� */
#define MPFID_MAX   1+TCP_MPFID_CNT /* �Œ蒷�������v�[��ID��������� */
#define CYCNO_MAX   1               /* �����N���n���h���ԍ���� */
#define ALMNO_MAX   1               /* �A���[���n���h���ԍ���� */

#include "nocfg4.h"

