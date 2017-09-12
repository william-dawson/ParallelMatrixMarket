test -n $CC  && unset CC
test -n $CXX && unset CXX
mkdir Build
cd Build
cmake ..
make
make test
