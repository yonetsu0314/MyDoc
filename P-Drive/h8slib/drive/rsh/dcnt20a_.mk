#-----------------------------------------------------------------------#
# File name	: dcnt20a_.mk	                                     		#
#-----------------------------------------------------------------------#
#$Header: p:/h8slib/drive/rsh/RCS/dcnt20a_.mk 1.1 1970/01/01 00:00:00 yonetsu Exp $
#$Log: dcnt20a_.mk $
# リビジョン 1.1  1970/01/01  00:00:00  yonetsu
# 初期リビジョン
# 
#-----------------------------------------------------------------------#
#				(c) Copyright 2000, ISHIDA  CO., LTD.					#
#				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					#
#				SHIGA JAPAN												#
#				(077) 553-4141											#
#-----------------------------------------------------------------------#
# Function																#
#		ROM化のための カウンター ライブラリーを作成する。				#
#-----------------------------------------------------------------------#
#
#	<<< デバッグ情報の定義 >>>
#
DEBUG	=1			# 0:nodebug, 1:debug
#
#
#
#	<< CPUタイプの定義>>
#
CPU		=7			#0:300stk, #1:300reg, #2:300hn, #3:300ha
					#4:2600n   #5:2600a   #6:2000n  #7:2000a

!include	"dcnt.mak"
