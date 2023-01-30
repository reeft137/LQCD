#!/bin/bash
LQCD_BASE_DIR=/home/puppy/LQCD
TSITES=$1
DATA_DIR=$2
FILE_NAME=$3

cd $DATA_DIR

# Jackknife Sampling
echo "*******-------*******-------*******-------*******-------*******"
echo "Jackknife sampling the data! "

