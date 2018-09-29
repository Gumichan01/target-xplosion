#!/usr/bin/env bash

# Usage:
#
#   $ ./tx-package.sh <name> vx.y.z-win32   // On linux
#   or
#   $ ./tx-package.sh ./bin/[Debug/Release]/<name> vx.y.z-win32   // On Windows
#

# Check if an array of strings contains a specific string
function contains() {
    local n=$#
    local value=${!n}
    for ((i=1;i < $#;i++)) {
        if [ "${!i}" == "${value}" ]; then
            echo "y"
            return 0
        fi
    }
    echo "n"
    return 1
}


# Parameters

if [ $# -lt 2 ]; then
    echo -e "Usage: \n $ ./tx-package.sh <name> vx.y.z"
    exit 1
fi


# Variables / Constant values

exec_path=$1
exec_file=$(basename $1)
VERSION=$2
release_dir=../release-tx/Gumichan01-Target_Xplosion_${VERSION}
LOG_FILE="packaging-"${VERSION}"-"$(date +%Y-%m-%d-%H:%M:%S)".log";

LINUX="Linux"


# Code

rm -rvf $release_dir/ | tee -a ${LOG_FILE}
echo $exec_file $release_dir/ | tee -a ${LOG_FILE}
echo "Create " $release_dir | tee -a ${LOG_FILE}
mkdir -p $release_dir/
cp -v *.txt $release_dir/ | tee -a ${LOG_FILE}

OLD_IFS=$IFS
IFS=' ' read -r -a array <<< $(uname -a);
IFS=$OLD_IFS

contains_linux=$(contains "${array[@]}" $LINUX)

# I need to copy DLL files if this script is executed on Windows
# Well technically, even if this condition is true, maybe I am on Mac {^.^'}
if [ ${contains_linux} == "n" ]; then

	echo "Trying to copy DLL file into " $release_dir " ..." | tee -a ${LOG_FILE}
	cp -v *.dll $release_dir/ | tee -a ${LOG_FILE}
fi

cp -v README.md $release_dir/ | tee -a ${LOG_FILE}
cp -R -v config/ $release_dir/ | tee -a ${LOG_FILE}
cp -R -v data/ $release_dir/ | tee -a ${LOG_FILE}
cp -R -v font/ $release_dir/ | tee -a ${LOG_FILE}
cp -R -v audio/ $release_dir/ | tee -a ${LOG_FILE}
cp -R -v image/ $release_dir/ | tee -a ${LOG_FILE}
cp -v $exec_path $release_dir/ | tee -a ${LOG_FILE}

if [ ${contains_linux} == "y" ]; then

	echo "Packaging " $release_dir " ..." | tee -a ${LOG_FILE}
	#tar -cvzf $release_dir.tar.gz $release_dir
	echo "Package " $release_dir " OK" | tee -a ${LOG_FILE}
fi

#./$release_dir/$exec_file
