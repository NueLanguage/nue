function epoch() {
    return Date.now() / 1000;
}

const startedAt = epoch();

let total = 0n;
for (let i=1n; i <= 1000000000n; i++) {
    total += i * i;
}

const taken = epoch() - startedAt;
console.log(`JavaScript took ${taken} seconds to run the 1 billion benchmark!`);
console.log(`Total: ${total}`);