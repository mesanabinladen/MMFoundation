MMFoundation — Quick HowTo

Overview

MMFoundation is a small C-based "Foundation-like" utility library used by the MapMaker project. It provides lightweight types and helpers such as `MMString`, `MMArray`, `MMData`, and `MMFileHandle` so you can avoid Objective-C runtime dependencies and write portable C code.

Overview

This HOWTO is intended for contributors working inside the `MMFoundation` library itself or for users of the library. It documents how to use the library's APIs from other sources inside the same repository (or from small example programs), and it explains internal conventions you should follow when adding or modifying code in `MMFoundation/`.

Including `MMFoundation` in another project

If you want to bring `MMFoundation` into the root of another repository, here are a few common approaches; replace `https://github.com/mesanabinladen/MMFoundation.git` with the repository HTTPS/SSH URL.

- Simple clone into your project root:

```sh
git clone https://github.com/mesanabinladen/MMFoundation.git MMFoundation
```

- As a git submodule (recommended when you want to keep upstream history separate and update easily):

```sh
git submodule add https://github.com/mesanabinladen/MMFoundation.git MMFoundation
git submodule update --init --recursive
```

- As a git subtree (useful if you prefer the code copied into your tree but want occasional pulls):

```sh
git remote add mmfoundation https://github.com/mesanabinladen/MMFoundation.git
git subtree add --prefix=MMFoundation mmfoundation main --squash
```

After adding the code, commit the change to your repository so the library becomes part of your project tree.

Include the header in your project 

```c
#include "MMFoundation/MMFoundation.h"
```

Create and free `MMString` objects:

```c
MMString *s = MMString_initWithCString("hello");
const char *c = MMString_cString(s);
MMString_release(s);
```

Array helpers:

```c
MMArray *a = MMArray_initWithObjects((void*)MMString_initWithCString("one"),
                                     (void*)MMString_initWithCString("two"),
                                     nil);
size_t n = MMArray_count(a);
MMArray_release(a);
```

String operations:

```c
MMRange r = MMString_rangeOfString(s, MMString_initWithCString("lo"));
MMArray *parts = MMString_componentsSeparatedByString(s, MMString_initWithCString(","));
MMString *repl = MMString_stringByReplacingOccurrencesOfString(s,
    MMString_initWithCString("old"), MMString_initWithCString("new"));
/* remember to release returned objects */
MMArray_release(parts);
MMString_release(repl);
```

Reading/writing files (inside library tests/examples)

Use `MMString_stringWithContentsOfFile` for UTF-8 files:

```c
MMError err = {0};
MMString *path = MMString_initWithCString("tests/data/sample.txt");
MMString *contents = MMString_stringWithContentsOfFile(path, MMUTF8StringEncoding, &err);
if (contents) {
    /* use then release */
    MMString_release(contents);
}
MMString_release(path);
```

Using `MMAutoreleasePool_init` / `MMAutoreleasePool_drain`

`MMFoundation` includes a very small autorelease-pool style helper for temporary objects. When a pool is active, objects created via `MM_init()` and its helpers are tracked and can be released when the pool is drained.

```c
MMAutoreleasePool_init();
{
    MMString *tmpString = MMString_initWithCString("temporary");
    MMString *tmpPath = MMString_initWithCString("/tmp/example.txt");
    MMData *data = MMData_initWithContentsOfFile(tmpPath);

    if (data) {
        /* use data here */
        MMData_release(data); // optional, pool will also clean it up
    }

    MMString_release(tmpString); // optional manual release
    MMString_release(tmpPath);  // optional manual release
}
MMAutoreleasePool_drain();
```

This is useful for parsing loops or temporary work scopes.

Conventions and tips for contributors

- Memory: most factory functions allocate heap objects; free them with the matching `MM..._release` or the generic `MM_release` function.
- Variadic constructors require a `nil` sentinel; always end argument lists with `nil`.
- Types: use the `MMTypes.h` types and the `type`/`retainCount` fields consistently when adding new objects.
- Error handling: `MMError` is minimal; extend it carefully and document new error codes in headers.
- [TODO] Tests/examples: put small runnable examples under `MMFoundation/examples/` or `tests/` and demonstrate expected usage and release semantics.

Extending the library

When adding new public functions:

1. Add the declaration to the appropriate header in `MMFoundation/`.
2. Implement the function in a `.c` file, follow existing style and memory conventions.
3. Rebuild the static archive and test you code.


