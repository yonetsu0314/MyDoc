#
#	tsk_tree.pl	1994.03.24	R.Nishimura
#
# ���
#	�^�X�N�̊֐��Ăяo���\���𒲂ׂ�
#
# �g�p���@
#	b:>perl tsk_tree.pl f_arg f_auto f_tree tsk_list
#
#
#	�e�֐������T�C�Y�ǂݍ���
#
open(FILE, $ARGV[0]) || die "Cannot open $ARGV[0]: $!" ;
while(<FILE>) {
	chop;
	if(index($_,',') >= $[) {
		@tmp = split(',');
		$size{$tmp[0]} = $tmp[1];
	} else {
		$size{$_} = 0;
	}
}
close(FILE);
#
#	�e�֐����I�ϐ��T�C�Y�ǂݍ���
#
open(FILE, $ARGV[1]) || die "Cannot open $ARGV[1]: $!" ;
while(<FILE>) {
	chop;
	@tmp = split(',');
	$file{$tmp[0]} = $tmp[1];
	if($#tmp > 1 ) {
		$size{$tmp[0]} += $tmp[2];
	} else {
		$size{$tmp[0]} += 0;
	}
}
close(FILE);
#
#	�e�֐��Ăяo���֐����X�g�ǂݍ���
#
open(FILE, $ARGV[2]) || die "Cannot open $ARGV[2]: $!" ;
while(<FILE>) {
	chop;
	if(index($_,',') >= $[) {
		@tmp = split(',');
		$f = $tmp[0];
		$clist{$f} = $_;
	} else {
		$clist{$_} = $_;
	}
}
close(FILE);
#
#	�X�^�b�N�T�C�Y�v�Z�֐��ǂݍ���
#
open(FILE, $ARGV[3]) || die "Cannot open $ARGV[3]: $!" ;
while(<FILE>) {
	chop;
	if(index($_,',') >= $[) {
		@tmp = split(',');
		$fn = $tmp[0];
		shift(@tmp);
		print "\n\f\n$fn ($size{$fn}) \[$file{$fn}\]";
		$max = 0;
		while($f = shift(@tmp)) {
			$c = 0;
			$s = &stack("$f");
			if($max <= $s) {
				$max = $s;
			}
		}
		$max += $size{$fn};
	} else {
		$max = $size{$_};
	}
}
close(FILE);
#
#
#
sub stack {
	local(@tmp);
	local($max);
	local($f);
	local($fn);
	local($s);
	local($a);
	local($cn);
	$c++;
	$cn = $c;
	$a = $_[0];
	$cn--;
	print "\n    ";
	$tree = "    ";
	while($cn != 0) {
		print "|   ";
		$tree .= "|   ";
		$cn--;
	}
	print "+--- $a ($size{$a}) \[$file{$a}\]";
	$tree .= "+--- $a ($size{$a})";
	$sfile = $file{$a};
	if(index($clist{$a},',') >= $[) {
		@tmp = split(',',$clist{$a});
		$fn = $tmp[0];
		shift(@tmp);
		$max = 0;
		while($f = shift(@tmp)) {
			$s = &stack("$f");
			if($max < $s) {
				$max = $s;
			}
		}
		$s = $size{$a};
		$s += $max;
	} else {
		$s = $size{$a};
	}
	$c--;
	return( $s );
}
#
#	end of file
#
