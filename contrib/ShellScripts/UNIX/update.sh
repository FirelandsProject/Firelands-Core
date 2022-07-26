#!/bin/bash

# Settings
homeDir=/home/server

binDir=$homeDir/bin
compileDir=$homeDir/build
confDir=$homeDir/etc
sourceDir=$homeDir/frostmourne-3_3_5

echo "########################"
echo "# Updating Core:"
echo "########################"
cd $sourceDir
git pull
echo "########################"
echo "# Compiling Core:"
echo "########################"
install_pkg() {
    cd $compileDir
    rm $compileDir/CMakeCache.txt
    rm $compileDir/revision.h
    rm $binDir/worldserver.backup
    cp -u $binDir/worldserver $binDir/worldserver.backup
    cmake $sourceDir -DWITH_MMAPS=0 -DAUTHSERVER=0 -DWITH_COREDEBUG=0 -DPREFIX=$homeDir -DCONF_DIR=$confDir -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_CXX_FLAGS="-m64 -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -O2 -pipe -msse3 -g -fno-strength-reduce -fno-delete-null-pointer-checks -fno-strict-aliasing -frename-registers -march=native" -DCMAKE_C_FLAGS="-m64 -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -O2 -pipe -msse3 -g -fno-strength-reduce -fno-delete-null-pointer-checks -fno-strict-aliasing -frename-registers -march=native"

    # Try making
    if make clean && make -j16 && make install; then
        cd $binDir
        ./telnetCallback.sh updateSuccess
        exit 1
    fi
    cd $binDir
    ./telnetCallback.sh updateError
}
install_pkg
