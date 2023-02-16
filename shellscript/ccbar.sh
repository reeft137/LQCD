#!/bin/bash
ulimit -n 1024

DATA_DIR=$1
LQCD_BASE_DIR=/home/puppy/LQCD
SPACESITES=32

cd $DATA_DIR
# rm -rf space-corr-result laplacian-tmp ks-result

# for type in $(ls); do
#   for time in $(ls $type); do
#     echo "Processing with \"$DATA_DIR/$type/$time\""

#     mkdir -p space-corr-result laplacian-tmp/$type/$time
#     $LQCD_BASE_DIR/bin/corr-space -spacelength $SPACESITES -corr $type.$time $type/$time/TR.*

#     mv $type.$time space-corr-result
#     mv $type/$time/lap.* laplacian-tmp/$type/$time
#   done
# done

# mkdir potential
cd laplacian-tmp
rm -rf v0tmp vstmp

for time in $(ls v); do
  mkdir -p v0tmp/$time vstmp/$time
  for idata in $(ls v/$time); do
    VDATA=$idata
    PSDATA=${idata/.v./.ps.}
    V0DATA=${idata/lap.TR.4pt.v./v0.}
    VSDATA=${idata/lap.TR.4pt.v./vs.}
    # echo $VDATA $PSDATA $V0DATA $VSDATA

    $LQCD_BASE_DIR/bin/v_0 -spacelength $SPACESITES -ofname v0tmp/$time/$V0DATA v/$time/$VDATA ps/$time/$PSDATA
    $LQCD_BASE_DIR/bin/v_s -spacelength $SPACESITES -ofname vstmp/$time/$VSDATA v/$time/$VDATA ps/$time/$PSDATA
  done
done
