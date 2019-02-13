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

# Create the sonar config file on the fly
GIT_SHORT_TAG=$(echo $GIT_TAG | cut -d $'v' -f2-)
echo "sonar.projectKey=storm:aff3ct:gitlab:$GIT_BRANCH"                  >  sonar-project.properties
#echo "sonar.projectName=AFF3CT"                                          >> sonar-project.properties
echo "sonar.projectVersion=$GIT_SHORT_TAG"                               >> sonar-project.properties
echo "sonar.login=$SONARQUBE_TOKEN"                                      >> sonar-project.properties
echo "sonar.sources=./src/"                                              >> sonar-project.properties
echo "sonar.exclusions=**/date.h, **/dirent.h"                           >> sonar-project.properties
echo "sonar.links.homepage=https://aff3ct.github.io/"                    >> sonar-project.properties
echo "sonar.links.scm=https://github.com/aff3ct/aff3ct/tree/$GIT_BRANCH" >> sonar-project.properties
echo "sonar.links.ci=https://gitlab.com/aff3ct/aff3ct/pipelines"         >> sonar-project.properties
echo "sonar.links.issue=https://github.com/aff3ct/aff3ct/issues"         >> sonar-project.properties
echo "sonar.language=c++"                                                >> sonar-project.properties
echo "sonar.cxx.cppcheck.reportPath=cppcheck/cppcheck.xml"               >> sonar-project.properties
echo "sonar.cxx.coverage.reportPath=code_coverage_report/aff3ct.xml"     >> sonar-project.properties

sonar-scanner