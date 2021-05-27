# My Malloc Assignment

-[x] create MyMalloc Class
-[x] create allocate method
-[x] create deallocate method
-[x] constructor
-[x] destructor
-[x] helper methods:
    -[x] printTable
    -[x] getCount
    -[x] getCapacity
    -[x] countCount
-[x] hash table:
    -[x] insert
    -[x] delete
    -[x] grow
    


## allocate function

-[x] should round up allocation size to next multiple of page size
-[x] allocate memory with mmap
-[x] insert allocation ptr and size to hash table

- possible hash function: 
```cpp
x >> VirtualAddressOffsetSizeInBits
```

## deallocate function

-[x] deallocate memory with munmap
-[x] remove allocation ptr and size from hash table (maybe using tombstone)


## requirements

-[x] include unit testing
-[x] micro benchmarks comparing MyMalloc to malloc (timing)
-[x] instructions on how to run program (new readme or comments)

