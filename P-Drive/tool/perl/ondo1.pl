#
#	ondo1.pl		1996.03.08	Hiro.Miyamoto
#
#		恒温槽の温度と時間を制御しながら、GPIBから
#		データを取り込む。
#
#		jperl ondo1.pl [ファイル名(拡張子なし)]
#
#	必要な実行ファイル
#	① s_timer2.exe	→ 設定時間が来たら'-1'を返す。
#	② gp_out1.exe	→ GPIBからの入力を取り込む。
#	③ wait.exe	→ 設定時間まで待機。
#	④ timedisp.exe	→ 時間を出力。
#
#	GPIBアドレス
#	-a1	周波数カウンタ
#	-a5	恒温槽
#	-a23	マルチメータ(白金抵抗用)
#
#
#＜測定条件設定＞
#
#①温度サイクル
@proc = (00,20_40,40,40_20,20,20_-10,-10,-10_20,20,1);
#	00:	初期状態
#	20:	20℃一定
#	20_40:	20℃から40℃
#	(1を入れると$proc[1](2つ目のサイクル)に戻る)

#②データ取り込み間隔[s]
$wait = 60;

#③温度勾配(1℃/($tr×$wait/60)分)
#(データ間隔が1分なら$tr=3で3分につき1℃あがる)
$tr = 6;

#④放置時間{ m? [min] s? [sec] }

#初期状態
$wait00 = m120;
#20℃
$wait20 = m180;
#40℃
$wait40 = m180;
#-10℃
$waitm10 = m180;

#⑤初期状態の温度
$te00 = 20;

$file = @ARGV[0] ;
if ($file eq ""){
	$file = t1 ;
}

#リセット
system (" gp_out1 -a5 OF5 ");
system (" gp_out1 -a5 ON5 ");
system (" gp_out1 -a1 C ");
system (" gp_out1 -a1 S0F3G3D0B1B3B4S2 ");
system (" gp_out1 -a1 E ");


for($i=0 ; $i<100 ; $i++) {
#初期状態
	if ($proc[$i] == 00){
		system (" s_timer2 -$wait00 ");
 		system (" gp_out1 -a5 TE$te00 ");
		do {
			system (" wait $wait ");
			system (" gp_out1 -a1 -f$file.dat ");
			system (" timedisp >> $file.dat ");
			system (" gp_out1 -a23 -f$file.dat ");
			$_ = system (" s_timer2 ");
			print " $te00℃(step$i)\n ";
		} while (int($_) != -256);
#20℃
	}elsif ($proc[$i] == 20){
		system (" s_timer2 -$wait20 ");
 		system (" gp_out1 -a5 TE$proc[$i] ");
		do {
			system (" wait $wait ");
			system (" gp_out1 -a1 -f$file.dat ");
			system (" timedisp >> $file.dat ");
			system (" gp_out1 -a23 -f$file.dat ");
			$_ = system (" s_timer2 ");
			print " 20℃(step$i)\n ";
		} while (int($_) != -256);
#40℃
	}elsif ($proc[$i] == 40){
		system (" s_timer2 -$wait40 ");
 		system (" gp_out1 -a5 TE$proc[$i] ");
		do {
			system (" wait $wait ");
			system (" gp_out1 -a1 -f$file.dat ");
			system (" timedisp >> $file.dat ");
			system (" gp_out1 -a23 -f$file.dat ");
			$_ = system (" s_timer2 ");
			print " 40℃(step$i)\n ";
		} while (int($_) != -256);
#-10℃
	}elsif ($proc[$i] == -10){
		system (" s_timer2 -$waitm10 ");
 		system (" gp_out1 -a5 TE$proc[$i] ");
		do {
			system (" wait $wait ");
			system (" gp_out1 -a1 -f$file.dat ");
			system (" timedisp >> $file.dat ");
			system (" gp_out1 -a23 -f$file.dat ");
			$_ = system (" s_timer2 ");
			print " -10℃(step$i)\n ";
		} while (int($_) != -256);
#20℃→40℃
	}elsif ($proc[$i] == 20_40){
		for ($j=21 ; $j<41 ; $j++) {
			for ($k=0 ; $k<$tr ; $k++) {
 				system (" gp_out1 -a5 TE$j ");
				system (" wait $wait ");
				system (" gp_out1 -a1 -f$file.dat ");
				system (" timedisp >> $file.dat ");
				system (" gp_out1 -a23 -f$file.dat ");
				print " 20_40(step$i) $j℃\n ";
			}
		}
#40℃→20℃
	}elsif ($proc[$i] == 40_20){
		for ($j=39 ; $j>19 ; $j--) {
			for ($k=0 ; $k<$tr ; $k++) {
 				system (" gp_out1 -a5 TE$j ");
				system (" wait $wait ");
				system (" gp_out1 -a1 -f$file.dat ");
				system (" timedisp >> $file.dat ");
				system (" gp_out1 -a23 -f$file.dat ");
				print " 40_20(step$i) $j℃\n ";
			}
		}
#20℃→-10℃
	}elsif ($proc[$i] == 20_-10){
		for ($j=19 ; $j>-11 ; $j--) {
			for ($k=0 ; $k<$tr ; $k++) {
 				system (" gp_out1 -a5 TE$j ");
				system (" wait $wait ");
				system (" gp_out1 -a1 -f$file.dat ");
				system (" timedisp >> $file.dat ");
				system (" gp_out1 -a23 -f$file.dat ");
				print " 20_-10(step$i) $j℃\n ";
			}
		}
#-10℃→20℃
	}elsif ($proc[$i] == -10_20){
		for ($j=-9 ; $j<21 ; $j++) {
			for ($k=0 ; $k<$tr ; $k++) {
 				system (" gp_out1 -a5 TE$j ");
				system (" wait $wait ");
				system (" gp_out1 -a1 -f$file.dat ");
				system (" timedisp >> $file.dat ");
				system (" gp_out1 -a23 -f$file.dat ");
				print " -10_20(step$i) $j℃\n ";
			}
		}
	}elsif ($proc[$i] == 1){
		$i = 0;
	}
	
}

print "　 終わったよ～♪♪\n"

