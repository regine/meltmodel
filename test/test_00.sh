#!/bin/bash

MELT_PATH=$1
MELT_SOURCE=$2

# Test name:
echo "Test # 0: presence of tools and Python modules needed by other tests."
# The list of files to delete when done.
files="foo.txt"

rm -f $files

tools="diff rm cp"
modules="json sys"
success=1

set -e

for tool in $tools;
do
    if [ -z `which $tool` ];
    then
        echo "ERROR: $tool is not on the path"
        success=0
    fi
done

for module in $modules;
do
    echo "import $module" | /usr/bin/env python &> foo.txt
    if [ -s foo.txt ];
    then
        echo "ERROR: Python module \"$module\" is not installed"
        success=0
    fi
done

/usr/bin/env python $MELT_PATH/json_to_degree.py $MELT_SOURCE/util/json_input/test/json_test_in.json -o=foo.txt -s
diff $MELT_SOURCE/util/json_input/test/json_test_out.txt foo.txt
if [ $? != 0 ];
then
    echo "util/json_to_degree.py is broken: output is different than expected"
    success=0
fi

rm -f $files; 
set +e

if [ $success == 0 ];
then
    exit 1
fi

exit 0
