#=============================================================================
# rcu32si.pl					1998.12.21  R.Nishimura
#
# original: rcu32si.r1			1998.10.19 r.nishimura
#
# SI�t�@�C���� RCU32�J�����p�ɕҏW����
# SISCR �\���̂��폜����
#
# perl rcu32si.pl odir ifiles
#   odir   : �o�̓f�B���N�g��
#   ifiles : ���̓t�@�C���i�����w��j
#=============================================================================
# ���̓t�@�C���ƂȂ� SI�t�@�C���Ƃ��ẮA
#�E�u�X�[�p�[�Ł`���v���琶�����ꂽ�t�@�C��
#�E��L�t�@�C���� CV�R�}���h�ŕϊ������t�@�C��
#�E��L�t�@�C��������� edit_si.pl3�ŕϊ������t�@�C��
#
# int(unsigned int)��short(unsigned short)�ɕύX����ꍇ�ɂ́A
# ���C�u������ suprdeg2.c ���C������K�v������B
# INT2(0x1000)�ł̎Q�Ƃ�(int *)����(short *)�ɕύX����
# BYTE2(0x2000)�ł̎Q�Ƃ�(word *)����(unsigned short *)�ɕύX����
#
# ���̕ύX���s�����R�Ƃ��ẮAint��short�������T�C�Y�ł���Ƃ����O���
# �v���O���~���O����Ă���ꍇ������ׂł���B
# ���̖����ꎞ�I�ɉ�����Ă���v���O����������̂ŁA�ύX����ꍇ�ɂ�
# ���̉�����������ɖ߂��K�v������B
#=============================================================================

$odir = shift(@ARGV);

foreach $ifile (@ARGV) {
	$ofile = $ifile;
	$ofile =~ s/^.+[:\\]//;
	$ofile = "$odir\\$ofile";
	open(IFP, "<$ifile") || die "$ifile ���I�[�v���ł��܂���B\n";
	open(OFP, ">$ofile") || die "$ofile ���I�[�v���ł��܂���B\n";
	print "$ifile => $ofile\n";
	undef $ifile;
	undef $ofile;
	while (<IFP>) {
		if (/^struct\s+\S+_val\s+{\s*$/) {
			print OFP;
			$loop = 0;
			$offset[$loop] = 0;
			while (<IFP>) {
#rn				s/^(\s+)int(\s+\S+)/$1short$2/;
#rn				s/^(\s+)word(\s+\S+)/$1unsigned short$2/;
#rn				s/^(\s+unsigned\s+)int(\s+\S+)/$1short$2/;
				print OFP;
				if (/^\};/) {
					last;
				} elsif ((/^\s+int\s+\S+/) 	||
						 (/^\s+unsigned\s+int\s+\S+/) ||
						 (/^\s+long\s+\S+/) 	||
						 (/^\s+unsigned\s+long\s+\S+/) ||
						 (/^\s+word\s+\S+/)) {
					if(int($offset[$loop] % 4) != 0) {
						$offset[$loop] = (int($offset[$loop] / 4) + 1) * 4;
					}
					$offset[$loop+1] = $offset[$loop] + 4;
					$loop++;
				} elsif ((/^\s+short\s+\S+/) ||
						 (/^\s+unsigned\s+short\s+\S+/)) {
					if(int($offset[$loop] % 2) != 0) {
						$offset[$loop] = (int($offset[$loop] / 2) + 1) * 2;
					}
					$offset[$loop+1] = $offset[$loop] + 2;
					$loop++;
				} elsif (/^\s+double\s+\S+/) {
					if(int($offset[$loop] % 4) != 0) {
						$offset[$loop] = (int($offset[$loop] / 4) + 1) * 4;
					}
					$offset[$loop+1] = $offset[$loop] + 8;
					$loop++;
				} elsif (/^\s+char\s+\S+\s*\[(\d+)\+(\d+)\]\w*\s*/) {
					$offset[$loop+1] = $offset[$loop] + $1 + $2;
					$loop++;
				} elsif (/^\s+char\s+\S+\s*\[(\d+)\]\w*\s*/) {
					$offset[$loop+1] = $offset[$loop] + $1;
					$loop++;
				}
			}
			$loop_max = $loop;
		} elsif ((/^static\s+SICEL\s+\S+_cel\[\]\s+=\s+{\s*$/) 			||
		         (/^static\s+SICEL\s+far\s+\S+_cel\[\]\s+=\s+{\s*$/) 	||
		         (/^SICEL\s+\S+_cel\[\]\s+=\s+{\s*$/)					||
		         (/^SICEL\s+far\s+\S+_cel\[\]\s+=\s+{\s*$/)) {
			print OFP;
			$loop = 0;
			while (<IFP>) {
				if (/^\};/) {
					print OFP;
					last;
				} elsif (/^(\s+{\s+)(\S+)(,.+)/) {
					print OFP "$1$offset[$loop]$3\n";
					$loop++;
				}
			}
			if( $loop_max != $loop ) {
				print OFP "!!!ERROR!!!";
				print "!!!ERROR!!!";
			}
		} else {
			/^static\s+SISCR\s+\w*\s*\w+_scr\s+=\s+{\s*$/ && last;
			/^SISCR\s+\w*\s*\w+_scr\s+=\s+{\s*$/ && last;
			s!^(#include\s+)"(.+)"!$1<rculib/$2>!;
			print OFP;
		}
	}
	close(IFP);
	close(OFP);
}
