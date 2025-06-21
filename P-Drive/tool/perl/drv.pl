$id = 0;
$cname = "[_a-zA-Z0-9()]+";
while (<>) {
	if ($id == 0) {
		if (/^\s*static\s*struct\s*dzr2093t\s*far\s*(.*)\[/) {
			print "$1\n";
			$id = 1;
		} elsif (m%^\#define\s+($cname)\s+($cname)\s+/\*(.*)\*/%) {
			print "$1,$2,$3\n";
		} elsif (/_DRV_PTN\s*\{\\/) {
			$id = 2;
		}
	} elsif ($id == 1) {
		s%/\*.*\*/%%g;
		s/\s+//g;
		s/{//g;
		s/}//g;
		if (/^;/) {
			$id = 0;
		} else {
			print "$_\n";
		}
	} elsif ($id == 2) {
		s%/\*.*\*/%%g;
		s/\s+//g;
		s/{//g;
		if (/},\\$/) {
			print "$`\n";
		} elsif (/},}$/) {
			print "$`\n";
			$id = 0;
		} else {
			$id = 0;
		}
	}
}
