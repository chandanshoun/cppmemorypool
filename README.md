# cppmemorypool


This cpp code contains a memory pool of 10 Chuk object.

Main purpose is to pre allocate memory and use placement new to allocate existing memory.

This way we can create a larger chunk of memory for objects in advance.

This will help to increase performancy when larger object has to be dynamically allocated. so avoid calling new/os call for new memory.

for compile
g++ -g memorypool.cpp -o memorypool

./memorypool
