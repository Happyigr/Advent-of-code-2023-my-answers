use day12::SpringLine;

fn main() {
    let input_springs = SpringLine::new_vec_from_file(String::from("input.txt"));

    // println!("The refracted data are: {:?}", input_springs);
    let mut ans: u32 = 0;

    for spring_iter in input_springs.iter() {
        println!("{:?}", spring_iter);
        ans += spring_iter.calculate2();
    }

    println!("{ans}");
}
