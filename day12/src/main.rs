use std::collections::HashMap;

use day12::{Cache, SpringLine};

fn main() {
    let mut input_springs = SpringLine::new_vec_from_file(String::from("input.txt"));
    let mut cache = Cache {
        cache: HashMap::new(),
    };

    // println!("The refracted data are: {:?}", input_springs);
    let mut ans: usize = 0;

    // part 1
    // for spring_iter in input_springs.iter() {
    //     ans += spring_iter.calculate1();
    // }

    // part 2
    for spring_iter in input_springs.iter_mut() {
        spring_iter.replicate(5);
        println!("{:?}", spring_iter);
        ans += cache.calculate_dyn(&spring_iter.springs, &spring_iter.nums, None);
    }

    println!("{ans}");
}
