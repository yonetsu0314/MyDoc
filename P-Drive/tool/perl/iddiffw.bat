	if "%3"=="" goto crt
:fil
	perl p:\tool\perl\iddiffw.pl %1 %2 > %3
	goto end
:crt
	perl p:\tool\perl\iddiffw.pl %1 %2
:end
