#! /bin/bash

if [ -d qemacs/src/hunspell-1.3.2 ];
then
    pushd qemacs/src/hunspell-1.3.2
    ./bootstrap.sh
    popd
fi

mkdir -p config
echo 'aclocal'
aclocal -I m4
echo 'libtoolize'
libtoolize
echo 'automake'
automake --add-missing
echo 'autoconf'
autoconf
echo 'end'
