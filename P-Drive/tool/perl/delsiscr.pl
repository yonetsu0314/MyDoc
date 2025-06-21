# delsiscr.pl						94.9.1 m. fukutome
# SI �t�@�C������ SISCR �\���̂��폜����

# perl delsiscr.pl odir ifiles
#   odir   : �o�̓f�B���N�g��
#   ifiles : ���̓t�@�C���i�����w��j

$odir = shift(@ARGV);

foreach $ifile (@ARGV) {
	$iname = $ifile;
	$iname =~ s/^.+://;
	$iname =~ s/^.+\\//;
	$ofile = "$odir\\$iname";
	open(IFP, "<$ifile") || die "$ifile ���I�[�v���ł��܂���B\n";
	open(OFP, ">$ofile") || die "$ofile ���I�[�v���ł��܂���B\n";
	print "$ifile => $ofile\n";
	$siscr_f = 0;
	while (<IFP>) {
		/^SISCR[ \t]+\w*[ \t]*\w+_scr[ \t]+=[ \t]+{[ \t]*$/ && last;
		print OFP;
	}
	close(IFP);
	close(OFP);
}
