#!/bin/bash
case $1 in
	Debug|Prof|Release)
		echo "Build for $1"
		;;
	*)
		echo "No such build type"
		exit
		;;
esac

if [ ! -d build/$1 ]; then mkdir -p build/$1; fi
cd $(readlink -f .)/build/$1
#export CC=icc CXX=icpc
export CC="ccache gcc" CXX="ccache g++"
cmake -G"Eclipse CDT4 - Unix Makefiles" -DECLIPSE_CDT4_GENERATE_SOURCE_PROJECT=TRUE -DCMAKE_BUILD_TYPE=$1 ../..
cp .project ../..
cp .cproject ../..
make || exit $?
cd ../..
ln -f $1/nv bin/
ln -f $1/mu bin/
