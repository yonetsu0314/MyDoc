#
#	sid_dir.pl	1994.02.01	R.Nishimura
#
# ���
#	�X�[�p�[�Ł`���t�@�C���̃R�����g��\������
#
# �g�p���@
#	b:>perl sid_dir.pl *.sid
#
foreach $fname (@ARGV) {
	-e $fname || die "Cannot open $fname: $!";
	open(FILE, $fname) || die "Cannot open $fname: $!" ;
	seek(FILE,32,0);
	read(FILE,$mess,128);
	@mess = split(/\000/, $mess);
	print $fname,"\t",$mess[0],"\n";
	close(FILE);
}
#
#	end of file
#
