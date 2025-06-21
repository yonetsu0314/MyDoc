#
#	ondo1.pl		1996.03.08	Hiro.Miyamoto
#
#		�P�����̉��x�Ǝ��Ԃ𐧌䂵�Ȃ���AGPIB����
#		�f�[�^����荞�ށB
#
#		jperl ondo1.pl [�t�@�C����(�g���q�Ȃ�)]
#
#	�K�v�Ȏ��s�t�@�C��
#	�@ s_timer2.exe	�� �ݒ莞�Ԃ�������'-1'��Ԃ��B
#	�A gp_out1.exe	�� GPIB����̓��͂���荞�ށB
#	�B wait.exe	�� �ݒ莞�Ԃ܂őҋ@�B
#	�C timedisp.exe	�� ���Ԃ��o�́B
#
#	GPIB�A�h���X
#	-a1	���g���J�E���^
#	-a5	�P����
#	-a23	�}���`���[�^(������R�p)
#
#
#����������ݒ聄
#
#�@���x�T�C�N��
@proc = (00,20_40,40,40_20,20,20_-10,-10,-10_20,20,1);
#	00:	�������
#	20:	20�����
#	20_40:	20������40��
#	(1�������$proc[1](2�ڂ̃T�C�N��)�ɖ߂�)

#�A�f�[�^��荞�݊Ԋu[s]
$wait = 60;

#�B���x���z(1��/($tr�~$wait/60)��)
#(�f�[�^�Ԋu��1���Ȃ�$tr=3��3���ɂ�1��������)
$tr = 6;

#�C���u����{ m? [min] s? [sec] }

#�������
$wait00 = m120;
#20��
$wait20 = m180;
#40��
$wait40 = m180;
#-10��
$waitm10 = m180;

#�D������Ԃ̉��x
$te00 = 20;

$file = @ARGV[0] ;
if ($file eq ""){
	$file = t1 ;
}

#���Z�b�g
system (" gp_out1 -a5 OF5 ");
system (" gp_out1 -a5 ON5 ");
system (" gp_out1 -a1 C ");
system (" gp_out1 -a1 S0F3G3D0B1B3B4S2 ");
system (" gp_out1 -a1 E ");


for($i=0 ; $i<100 ; $i++) {
#�������
	if ($proc[$i] == 00){
		system (" s_timer2 -$wait00 ");
 		system (" gp_out1 -a5 TE$te00 ");
		do {
			system (" wait $wait ");
			system (" gp_out1 -a1 -f$file.dat ");
			system (" timedisp >> $file.dat ");
			system (" gp_out1 -a23 -f$file.dat ");
			$_ = system (" s_timer2 ");
			print " $te00��(step$i)\n ";
		} while (int($_) != -256);
#20��
	}elsif ($proc[$i] == 20){
		system (" s_timer2 -$wait20 ");
 		system (" gp_out1 -a5 TE$proc[$i] ");
		do {
			system (" wait $wait ");
			system (" gp_out1 -a1 -f$file.dat ");
			system (" timedisp >> $file.dat ");
			system (" gp_out1 -a23 -f$file.dat ");
			$_ = system (" s_timer2 ");
			print " 20��(step$i)\n ";
		} while (int($_) != -256);
#40��
	}elsif ($proc[$i] == 40){
		system (" s_timer2 -$wait40 ");
 		system (" gp_out1 -a5 TE$proc[$i] ");
		do {
			system (" wait $wait ");
			system (" gp_out1 -a1 -f$file.dat ");
			system (" timedisp >> $file.dat ");
			system (" gp_out1 -a23 -f$file.dat ");
			$_ = system (" s_timer2 ");
			print " 40��(step$i)\n ";
		} while (int($_) != -256);
#-10��
	}elsif ($proc[$i] == -10){
		system (" s_timer2 -$waitm10 ");
 		system (" gp_out1 -a5 TE$proc[$i] ");
		do {
			system (" wait $wait ");
			system (" gp_out1 -a1 -f$file.dat ");
			system (" timedisp >> $file.dat ");
			system (" gp_out1 -a23 -f$file.dat ");
			$_ = system (" s_timer2 ");
			print " -10��(step$i)\n ";
		} while (int($_) != -256);
#20����40��
	}elsif ($proc[$i] == 20_40){
		for ($j=21 ; $j<41 ; $j++) {
			for ($k=0 ; $k<$tr ; $k++) {
 				system (" gp_out1 -a5 TE$j ");
				system (" wait $wait ");
				system (" gp_out1 -a1 -f$file.dat ");
				system (" timedisp >> $file.dat ");
				system (" gp_out1 -a23 -f$file.dat ");
				print " 20_40(step$i) $j��\n ";
			}
		}
#40����20��
	}elsif ($proc[$i] == 40_20){
		for ($j=39 ; $j>19 ; $j--) {
			for ($k=0 ; $k<$tr ; $k++) {
 				system (" gp_out1 -a5 TE$j ");
				system (" wait $wait ");
				system (" gp_out1 -a1 -f$file.dat ");
				system (" timedisp >> $file.dat ");
				system (" gp_out1 -a23 -f$file.dat ");
				print " 40_20(step$i) $j��\n ";
			}
		}
#20����-10��
	}elsif ($proc[$i] == 20_-10){
		for ($j=19 ; $j>-11 ; $j--) {
			for ($k=0 ; $k<$tr ; $k++) {
 				system (" gp_out1 -a5 TE$j ");
				system (" wait $wait ");
				system (" gp_out1 -a1 -f$file.dat ");
				system (" timedisp >> $file.dat ");
				system (" gp_out1 -a23 -f$file.dat ");
				print " 20_-10(step$i) $j��\n ";
			}
		}
#-10����20��
	}elsif ($proc[$i] == -10_20){
		for ($j=-9 ; $j<21 ; $j++) {
			for ($k=0 ; $k<$tr ; $k++) {
 				system (" gp_out1 -a5 TE$j ");
				system (" wait $wait ");
				system (" gp_out1 -a1 -f$file.dat ");
				system (" timedisp >> $file.dat ");
				system (" gp_out1 -a23 -f$file.dat ");
				print " -10_20(step$i) $j��\n ";
			}
		}
	}elsif ($proc[$i] == 1){
		$i = 0;
	}
	
}

print "�@ �I�������`���\n"

