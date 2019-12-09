#!/bin/bash

# SAT

START=$(date +%s)
c=0
s=0
w=0
# ../../.././microsat++
# z3
solver="../../.././microsat++"
fname="bench1"
if [ -d $fname ]; then
  rm $fname/*
else
  mkdir $fname
fi

for i in $(cat name1); do
  $solver ../../bench1/sat/$i >results 2>&1
  if (grep -q "[^n]sat" results) || (grep -q "^sat" results); then
    echo "$i Pass!"
    let "c+=1"
    let "s+=1"
    cp results $fname/result_sat_pass_"$i"
  else
    echo "$i Wrong!"
    let "s+=1"
    let "w+=1"
    cp results $fname/result_sat_wrong_"$i"
  fi

  rm -f results
done

for i in $(cat name2); do
  $solver ../../bench1/unsat/$i >results 2>&1
  if grep -q "unsat" results; then
    echo "$i Pass!"
    let "c+=1"
    let "s+=1"
    cp results $fname/result_unsat_pass_"$i"
  else
    echo "$i Wrong!"
    let "s+=1"
    let "w+=1"
    cp results $fname/result_unsat_wrong_"$i"
  fi

  rm -f results
done

echo "-------- Your Result --------"
echo "Pass: $c/$s"

END=$(date +%s)
DIFF=$(($END - $START))
echo "Took $DIFF seconds."
