#
#	chkci.pl	1993.11.01	R.Nishimura
#
# ���
#	�J�����g��ƃf�B���N�g���ɂ��ׂẴ\�[�X�R�[�h�t�@�C�����W��
#	���L�̃R�}���h�����s���邱�Ƃɂ����
#	�`�F�b�N�C���Ώۃt�@�C���i�������݉\�����j��\������
#
#		b:>perl chkci.pl *.*
#
foreach $fname (@ARGV) {
	-e $fname || die "Cannot open $fname: $!";
	next unless (-w $fname);
	print	$fname,"\n";
}
#
#	end of file
#
