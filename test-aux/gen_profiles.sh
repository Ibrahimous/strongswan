#!/bin/sh
##
## gen_profile.sh for libcommon in /tmp/phil/libcommon
##
##

set -e
set -x

output_dir=$1

for file in `ls $output_dir`; do
  if [ -d $file ]; then
    echo "<br /><h3>$file</h3><hr />" >> $output_dir/profile.html
    for graph in `find $output_dir/$file -iname "*_gprof.png"`; do
      name=${graph%%.png}
      name=${name##*/}
      graph=${graph##*html/}
      echo "<a href=\"$graph\">$name</a><br />" >> $output_dir/profile.html
  done
fi
done
echo "</p></body></html>" >> $output_dir/profile.html
