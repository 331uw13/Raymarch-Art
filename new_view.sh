#!/bin/bash


name=$1

if [[ $name == "" ]] ; then
	echo "usage: ./new_file <name>"
	exit
fi

filename="src/views/$1_view"

if [ -f $filename.c ] ; then
	echo "file already exists!"
	exit
fi


name2="$(echo $name | tr [a-z] [A-Z])_VIEW_H"

header_code="
#ifndef $name2
\n#define $name2
\n#include \"../state.h\"
\n
\nvoid $(echo $name)_open(STATE st);
\nvoid $(echo $name)_close(STATE st);
\nvoid $(echo $name)_update(STATE st, int key);
\n
\n#endif
"

code="
#include <GL/gl.h>
\n#include \"$(echo $name)_view.h\"
\n
\nvoid $(echo $name)_open(STATE st) {
\n}
\n
\nvoid $(echo $name)_close(STATE st) {
\n}
\n
\nvoid $(echo $name)_update(STATE st, int key) {
\n}
\n
"


echo -e $header_code

echo "---------------------"


echo -e $code

echo -e $header_code > $filename.h
echo -e $code > $filename.c



