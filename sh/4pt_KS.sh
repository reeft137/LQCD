#!/bin/bash
DATA_DIR=$1
LQCD_BASE_DIR=/home/puppy/LQCD
SPACESITES=32
TYPE=ps

cd $DATA_DIR
rm -rf corr_4pt_result

for i in $(ls); do
  echo "Processing with \"$i\"..."

  # $LQCD_BASE_DIR/bin/space_corr -spacelength $SPACESITES -corr ./$i.4ptcorr.$TYPE $i/TR.*
  $LQCD_BASE_DIR/bin/space_corr -spacelength $SPACESITES -corr ./$i.4ptcorr.$TYPE $i/4pt.*
  
  mv $i/$i.4ptcorr.$TYPE ./
done

mkdir corr_4pt_result
mv *.4ptcorr.* corr_4pt_result

echo "*******-------*******-------*******-------*******-------*******"
echo "Finished! "
echo "*******-------*******-------*******-------*******-------*******"
echo " "
