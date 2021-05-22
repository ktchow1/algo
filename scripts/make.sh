#!/usr/bin/env bash
BUILD_DIR="build/debug"

pushd "$(dirname "${BASH_SOURCE[0]}")/.." &> /dev/null
mkdir -p "${BUILD_DIR}"
pushd "${BUILD_DIR}" &> /dev/null

cmake -DCMAKE_BUILD_TYPE=Debug ../..
rc=${?}
if [[ ${rc} -ne 0 ]]; then
  exit ${rc}
fi

make -j4 
rc=${?}
if [[ ${rc} -ne 0 ]]; then
  exit ${rc}
fi

popd &> /dev/null
popd &> /dev/null
