use std::fs;

use day13::part1::MyMap1;
use day13::part2::MyMap2;

fn main() {
    let content = fs::read_to_string("input.txt").unwrap();

    let content: Vec<&str> = content.split("\n\n").collect();

    // part 1
    // let content: Vec<MyMap1> = content.iter().map(|m| return MyMap1::new(m)).collect();
    // part 2
    let content: Vec<MyMap2> = content.iter().map(|m| return MyMap2::new(m)).collect();

    // println!("{:?}", content);

    let mut ans = 0;
    for map in content {
        println!("\n{}", map);

        let hor_mirror = map.find_mirror_horiz().unwrap_or_else(|e| {
            println!("{e}");
            return (0, 0);
        });
        let vert_mirror = map.find_mirror_vert().unwrap_or_else(|e| {
            println!("{e}");
            return (0, 0);
        });

        ans += hor_mirror.1 * 100 + vert_mirror.1;
    }

    println!("\n\nAns = {ans}");
}
