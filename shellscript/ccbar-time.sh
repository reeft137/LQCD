#!/bin/bash
ulimit -n 1024

DATA_DIR=$1
LQCD_BASE_DIR=/home/puppy/LQCD
SPACESITES=32

cd $DATA_DIR

# Step 0: Sort
for i in {00..31}; do
  mkdir -p raw/$i
  mv raw/*.+$i.* raw/$i
done

# Step 1: Calculate correlators and laplacianed data
for type in $(ls raw); do
  for time in $(ls raw/$type); do
    mkdir -p space-corr-result laplacian-tmp/$type/$time

    echo "Processing with \"$DATA_DIR/raw/$type/$time\""
    $LQCD_BASE_DIR/bin/corr-space -spacelength $SPACESITES -corr space-corr-result/$type.$time raw/$type/$time/TR.*

    mv raw/$type/$time/lap.* laplacian-tmp/$type/$time
  done
done

# Step 2: Calculate V_0 and V_s 
# (1) Generate V_0 and V_s of different configurations
for time in $(ls laplacian-tmp/v); do
  mkdir -p pot-tmp/v0/$time pot-tmp/vs/$time
  for idata in $(ls laplacian-tmp/v/$time); do
    V_DATA=$idata
    PS_DATA=${idata/.v./.ps.}
    V0_DATA=${idata/lap.TR.4pt.v./v0.}
    VS_DATA=${idata/lap.TR.4pt.v./vs.}

    $LQCD_BASE_DIR/bin/v_0 -spacelength $SPACESITES -ofname pot-tmp/v0/$time/$V0_DATA laplacian-tmp/v/$time/$V_DATA laplacian-tmp/ps/$time/$PS_DATA
    $LQCD_BASE_DIR/bin/v_s -spacelength $SPACESITES -ofname pot-tmp/vs/$time/$VS_DATA laplacian-tmp/v/$time/$V_DATA laplacian-tmp/ps/$time/$PS_DATA
  done
done
# (2) Jackknife average
for type in $(ls pot-tmp); do
  for time in $(ls pot-tmp/$type); do
    echo "Processing with \"$DATA_DIR/pot-tmp/$type/$time\""

    $LQCD_BASE_DIR/bin/corr-space -spacelength $SPACESITES -pot space-corr-result/$type.$time pot-tmp/$type/$time/$type.*
  done
done