////////////////////////////////////////////////////////////
//
//	MS7750SE01 評価ボードの初期設定マクロ（Ver1.00）
//
//  Copyright (C) 2000 COMPUTEX CO.,LTD.
//
////////////////////////////////////////////////////////////
Outport(3,ff800000,00000008)
Outport(2,ff800004,2e3c)
Outport(3,ff800008,02720777)
Outport(3,ff80000c,ffff4fff)
Outport(3,ff800010,01717771)
Outport(3,ff800014,8801001C)
Outport(1,ff940110,00)
Outport(2,ff800018,0000)
Outport(2,ff800020,a500)
Outport(2,ff800024,a508)
Outport(2,ff800028,a400)
Outport(2,ff80001c,a510)
// Outport(2,ff80001c,a508)
Outport(3,ff00001c,00000808)
Wait(1)
Outport(3,ff800014,C801001C)
Outport(1,ff940110,00)
Outport(2,ff80001c,a518)
// Outport(2,ff80001c,a50c)
Outport(3,ff00001c,00000100)

