ci -r4.2 -u -m"FIFOﾃｽﾄｺﾏﾝﾄﾞ追加" bootcmds.h p:/presto/dmu_v7/bootload/rcs/bootcmds.h
ci -r3.2 -u -m"rsstr(" 04.12.7\n")追加、BTWRITERを削除" bootcmd2.c p:/presto/dmu_v7/bootload/rcs/bootcmd2.c
ci -r3.2 -u -m"FIFOﾃｽﾄｺﾏﾝﾄﾞ追加" bootcmd3.c p:/presto/dmu_v7/bootload/rcs/bootcmd3.c
ci -r3.3 -u -m"BTWRITERを削除" init.c p:/presto/dmu_v7/bootload/rcs/init.c
ci -r3.2 -u -m"BTWRITERを削除,ｱﾌﾟﾘ用BIOSを使用" initsk.c p:/presto/dmu_v7/bootload/rcs/initsk.c
ci -r2.5 -u -m"BTWRITERを削除" remmon.c p:/presto/dmu_v7/bootload/rcs/remmon.c
ci -r1.9 -u -m"BTWRITERを削除" stat_eth.c p:/presto/dmu_v7/bootload/rcs/stat_eth.c
ci -r1.7 -u -m"rom_idの登録変更" supapl.c p:/presto/dmu_v7/bootload/rcs/supapl.c
ci -r1.2 -u -m"t0051に変更" t0026.c p:/presto/dmu_v7/bootload/rcs/t0051.c
ci -r1.3 -u -m"ｱﾌﾟﾘ用BIOSを使用,FIFOをBIOSに分離" t0026.mk p:/presto/dmu_v7/bootload/rcs/t0026.mk
rcs -c"#* " p:/presto/dmu_v7/bootload/rcs/t0026.mk
co t0026.mk p:/presto/dmu_v7/bootload/rcs/t0026.mk
ci -r2.2 -u -m"ｱﾌﾟﾘ用BIOSを使用" sect.asm p:/presto/dmu_v7/bootload/rcs/sect.asm
