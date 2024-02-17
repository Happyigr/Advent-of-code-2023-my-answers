use std::collections::HashMap;

use day14::{Dirs, Platform};

fn main() {
    let mut platform = Platform::new("input.txt");
    let dirs = vec![Dirs::North, Dirs::West, Dirs::South, Dirs::East];

    let loops = ((1000000000 - 98) % 36) + 98;

    for i in 1..=loops {
        for dir in dirs.iter() {
            platform.tilt(dir);
        }
        println!("{i} {}", platform.count_stones());
    }

    println!("{}", platform.count_stones());

    // if we start this code we will find, that there are a pattern, with this pattern we can
    // calculate the end platform with a little it of math
    // let mut results: HashMap<usize, i32> = HashMap::new();
    // for i in 1..100000000 {
    //     for dir in dirs.iter() {
    //         platform.tilt(dir);
    //     }
    //     // println!("{platform}");
    //
    //     if results.contains_key(&platform.count_stones()) {
    //         println!(
    //             "iteration {} has the equal pattern to {}",
    //             i,
    //             results[&platform.count_stones()]
    //         );
    //     } else {
    //         results.insert(platform.count_stones(), i);
    //     }
    //     // println!("\n{i}\n");
    // }
}
