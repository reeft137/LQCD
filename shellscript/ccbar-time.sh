#!/bin/bash
ulimit -n 1024

DATA_DIR=$1
DATA_DIR_BASE=$(basename $DATA_DIR)
LQCD_BASE_DIR=/home/puppy/LQCD
TIMESITES=64

cd $DATA_DIR
echo "Working Directory: "
echo "$DATA_DIR"
echo " "

rm -rf $DATA_DIR_BASE.result
rm -rf resample_tmp
mkdir $DATA_DIR_BASE.result

# Step 0: Sort
# mkdir s v t av ps; mv *.s.* s; mv *.v.* v; mv *.t.* t; mv *.av.* av; mv *.ps.* ps
# mkdir raw; mv s v t av ps raw

# Step 1: Calculate correlators and effective masses
for type in $(ls raw); do
  mkdir -p resample_tmp/$type

  echo "Processing with \"$DATA_DIR_BASE/raw/$type\""
  $LQCD_BASE_DIR/bin/time -timelength $TIMESITES -corr $DATA_DIR_BASE.result/$type raw/$type/2pt.*

  mv raw/$type/js.* resample_tmp/$type
done
# mkdir -p resample_tmp/

# echo "Processing with \"$DATA_DIR_BASE/raw/av\""
# $LQCD_BASE_DIR/bin/time -timelength $TIMESITES -corr $DATA_DIR_BASE.result/av raw/av/2pt.*

# mv raw/av/js.* resample_tmp/av
# rm raw/av/exp.*

# # Step 2: Fit the mass
# # (1) Generate V_0 and V_s of different configurations
# for time in $(ls laplacian-tmp/v); do
#   mkdir -p pot-tmp/v0/$time pot-tmp/vs/$time
#   for idata in $(ls laplacian-tmp/v/$time); do
#     V_DATA=$idata
#     PS_DATA=${idata/.v./.ps.}
#     V0_DATA=${idata/lap.TR.4pt.v./v0.}
#     VS_DATA=${idata/lap.TR.4pt.v./vs.}

#     $LQCD_BASE_DIR/bin/v_0 -spacelength $TIMESITES -ofname pot-tmp/v0/$time/$V0_DATA laplacian-tmp/v/$time/$V_DATA laplacian-tmp/ps/$time/$PS_DATA
#     $LQCD_BASE_DIR/bin/v_s -spacelength $TIMESITES -ofname pot-tmp/vs/$time/$VS_DATA laplacian-tmp/v/$time/$V_DATA laplacian-tmp/ps/$time/$PS_DATA
#   done
# done

# rm -rf resample_tmp
