function epoch(): number {
    return Date.now() / 1000;
}

async function main() {
    const started = epoch();

    let total = BigInt(0);
    for (let i = BigInt(1); i <= BigInt(1000000000); i++) {
        total += BigInt(i * i);
    }

    const taken = epoch() - started;

    console.log(`TypeScript took ${taken.toFixed(9)} seconds to calculate the sum of squares from 1 to 1 billion:`);
    console.log(`Total: ${total.toString()}`);
}

main();