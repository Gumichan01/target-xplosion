#!/usr/bin/env bash

# Usage:
#
#   $ ./tx-package.sh <name> vx.y.z-win32   // On linux
#   or
#   $ ./tx-package.sh ./bin/[Debug/Release]/<name> vx.y.z-win32   // On Windows
#

if [ $# -lt 2 ]; then
    echo -e "Usage: \n $ ./tx-package.sh <name> vx.y.z"
    exit 1
fi

exec_path=$1
exec_file=$(basename $1)
release_dir=../release-tx/Gumichan01-Target_Xplosion_$2

rm -rf $release_dir/
echo $exec_file $release_dir/ && \
mkdir -p $release_dir/ && \
cp -v *.txt $release_dir/ && \
cp -v *.dll $release_dir/ && \
cp -v README.md $release_dir/ && \
cp -R -v config/ $release_dir/ && \
cp -R -v data/ $release_dir/ && \
cp -R -v font/ $release_dir/ && \
cp -R -v audio/ $release_dir/ && \
cp -R -v image/ $release_dir/ && \
cp -v $exec_path $release_dir/ #&&
#./$release_dir/$exec_file
