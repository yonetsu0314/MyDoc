#*$Header: p:/presto/mkinf/rcs/makeinf.bat 1.2 1970/01/01 00:00:00 sakaguti Exp $
#*$Log: makeinf.bat $
#* ƒŠƒrƒWƒ‡ƒ“ 1.2  1970/01/01  00:00:00  sakaguti
#* mkinf2 -dmu‚É•ÏX
#* 
#* ƒŠƒrƒWƒ‡ƒ“ 1.1  1970/01/01  00:00:00  sakaguti
#* ‚c‚`‚b‚rQ‚y—p‚ð‚b‚b‚v‚q—p‚É‘‚«Š·‚¦
#* 
SET PACKNUM=P1005
SET PACKVER=4.2
SET FPGA=R6302
SET WCU=R6510
SET DMU=T0013

echo %PACKNUM% %PACKVER% > infinfo.txt
type %FPGA%.hed >> infinfo.txt
type %WCU%.hed >> infinfo.txt
type %DMU%.hed >> infinfo.txt

mkinf2 -dmu
