#!/bin/bash

cd $1

for i in $(ls); do
  PSDATA=$(echo $i)
  VDATA=$(echo $i | sed 's/.ps./.v./')
  echo $PSDATA $VDATA
done
