@echo off

if not exist build/ (
    mkdir build
)

pushd build
call cmake --build ./ --target clean
call cmake ../ -A x64
popd build
