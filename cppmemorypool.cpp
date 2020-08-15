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
    Chunk* next;
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

    void deallocate(T*, std::size_t);
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

 /**
  * Allocates a larger block (pool) for chunks.
  */
  T* allocateBlock();
};

template <typename T> int PoolAllocator<T>::count = 0;
template <typename T> T* PoolAllocator<T>::mAlloc =nullptr;


template <typename T> [[nodiscard]] T* PoolAllocator<T>::allocate() {

    // No chunks left in the current block, or no any block
    // exists yet. Allocate a new one, passing the chunk size:

 //   if (size > std::numeric_limits<std::size_t>::max() / sizeof(T))
   //     throw std::bad_alloc();


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

    // Once the block is allocated, we need to chain all
    // the chunks in this block:
/*
    T* chunk = blockBegin;

    for (int i = 0; i < mChunksPerBlock - 1; ++i) {
        chunk->next =
            reinterpret_cast<T*>(reinterpret_cast<char*>(chunk) + chunkSize);
        chunk = chunk->next;
    }

    chunk->next = nullptr;
*/
    return blockBegin;
}

template<typename T> void PoolAllocator<T>::deallocateBlock(T* chunk)
{
    std::cout << "freeing chunk now is " << chunk << std::endl;
    free(chunk);
}

/**
 * Puts the chunk into the front of the chunks list.
 */

 template<typename T> void PoolAllocator<T>::deallocate(T* chunk, size_t size) {

     // The freed chunk's next pointer points to the
     // current allocation pointer:

     //reinterpret_cast<T*>(chunk)->next = mAlloc;

     // And the allocation pointer is now set
     // to the returned (free) chunk:
   //  std::free(chunk);
   //  mAlloc = reinterpret_cast<T*>(chunk);
//     report(chunk, size, 0);
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

/**
 * The allocator class.
 *
 * Features:
 *
 *   - Parametrized by number of chunks per block
 *   - Keeps track of the allocation pointer
 *   - Bump-allocates chunks
 *   - Requests a new larger block when needed
 *
 */

/*
class PoolAllocator {
public:
    PoolAllocator(size_t chunksPerBlock)
        : mChunksPerBlock(chunksPerBlock) {}

    void* allocate(size_t size);
    void deallocate(void* ptr, size_t size);

private:
*/
/**
     * Number of chunks per larger block.
     */
  //  size_t mChunksPerBlock;

    /**
     * Allocation pointer.
     */
    //Chunk* mAlloc = nullptr;

    /**
     * Allocates a larger block (pool) for chunks.
     */
    //Chunk* allocateBlock(size_t);
//};


/**
 * Returns the first free chunk in the block.
 *
 * If there are no chunks left in the block,
 * allocates a new block.
 */
/*
void* PoolAllocator::allocate(size_t size) {

    // No chunks left in the current block, or no any block
    // exists yet. Allocate a new one, passing the chunk size:

    if (mAlloc == nullptr) {
        mAlloc = allocateBlock(size);
    }

    // The return value is the current position of
    // the allocation pointer:

    Chunk* freeChunk = mAlloc;

    // Advance (bump) the allocation pointer to the next chunk.
    //
    // When no chunks left, the `mAlloc` will be set to `nullptr`, and
    // this will cause allocation of a new block on the next request:

    mAlloc = mAlloc->next;

    return freeChunk;
}
*/

/**
 * Allocates a new block from OS.
 *
 * Returns a Chunk pointer set to the beginning of the block.
 */
/*
Chunk* PoolAllocator::allocateBlock(size_t chunkSize) {
    cout << "\nAllocating block (" << mChunksPerBlock << " chunks):\n\n";

    size_t blockSize = mChunksPerBlock * chunkSize;

    // The first chunk of the new block.
    Chunk* blockBegin = reinterpret_cast<Chunk*>(malloc(blockSize));

    // Once the block is allocated, we need to chain all
    // the chunks in this block:

    Chunk* chunk = blockBegin;

    for (int i = 0; i < mChunksPerBlock - 1; ++i) {
        chunk->next =
            reinterpret_cast<Chunk*>(reinterpret_cast<char*>(chunk) + chunkSize);
        chunk = chunk->next;
    }

    chunk->next = nullptr;

    return blockBegin;
}
*/
/**
 * Puts the chunk into the front of the chunks list.
 */
/*
void PoolAllocator::deallocate(void* chunk, size_t size) {

    // The freed chunk's next pointer points to the
    // current allocation pointer:

    reinterpret_cast<Chunk*>(chunk)->next = mAlloc;

    // And the allocation pointer is now set
    // to the returned (free) chunk:

    mAlloc = reinterpret_cast<Chunk*>(chunk);
}
*/
/**
 * The `Object` structure uses custom allocator,
 * overloading `new`, and `delete` operators.
 */
/*
struct Object {

    // Object data, 16 bytes:

    uint64_t data[2];

    // Declare out custom allocator for
    // the `Object` structure:

    static PoolAllocator allocator;

    static void* operator new(size_t size) {
        return allocator.allocate(size);
    }

    static void operator delete(void* ptr, size_t size) {
        return allocator.deallocate(ptr, size);
    }
};

// Instantiate our allocator, using 8 chunks per block:

PoolAllocator Object::allocator{ 8 };
*/


/*

int main(int argc, char const* argv[]) {
    
    // Allocate 10 pointers to our `Object` instances:
    auto start = high_resolution_clock::now();

    constexpr int arraySize = 10;

    Object* objects[arraySize];

    // Two `uint64_t`, 16 bytes.
    cout << "size(Object) = " << sizeof(Object) << endl << endl;

    // Allocate 10 objects. This causes allocating two larger,
    // blocks since we store only 8 chunks per block:

    cout << "About to allocate " << arraySize << " objects" << endl;

    for (int i = 0; i < arraySize; ++i) {
        objects[i] = new Object();
        cout << "new [" << i << "] = " << objects[i] << endl;
    }

    cout << endl;

    // Deallocated all the objects:

    for (int i = arraySize-1; i >= 0; --i) 
    {
        cout << "delete [" << i << "] = " << objects[i] << endl;
        delete objects[i];
    }

    cout << endl;

    // New object reuses previous block:

    objects[0] = new Object();
    cout << "new [0] = " << objects[0] << endl << endl;
    auto stop = high_resolution_clock::now();

    // Get duration. Substart timepoints to  
    // get durarion. To cast it to proper unit 
    // use duration cast method 
    auto duration = duration_cast<microseconds>(stop - start);

    cout << "Time taken by function: "
        << duration.count() << " microseconds" << endl;
        
 /*
    auto start = high_resolution_clock::now();

    for (int i = 0; i < 10; i++)
    {
        Object* object = new Object();
    }
    auto stop = high_resolution_clock::now();

    // Get duration. Substart timepoints to  
    // get durarion. To cast it to proper unit 
    // use duration cast method 
    auto duration = duration_cast<microseconds>(stop - start);

    cout << "Time taken by function: "
        << duration.count() << " microseconds" << endl;
*/

//return 0;
//}
