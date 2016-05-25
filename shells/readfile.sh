#!/bin/bash

filename=$1

if [ ! -r "$filename" ] ; then
    echo "Error : Can not read $filename"
    exit 1
fi

echo "Contents of file ${filename} : "
while read myline
do
    echo "$myline"
done < $filename
echo "End of ${filename}"
echo "Checksum : `md5sum $filename`"
