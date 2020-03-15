#!/bin/bash
set -x

cppcheck --version

mkdir cppcheck
#cppcheck --suppress=missingIncludeSystem -I./src/ --force --enable=all --std=c++11 -U_MSC_VER ./src/ 2> cppcheck_all.log
find .\/include\/ -type f -follow -print | grep "[.]h$\|[.]hpp$\|[.]hxx$\|[.]cpp$" >  src_files.txt
find .\/src\/     -type f -follow -print | grep "[.]h$\|[.]hpp$\|[.]hxx$\|[.]cpp$" >> src_files.txt
# cppcheck for sonarqube
cppcheck --language=c++ --suppress=missingIncludeSystem --force --enable=all --std=c++11 -U_MSC_VER --file-list=src_files.txt --xml --xml-version=2 2> cppcheck/cppcheck.xml
cppcheck --language=c++ --suppress=missingIncludeSystem --force --enable=all --std=c++11 -U_MSC_VER --file-list=src_files.txt 2> cppcheck/cppcheck_all.log
cat cppcheck/cppcheck_all.log | grep "(error)"          > cppcheck/cppcheck_error.log
cat cppcheck/cppcheck_all.log | grep "(warning)"        > cppcheck/cppcheck_warning.log
cat cppcheck/cppcheck_all.log | grep "(performance)"    > cppcheck/cppcheck_performance.log
cat cppcheck/cppcheck_all.log | grep "(style)"          > cppcheck/cppcheck_style.log
cat cppcheck/cppcheck_all.log | grep "(portability)"    > cppcheck/cppcheck_portability.log
cat cppcheck/cppcheck_all.log | grep "(information)"    > cppcheck/cppcheck_information.log
cat cppcheck/cppcheck_all.log | grep "(unusedFunction)" > cppcheck/cppcheck_unusedFunction.log
cat cppcheck/cppcheck_all.log | grep "(missingInclude)" > cppcheck/cppcheck_missingInclude.log

COUNT=$(wc -l < cppcheck/cppcheck_error.log )

if [ $COUNT -gt 1 ]; then
	echo "Error count is $COUNT! cppcheck run failed :-(.";
	echo ""
	echo "Errors list:"
	cat cppcheck/cppcheck_error.log
	exit 1;
elif [ $COUNT -gt 0 ]; then
	echo "There is one false positive error.";
	echo ""
	echo "Errors list:"
	cat cppcheck/cppcheck_error.log
else
	echo "There is no error :-)."
fi

exit 0;
