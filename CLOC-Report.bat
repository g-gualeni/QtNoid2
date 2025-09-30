
echo CLOC Report - Generated on %date% %time% > CLOC-Report.txt

echo. >> CLOC-Report.txt
echo. >> CLOC-Report.txt
echo QtNoidCore >> CLOC-Report.txt
..\cloc-2.06.exe QtNoidCore --md >> CLOC-Report.txt

echo. >> CLOC-Report.txt
echo. >> CLOC-Report.txt
echo QtNoidCommon >> CLOC-Report.txt
..\cloc-2.06.exe QtNoidCommon --md >> CLOC-Report.txt

echo. >> CLOC-Report.txt
echo. >> CLOC-Report.txt
echo QtNoidApp >> CLOC-Report.txt
..\cloc-2.06.exe QtNoidApp --md >> CLOC-Report.txt

echo. >> CLOC-Report.txt
echo. >> CLOC-Report.txt
echo Examples >> CLOC-Report.txt
..\cloc-2.06.exe Examples --md >> CLOC-Report.txt
