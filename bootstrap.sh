#! /bin/bash

if [ -d qemacs/src/hunspell-1.3.2 ];
then
    pushd qemacs/src/hunspell-1.3.2
    ./bootstrap.sh
    popd
fi

mkdir -p config
aclocal -I m4
libtoolize
automake --add-missing
autoconf
