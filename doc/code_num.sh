find . -name "*.c" or  -name "*.h" |xargs cat|grep -v ^$|wc -l