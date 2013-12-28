#!/bin/sh

#### Hardcorded settings.

RM="rm -f"
MV="mv -f"
SED=sed
MAKE=make
MAKEFILE=makefile

#### Script starts from here.

if [ $# -ne 2 ]; then
	echo Give a source code location and an installation directories of NTL as follows:
	echo "    $0 SOURCE-CODE INSTALLATION"
	exit 1
fi
NTLDIRPATH=$1
echo NTL source code directory is $NTLDIRPATH
pushd $2
PREFIX=`pwd`
popd
echo NTL installation directory is $PREFIX

ARCHNAME=`uname -s`
case $ARCHNAME in
	Darwin)
		CC=cc
		CXX=c++
		if [ ! `which glibtool` ]; then
			echo GNU libtool is needed, it does not work with Mac OSX built-in one.
			exit 1
		fi
		LIBTOOL=glibtool
		;;
	*)
		echo Unknown operating system name $ARCHNAME
		exit 1
		;;
esac


WARNS="-Wall -Wextra -Wformat=2 -Wstrict-aliasing=2 -Wpointer-arith -Wbad-function-cast -Wcast-qual -Wcast-align -Wconversion -Wfloat-equal -Wredundant-decls -Wshadow"
OPTS="-O3"

CFLAGS="$OPTS $WARNS"
CXXFLAGS="$OPTS $WARNS"

set -x

pushd $NTLDIRPATH/src

./configure PREFIX=$PREFIX \
	CC=$CC CFLAGS="$CFLAGS" CXX=$CXX CXXFLAGS="$CXXFLAGS" \
	SHARED=on LIBTOOL=$LIBTOOL

$SED -e "s/--mode=compile/--mode=compile --tag=CXX/" $MAKEFILE > $MAKEFILE.temp && \
    $MV $MAKEFILE.temp $MAKEFILE
$SED -e "s/--mode=link/--mode=link --tag=CXX/" $MAKEFILE > $MAKEFILE.temp && \
    $MV $MAKEFILE.temp $MAKEFILE

$RM all

$MAKE -j
$MAKE -j check

echo NOTE: NTL will be installed into $PREFIX .

popd

