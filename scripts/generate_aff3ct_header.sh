#!/bin/bash

SRC_DIR=".\/include"
if [ -z "$1" ]; then
  AFF3CT_HEADER=.\/include\/aff3ct.hpp
else
  AFF3CT_HEADER="$1"
fi

# create the aff3ct header file with the C++ header
echo "/*!"                                                                      >  $AFF3CT_HEADER
echo " * \\file"                                                                >> $AFF3CT_HEADER
echo " * \\brief AFF3CT main header file, include all the other headers."       >> $AFF3CT_HEADER
echo " *"                                                                       >> $AFF3CT_HEADER
echo " * \\section LICENSE"                                                     >> $AFF3CT_HEADER
echo " * This file is under MIT license (https://opensource.org/licenses/MIT)." >> $AFF3CT_HEADER
echo " */"                                                                      >> $AFF3CT_HEADER
echo ""                                                                         >> $AFF3CT_HEADER
echo "#ifndef AFF3CT_HPP"                                                       >> $AFF3CT_HEADER
echo "#define AFF3CT_HPP"                                                       >> $AFF3CT_HEADER
echo ""                                                                         >> $AFF3CT_HEADER

headersh=$(find .\/include\/ -type f -follow -print | # find all source files
           grep "[.]h$" |                             # take all .h files
           sed 's/[[:blank:]]\+/\n/g')                # change end lines with '\n'

headershpp=$(find .\/include\/ -type f -follow -print | # find all source files
             grep "[.]hpp$" |                           # take all .hpp files
             grep -v aff3ct.hpp |                       # remove aff3ct.hpp from the file list
             sed 's/[[:blank:]]\+/\n/g')                # change end lines with '\n'

headers="$headersh $headershpp"
# sort the header by alphabetical order
headers=$(echo $headers | tr " " "\n" | sort -f -d | tr "\n" " " ; echo)

for f in $headers; do
  guard=$(grep -m1 "#ifndef" < $f)
  if [ "$guard" != "#ifndef DOXYGEN_SHOULD_SKIP_THIS" ]; then
    echo $guard >> $AFF3CT_HEADER # add a guard to the header file to accelerate compilation
    rc=$?; if [[ $rc != 0 ]]; then echo "No #ifndef in file '$f'."; exit 1; fi

    echo $f | sed "s/$SRC_DIR\//#include </" | # change "./src" with "#include <"
    sed 's|$|>|g' >> $AFF3CT_HEADER # add at the end of each line the ">" to close the 'include'
    echo "#endif" >> $AFF3CT_HEADER # close the guard
  fi
done

echo -ne "\n#endif" >> $AFF3CT_HEADER # add some empty lines in the aff3ct header file and close the 'define' header