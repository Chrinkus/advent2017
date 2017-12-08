const fs = require('fs');

fs.readFile('../inputs/day6_input.txt', 'utf8', logCycles);

function logCycles(err, data) {
    if (err) throw err;

    const dataArr = data.trim().split(/\s+/).map(Number);

    console.log(getCycles(dataArr));
}

function getCycles(arr) {
    let cycles = 0;
    const seen = {};

    while (true) {
        const snap = arr.join('-');

        if (seen[snap]) return cycles;
        seen[snap] = cycles;

        redistributeBlocks(arr);

        cycles++;
    }
}

function redistributeBlocks(arr) {
    let max = Math.max(...arr);

    const loc = arr.indexOf(max),
          l = arr.length;

    arr[loc] = 0;                           // "remove" block
    for (let i = loc + 1; max > 0; i++) {
        i = i % l;                          // handle wrap
        arr[i]++;
        max--;
    }
}
