#!/bin/sh
vncserver :12 -geometry 1024x768
google-chrome --display=:12
count=1
while read url
do
    google-chrome --display=:12 $url
    sleep 3
    import -window root $url.jpg

    count=`expr $count + 1`
done < log_file.txt

vncserver -kill :12
