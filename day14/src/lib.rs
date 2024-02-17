use core::fmt::Display;
use std::{fs, usize};

pub enum Dirs {
    North,
    West,
    South,
    East,
}

#[derive(Debug)]
pub struct Platform {
    pub map: Vec<String>,
    empty_map: Vec<String>,
    stones: Vec<(usize, usize)>,
    hashes: Vec<(usize, usize)>,
}

impl Display for Platform {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        for (l1, l2) in self.map.iter().zip(self.empty_map.iter()) {
            write!(f, "\n{}    {}", l1, l2)?;
        }
        Ok(())
    }
}

impl Platform {
    pub fn new(path_to_file: &str) -> Platform {
        let content = fs::read_to_string(path_to_file).unwrap();
        let map: Vec<String> = content
            .split("\n")
            .filter(|l| l != &"")
            .map(|l| l.to_string())
            .collect();

        let empty_map: Vec<String> = map.iter().map(|l| l.replace("O", ".")).collect();

        let mut stones: Vec<(usize, usize)> = vec![];
        let mut hashes: Vec<(usize, usize)> = vec![];

        for (y, line) in map.iter().enumerate() {
            for (x, ch) in line.chars().enumerate() {
                if ch == 'O' {
                    stones.push((x, y));
                }
                if ch == '#' {
                    hashes.push((x, y));
                }
            }
        }

        Platform {
            map,
            empty_map,
            stones,
            hashes,
        }
    }

    pub fn count_stones(&self) -> usize {
        let mut ans = 0;
        for stone in self.stones.iter() {
            ans += self.map.len() - stone.1;
        }
        ans
    }

    pub fn tilt(&mut self, dir: &Dirs) {
        let mut tilted_stones: Vec<(usize, usize)> = vec![];

        // we sorting stones so that the first stone will be near to the target border by current
        // direction
        self.sort_stones(&dir);

        for stone in self.stones.iter() {
            // calculating the route to the nearest border with current direction
            let route: Vec<(usize, usize)> = match dir {
                Dirs::North => (0..=stone.1).rev().map(|y| (stone.0, y)).collect(),
                Dirs::South => (stone.1..self.map.len()).map(|y| (stone.0, y)).collect(),
                Dirs::West => (0..=stone.0).rev().map(|x| (x, stone.1)).collect(),
                Dirs::East => (stone.0..self.map[0].len()).map(|x| (x, stone.1)).collect(),
            };

            // if the route has only the initial position of the stone then it will be on this
            // position
            if route.len() == 1 {
                tilted_stones.push(*stone);
                continue;
            }

            // dont know how to do it with windows
            // for pos in route.windows(2) {}

            // we check if there are the hashes or stones in the route and then if it is so set the
            // stone in the place of the previous route
            for i in 1..route.len() {
                if tilted_stones.contains(&route[i]) || self.hashes.contains(&route[i]) {
                    tilted_stones.push(route[i - 1]);
                    break;
                }
                // if it is the end of the route then we place the stone on the end of the route
                if i == route.len() - 1 {
                    tilted_stones.push(route[i]);
                }
            }
        }

        let mut content: Vec<String> = self.empty_map.clone();

        // for the eyes
        for stone in tilted_stones.iter() {
            content[stone.1].replace_range(stone.0..stone.0 + 1, "O");
        }

        self.stones = tilted_stones;
        self.map = content;
    }

    fn sort_stones(&mut self, dir: &Dirs) {
        match dir {
            // the nearest to the upper side is first
            Dirs::North => self.stones.sort_by(|a, b| {
                if a.1 != b.1 {
                    a.1.cmp(&b.1)
                } else {
                    a.0.cmp(&b.0)
                }
            }),
            // the nearest to the left side is first
            Dirs::West => self.stones.sort_by(|a, b| {
                if a.0 != b.0 {
                    a.0.cmp(&b.0)
                } else {
                    a.1.cmp(&b.1)
                }
            }),
            // the nearest to the down side is first
            Dirs::South => self.stones.sort_by(|a, b| {
                if a.1 != b.1 {
                    b.1.cmp(&a.1)
                } else {
                    a.0.cmp(&b.0)
                }
            }),
            // the nearest to the right side is first
            Dirs::East => self.stones.sort_by(|a, b| {
                if a.0 != b.0 {
                    b.0.cmp(&a.0)
                } else {
                    a.1.cmp(&b.1)
                }
            }),
        }
    }
}
