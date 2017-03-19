Look at impala UDF Development Cloudera pages for more details. Its pretty well explained.
install impala development package
yum install gcc-c++ cmake boost-devel

Minimum cmake version required 3.2.3. 

You may build from source  
https://cmake.org/download/

or

install it from here.
https://cmake.org/files/v3.7/cmake-3.7.2-Linux-x86_64.sh
export JAVA_HOME=/usr/java/latest  or <where ever your JAVA HOME is>
cmake .
make
