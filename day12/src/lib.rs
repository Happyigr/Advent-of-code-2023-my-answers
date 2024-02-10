use itertools::Itertools;
use std::fs;

fn setup() -> Vec<SpringLine> {
    SpringLine::new_vec_from_file(String::from("example.txt"))
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn correct_reading() {
        let example_springs = SpringLine::new(&String::from("?###???????? 3,2,1"));
        assert_eq!(String::from("?###????????"), example_springs.springs);
        assert_eq!(vec![3, 2, 1], example_springs.nums);
    }

    #[test]
    fn correct_checking() {
        let example = SpringLine::new(".##.###.# 2,3,1");
        assert_eq!(true, check_correct(&example.springs, &example.nums));
    }

    #[test]
    #[ignore]
    fn correct_calculating1() {
        let example_springs = setup();
        let ans_vec: Vec<u32> = vec![1, 4, 1, 1, 4, 10];
        let mut calculated_vec: Vec<u32> = vec![];

        for spring_line in example_springs.iter() {
            calculated_vec.push(spring_line.calculate1());
        }

        assert_eq!(ans_vec, calculated_vec);
    }

    #[test]
    #[ignore]
    fn correct_calculating2() {
        let example_springs = setup();
        let ans_vec: Vec<u32> = vec![1, 16384, 1, 16, 2500, 506250];
        let mut calculated_vec: Vec<u32> = vec![];

        for spring_line in example_springs.iter() {
            calculated_vec.push(spring_line.calculate2());
        }

        assert_eq!(ans_vec, calculated_vec);
    }
}

pub fn check_correct(line: &String, nums: &Vec<u32>) -> bool {
    let mut hashes: Vec<u32> = vec![];
    let mut founded_hashes: u32 = 0;

    for ch in line.chars() {
        match ch {
            '#' => founded_hashes += 1,
            '.' => {
                if founded_hashes != 0 {
                    hashes.push(founded_hashes);
                    founded_hashes = 0;
                }
            }
            _ => eprintln!("wrong reading off the line {} with nums: {:?}", line, nums),
        }
    }

    if founded_hashes != 0 {
        hashes.push(founded_hashes);
    }

    hashes == *nums
}
#[derive(Debug)]
pub struct SpringLine {
    pub springs: String,
    pub nums: Vec<u32>,
}

impl SpringLine {
    pub fn calculate2(&self) -> u32 {
        let mut ans: u32 = 0;

        let first_combos = self.calculate1();
        let added_springs;

        if self.springs.chars().next() == Some('?') && self.springs.chars().last() == Some('?') {
            added_springs = SpringLine {
                springs: format!("?{}?", self.springs),
                nums: self.nums.clone(),
            };
        } else {
            added_springs = SpringLine {
                springs: if self.springs.chars().last() == Some('#') {
                    format!(".{}", self.springs)
                } else {
                    format!("?{}", self.springs)
                },
                nums: self.nums.clone(),
            };
        }
        println!("{:?}", added_springs);
        let added_combos = added_springs.calculate1();
        ans += first_combos * added_combos.pow(4);

        ans
    }

    pub fn calculate1(&self) -> u32 {
        let mut ans: u32 = 0;
        let question_marks = self
            .springs
            .chars()
            .enumerate()
            .filter(|(_, ch)| *ch == '?')
            .map(|(i, _)| i);

        // let hashes_dots = self.springs.replace('?', ".");
        let powerset = Itertools::powerset(question_marks.clone());

        // println!("{hashes_dots}");
        // println!("questions - {:?}", question_marks);

        for set in powerset {
            let mut new_map = self.springs.replace('?', ".");
            for place in set {
                new_map.replace_range(place..place + 1, "#");
            }
            if check_correct(&new_map, &self.nums) {
                ans += 1;
            }
        }

        ans
    }

    pub fn new_vec_from_file(file_path: String) -> Vec<SpringLine> {
        fs::read_to_string(file_path)
            .unwrap()
            .lines()
            .map(|line| SpringLine::new(line))
            .collect()
    }

    pub fn new(line: &str) -> SpringLine {
        let changed_line: Vec<&str> = line.split_whitespace().collect();

        SpringLine {
            springs: changed_line[0].to_string(),
            nums: changed_line[1]
                .split(',')
                .map(|n| n.parse().expect("Failed to parse integer"))
                .collect(),
        }
    }
}
