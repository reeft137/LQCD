#!/bin/bash
ulimit -n 1024

DATA_DIR=$1
DATA_DIR_BASE=$(basename $DATA_DIR)
LQCD_BASE_DIR=/home/puppy/LQCD
SPACESITES=32

cd $DATA_DIR

rm -rf $DATA_DIR_BASE.result
mkdir $DATA_DIR_BASE.result

# # Step 0: Sort
# for type in $(ls raw); do
#   for i in {00..31}; do
#     mkdir -p raw/$type/$i
#     mv raw/$type/*.+$i.* raw/$type/$i
#   done
# done

# Step 1: Calculate correlators and pre-potential
# (1) Correlators and move laplacianed data to laplacian-tmp
for type in $(ls raw); do
  for time in $(ls raw/$type); do
    mkdir -p laplacian-tmp/$type/$time

    echo "Processing with \"$DATA_DIR_BASE/raw/$type/$time\""
    $LQCD_BASE_DIR/bin/space -spacelength $SPACESITES -corr $DATA_DIR_BASE.result/$type.$time raw/$type/$time/TR.*

    mv raw/$type/$time/lap.* laplacian-tmp/$type/$time
  done
done
# # (2) Jackknife average on the laplacianed data (pre-potential)
# for type in $(ls laplacian-tmp); do
#   for time in $(ls laplacian-tmp/$type); do
#     echo "Processing with \"$DATA_DIR_BASE/laplacian-tmp/$type/$time\""

#     $LQCD_BASE_DIR/bin/space -spacelength $SPACESITES -sphout $DATA_DIR_BASE.result/pre.$type.$time laplacian-tmp/$type/$time/lap.*
#   done
# done

# Step 2: Calculate V_0 and V_s
# (1) Generate V_0 and V_s of different configurations
for time in $(ls laplacian-tmp/v); do
  mkdir -p pot-tmp/v0/$time pot-tmp/vs/$time
  for idata in $(ls laplacian-tmp/v/$time); do
    V_DATA=$idata
    PS_DATA=${idata/.v./.ps.}
    V0_DATA=${idata/lap.TR.4pt.v./v0.}
    VS_DATA=${idata/lap.TR.4pt.v./vs.}

    $LQCD_BASE_DIR/bin/KSpot -v0 -spacelength $SPACESITES -ofname pot-tmp/v0/$time/$V0_DATA laplacian-tmp/v/$time/$V_DATA laplacian-tmp/ps/$time/$PS_DATA
    $LQCD_BASE_DIR/bin/KSpot -vs -spacelength $SPACESITES -ofname pot-tmp/vs/$time/$VS_DATA laplacian-tmp/v/$time/$V_DATA laplacian-tmp/ps/$time/$PS_DATA
  done
done
# (2) Jackknife average
for type in $(ls pot-tmp); do
  for time in $(ls pot-tmp/$type); do
    echo "Processing with \"$DATA_DIR_BASE/pot-tmp/$type/$time\""

    $LQCD_BASE_DIR/bin/space -spacelength $SPACESITES -sphout $DATA_DIR_BASE.result/$type.$time pot-tmp/$type/$time/$type.*
  done
done

rm -rf pot-tmp laplacian-tmp