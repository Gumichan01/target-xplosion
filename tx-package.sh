#!/usr/bin/env bash

#
#   Target Xplosion Packaging
#
#   This "portable" script (Linux / Unix / Windows [* Bash]) generates a delivery package.
#   You must provide the executable file and the version number, so the package can be properly generated.
#
#
# Usage:
#
#   $ ./tx-package.sh <name> vx.y.z   // On Linux
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

EXEC_PATH=$1
EXEC_FILE=$(basename $1)
VERSION=$2
VERSION_DIR=Gumichan01-Target_Xplosion_${VERSION}
RELEASE_PATH=../release-tx
VERSION_PATH=${RELEASE_PATH}/${VERSION_DIR}
LOG_FILE=${PWD}/"packaging-"${VERSION}"-"$(date +%Y-%m-%d-%H:%M:%S)".log";

KERNEL_ARRAY="GNU Linux Unix Darwin FreeBSD OpenBSD DragonFly"
KERNEL=$(uname --kernel-name)

TX_VERSION="v0.5.5"
SYS_ARCH=$(uname --machine)

DESKTOP_ENTRY="[Desktop Entry]"
NAME="Target Xplosion"
EXEC=targetxplosion-${TX_VERSION}-${SYS_ARCH}
ICON="tx"
TYPE="Application"
CATEGORIES="Utility;"
NAME_JP="ターゲットエクスプロージョン"
COMMENT="2D retro shoot'em up game - by Gumichan01"
COMMENT_FR="Un shoot'em up retro 2D - par Gumichan01"
TERMINAL="false"

# Code

rm -rvf ${VERSION_PATH}/ | tee -a ${LOG_FILE}
echo $EXEC_FILE ${VERSION_PATH}/ | tee -a ${LOG_FILE}
echo "Create " ${VERSION_PATH} | tee -a ${LOG_FILE}
mkdir -p ${VERSION_PATH}/
cp -v *.txt ${VERSION_PATH}/ | tee -a ${LOG_FILE}

OLD_IFS=$IFS
IFS=' ' read -r -a array <<< ${KERNEL_ARRAY};
IFS=$OLD_IFS

CONTAINS_KERNEL=$(contains "${array[@]}" ${KERNEL})

# I need to copy DLL files if this script is executed on Windows
if [ ${CONTAINS_KERNEL} == "n" ]; then

	echo "Trying to copy DLL file into " ${VERSION_PATH} " ..." | tee -a ${LOG_FILE}
	cp -v *.dll ${VERSION_PATH}/ | tee -a ${LOG_FILE}
fi

cp -v README.md ${VERSION_PATH}/ | tee -a ${LOG_FILE}
cp -R -v config/ ${VERSION_PATH}/ | tee -a ${LOG_FILE}
cp -R -v data/ ${VERSION_PATH}/ | tee -a ${LOG_FILE}
cp -R -v font/ ${VERSION_PATH}/ | tee -a ${LOG_FILE}
cp -R -v audio/ ${VERSION_PATH}/ | tee -a ${LOG_FILE}
cp -R -v image/ ${VERSION_PATH}/ | tee -a ${LOG_FILE}
cp -v $EXEC_PATH ${VERSION_PATH}/ | tee -a ${LOG_FILE}

echo "Packaging " ${VERSION_DIR} " ..." | tee -a ${LOG_FILE}
cd ${RELEASE_PATH}
tar -cvzf ${VERSION_DIR}.tar.gz ${VERSION_DIR}
sha1sum ${VERSION_DIR}.tar.gz | tee -a ${LOG_FILE}
cd -
echo "Package " ${VERSION_DIR} " OK" | tee -a ${LOG_FILE}

# Uncomment this line of code if you want to launch the program and test it
#./${VERSION_PATH}/$EXEC_FILE
