#!/bin/bash
set -x

if [ -z "$THREADS" ]
then
	echo "The 'THREADS' environment variable is not set, default value = 1."
	THREADS=1
fi

cd doc/
pip3 install -r requirements.txt
rc=$?; if [[ $rc != 0 ]]; then exit $rc; fi

# export SPHINX_BUILDERNAME="latex"
# make clean
# sphinx-build -M latexpdf source build -j $THREADS
# rc=$?; if [[ $rc != 0 ]]; then exit $rc; fi

export SPHINX_BUILDERNAME="html"
make clean
sphinx-build -M html source build -j $THREADS
rc=$?; if [[ $rc != 0 ]]; then exit $rc; fi
