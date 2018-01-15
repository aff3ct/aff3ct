#!/bin/bash
set -x

cppcheck --version

#cppcheck --suppress=missingIncludeSystem -I./src/ --force --enable=all --std=c++11 -U_MSC_VER ./src/ 2> cppcheck_all.log
find .\/src\/ -type f -follow -print | grep "[.]h$\|[.]hpp$\|[.]hxx$\|[.]cpp$" > src_files.txt
cppcheck --language=c++ --suppress=missingIncludeSystem --force --enable=all --std=c++11 -U_MSC_VER --file-list=src_files.txt 2> cppcheck_all.log
cat cppcheck_all.log | grep "(error)"          > cppcheck_error.log
cat cppcheck_all.log | grep "(warning)"        > cppcheck_warning.log
cat cppcheck_all.log | grep "(performance)"    > cppcheck_performance.log
cat cppcheck_all.log | grep "(style)"          > cppcheck_style.log
cat cppcheck_all.log | grep "(portability)"    > cppcheck_portability.log
cat cppcheck_all.log | grep "(information)"    > cppcheck_information.log
cat cppcheck_all.log | grep "(unusedFunction)" > cppcheck_unusedFunction.log
cat cppcheck_all.log | grep "(missingInclude)" > cppcheck_missingInclude.log

# Uncomment this block if you want a failure when there is errors
COUNT=$(wc -l < cppcheck_error.log )

if [ $COUNT -gt 5 ]; then
	echo "Error count is $COUNT! cppcheck run failed :-(.";
	echo ""
	echo "Errors list:"
	cat cppcheck_error.log
	exit 1;
else
	echo "There is no error :-)."
fi

exit 0;
