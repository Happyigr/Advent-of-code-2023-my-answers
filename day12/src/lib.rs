use itertools::Itertools;
use std::{collections::HashMap, fs};

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
        let mut example_springs = setup();
        let ans_vec: Vec<usize> = vec![1, 16384, 1, 16, 2500, 506250];
        let mut calculated_vec: Vec<usize> = vec![];
        let mut cache = Cache {
            cache: HashMap::new(),
        };

        for spring_line in example_springs.iter_mut() {
            spring_line.replicate(5);
        }

        for spring_line in example_springs.iter() {
            calculated_vec.push(cache.calculate_dyn(&spring_line.springs, &spring_line.nums, None));
        }

        assert_eq!(ans_vec, calculated_vec);
    }
}

pub fn check_correct(line: &str, nums: &Vec<usize>) -> bool {
    let mut hashes: Vec<usize> = vec![];
    let mut founded_hashes: usize = 0;

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

pub struct Cache {
    pub cache: HashMap<(String, Vec<usize>, Option<usize>), usize>,
}

impl Cache {
    // is_group has 3 states:
    // None - the group is not started
    // Some(_) - the group is started
    // Some(0) - the group is ended
    pub fn calculate_dyn(
        &mut self,
        springs: &str,
        nums: &[usize],
        group_length: Option<usize>,
    ) -> usize {
        // if there are no groups more and the group is not started and there are no hashes more
        // return 1
        if nums.len() == 0 && group_length.is_none() {
            let hashes_left = springs.find('#').is_some();

            return if hashes_left { 0 } else { 1 };
        }

        // if there are no symbols more and the group is started or ended and there are no
        // groups more return 1
        if springs.is_empty() {
            match group_length {
                Some(0) | None => {
                    if nums.is_empty() {
                        return 1;
                    } else {
                        return 0;
                    }
                }
                Some(_) => return 0,
            }
        }

        // the behavior of the function in depences of the current sym and group_length
        match (&springs[0..1], group_length) {
            (".", None) => return self.cached_calculate_dyn(&springs[1..], &nums, None),
            (".", Some(0)) => return self.cached_calculate_dyn(&springs[1..], &nums, None),
            (".", Some(_)) => return 0,

            ("#", None) => return self.cached_calculate_dyn(springs, &nums[1..], Some(nums[0])),
            ("#", Some(0)) => return 0,
            ("#", Some(v)) => return self.cached_calculate_dyn(&springs[1..], &nums, Some(v - 1)),

            ("?", None) => {
                return self.cached_calculate_dyn(springs, &nums[1..], Some(nums[0]))
                    + self.cached_calculate_dyn(&springs[1..], nums, None)
            }
            ("?", Some(0)) => return self.cached_calculate_dyn(&springs[1..], &nums, None),
            ("?", Some(v)) => return self.cached_calculate_dyn(&springs[1..], &nums, Some(v - 1)),

            (_, _) => unimplemented!("This not implemented yet"),
        }
    }

    fn cached_calculate_dyn(
        &mut self,
        springs: &str,
        nums: &[usize],
        group_length: Option<usize>,
    ) -> usize {
        if self
            .cache
            .contains_key(&(springs.to_string(), nums.to_vec(), group_length))
        {
            return *self
                .cache
                .get(&(springs.to_string(), nums.to_vec(), group_length))
                .unwrap();
        }

        let result = self.calculate_dyn(springs, nums, group_length);
        self.cache
            .insert((springs.to_string(), nums.to_vec(), group_length), result);

        return result;
    }
}

#[derive(Debug)]
pub struct SpringLine {
    pub springs: String,
    pub nums: Vec<usize>,
}

impl SpringLine {
    pub fn replicate(&mut self, num: usize) {
        let s = &self.springs.clone();
        let nums = &self.nums.clone();

        println!("{:?}", self.nums);
        for _i in 0..(num - 1) {
            self.springs.push('?');
            for n in nums {
                self.nums.push(*n);
            }
            self.springs.push_str(&s);
        }

        print!(" is than {:?}", self.nums);
        println!("{s} is than {}", self.springs);
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
