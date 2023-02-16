#!/bin/bash
cd $1

for i in {00..31}; do
  mkdir $i
  mv *.+$i.* $i
done
