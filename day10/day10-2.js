const fs = require('fs');

const getSparseHash = input => {
    const hash      = Array(256).fill(0).map((_, i) => i),
          suffix    = [17, 31, 73, 47, 23],
          lengths   = input.trim().split('')
                        .map(x => x.charCodeAt())
                        .concat(suffix);

    let pos  = 0,
        skip = 0;

    for (let j = 0; j < 64; j++) {
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

            pos = (pos + len + skip) % hash.length;
            skip++;
        });
    }

    return hash;
};

const getDenseHash = hash => {
    let dense = "";

    for (let i = 0; i < 16; i++) {
        dense += hash
            .slice(i * 16, i * 16 + 16)
            .reduce((pre, cur) => pre ^ cur)
            .toString(16);
    }

    return dense;
};

fs.readFile('../inputs/day10_input.txt', 'utf8', (err, data) => {
    if (err) throw err;

    console.log(getDenseHash(getSparseHash(data)));
});
