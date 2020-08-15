#include <cstdlib>
#include <new>
#include<vector>
#include <limits>
#include <iostream>
#include <algorithm> 
#include <chrono> 

using std::cout;
using std::endl;
using namespace std::chrono;

//Chunk size is 8 bytes.
struct Chunk {
    /**
     * When a chunk is free, the `next` contains the
     * address of the next chunk in a list.
     *
     * When it's allocated, this space is used by
     * the user.
     */
    Chunk()
    {

    }
    Chunk(bool f):flag(f)
    {

    }
    bool flag;
};


template <class T>
class PoolAllocator
{
public:
    typedef T value_type;
    PoolAllocator(size_t chunksPerBlock)
        : mChunksPerBlock(chunksPerBlock) {}


    PoolAllocator() { };
    template <class U> constexpr PoolAllocator(const PoolAllocator <U>&) noexcept {}

    [[nodiscard]] T* allocate();

    void deallocateBlock(T*);
    static int count;
private:
    
    size_t mChunksPerBlock;

    void report(T* p, std::size_t n, bool alloc = true) const {
        std::cout << (alloc ? "Alloc: " : "Dealloc: ") << sizeof(T) * n
            << " bytes at " << std::hex << std::showbase
            << reinterpret_cast<void*>(p) << std::dec << '\n';
    }

 /**
 * Allocation pointer.
 */
   static T* mAlloc;
};

template <typename T> int PoolAllocator<T>::count = 0;
template <typename T> T* PoolAllocator<T>::mAlloc =nullptr;


template <typename T> [[nodiscard]] T* PoolAllocator<T>::allocate() {

    if (mAlloc == nullptr) {
        mAlloc = allocateBlock();
    }

    // The return value is the current position of
    // the allocation pointer:
    // Advance (bump) the allocation pointer to the next chunk.
    //
    // When no chunks left, the `mAlloc` will be set to `nullptr`, and
    // this will cause allocation of a new block on the next request:

    //Going to next address
    T* freeChunk=reinterpret_cast<T*>(reinterpret_cast<char*>(mAlloc) + sizeof(T)*count);
    count++;
    std::cout << "address now is " << freeChunk << " count is" << count << std::endl;
    //report(freeChunk, size, 1);
    return freeChunk;
}


template <typename T> T* PoolAllocator<T>::allocateBlock() {

    cout << "\nAllocting block size (" << mChunksPerBlock * sizeof(T) << " chunks):\n\n";
    // The first chunk of the new block.
    T* blockBegin = static_cast<T*>(malloc(mChunksPerBlock * sizeof(T)));
    return blockBegin;
}

template<typename T> void PoolAllocator<T>::deallocateBlock(T* chunk)
{
    std::cout << "freeing chunk now is " << chunk << std::endl;
    free(chunk);
}


int main()
{
    PoolAllocator<Chunk> v(12);
    Chunk *ch[10];

    for (int i = 0; i < 10; i++)
    {
       ch[i]= new (v.allocate()) Chunk(true);
    } 
    std::cout << ch[3]->flag << std::endl;
    //Now deallocate complete chunk.
    v.deallocateBlock(ch[0]);
}

