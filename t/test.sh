#!/bin/sh

md5=md5sum

function hash {
	$md5 $1 | awk '{print $1}'
}

function btest {
	cp $1 tmp
	../bdiff $1 $2 > tmp.ired
	../ired -n tmp < tmp.ired
	test `hash tmp` = `hash $2`
	ret=$?
	echo $ret $1 $2
	if [ $ret = 1 ]; then
		if [ -n "`echo $1| grep file`" ]; then
			diff -u $2 tmp
		fi
	fi
#/	rm -f tmp.ired tmp
}

(cd .. && make -s)

if [ -n "$2" ]; then
	btest $1 $2
else
	for i in `seq 1 4`; do
		for j in `seq 1 4`; do
			btest file$i file$j
		done
	done
fi

#btest /etc/services /etc/fstab

#btest /bin/true /bin/false
#radiff /bin/false tmp 
