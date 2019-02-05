#! /bin/sh
# replace a text string in certain filename
# $1 is search patter $2 is replacement pattern
# $3 is the file extenstion
find . -type f -name "*.$3" -print| while read i
do
        echo $1
	echo $2
#        echo $i | sed 's|Sys|System|g'
        echo $i | sed "s|$1|$2|g"
        name=`echo $i | sed "s|$1|$2|g"`
	mv $i $name 
done

#replace the contents of the files as well
#... this doesn't quite work yet...
#sed -i -- 's/$1/$2/g' include/*.$3
#sed -i -- 's/$1/$2/g' src/*.$3
