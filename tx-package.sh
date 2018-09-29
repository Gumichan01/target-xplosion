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


if [ $# -lt 2 ]; then
    echo -e "Usage: \n $ ./tx-package.sh <name> vx.y.z"
    exit 1
fi

exec_path=$1
exec_file=$(basename $1)
release_dir=../release-tx/Gumichan01-Target_Xplosion_$2
LINUX="Linux"

rm -rf $release_dir/
echo $exec_file $release_dir/
mkdir -p $release_dir/
cp -v *.txt $release_dir/

OLD_IFS=$IFS
IFS=' ' read -r -a array <<< $(uname -a);
IFS=$OLD_IFS

# I need to copy DLL files if this script is executed on Windows
# Well technically, even if this condition is true, maybe I am on Mac {^.^'}
if [ $(contains "${array[@]}" $LINUX) == "n" ]; then

	echo "OK"
	cp -v *.dll $release_dir/
fi

cp -v README.md $release_dir/
cp -R -v config/ $release_dir/
cp -R -v data/ $release_dir/
cp -R -v font/ $release_dir/
cp -R -v audio/ $release_dir/
cp -R -v image/ $release_dir/
cp -v $exec_path $release_dir/
#./$release_dir/$exec_file
