#!/bin/bash
set -x

if [[ ( -z "$CI_AFF3CT_DEPLOY" ) || ( "$CI_AFF3CT_DEPLOY" != "ON" ) ]]
then
	echo "This job is disabled, try to set the CI_AFF3CT_DEPLOY environment variable to 'ON' to enable it."
	exit 1
fi

if [ -z "$SONARQUBE_TOKEN" ]
then
	echo "The SONARQUBE_TOKEN environment variable is not defined!"
	exit 1
fi

if [ -z "$GIT_TAG" ]
then
	echo "The GIT_TAG environment variable is not defined!"
	exit 1
fi

if [ -z "$GIT_BRANCH" ]
then
	echo "The GIT_BRANCH environment variable is not defined!"
	exit 1
fi

if [ -z "$REPORTS_DIR" ]
then
	echo "The 'REPORTS_DIR' environment variable is not set, default value = 'clang_reports'."
	REPORTS_DIR="clang_reports"
fi

sed -i "s:{{ROOT_DIR}}:$PWD:g" $REPORTS_DIR/aff3ct-build.log
sed -i "s:{{ROOT_DIR}}:$PWD:g" $REPORTS_DIR/clang-tidy-report.txt
sed -i "s:{{ROOT_DIR}}:$PWD:g" $REPORTS_DIR/scan-build-*/*.plist

# list of standard C++ includes in the system
STD_CXX_INC=$(echo | gcc -xc++ -E -Wp,-v - 2>&1 | grep "^ " | sed -e :a -e '$!N; s/\n/,/; ta')
PROJECT_CXX_INC="./src/, ./include/, ./lib/date/include/date, ./lib/MIPP/src/, ./lib/MSVC/include/, ./lib/rang/include/, ./lib/cli/src/"
GCC_REGEX='(?<file>.*):(?<line>[0-9]+):[0-9]+:\\x20warning:\\x20(?<message>.*)\\x20\\[(?<id>.*)\\]'

# Create the sonar config file on the fly
GIT_SHORT_TAG=$(echo $GIT_TAG | cut -d $'v' -f2-)
echo "sonar.projectKey=storm:aff3ct:gitlab:$GIT_BRANCH"                  >  sonar-project.properties
echo "sonar.projectVersion=$GIT_SHORT_TAG"                               >> sonar-project.properties
echo "sonar.host.url=$SONARQUBE_HOST"                                    >> sonar-project.properties
echo "sonar.scm.disabled=false"                                          >> sonar-project.properties
echo "sonar.scm.provider=git"                                            >> sonar-project.properties
echo "sonar.login=$SONARQUBE_TOKEN"                                      >> sonar-project.properties
echo "sonar.sources=./src/,./include/"                                   >> sonar-project.properties
echo "sonar.links.homepage=https://aff3ct.github.io/"                    >> sonar-project.properties
echo "sonar.links.scm=https://github.com/aff3ct/aff3ct/tree/$GIT_BRANCH" >> sonar-project.properties
echo "sonar.links.ci=https://gitlab.com/aff3ct/aff3ct/pipelines"         >> sonar-project.properties
echo "sonar.links.issue=https://github.com/aff3ct/aff3ct/issues"         >> sonar-project.properties
echo "sonar.language=c++"                                                >> sonar-project.properties
echo "sonar.cxx.includeDirectories=$STD_CXX_INC, $PROJECT_CXX_INC"       >> sonar-project.properties
echo "sonar.cxx.errorRecoveryEnabled=true"                               >> sonar-project.properties
echo "sonar.cxx.gcc.reportPath=$REPORTS_DIR/aff3ct-build.log"            >> sonar-project.properties
echo "sonar.cxx.gcc.charset=UTF-8"                                       >> sonar-project.properties
echo "sonar.cxx.gcc.regex=$GCC_REGEX"                                    >> sonar-project.properties
echo "sonar.cxx.clangsa.reportPath=$REPORTS_DIR/*/*.plist"               >> sonar-project.properties
echo "sonar.cxx.clangtidy.reportPath=$REPORTS_DIR/clang-tidy-report.txt" >> sonar-project.properties
echo "sonar.cxx.cppcheck.reportPath=cppcheck/cppcheck.xml"               >> sonar-project.properties
echo "sonar.cxx.coverage.reportPath=code_coverage_report/aff3ct.xml"     >> sonar-project.properties

sonar-scanner