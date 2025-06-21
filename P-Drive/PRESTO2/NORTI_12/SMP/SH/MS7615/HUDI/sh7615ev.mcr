//------------------------------------------------------------------------------
// �����ݒ�}�N��
//   �^�[�Q�b�g�{�[�h:������LSI�V�X�e���Y MS7615SE01
//   �f�o�b�K:CSIDE SH7615-H
//   �v���[�u:H-UDI Probe H010 
//
// 26/Apr/2001 Created
// 09/Oct/2002 FMR���������ŏ��ɕύX                                KA          *
//
// I��:E��:P�� = 4:4:1 (source CLK=15MHz)
//------------------------------------------------------------------------------

{init
Wait(1)
RegisterChange(PC, 3, 400)          // Set PC as dummy (note1)

// FMR�ύX
FileDialogLoadResetOut(false)
FileLoad(7, FMR7615.mot, 1000E000, 1, true, false, false)
RegisterChange(PC, 3, 1000E000)
GoUntil(false, 1000E018)            // FMR (note2)
while{IsRunning()}

Outport(3, 0xFFFFFDE0, 0xa55a63f1)  // BCR1 (note3)
Outport(3, 0xFFFFFDE4, 0xa55a02f8)  // BCR2
Outport(3, 0xFFFFFDFC, 0xa55a0c01)  // BCR3
Outport(3, 0xFFFFFDE8, 0xa55aab7f)  // WCR1
Outport(3, 0xFFFFFDC0, 0xa55a000b)  // WCR2
Outport(3, 0xFFFFFDC4, 0xa55a0010)  // WCR3
Outport(3, 0xFFFFFDEC, 0xa55a5dd8)  // MCR
Outport(3, 0xFFFFFDF8, 0xa55a0039)  // RTCOR
Outport(3, 0xFFFFFDF4, 0xa55a0000)  // RTCNT
Outport(3, 0xFFFFFDF0, 0xa55a0008)  // RTCSR
Outport(2, 0xFFFF0088, 0x0000)      // SDMR
Inport(3, 0x06000000)               // Dummy read BANK1
Inport(3, 0x06800000)               // Dummy read BANK2
Inport(3, 0x07000000)               // Dummy read BANK3
Inport(3, 0x07800000)               // Dummy read BANK4

int pc
pc = Inport(3, 0x06000000)          // Get reset vector
RegisterChange(PC, 3, pc)           // Set PC at BOOT address
}
init

// (note1)
// CSIDE���A�N������RESET�Ԓn(0�Ԓn:ROM�G���A)�̒l��PC�ɃZ�b�g���悤�Ƃ�
// ���ꍇ�AROM�G���A�̒l���s��ł��ꂪ��A�h���X�ŃG���[�ƂȂ�̂�����B

// (note2)
// FMR���W�X�^�̕ύX�p�̃R�[�h�́A����X/Y��������Ŏ��s�����K�v������B
// �ύX�p�̃R�[�h FMR7615.mot �����X/Y�������փ��[�h���Ď��s�B

// (note3)
// H-UDI�ł́A�u���[�N���ɁABSC�̃��W�X�^�E�A�h���X�����L�̂悤�ɕω�����B
// H'FFFFFFC0�`H'FFFFFFFF �� H'FFFFFDC0�`H'FFFFFDFF
