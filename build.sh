#!/bin/bash
build_type='DEBUG'
if test $1 = '-r'
  then
  build_type='RELEASE'
fi
cmake -DCMAKE_BUILD_TYPE=$build_type -S . -B ./build -GNinja
sub_bin=$(echo $build_type | tr [:upper:] [:lower:])
echo "- Binaries in ./bin/$sub_bin -"
if test -z $(find ./bin/$sub_bin -name 'resources')
then
  echo "- Created links for resources -"
  ln -s $(pwd)/resources $(pwd)/bin/"$sub_bin"/resources
fi
cd build
ninja
ctest
cd ..
