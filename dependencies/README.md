# allocator

zosia@Heaven:~/alloc/new$ g++ -o try try.cpp -I~/alloc/new ~/alloc/new/build/liballocator_lib.a


docker build -t alloc_image . // env installation
docker container run -it alloc_image /bin/bash

git clone https://github.com/kuriataz/allocator.git
cd allocator/dependencies

./install_lib.sh ../new
cd new

g++ -o try try.cpp -Iapp/allocator/new app/allocator/new/build/liballocator_lib.a
./try