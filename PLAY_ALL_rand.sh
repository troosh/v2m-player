find ftp.modland.com/V2 -iname *.v2m -print0 | \
     shuf -z --random-source=/dev/urandom | \
     while read -d $'\0' f
do
./tinyplayer "$f"
done
