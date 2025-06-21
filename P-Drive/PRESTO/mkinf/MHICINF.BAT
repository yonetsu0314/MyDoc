#*$Header: p:/presto/mkinf/rcs/mhicinf.bat 1.3 1970/01/01 00:00:00 sakaguti Exp $
#*$Log: mhicinf.bat $
#* ƒŠƒrƒWƒ‡ƒ“ 1.3  1970/01/01  00:00:00  sakaguti
#* MAKEINF.BAT‚ðMHIC—p‚É‘‚«Š·‚¦BMHICINF.BAT‚Æ‚·‚é
#* 
#* ƒŠƒrƒWƒ‡ƒ“ 1.2  1970/01/01  00:00:00  sakaguti
#* mkinf2 -dmu‚É•ÏX
#* 
#* ƒŠƒrƒWƒ‡ƒ“ 1.1  1970/01/01  00:00:00  sakaguti
#* ‚c‚`‚b‚rQ‚y—p‚ð‚b‚b‚v‚q—p‚É‘‚«Š·‚¦
#* 
SET WCU=T0007

type %WCU%.hed > infinfo.txt

mkinf2 -mhic
