#!/bin/sh

FILES=4
jot 1 >/dev/null
if [ $? = 0 ]; then
	SEQ=`jot $FILES 1 $FILES`
else
	md5=`seq 1 $FILES`
fi


md5sum -h 2>/dev/null
if [ $? = 0 ]; then
	md5=md5sum
else
	md5=md5
fi

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
	for i in $SEQ ; do
		for j in $SEQ ; do
			btest file$i file$j
		done
	done
fi

#btest /etc/services /etc/fstab

#btest /bin/true /bin/false
#radiff /bin/false tmp 
