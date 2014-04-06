#!/bin/bash
make clean
make

echo $'****BEGINNING TEST****\n\n' > test.txt
SUM=0
echo "individual run times setup/tear-down (ns)" >> test.txt
for i in {1..10}
do
	START=$(date +%s%N)
	./test_client $1 9191 kvkell password ls
	END=$(date +%s%N)
	DIFF=$(( $END -$START ))
	SUM=$(( $DIFF + $SUM ))
	echo "$DIFF" >> test.txt
done

AVERAGE=$(( $SUM/10 ))
echo " " >> test.txt
echo "average setup/tear-down (ns): $AVERAGE" >> test.txt

AVERAGE=$(( $AVERAGE/1000000 ))
echo "average setup/tear-down (ms): $AVERAGE" >> test.txt

SUM=0
echo " " >> test.txt
echo "individual run times cat of 1MB file (ns)" >> test.txt
./client $1 9191 kvkell password dd if=/dev/zero of=big_file.txt count=1024 bs=1024

for i in {1..10}
do
	START=$(date +%s%N)
	./client $1 9191 kvkell password cat big_file.txt
	END=$(date +%s%N)
	DIFF=$(( $END -$START ))
	SUM=$(( $DIFF + $SUM ))
	echo "$DIFF" >> test.txt
done

./client $1 9191 kvkell password rm big_file.txt

AVERAGE=$(( $SUM/10 ))
echo " " >> test.txt
echo "average cat 1MB file (ns): $AVERAGE" >> test.txt

MB_AVERAGE=$(( $AVERAGE/1000000 ))
echo "average cat 1MB file (ms): $MB_AVERAGE" >> test.txt

echo "average throughput (MB/s):" >> test.txt
#echo "37/43" | bc >> test.txt
echo "scale=10;( 1/$AVERAGE ) * 1000000000" | bc >> test.txt
