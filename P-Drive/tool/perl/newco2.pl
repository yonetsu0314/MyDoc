#
#	newco2.pl	1994.04.14	R.Nishimura
#
# ���
#	RCS�� ident �o�̓t�@�C����ǂݍ���� 
#	$Log$ �̓W�J���ʂ��Q�Ƃ��A
#	�\���t�@�C���̍ŐV��S�� ���[�h�I�����[�� �`�F�b�N�A�E�g����B
#		      ~~~~~~     ~~~~~~~~~~~~~~~~
#	newco.pl �Ƃ̈Ⴂ�́A�������̃I�v�V�������ݒ�ł���@�\���ǉ�����Ă���
#	�Ⴆ�΁A�X�e�[�^�X���q�����ł���ŐV���r�W�������`�F�b�N�A�E�g
#	�������ꍇ�ɂ͈ȉ��̂悤�Ɏg�p����Ηǂ��B
#
#	b:>newco r0036-1.14 -sRel
#
#	���ӁF�I�v�V�����́A�Ō�ɋL�q���ĉ�����
#
# �g�p���@
#	1.RCS�t�@�C���̗L��ꏊ���w��
#	  ���ϐ��̐ݒ�
#		b:>set RCS=p:\wcu\main\rz_wcu\rcs\
#	  �܂��̓J�����g�f�B���N�g���[�̐ݒ�
#		b:>p:
#		p:>cd \wcu\main\rz_wcu\rcs
#		p:>b:
#	2.�`�F�b�N�A�E�g��̐ݒ�Ǝ��s
#		b:>cd \work
#		b:>perl newco2.pl r0003_1.2
#
open(FILE, $ARGV[0]) || die "Cannot open $ARGV[0]: $!" ;
while(<FILE>) {
	tr/A-Z/a-z/;
	if (/\$log: (\S+) */) {
		if( index($1,"p:/") >= $[ ) {
			print `co $ARGV[1] $1`;
		} elsif( index($1,"p:\\") >= $[ ) {
			print `co $ARGV[1] $1`;
		} else {
			@fname = split(/\W/, $1);
			if($ENV{'RCS'} ne '') {
				print `co $ARGV[1] $ENV{'RCS'}$fname[$#fname-1].$fname[$#fname]`;
			} else {
				print `co $ARGV[1] p:$fname[$#fname-1].$fname[$#fname]`;
			}
		}
	}
}
close(FILE);
#
#	end of file
#
