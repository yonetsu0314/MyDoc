#
#	inccopy.pl	1994.12.23	R.Nishimura
#
# ���
#	�w�肳�ꂽ�t�@�C�����A�J�����g�f�B���N�g���ɑ��݂��Ȃ�
#	�t�@�C�����ɂ��ăR�s�[����B
#	�R�s�[�����t�@�C�����̂����A�g���q�̂݃��j�[�N(.1�`.999)�Ȃ��̂Ƃ���B#
# �g�p���@
#	b:>perl inccopy.pl datafile
#
#
@rel = split(/\./, $ARGV[0]);
if($#rel == 0){	$a = 1;}
else	      {	$a = $#rel;}
$rel[$a] = $num;
do {
	$rel[$a]++;
	$rel = join('.', @rel);
} while -e $rel;
print `copy $ARGV[0] $rel`;
