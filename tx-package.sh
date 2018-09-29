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

VERSION_DIR=../release-tx/Gumichan01-Target_Xplosion_${VERSION}
LOG_FILE="packaging-"${VERSION}"-"$(date +%Y-%m-%d-%H:%M:%S)".log";

LINUX="Linux"


# Code

rm -rvf ${VERSION_DIR}/ | tee -a ${LOG_FILE}
echo $exec_file ${VERSION_DIR}/ | tee -a ${LOG_FILE}
echo "Create " ${VERSION_DIR} | tee -a ${LOG_FILE}
mkdir -p ${VERSION_DIR}/
cp -v *.txt ${VERSION_DIR}/ | tee -a ${LOG_FILE}

OLD_IFS=$IFS
IFS=' ' read -r -a array <<< $(uname -a);
IFS=$OLD_IFS

contains_linux=$(contains "${array[@]}" $LINUX)

# I need to copy DLL files if this script is executed on Windows
# Well technically, even if this condition is true, maybe I am on Mac {^.^'}
if [ ${contains_linux} == "n" ]; then

	echo "Trying to copy DLL file into " ${VERSION_DIR} " ..." | tee -a ${LOG_FILE}
	cp -v *.dll ${VERSION_DIR}/ | tee -a ${LOG_FILE}
fi

cp -v README.md ${VERSION_DIR}/ | tee -a ${LOG_FILE}
cp -R -v config/ ${VERSION_DIR}/ | tee -a ${LOG_FILE}
cp -R -v data/ ${VERSION_DIR}/ | tee -a ${LOG_FILE}
cp -R -v font/ ${VERSION_DIR}/ | tee -a ${LOG_FILE}
cp -R -v audio/ ${VERSION_DIR}/ | tee -a ${LOG_FILE}
cp -R -v image/ ${VERSION_DIR}/ | tee -a ${LOG_FILE}
cp -v $exec_path ${VERSION_DIR}/ | tee -a ${LOG_FILE}

if [ ${contains_linux} == "y" ]; then

	echo "Packaging " ${VERSION_DIR} " ..." | tee -a ${LOG_FILE}
	#tar -cvzf ${VERSION_DIR}.tar.gz ${VERSION_DIR}
	echo "Package " ${VERSION_DIR} " OK" | tee -a ${LOG_FILE}
fi

#./${VERSION_DIR}/$exec_file
