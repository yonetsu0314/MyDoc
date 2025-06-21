	if "%2"=="" goto crt
:fil
	perl p:\tool\perl\ulockci.pl %1 > %2
	goto end
:crt
	perl p:\tool\perl\ulockci.pl %1
:end
