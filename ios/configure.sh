#!/bin/bash
set -e

BASEDIR=$(dirname $0)

rm -rf $BASEDIR/boost.framework
cp -R $BASEDIR/../lib/casablanca/Build_iOS/boost.framework $BASEDIR/boost.framework

rm -rf $BASEDIR/ios-cmake
cp -R $BASEDIR/../lib/casablanca/Build_iOS/ios-cmake $BASEDIR/ios-cmake

rm -rf $BASEDIR/build.ios
mkdir $BASEDIR/build.ios
pushd $BASEDIR/build.ios
cmake .. -DCMAKE_BUILD_TYPE=Release
make
popd
echo "===="
echo "The final library is available in 'build.ios/libodata-library.a' and 'build.ios/libodata-client.a'"
