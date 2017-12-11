const fs = require('fs');

fs.readFile('../inputs/day10_input.txt', 'utf8', (err, data) => {
    if (err) throw err;

    const list      = Array(256).fill(0).map((_, i) => i),
          suffix    = [17, 31, 73, 47, 23],
          lengths   = data.trim().split('')
                          .map(x => x.charCodeAt())
                          .concat(suffix);

    let pos  = 0,
        skip = 0;

    for (let j = 0; j < 64; j++) {
        lengths.forEach(len => {
            const subList = [];

            for (let i = pos; i < pos + len; i++) {
                subList.push(list[i % list.length]);
            }

            subList
                .reverse()
                .forEach((x, i) => {
                    list[(pos + i) % list.length] = x;
                });

            pos = (pos + len + skip) % list.length;
            skip++;
        });
    }

    let hexHash = "";
    for (let i = 0; i < 16; i++) {
        hexHash += list
            .slice(i * 16, i * 16 + 16)
            .reduce((pre, cur) => pre ^ cur)
            .toString(16);
    }

    console.log(hexHash);
});
