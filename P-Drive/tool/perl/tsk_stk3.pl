#
#	task_stk3.pl	1994.03.24	R.Nishimura
#
# 解説
#	タスクのユーザスタックサイズ必要量調べる
#
# 使用方法
#	b:>perl tsk_stk3.pl f_arg f_auto f_tree t_stk tsk_list
#
#
#	各関数引数サイズ読み込み
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
#	各関数動的変数サイズ読み込み
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
#	各関数呼び出し関数リスト読み込み
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
#	スタックサイズ設定値読み込み
#
open(FILE, $ARGV[3]) || die "Cannot open $ARGV[3]: $!" ;
while(<FILE>) {
	chop;
	if(index($_,',') >= $[) {
		@tmp = split(',');
		$stksize{$tmp[0]} = $tmp[1];
	} else {
		$size{$_} = 0;
	}
}
close(FILE);
#
#	スタックサイズ計算
#
open(FILE, $ARGV[4]) || die "Cannot open $ARGV[4]: $!" ;
while(<FILE>) {
	chop;
	if(index($_,',') >= $[) {
		@tmp = split(',');
		$fn = $tmp[0];
		shift(@tmp);
		$max = 0;
		while($f = shift(@tmp)) {
			$c = 0;
			$s = &stack("$f");
			if($max <= $s) {
				$max = $s;
			}
		}
		$setstk = $stksize{$fn};
		$max += $size{$fn};
		$task = $fn;
		$stack = $max;
		write;
	} else {
		$setstk = $stksize{$_};
		$max = $size{$_};
		$task = $_;
		$stack = $max;
		write;
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
	if(index($clist{$a},',') >= $[) {
		@tmp = split(',',$clist{$a});
		$fn = $tmp[0];
		$rf = shift(@tmp);
		$max = 0;
		@work = @rfunc;
		$exst = 0;
		foreach $wk (@work) {
			if($wk eq $rf) {
				$exst = 1;
			}
		}
		if($exst == 0) {
			push(@rfunc, $rf);
			while($f = shift(@tmp)) {
				$s = &stack("$f");
				if($max < $s) {
					$max = $s;
				}
			}
			pop(@rfunc);
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
format	STDOUT_TOP = 
-------------------------------------------------------------------------------
Main(Task) Function Name      Max. Stack Size	Set Stack Size
-------------------------------------------------------------------------------
.
format	STDOUT = 
@<<<<<<<<<<<<<<<<<<<<<<<<<<<< @########         @########
$task,                        $stack,           $setstk
.
#
#	end of file
#
