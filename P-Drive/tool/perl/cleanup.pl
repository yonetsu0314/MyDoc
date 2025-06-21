# cleanup.pl						1995.1.9
# �J�����g�f�B���N�g������s�v�ȃ\�[�X�t�@�C�����폜����B
# .lnk �t�@�C���Ɏw�肳��Ă���I�u�W�F�N�g�𐶐��\�� .c, .asm �t�@�C��
# �ȊO�� .c, .asm �t�@�C�����폜����B
# .si �t�@�C���� .c �t�@�C�����������A��`����Ă���t�@�C���ȊO���폜����B
# �������������ɂ� #include �݂̂��L�[�Ƃ��Ă��邽�߁A#if, #ifdef ���ɂ�
# �Ή��ł��Ă��Ȃ��B
# �g�p���@�Fperl p:\tool\perl\cleanup.pl xxxx.lnk els_yyy
#	xxxx.lnk : �����N�t�@�C�����s�O�� make �ɂ��쐬���Ă�������
#	els_xxx  : .si �t�@�C���̂���f�B���N�g��

&conout("cleanup.pl  m.fukutome 1995/01/09\n\n");

if (defined($ARGV[1])) {
     $si_dir = $ARGV[1];
     pop ARGV;
}

if (! -e $ARGV[0]) {
     die "$ARGV[0] ��������܂���B";
}

# �폜���Ȃ��t�@�C���̃��X�g���쐬����B
while (<>) {
     chop;
     s/\+//;
     y/A-Z/a-z/;
     split;
     next if /[:\\.]/;
     $valid_file{$_[0]} = 1;
}

open(CONOUT, ">CON");
&conout(
     "�t�@�C���폜�̍ۂɊm�F�����邩�ǂ������w�肵�܂��B\n" .
     "������ N �܂��� n �ƃ^�C�v����Ɗm�F�������Ƀt�@�C�����폜���܂��B\n\n" .
     "�t�@�C���폜���̊m�F�����܂����H (y/n) [y]\033[2D"
);
open(CONIN, "<CON");
$c = getc(CONIN);
close(CONIN);
if ($c eq 'n' || $c eq 'N') {
     $no_confirm = 1;
}
&conout("\n");

&del_file("c");
&del_file("asm");

&conout("\n.si �t�@�C���폜�̏������ł��B\n\n");
while (($key, $value) = each %valid_file) {
     push(cfile, "$key.c");
}

# .si �t�@�C���̍폜����
undef %valid_file;

if (defined($si_dir)) {
     $si_dir =~ y/A-Z/a-z/;
     $dir = "$si_dir\\";
}

while($ifile = pop(cfile)) {
     if (-e $ifile) {
          open(IFP, $ifile);
          while(<IFP>) {
               if (/^[ \t]*\#include[ \t]/) {
                    if (/[\"<](.+)\.[Ss][Ii][\">]/) {
                         $name = $1;
                         $name =~ y/A-Z/a-z/;
                         $valid_file{"$name"} = 1;
                    }
               }
          }
          close(IFP);
     }
}

&del_file(si);

# �w�肵���g���q�̃t�@�C���̍폜����
sub del_file {		# �����͊g���q
     $ext = $_[0];
     system "dir $dir*.$ext > ${ext}_lst.tmp";
     open(DEL_LST, "${ext}_lst.tmp");
     while (<DEL_LST>) {
          if (/^[\w\d]/) {
               y/A-Z/a-z/;
               split;
               $_[0] =~ /([^\\:]+)$/;
               if ($valid_file{$1} != 1) {
                    &confirm_del("$dir$_[0].$ext");
               }
          }
     }
     close(DEL_LST);
}

# �t�@�C���폜�̊m�F�����߂�B�f�t�H���g�͍폜���Ȃ��B

sub confirm_del {	# �����̓t�@�C����
     if ($no_confirm != 1) {
          &conout("$_[0] ���폜���܂��B��낵���ł����H(y/n) [n]\033[2D");
          open(CONIN, "<CON");
          $c = getc(CONIN);
          close(CONIN);
     } else {
          $c = 'y';
     }
     if ($c eq 'y' || $c eq 'Y') {
          chmod 0777, $_[0];
          unlink $_[0];
          &conout("$_[0] ���폜���܂����B\n");
     }
}

sub conout {
     open(CONOUT, ">CON");
     print CONOUT $_[0];
     open(CONOUT, ">CON");
}
