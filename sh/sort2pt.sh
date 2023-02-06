#!/bin/bash
DIR=$1

cd $1

mkdir s v t av ps

mv *.s.* s
mv *.v.* v
mv *.t.* t
mv *.av.* av
mv *.ps.* ps
