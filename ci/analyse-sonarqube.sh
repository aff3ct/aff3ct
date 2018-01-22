#!/bin/bash
set -x

VERSION=$(git tag | tail -n 1 | cut -d $'v' -f2-)

find .\/src\/ -type f -follow -print | grep "[.]h$\|[.]hpp$\|[.]hxx$\|[.]cpp$" > src_files.txt
sed -i -e 's/.\/src\/Tools\/date.h//g' src_files.txt
sed -i -e 's/.\/src\/Tools\/MSVC\/dirent.h//g' src_files.txt
cppcheck --language=c++ --suppress=missingIncludeSystem --force --enable=all --std=c++11 -U_MSC_VER --file-list=src_files.txt --xml --xml-version=2 2> cppcheck.xml

# Create the sonar config file on the fly
echo "sonar.projectKey=STORM:AFF3CT"                                  >  sonar-project.properties
echo "sonar.projectName=AFF3CT"                                       >> sonar-project.properties
echo "sonar.projectVersion=$VERSION"                                  >> sonar-project.properties
echo "sonar.login=b20709e3e9263173baa66e62d8c65a9e56b04aea"           >> sonar-project.properties
echo "sonar.sources=./src/"                                           >> sonar-project.properties
echo "sonar.exclusions=./src/Tools/date.h, ./src/Tools/MSVC/dirent.h" >> sonar-project.properties
echo "sonar.links.homepage=https://aff3ct.github.io/"                 >> sonar-project.properties
echo "sonar.links.scm=https://github.com/aff3ct/aff3ct"               >> sonar-project.properties
echo "sonar.links.ci=https://gitlab.inria.fr/fec/aff3ct/pipelines"    >> sonar-project.properties
echo "sonar.links.issue=https://github.com/aff3ct/aff3ct/issues"      >> sonar-project.properties
echo "sonar.language=c++"                                             >> sonar-project.properties
echo "sonar.cxx.cppcheck.reportPath=cppcheck.xml"                     >> sonar-project.properties

sonar-scanner