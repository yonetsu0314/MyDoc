#
#	arctext.pl		1995.11.08	R.Nishimura
#
# ���
#	�`�q�b�r�b�n�o�d�łg�d�w�`���ł̈󎚃C���[�W���o�͂����t�@�C����
#	�v�������������ł̂l���������ȂǂŁO�l���폜�����t�@�C����ǂݍ����
#	�e�L�X�g�����𒊏o����B
#
# �g�p���@
#	b:>perl arctext.pl arc.txt
#
#
$first = 1;
foreach $fname (@ARGV) {
	-e $fname || die "Cannot open $fname: $!";
	open(FILE, $fname) || die "Cannot open $ARGV[0]: $!" ;
	while(<FILE>) {
		if (/^\d/) {
			if($first) {
				print "\n";
				$first = 0;
			}
			@str = split(/\s/);
			print $str[$#str];
		} else {
			$first = 1;
		}
	}
	close(FILE);
}
#
#	end of file
#
