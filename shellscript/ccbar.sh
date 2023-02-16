#!/bin/bash
ulimit -n 1024

DATA_DIR=$1
LQCD_BASE_DIR=/home/puppy/LQCD
SPACESITES=32

cd $DATA_DIR
rm -rf space-corr-result laplacian-tmp ks-result

for type in $(ls); do
  for time in $(ls $type); do
    echo "Processing with \"$DATA_DIR/$type/$time\""

    mkdir -p space-corr-result laplacian-tmp/$type/$time
    $LQCD_BASE_DIR/bin/corr-space -spacelength $SPACESITES -corr $type.$time $type/$time/TR.*

    mv $type.$time space-corr-result
    mv $type/$time/lap.* laplacian-tmp/$type/$time
  done
done


