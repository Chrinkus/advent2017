const fs = require('fs');

const getHash = input => {
    const lengths = data.split(',').map(Number),
          hash = Array(256).fill(0).map((_, i) => i);

    let pos = 0,
        skip = 0;

    lengths.forEach(len => {
        const subList = [];

        for (let i = pos; i < pos + len; i++) {
            subList.push(hash[i % hash.length]);
        }

        subList
            .reverse()
            .forEach((x, i) => {
                hash[(pos + i) % hash.length] = x;
            });

        pos += len + skip;
        skip++;
    });

    return hash;
});

fs.readFile('../inputs/day10_input.txt', 'utf8', (err, data) => {
    if (err) throw err;

    const hash = getHash(data);

    console.log(hash[0] * hash[1]);
});
