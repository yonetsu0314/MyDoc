#p:/include\dzcmd\dzr209.h(107) : warning C4001: nonstandard extension used - 'bitfield types other than int'
#els_
#上記ワーニングをエラーリストより削除する。	T.Itami
open(FILE, $ARGV[0]) || die "Cannot open $ARGV[0]: $!" ;
while(<FILE>) {
	$cp = "p:/include\.dzcmd\.dzr209.h";
	if(/^$cp/) {
	}else {
                print;
	}
}
close(FILE);
#
#	end of file
#
