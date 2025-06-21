////////////////////////////////////////////////////////////
//
//	MS7727 評価ボードの初期設定マクロ（Ver1.00）
//
//  Copyright (C) 2000 COMPUTEX CO.,LTD.
//
////////////////////////////////////////////////////////////
Outport(2,ffffff60,0008)  // BCR1  ｴﾘｱ3をSDRAM
Outport(2,ffffff62,2EF0)  // BCR2
Outport(2,ffffff64,0C30)  // WCR1
Outport(2,ffffff66,BEDD)  // WCR2  ﾚｲﾃﾝｼ2
Outport(2,ffffff68,002C)  // MCR   ﾓｰﾄﾞｾｯﾄｵﾝ､AMX=0､ｵｰﾄﾘﾌﾚｯｼｭ
Outport(2,ffffff6C,0000)  // PCR
Outport(2,ffffff6e,a508)  // RTCSR CKIO/4(8.25MHz--0.12us)
Outport(2,ffffff70,A500)  // RTCNT
Outport(2,ffffff72,a540)  // RTCOR 15.6us=0.12usx130---->HEX=82
Outport(2,ffffff74,A400)  // RFCR
Outport(1,ffffe088,00)    // SDMR //

