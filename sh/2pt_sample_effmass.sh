#!/bin/bash
LQCD_BASE_DIR=/home/puppy/LQCD
TSITES=$1
DATA_DIR=$2

cd $DATA_DIR

# jackknife sampling and calculating the correlation function
echo "*******-------*******-------*******-------*******-------*******"
echo "Jackknife sampling the data! "
echo "*******-------*******-------*******-------*******-------*******"
echo " "

for i in $(ls); do
  echo "Processing with \"$i\"..."
  cd $i
  $LQCD_BASE_DIR/bin/jackknife_sampling -2pt $TSITES 2pt.*
  $LQCD_BASE_DIR/bin/jackknife_average -2pt $TSITES corr_2pt.$i sample.*
  cd ..
  echo " "
done

echo "*******-------*******-------*******-------*******-------*******"
echo "Finished! "
echo "*******-------*******-------*******-------*******-------*******"
echo " "

# calculate effmass
echo "*******-------*******-------*******-------*******-------*******"
echo "Calculating the effective mass! "
echo "*******-------*******-------*******-------*******-------*******"
echo " "

for i in $(ls); do
  echo "Processing with \"$i\"..."
  cd $i
  for j in $(ls sample.*); do
    $LQCD_BASE_DIR/bin/eff_m_exp $TSITES $j
  done
  $LQCD_BASE_DIR/bin/jackknife_average -2pt $TSITES eff_m_exp.$i eff_m_exp.*
  cd ..
  echo " "
done

# move the final data
echo "*******-------*******-------*******-------*******-------*******"
echo "Move the final data to the top! "
echo "*******-------*******-------*******-------*******-------*******"
echo " "

for i in $(ls); do
  mv $i/eff_m_exp.$i ./
  mv $i/corr_2pt.$i ./
done

mkdir result
mv eff_m_exp.* result
mv corr_2pt.* result

echo "*******-------*******-------*******-------*******-------*******"
echo "Finished! "
echo "*******-------*******-------*******-------*******-------*******"
echo " "
