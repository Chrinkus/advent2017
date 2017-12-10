const fs = require('fs');

fs.readFile('../inputs/day10_input.txt', 'utf8', (err, data) => {

    const lengths = data.split(',').map(Number),
          list = Array(256).fill(0).map((_, i) => i);

    let pos = 0,
        skip = 0;

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

        pos += len + skip;
        skip++;
    });

    console.log(list[0] * list[1]);
});
