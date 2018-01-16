#!/bin/bash

SRC_DIR=".\/src"
AFF3CT_HEADER=.\/src\/aff3ct.hpp

echo -e "#ifndef AFF3CT_HPP\n#define AFF3CT_HPP\n" > $AFF3CT_HEADER # create the aff3ct header file with the c++ header

find .\/src\/ -type f -follow -print |  # find all source files
	grep "[.]h$" |                      # take all .h files
	grep -v date.h |                    # remove date.h from the file list
	grep -v dirent.h |                  # remove dirent.h from the file list
    sed 's/[[:blank:]]\+/\n/g' |        # change end lines with '\n'
    sed "s/$SRC_DIR\//#include </" |    # change "./src" with "#include <"
    sed 's|$|>|g' >> $AFF3CT_HEADER     # add at the end of each line the ">" to close the 'include'
                                        # and add the result in the aff3ct header file

echo -ne "\n" >> $AFF3CT_HEADER        # add some empty lines in the aff3ct header file

find .\/src\/ -type f -follow -print |  # find all source files
    grep "[.]hpp$" |                    # take all .hpp files
	grep -v aff3ct.hpp |                # remove aff3ct.hpp from the file list
    sed 's/[[:blank:]]\+/\n/g' |        # change end lines with '\n'
    sed "s/$SRC_DIR\//#include </" |    # change "./src" with "#include <"
    sed 's|$|>|g' >> $AFF3CT_HEADER     # add at the end of each line the ">" to close the 'include'
                                        # and add the result in the aff3ct header file

echo -ne "\n#endif" >> $AFF3CT_HEADER  # add some empty lines in the aff3ct header file and close the 'define' header