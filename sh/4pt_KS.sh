#!/bin/bash
DATA_DIR=$1
LQCD_BASE_DIR=/home/puppy/LQCD
SPACESITES=32
TYPE=ps

cd $DATA_DIR
rm -rf correlator_result

for i in $(ls); do
  echo "Processing with \"$i\"..."

  $LQCD_BASE_DIR/bin/ccbarmain -maxline $SPACESITES -corr_4pt ./$i.4ptcorr.$TYPE $i/TR.*
  
  mv $i/$i.4ptcorr.$TYPE ./
done

mkdir correlator_result
mv *.4ptcorr.* correlator_result

echo "*******-------*******-------*******-------*******-------*******"
echo "Finished! "
echo "*******-------*******-------*******-------*******-------*******"
echo " "
