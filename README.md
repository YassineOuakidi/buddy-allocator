# 🧠 Buddy Memory Allocator

A custom memory allocator implementing the **Buddy Memory Allocation** algorithm in C using `mmap`. This project demonstrates low-level memory management techniques by simulating how memory is divided, allocated, and merged using binary buddy blocks.

---

## 📂 Project Structure

```
.
├── inc/
│   └── buddy_alloc.h       # Header file containing struct and function declarations
├── src/
│   └── buddy.c             # Implementation of allocation and deallocation
│   └── init.c              #initializing data
│   └── utils.c      
├── main.c                  # Example usage or tests (add your tests here)
│   └── main.c                 
├── README.md
```

---

## 🧱 Features

- Allocation of memory blocks with size rounded up to the nearest power of two.
- Efficient splitting of blocks to fit allocation requests (`split` logic).
- Merging free buddy blocks back into larger blocks (`merge` logic).
- Memory metadata is managed using `node_t` structures allocated via `mmap`.
- Support for freeing allocated memory with automatic buddy merging.

---

## 🛠️ How It Works

- **Allocation (`buddy_alloc`)**:  
  Finds the first fitting free block, splits it if necessary to match the requested size, and returns a pointer to the start of the memory block.

- **Freeing (`buddy_free`)**:  
  Marks the block as free and attempts to merge it with its buddy recursively.

- **Splitting**:  
  Blocks are split recursively until the block size matches the request.

- **Merging**:  
  When a block is freed, it attempts to merge with its buddy (if it's also free and same-sized).

---

## 🧪 Example Usage

```c
void *ptr = buddy_alloc(128);  // Allocate 128 bytes
buddy_free(ptr);               // Free the allocated memory
```

---

## 📌 Requirements

- Linux (for `mmap`, `munmap`)
- GCC or Clang
- Basic C standard library

---

## 🔧 Build
 - Compile manually:

```bash
gcc -o test test/main.c src/buddy_alloc.c -Iinc
```

---

## 🚧 TODO

- Add allocation failure handling
- Memory usage statistics
- Unit tests
- Optional debug logging

---

## 📚 References

- [Buddy memory allocation - Wikipedia](https://en.wikipedia/wiki/Buddy_memory_allocation)
- `mmap` and `munmap` manual pages

---

## 🧑‍💻 Author

**Ouakidi Yassine** – [GitHub Profile](https://github.com/YassineOuakidi)

---

## 📄 License

MIT License (or add your preferred license)
