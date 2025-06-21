# mktxthd.pl						1994.9.1 m.fukutome
#
# ������Ή��̂��߂̃e�L�X�g�w�b�_�t�@�C�����쐬����
#
#   perl mktxthd.pl ofile suf1 suf2 [suf3 ...]
#	ofile : �o�̓t�@�C����
#	sufN  : ���̓t�@�C�����C���q
#
#   ���̓t�@�C���� ifileN �� ofile �� sufN ���玩�����������B�����K����
#   ofile �� xxx_yyy.zzz �Ƃ���ƁAifileN �� xxx_sufN.zzz �ƂȂ�B
#   ifile1 ���x�[�X�t�@�C���ɂȂ�A���̃t�@�C���ɂ����`�͑S�Ă̓��̓t�@�C��
#   �ŏo����������ђ�`���e�̐��̑Ή����Ƃ�Ă���K�v������܂��Bifile1 ��
#   ������`�͖�������܂��B�܂��e ifile �� �ŏ��� #pragma �s�܂ł͏����̑Ώ�
#   �ɂȂ�܂���B
#   ���̓t�@�C�����̗L���Ȓ�`�͎��̒ʂ�ł��B
#	1) enum
#	2) STRING
#	3) STRINGS
#	4) #define
#   �����̒�`�����̂悤�Ȓ�`�ɕϊ�����ďo�͂���܂��B
#	�R�����g�͑S�č폜����܂��B
#	1) enum ��`�� ifile1 �̓��e�����̂܂܏o�͂���܂��B
#	2) STRING(x, y) ��
#		STRING(x)
#		DEFM(y)			#ifile1
#		DEFM(y)			#ifile2
#		  |			   |
#		DEFM(y)			#ifileN
#		ENDS
#		#define	x x[lang]
#	   �ƕϊ�����܂��B
#	3) STRINGS(x)
#	   DEFM(y1)
#	     |
#	   DEFM(yN)
#	   ENDS		��
#		STRINGS(x,N)
#		DEFBLK			#ifile1
#		DEFM(y1)
#		  |
#		DEFM(y1)
#		ENDBLK
#		DEFBLK			#ifile2
#		DEFM(y1)
#		  |
#		DEFM(y1)
#		ENDBLK
#		  �c
#		ENDS
#		#define	x x[lang]
#	   �ƕϊ�����܂��B
#	4) #define x y  ��
#		STRING(x)
#		DEFM(y)			#ifile1
#		DEFM(y)			#ifile2
#		  |			   |
#		DEFM(y)			#ifileN
#		ENDS
#		#define	x x[lang]
#	   �ƕϊ�����܂��B
#
#============================================================================
($dmax = @ARGV) < 2 && die "���̓t�@�C�������w�肳��Ă��܂���B\n";
$dmax > 5 && die "���̓t�@�C�����������܂��B\n";
$dmax < 1 && die "�o�̓t�@�C�������w�肳��Ă��܂���B\n";

undef $jpn_f;
$oname = $ARGV[0];
$bnm = $oname;
$bnm =~ s/\.(.+)$//;
$ext = $1;
$bnm =~ s/_.*//;

for ($i = 1; $i < $dmax; ++$i) {
	$iname[$i-1] = "${bnm}_$ARGV[$i].$ext";
	-s $iname[$i-1] ||
	die "�t�@�C���i$iname[$i-1]�j�����݂��Ȃ����A�܂��̓T�C�Y�� 0 �ł��B\n";
	$iname[$i-1] =~ /_jpn\./i && ($jpn_f = 1);
}
--$dmax;

@ifpp = ("BFP", "IFP1", "IFP2", "IFP3", "IFP4");

#============================================================================

# �e���|�����t�@�C�����̍쐬�B���ϐ� TMP �܂��� TEMP �Œ�`����Ă���
# �f�B���N�g�����g�p

$tname	=	"mktxthd.tmp";	# �e���|�����t�@�C����

if (defined $ENV{'TMP'}) {
	$tmp_dir = $ENV{'TMP'};
} elsif (defined $ENV{'TEMP'}) {
	$tmp_dir = $ENV{'TEMP'};
}

if (defined $tmp_dir) {
	$len = length($tmp_dir);
	if (substr($tmp_dir, $len - 1, 1) ne '\\') {
		$tmp_dir .= '\\';
	}
	$tname = $tmp_dir . $tname;
}

#============================================================================

&setup_ifp;

open(OFP,">$oname") || die "�t�@�C���i$oname�j���I�[�v���ł��܂���B\n";

$old_fp = select(OFP);

print "/* Original Files:\n";
for ($i = 0; $i < $dmax; ++$i) {
	print "$header[$i]";
}
print "*/\n\n";

#============================================================================
# ���{��p�t�@�C�����܂܂�Ă���ꍇ�A�V���{�� USE_JPN ���`����
# �i�����L�[������L���ɂ��邽�߁j

print "#define\tUSE_JPN\n" if defined $jpn_f;
print "#define\tLANG_MAX\t($dmax)\n\n";

#============================================================================

while (<BFP>) {
	&delete_rem(0);
	if (/^enum/) {
		&enum_func;
	} elsif (/^STRING\(/) {
		&string_func;
	} elsif (/^STRINGS\(/) {
		&strings_func;
	} elsif (/^#define[ \t]/) {
		&define_func;
	}
}

print "/*======================== EOF ===============================*/\n";

&cleanup_ifp;
select($old_fp);
close(OFP);
close(BFP);

#============================================================================
sub enum_func {			# enum ��`�̏���
	print;
	while (<BFP>) {
		&delete_rem(0);
		print "\t$_" if /[a-z0-9_]/i;
		/};/ && last;
	}
	print "};\n\n";
}

sub string_func {		# STRING ��`�̏���
	s/^[ \t]+//;
	s/[ \t]+$//;
	/STRING\([ \t]*([a-zA-Z0-9_]+)[ \t]*,[ \t]*(.+)\)$/;
	$str_nm = $1;
	$str_def = $2;
	print "STRING($str_nm)\n";
	print "\tDEFM($str_def)\n";
	for ($i = 1; $i <$dmax; ++$i) {
		$find_flg = 0;
		&open_ifp($i);
		while (<$ifp>) {
			&delete_rem($i);
			if (/STRING\([ \t]*$str_nm[ \t]*,[ \t]*(.+)\)$/) {
				print "\tDEFM($1)\n";
				$find_flg = 1;
				last;
			}
		}
		&close_ifp;
		$find_flg == 1 ||
			die "$iname[$i] �� $str_nm ������܂���B\n";
	}
	print "ENDS\n";
	print "#define\t$str_nm\t${str_nm}[lang]\n\n";
}

sub strings_func {		# STRINGS ��`�̏���
	s/^[ \t]+//;
	s/[ \t]+$//;
	/STRINGS\([ \t]*([a-zA-Z0-9_]+)[ \t]*\)$/;
	$str_nm = $1;
	&clear_tmp;
	&open_tmp;
	$defm_cnt = 0;
	while (<BFP>) {
		&delete_rem(0);
		/^ENDS$/ && last;
		if (/^DEFM/) {
			print TFP "\t$_";
			++$defm_cnt;
		}
	}
	&close_tmp;
	print "STRINGS($str_nm,$defm_cnt)\n";
	print "    DEFBLK\n";
	&copy_tmp;
	print "    ENDBLK\n";
	for ($i = 1; $i <$dmax; ++$i) {
		$find_flg = 0;
		&open_ifp($i);
		while (<$ifp>) {
			&delete_rem($i);
			if (/STRINGS\([ \t]*$str_nm[ \t]*\)$/) {
				$find_flg = 1;
				last;
			}
		}
		$find_flg == 1 ||
			die "$iname[$i] �� $str_nm ������܂���B\n";
		print "    DEFBLK\n";
		$defm_n = 0;
		while (<$ifp>) {
			&delete_rem($i);
			/^ENDS$/ && last;
			if (/^DEFM/) {
				print "\t$_";
				++$defm_n;
			}
		}
		&close_ifp;
		print "    ENDBLK\n";
		$defm_cnt != $defm_n &&
			die "$iname[$i] �� $str_nm �̒�`�����قȂ�܂��B\n";
	}
	print "ENDS\n";
	print "#define\t$str_nm\t${str_nm}[lang]\n\n";
}

sub define_func {		# #define ��`�̏���
	s/^[ \t]*#define[ \t]+//;
	s/[ \t]+$//;
	/^([a-zA-Z0-9_]+)[ \t]+(.+)$/;
	$str_nm = $1;
	$str_def = $2;
	print "STRING($str_nm)\n";
	print "\tDEFM($str_def)\n";
	for ($i = 1; $i <$dmax; ++$i) {
		$find_flg = 0;
		&open_ifp($i);
		while (<$ifp>) {
			&delete_rem($i);
			if (/#define[ \t]*$str_nm[ \t]*(.+)$/) {
				print "\tDEFM($1)\n";
				$find_flg = 1;
				last;
			}
		}
		&close_ifp;
		$find_flg == 1 ||
			die "$iname[$i] �� $str_nm ������܂���B\n";
	}
	print "ENDS\n";
	print "#define\t$str_nm\t${str_nm}[lang]\n\n";
}

#==============================================================================
# �R�����g�폜
sub delete_rem {
	$rem_n = shift(@_);
	$rem = $_;
	if ($rem_f[$rem_n] != 0) {
		if (($end = index($_,"*/")) >= $[) {
			$_ = substr($_,$end+2);
			$rem_f[$rem_n] = 0;
		} else {
			s/^.*$//;
			return;
		}
	}
	while (($beg = index($_,"/*")) >= $[) {
		if (($end = index($_,"*/")) >= $[) {
			$_ = substr($_,0,$beg).substr($_,$end+2);
		} else {
			$rem_f[$rem_n] = 1;
			$_ = substr($_,0,$beg);
		}
	}
	s/^[ \t]+//;
	s/[ \t]+$//;
}

#==============================================================================
# ���̓t�@�C������T�u���[�`��

sub setup_ifp {
	for ($ii = 0; $ii < $dmax; ++$ii) {
		open($ifpp[$ii], "<$iname[$ii]") ||
			die "�t�@�C���i$iname[$ii]�j���I�[�v���ł��܂���B\n";
		$rem_f[$ii] = 0;
		$header[$ii] = "$iname[$ii]\n";
	}
	for ($ii = 0; $ii < $dmax; ++$ii) {
		$ifp = $ifpp[$ii];
		while (<$ifp>) {
			&delete_rem($ii);
			if ($rem =~ /\$Header.*\$/) {
				$rem =~ s/^.*\$Header:?[ \t]*//;
				$rem =~ s/[ \t]*Exp[ \t]*\$.*$//;
				$header[$ii] = $rem;
			}
			/#pragma/ && last;
		}
		print "$header[$ii]";
	}
}

sub open_ifp {
	$ii = shift(@_);
#	open($ifp, "<$iname[$ii]") ||
#		die "�t�@�C���i$iname[$ii]�j���I�[�v���ł��܂���B\n";
	$ifp = $ifpp[$ii];
}

sub close_ifp {
}

sub cleanup_ifp {
	for ($ii = 1; $ii < $dmax; ++$ii) {
		close($ifpp[$ii]);
	}
}

#==============================================================================
# �e���|�����t�@�C������T�u���[�`��

sub clear_tmp {		# �e���|�����t�@�C���̏���
	unlink $tname;
}

sub open_tmp {
	open(TFP,">$tname") || die 
		"�e���|�����t�@�C���i$tname�j���I�[�v���ł��܂���B";
}

sub close_tmp {
	close TFP;
}

sub copy_tmp {
	open(TFP,"<$tname") || die 
		"�e���|�����t�@�C���i$tname�j���I�[�v���ł��܂���B";
	while(<TFP>) {
		print;
	}
	close TFP;
}

#==============================================================================
