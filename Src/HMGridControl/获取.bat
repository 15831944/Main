@echo off
echo ��ǰ�ļ��е������ļ��У�
set file=�ļ���.txt
set show=��������������

echo %show%
dir *.h /s /b /o:e >%file%

echo "="ren "& """" &A1 &"""" & " "& """"&B1 &"""" ">>%file%
type %file%

echo %show%

pause
