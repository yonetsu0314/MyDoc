SET SH_COMPILER_BASE=C:\usr\local\SH\7_1_1
SET NORTI_BASE=P:\PRESTO2\NORTI_12
SET INC_BASE=P:\PRESTO2\SHINC
SET SHC_LIB=%SH_COMPILER_BASE%\bin
SET SHC_INC=%SH_COMPILER_BASE%\include

if exist *.obj del *.obj
if exist *.lib del *.lib
make -f PRCSH2B.MK > tag
if exist *.obj del *.obj
if exist *.lib del *.lib
make -f PRCSH2B_.MK >> tag
if exist *.obj del *.obj
if exist *.lib del *.lib
make -f PRCSH3B.MK >> tag
if exist *.obj del *.obj
if exist *.lib del *.lib
make -f PRCSH3B_.MK >> tag
