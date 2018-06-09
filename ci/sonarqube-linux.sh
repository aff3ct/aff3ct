#!/bin/bash
set -x

## gcc
#mkdir build_sonar
#cd build_sonar
#cmake .. -G"Unix Makefiles" -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Release -DENABLE_GSL=ON -DCMAKE_CXX_FLAGS="-Wall -funroll-loops -march=native -DMULTI_PREC"
#THREADS=$(grep -c ^processor /proc/cpuinfo)
#build-wrapper-linux-x86-64 --out-dir bw-output make -j $THREADS clean all
#cd ..

# Create the sonar config file on the fly
BRANCH=$(git branch --remote --verbose --no-abbrev --contains | sed -rne 's/^[^\/]*\/([^\ ]+).*$/\1/p')
VERSION=$(git tag | tail -n 1 | cut -d $'v' -f2-)
echo "sonar.projectKey=storm:aff3ct:gitlab:$BRANCH"                  >  sonar-project.properties
#echo "sonar.projectName=AFF3CT"                                      >> sonar-project.properties
echo "sonar.projectVersion=$VERSION"                                 >> sonar-project.properties
echo "sonar.login=c6ab3ccd4dd7a8d9aa3e90d5766c74e0c9889947"          >> sonar-project.properties
echo "sonar.sources=./src/"                                          >> sonar-project.properties
echo "sonar.exclusions=**/date.h, **/dirent.h"                       >> sonar-project.properties
echo "sonar.links.homepage=https://aff3ct.github.io/"                >> sonar-project.properties
echo "sonar.links.scm=https://github.com/aff3ct/aff3ct"              >> sonar-project.properties
echo "sonar.links.ci=https://gitlab.inria.fr/fec/aff3ct/pipelines"   >> sonar-project.properties
echo "sonar.links.issue=https://github.com/aff3ct/aff3ct/issues"     >> sonar-project.properties
echo "sonar.language=c++"                                            >> sonar-project.properties
echo "sonar.cxx.cppcheck.reportPath=cppcheck/cppcheck.xml"           >> sonar-project.properties
echo "sonar.cxx.coverage.reportPath=code_coverage_report/aff3ct.xml" >> sonar-project.properties
#echo "sonar.cfamily.build-wrapper-output=./build_sonar/bw-output"    >> sonar-project.properties

sonar-scanner
