#!/bin/bash
rootdir='../..'
cmd=$(clang++ -### -c -I${rootdir}/include -I${rootdir}/src -I${rootdir}/lib/MIPP/src -I${rootdir}/lib/cli/src -I${rootdir}/lib/rang/include -I${rootdir}/lib/date/include/date -std=gnu++11 ${rootdir}/include/aff3ct.hpp 2>&1 |tail -1|sed 's/^ //;s/emit-pch/ast-print/;s/"-o" "[^"]*"/"-o" "aff3ct_ast.txt"/');
eval "$cmd"
