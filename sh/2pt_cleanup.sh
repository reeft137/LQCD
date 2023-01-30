#!/bin/bash
DATA_DIR=$1

cd $DATA_DIR

# cleanup all
echo "*******-------*******-------*******-------*******-------*******"
echo "Deleting jackknife sampled data"
echo "*******-------*******-------*******-------*******-------*******"
echo " "

rm -rf result

for i in $(ls); do
  echo "Processing with \"$i\"..."
  cd $i
  rm -f sample.*
  rm -f txt.*
  rm -f eff_m_exp.*
  echo " "
  cd ..
done

echo "*******-------*******-------*******-------*******-------*******"
echo "Finished! "
echo "*******-------*******-------*******-------*******-------*******"
echo " "
