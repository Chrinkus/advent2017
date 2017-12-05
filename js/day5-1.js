const fs = require('fs');

fs.readFile('../inputs/day5_input.txt', 'utf8', logSteps);

function logSteps(err, data) {
    if (err) throw err;

    const jumps = data.trim().split('\n').map(j => Number(j));

    let steps = 0;
    for (let i = 0; i < jumps.length; ) {
        i += jumps[i]++;    // @_@
        steps++;
    }
    console.log(steps);
}
