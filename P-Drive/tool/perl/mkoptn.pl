# mkopt.pl 				m.fukutome 1995.01.09
# ���b�Z�[�W���o�͂���B���C�N�t�@�C�����Ŏg�p�B

$_ = $ARGV[0];

s/\\!/\"/g;
s/\\@/\#/g;
print "$_\n";
