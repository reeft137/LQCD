#!/bin/bash
LQCD_BASE_DIR=/home/puppy/LQCD
TSITES=$1
DATA_DIR=$2

cd $DATA_DIR

for i in $(ls); do
    echo "Processing with \"$i\"..."
    $LQCD_BASE_DIR/bin/bin2txt $TSITES $i
done

rm -rf ./2pt.*

for file_old in $(ls | grep txt.); do
    file_new=$(echo "$file_old" | sed 's/txt.//g')
    mv $file_old $file_new
done
