const fs = require('fs');

fs.readFile('../inputs/day5_input.txt', 'utf8', logStepsTwist);

function logStepsTwist(err, data) {
    if (err) throw err;

    const jumps = data.trim().split('\n').map(Number);

    let steps = 0;
    for (let i = 0; i < jumps.length; ) {
        const t = i;
        i += jumps[i];

        jumps[t] += jumps[t] < 3 ? 1 : -1;
        steps++;
    }
    console.log(steps);
}
