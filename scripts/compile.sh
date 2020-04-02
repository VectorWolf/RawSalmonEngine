#!/bin/bash
set -e
usage="Usage: ./compile.sh [CONFIG] [PLATFORM] [BIT]
CONFIG: \"Release\", \"Debug\", \"Profile\"
PLATFORM: \"linux\", \"windows\"
BIT: \"64\", \"32\""
if [ "$1" ==  "--help" ] || [ "$1" == "-h" ]
then
    echo "$usage"
    exit
fi

if [ "$1" ]
then
    C=$1
else
    C=$CONFIG
fi

if [ "$2" ]
then
    P=$2
else
    P=$PLATFORM
fi

if [ "$3" ]
then
    B=$3
else
    B=$BIT
fi

if [ "$C" != "Release" ] && [ "$C" != "Debug" ] && [ "$C" != "Profile" ]
then
    echo "CONFIG parameter or env var is not properly set! Value is: \"${C}\"!"
    echo "Please check help page via \"./compile -h\""
    exit 1
fi
if [ "$P" != "linux" ] && [ "$P" != "windows" ]
then
    echo "PLATFORM parameter or env var is not properly set! Value is: \"${P}\"!"
    echo "Please check help page via \"./compile -h\""
    exit 1
fi
if [ "$B" != "64" ] && [ "$B" != "32" ]
then
    echo "BIT parameter or env var is not properly set! Value is: \"${B}\"!"
    echo "Please check help page via \"./compile -h\""
    exit 1
fi

cd "$(dirname "${BASH_SOURCE[0]}")"
cd ../
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=${C} -DCMAKE_TOOLCHAIN_FILE="../cmake/${P}${B}-toolchain.cmake" ..
make -j$(nproc)
make install
echo "Successfully compiled"
