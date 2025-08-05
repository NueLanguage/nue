# Changes to Nue

This document outlines some of the changes made to the Nue Programming Language after proper development has been started (and old things are thrown away).

## Sun 3 Aug 2025

The discussions held today do not pertain to early development of the programming language but features related to the language's future standard libraries and use, etc.

TLDR:

- Coroutines are cooperative, single0threaded, safe and predictable (lika Lua)
- Threads are explicit with parallel execution for heavy or blocking tasks
- Mutexes are explicit but beginner friendly, with very strong documentation and examples
- Deferred the idea of hidden runtime locks or implicit blocking behaviour
- Clearly separated sequential coroutine logic from multithreaded code

### Mixing of concurrency models in Nue

An in-depth discussion was held regarding how concurrency should work in Nue. Initially, as someone with 5 years in commercial development with Luvit/LuaJITm there were considerations arouund collapsing coroutines and threads into one concept (making all coroutines execute as OS threads). However, this approach obviously became unrealistic because of race conditions and the complexity it would introduce into Nue's runtime and semantics.

Instead, I decided on a clear separation between coroutines and threads.

Coroutines will remain **single-threaded and cooperative**, behaving exactly like Lua's coroutines. This preserves deterministic and sequential execution without race conditions and is also an incredibly inviting feature to people coming from a Lua background.

Threads will represent **true OS threads**, intended for blocking operations or CPU bound work, allowing for concurrency/parallelism.

This hybrid design means that coroutines remain safe and simple for regular code, whereas threads are available explicitly for advanced concurrency needs but require deliberate handling of shared state.

### Mutexes and thread safety

Instead of hiding thread safety concerns behind implicit runtime locks (which actually would counterintuitively cause confusing, program-wide stalls in some occasions (in separate testing)), Nue will just teach concurrency very directly, plainly and accessibly (according to Nue's ethos).

- Mutexes will be introduces as a first-class cocnept for managing shared state between threads
- - Documentation will explain concurrency and mutexes in very plain and beginner friendly language and using practical analogies (personal note: "shared pencil" analogy written down in paper, maybe transfer it here later) to demystify race conditions and locks
- Nue's ethos of "accessibility without compromise" we will **not** silently insert locks or hide concurrency mechanics in ways that contradict what a user expects.

#### Some propossed syntax for later

```nue
private mutex m = mutex.new()
private number shared = 0

threads.spawn(function() {
    m.lock()
    shared += 1
    m.unlock()
})

threads.spawn(function() {
    m.lock()
    shared += 1
    m.unlock()
})

threads.waitAll() // waits for all threads to finish execution
print(shared) // should output 2, since we waited for both threads to finish
```

Whilst diving a bit more in-depth about proper mutex use and concurrency etc, I realised that concurrency is not that hard. It is just taught in a very convoluted way which beginners fail to understand. So to be honest, this isn't even a language feature, we just need transparent documentation.

## Mon 4 Aug 2025

The changes made today simplify Nue by removing redundant concepts such as `void` and introducing clearer, more intuitive control flow (`repeat until`), while reinforcing its focus on accessibility and logical design.

### Removal of the `void` keyword

The `void` keyword has been **entirely removed** from Nue. This aims to remove unnecessary duplication where we had a Java-style `void` vs `null` distinction which is entirely pointless in a language like Nue. `null` now more aligns with Lua's `nil`.

- Functions that previously used `-> void` should now omit a return type entirely:

  ```nue
    // before
    private function printMessage(string msg) -> void {
        print(msg)
    }

    // after
    private function printMessage(string msg) {
        print(msg)
    }
    ```

- Explicit `-> null` is now **illegal**, as there is no meaningful distinction between "no return value" and returning `null`, unlike before.
- `null` now fulfills all previous roles of `void`, covering implicit "no return value" from functions, explicit variable deallocation, and nullable types via unions like `[T, null]` or shorthand `T?`

### Addition of `repeat { ... } until <condition>`

Introduced the `repeat` and `until` keywords, adding support for "do-while"-style loops like in Lua:

```nue
private number x = 0

repeat {
    x++
} until x >= 5
```

This construct executes the block at least once before evaluating the condition.
