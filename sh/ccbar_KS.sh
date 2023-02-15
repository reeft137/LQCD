#!/bin/bash
DATA_DIR=$1
LQCD_BASE_DIR=/home/puppy/LQCD
DATA_BASE=/home/puppy/LQCD/data
SPACESITES=32

cd $DATA_DIR
rm -rf 4ptresult
mkdir 4ptresult

for type in $(ls); do
  for i in $(ls); do
    echo "Processing with \"$type/$i\"..."
    $LQCD_BASE_DIR/bin/corr4+lap -spacelength $SPACESITES -corr $i.4ptcorr.$type $i/TR.*

    mv $i/$i.4ptcorr.$TYPE ./
  done
done

mkdir corr_4pt_result
mv *.4ptcorr.* corr_4pt_result
