private var table time = import("time")

private var function benchmark = function() -> number {
    private var number startedAt = time.unix()

    private var number total = 0
    for i in range(1, 1000000000) {
        total += i ^ 2
    }

    return time.unix() - startedAt
}

print("nue took " + tostring(benchmark) + " seconds to run the 1 billion benchmark.")