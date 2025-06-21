//------------------------------------------------------------------------------
//
// 初期設定マクロ
//   ターゲットボード:コンピューテックス SH7615EVA BOD+
//   デバッガ:CSIDE SH7615-H
//   プローブ:H-UDI Probe H010 
//
// 26/Apr/2001 Created
// 09/Oct/2002 FMR初期化を最初に変更                                KA          *
//
// Iφ:Eφ:Pφ = 4:4:1 (source CLK=15MHz)
//------------------------------------------------------------------------------

{init
Wait(1)
RegisterChange(PC, 3, 400)          // Set PC as dummy (note1)

// FMR変更
FileDialogLoadResetOut(false)
FileLoad(7, FMR7615.mot, 1000E000, 1, true, false, false)
RegisterChange(PC, 3, 1000E000)
GoUntil(false, 1000E018)            // FMR (note2)
while{IsRunning()}

Outport(3, 0xFFFFFDE0, 0xa55a0001)  // BCR1 (note3)
Outport(3, 0xFFFFFDE4, 0xa55a00f8)  // BCR2
Outport(3, 0xFFFFFDFC, 0xa55a0001)  // BCR3
Outport(3, 0xFFFFFDE8, 0xa55a005f)  // WCR1
Outport(3, 0xFFFFFDC0, 0xa55a000b)  // WCR2
Outport(3, 0xFFFFFDC4, 0xa55a0000)  // WCR3
Outport(3, 0xFFFFFDEC, 0xa55a5dc8)  // MCR
Outport(3, 0xFFFFFDF8, 0xa55a0039)  // RTCOR
Outport(3, 0xFFFFFDF4, 0xa55a0000)  // RTCNT
Outport(3, 0xFFFFFDF0, 0xa55a0008)  // RTCSR
Outport(2, 0xFFFF0088, 0x0000)      // SDMR
Inport(3, 0x06000000)               // Dummy read BANK1
Inport(3, 0x06400000)               // Dummy read BANK2
Inport(3, 0x06800000)               // Dummy read BANK3
Inport(3, 0x06c00000)               // Dummy read BANK4

int pc
pc = Inport(3, 0x06000000)          // Get reset vector
RegisterChange(PC, 3, pc)           // Set PC at BOOT address
}
init

// (note1)
// CSIDEが、起動時にRESET番地(0番地:ROMエリア)の値をPCにセットしようとし
// た場合、ROMエリアの値が不定でそれが奇数アドレスでエラーとなるのを回避。

// (note2)
// FMRレジスタの変更用のコードは、内蔵X/Yメモリ上で実行される必要がある。
// 変更用のコード FMR7615.mot を内蔵X/Yメモリへロードして実行。

// (note3)
// H-UDIでは、ブレーク時に、BSCのレジスタ・アドレスが下記のように変化する。
// H'FFFFFFC0～H'FFFFFFFF → H'FFFFFDC0～H'FFFFFDFF
