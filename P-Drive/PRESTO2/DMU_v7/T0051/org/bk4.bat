ci -r4.2 -u -m"FIFOýĺ���ޒǉ�" bootcmds.h p:/presto/dmu_v7/bootload/rcs/bootcmds.h
ci -r3.3 -u -m"drive\flash.h���Q��" bootcmd1.c p:/presto/dmu_v7/bootload/rcs/bootcmd1.c
ci -r3.2 -u -m"rsstr(04.12.7)�ǉ��ABTWRITER���폜 drive\flash.h���Q��" bootcmd2.c p:/presto/dmu_v7/bootload/rcs/bootcmd2.c
ci -r3.2 -u -m"FIFOýĺ���ޒǉ� drive\flash.h���Q��" bootcmd3.c p:/presto/dmu_v7/bootload/rcs/bootcmd3.c
ci -r2.6 -u -m"drive\flash.h���Q��" bootmisc.c p:/presto/dmu_v7/bootload/rcs/bootmisc.c
ci -r3.3 -u -m"BTWRITER���폜 INIT()�ű���BIOS�ڍs���s��" init.c p:/presto/dmu_v7/bootload/rcs/init.c
ci -r3.2 -u -m"BTWRITER���폜" initsk.c p:/presto/dmu_v7/bootload/rcs/initsk.c
ci -r2.5 -u -m"BTWRITER���폜 drive\flash.h���Q��" remmon.c p:/presto/dmu_v7/bootload/rcs/remmon.c
ci -r1.9 -u -m"BTWRITER���폜" stat_eth.c p:/presto/dmu_v7/bootload/rcs/stat_eth.c
ci -r1.7 -u -m"rom_id�̓o�^�ύX" supapl.c p:/presto/dmu_v7/bootload/rcs/supapl.c
ci -r1.2 -u -m"t0051�ɕύX ײ����ID�̉���������" t0026.c p:/presto/dmu_v7/bootload/rcs/t0051.c
ci -r1.3 -u -m"���ؗpBIOS���g�p BIOS\CF,BIOS\FLASH�ǉ�" t0026.mk p:/presto/dmu_v7/bootload/rcs/t0026.mk
rcs -c"#* " p:/presto/dmu_v7/bootload/rcs/t0026.mk
co t0026.mk p:/presto/dmu_v7/bootload/rcs/t0026.mk
ci -r2.2 -u -m"���ؗpBIOS���g�p" sect.asm p:/presto/dmu_v7/bootload/rcs/sect.asm
