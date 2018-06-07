#!/bin/bash
set -x

mkdir code_coverage_report || true
genhtml code_coverage_files/aff3ct_clean.info --output-directory ./code_coverage_files/
rc=$?; if [[ $rc != 0 ]]; then exit $rc; fi

# for sonarqube
lcov_cobertura.py code_coverage_files/aff3ct_clean.info --output code_coverage_files/aff3ct_clean.xml
rc=$?; if [[ $rc != 0 ]]; then exit $rc; fi