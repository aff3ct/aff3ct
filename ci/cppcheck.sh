#!/bin/bash

find .\/src\/ -type f -follow -print | grep "[.]h$\|[.]hpp$\|[.]hxx$\|[.]cpp$" > src_files.txt
cppcheck --force --enable=all --std=c++11 --file-list=src_files.txt 2> cppcheck.log
cat cppcheck.log | grep "(error)" > cppcheck_error.log
cat cppcheck.log | grep "(warning)" > cppcheck_warning.log
cat cppcheck.log | grep "(performance)" > cppcheck_performance.log
cat cppcheck.log | grep "(style)" > cppcheck_style.log
cat cppcheck.log | grep "(portability)" > cppcheck_portability.log
cat cppcheck.log | grep "(information)" > cppcheck_information.log
cat cppcheck.log | grep "(unusedFunction)" > cppcheck_unusedFunction.log
cat cppcheck.log | grep "(missingInclude)" > cppcheck_missingInclude.log

#COUNT=$(wc -l < cppcheck_error.log )
#
#if [ $COUNT -gt 0 ]; then
#  echo "Error count is $COUNT! cppcheck run failed :-(.";
#  echo ""
#  echo "Errors list:"
#  cat cppcheck_error.log
#  exit 1;
#else
#  echo "There is no error :-)."
#fi

exit 0;