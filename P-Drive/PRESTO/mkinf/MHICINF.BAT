#*$Header: p:/presto/mkinf/rcs/mhicinf.bat 1.3 1970/01/01 00:00:00 sakaguti Exp $
#*$Log: mhicinf.bat $
#* リビジョン 1.3  1970/01/01  00:00:00  sakaguti
#* MAKEINF.BATをMHIC用に書き換え。MHICINF.BATとする
#* 
#* リビジョン 1.2  1970/01/01  00:00:00  sakaguti
#* mkinf2 -dmuに変更
#* 
#* リビジョン 1.1  1970/01/01  00:00:00  sakaguti
#* ＤＡＣＳ＿Ｚ用をＣＣＷＲ用に書き換え
#* 
SET WCU=T0007

type %WCU%.hed > infinfo.txt

mkinf2 -mhic
