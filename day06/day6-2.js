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
    let times = 0;

    while (true) {
        const snap = getSnapShot(arr);      // convert to string for object
                                            // storage & comparison
        if (seen[snap] && ++times === 2) {
            return cycles - seen[snap];
        }

        seen[snap] = cycles;

        redistributeBlocks(arr);

        cycles++;
    }
}

function getSnapShot(arr) {
    // convert array of integers to string with spaces
    return arr.reduce((snapShot, n) => snapShot += n + ' ', "");
}

function redistributeBlocks(arr) {
    let max = getMax(arr);

    const loc = arr.indexOf(max),
          l = arr.length;

    arr[loc] = 0;                           // "remove" block
    for (let i = loc + 1; max > 0; i++) {
        i = i % l;                          // handle wrap
        arr[i]++;
        max--;
    }
}

function getMax(arr) {
    return arr.reduce((max, next) => next > max ? next : max);
}
