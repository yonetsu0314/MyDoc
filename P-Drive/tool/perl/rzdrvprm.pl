$cname = "[_a-zA-Z][_a-zA-Z0-9]+";

while (<>) {
	if (/^\s*\#define\s+(\w+)_OPEN_TIME\s+\((\w+)\)\s*/) {
		print "$1,Open Time,$2\n";
	} elsif (/^\s*\#define\s+(\w+)_ACTUATOR\s+(\w+)\s*/) {
		print "$1,Actuator Type,$2\n";
	} elsif (/^\s*\#define\s+(\w+)_PHASE_TYPE\s+(\w+)\s*/) {
		print "$1,Phase Type,$2\n";
	} elsif (/^\s*\#define\s+(\w+)_STOP_MODE\s+(\w+)\s*/) {
		print "$1,Drive Stop Parm,$2\n";
	} elsif (/^\s*\#define\s+(\w+)_SPEED\s+\((\w+)\)\s*/) {
		print "$1,Speed,$2\n";
	} elsif (/^\s*\#define\s+(\w+)_IGNORE_PLS\s+\((\w+)\)\s*/) {
		print "$1,Min. Output Pls Cnt,$2\n";
	} elsif (/^\s*\#define\s+(\w+)_CAM_SLIT\s+\((\w+)\)\s*/) {
		print "$1,Stop Delay Pls Cnt,$2\n";
	} elsif (/^\s*\#define\s+(\w+)_BR_CL_DELAY\s+\((\w+)\)\s*/) {
		print "$1,Break-Clutch Delay Pulse,$2\n";
	} elsif (/^\s*\#define\s+(\w+)_CL_BR_DELAY\s+\((\w+)\)\s*/) {
		print "$1,Clutch-Break Delay Pulse,$2\n";
	} elsif (/^\s*\#define\s+(\w+)_BRAKE_TIME\s+\((\w+)\)\s*/) {
		print "$1,Break Time,$2\n";
	} elsif (/^\s*\#define\s+(\w+)_ERROR_PLS\s+\((\w+)\)\s*/) {
		print "$1,Err Detect Pls Cnt,$2\n";
	} elsif (/^\s*\#define\s+(\w+)_NOF_PATTERN\s+\((\w+)\)\s*/) {
		print "$1,Number of Pattern,$2\n";
	} elsif (/^\s*\#define\s+(\w+)_DRV_PTN\s+/) {
		print "$1,Drive Pattern,Speed,Repeat\n";
		$i = 0;
	} elsif (/^\s+\{(\d+),\s*(\d+)\},/) {
		$i++;
		print ",$i,$1,$2\n";
	} elsif (/^\s+\{\s*-(\d+),\s*(\d+)\},/) {
		$i++;
		print ",$i,-$1,$2\n";
	} elsif (/^\s+\{\s*(\d+),\s*-(\d+)\},/) {
		$i++;
		print ",$i,$1,-$2\n";
	} elsif (/^\s+\{\s*-(\d+),\s*-(\d+)\},/) {
		$i++;
		print ",$i,-$1,-$2\n";
	}
}
