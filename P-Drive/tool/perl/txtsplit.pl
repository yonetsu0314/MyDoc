# txsplit.pl
# C �\�[�X�t�@�C���𕪊�����B

#   perl txsplit.pl filename
#   filename : ��������t�@�C����
#   �t�@�C������ #pragma �̋L�q����Ă���s�Ńt�@�C���𕪊�����B�o�̓t�@�C��
#   ���͓��̓t�@�C���� #pragma �s�ɋL�q����Ă��閼�O�Ŏw�肷��B
#       �o�̓t�@�C�����̐����K��
#         ���̓t�@�C����                   xxx_yyy.zzz
#         pragma �s�Ŏw�肳��Ă��閼�O    aaa
#         �Ƃ���ƁA�o�̓t�@�C������       aaa_yyy.HH
#       pragam �s
#         #pragma xxxxx "name"
#   ���ӓ_
#   1) �ŏ��� pragma �s�܂ł͏o�͂���܂���B
#   2) #define, #pragma �ȊO�̃v���v���Z�b�T�s�͏o�͂���܂���B
#   3) �R�����g�͍l������Ă��܂���B

$suf = $ARGV[0];
$suf =~ s/\..*$//;
$suf =~ /(_\w*)$/;
$suf = $1;

$o_f = 0;
while (<>) {
	if (/^#(\w+)/) {
		if ($1 eq 'pragma') {
			/"(\w+)"/;
			$o_f == 1 && close(OFP);
			$oname = "$1$suf.hh";
			open(OFP, ">$oname")
				|| die "Can't open file ($oname)\n";
			$o_f = 1;
			print OFP;
			next;
		} elsif ($1 ne "define") {
			next;
		}
	}
	print OFP if $o_f == 1;
}

$o_f == 1 && close(OFP);
