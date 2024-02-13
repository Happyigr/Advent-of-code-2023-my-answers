use std::fmt::Display;

#[derive(Debug)]
pub struct MyMap2<'a> {
    map: Vec<&'a str>,
}

impl<'a> Display for MyMap2<'a> {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        for line in self.map.iter() {
            write!(f, "\n{}", line)?;
        }
        Ok(())
    }
}

impl<'a> MyMap2<'a> {
    pub fn new(map: &str) -> MyMap2 {
        MyMap2 {
            map: map.split("\n").filter(|m| m != &"").collect(),
        }
    }
    pub fn find_mirror_vert(&self) -> Result<(usize, usize), String> {
        for i in 0..self.map[0].len() - 1 {
            let mut glitch = 0;
            let mut is_mirror = true;

            for line in self.map.iter() {
                if line[i..i + 1] != line[i + 1..i + 2] {
                    if glitch == 1 {
                        is_mirror = false;
                        break;
                    }
                    glitch += 1;
                }
            }

            if is_mirror {
                let mirror = (i, i + 1);
                println!("\nvert mirror found on ({},{})", i, i + 1);
                if self.check_vert_mirror(&mirror) {
                    println!("Mirror is real");
                    return Ok(mirror);
                }
                println!("Mirror isn't real");
            }
        }
        return Err(String::from("\nThe vert mirror is not found"));
    }

    fn check_vert_mirror(&self, mirror: &(usize, usize)) -> bool {
        let symbols_left = mirror.0 + 1;
        let min_length = symbols_left.min(self.map[0].len() - symbols_left);
        let mut glitches = 0;

        for line in self.map.iter() {
            let left = &line[(mirror.0 + 1 - min_length)..mirror.0 + 1];
            // reversing the right part of line
            let mut right: Vec<char> = line[mirror.1..(mirror.1 + min_length)]
                .chars()
                // .clone()
                .collect();
            right.reverse();
            let right: String = right.into_iter().collect();

            // println!("Left part: {:?}\nRight part: {:?}", left, right);

            if left != right {
                for i in 0..left.len() {
                    if left[i..i + 1] != right[i..i + 1] {
                        glitches += 1;
                    }
                }
            }
        }

        glitches == 1
    }

    pub fn find_mirror_horiz(&self) -> Result<(usize, usize), String> {
        for i in 0..(self.map.len() - 1) {
            let mut glitch = 0;
            for ch_i in 0..self.map[0].len() {
                if self.map[i][ch_i..ch_i + 1] != self.map[i + 1][ch_i..ch_i + 1] {
                    glitch += 1;
                }
            }

            if glitch <= 1 {
                println!("\nhoriz mirror founded on ({}, {})", i, i + 1);

                let mirror = (i, i + 1);
                if self.check_mirror_horiz(&mirror) {
                    println!("Mirror is real");
                    return Ok((i, i + 1));
                }

                println!("Mirror isn't real")
            }
        }
        return Err(String::from("\nThe horizontal mirror is not found"));
    }

    fn check_mirror_horiz(&self, mirror: &(usize, usize)) -> bool {
        let lines_bottom = self.map.len() - mirror.1;
        let mut glitches = 0;

        // for i to the nearest end of map
        for i in 0..lines_bottom.min(self.map.len() - lines_bottom) {
            if self.map[mirror.0 - i] != self.map[mirror.1 + i] {
                for ch_i in 0..self.map[0].len() {
                    if self.map[mirror.0 - i][ch_i..ch_i + 1]
                        != self.map[mirror.1 + i][ch_i..ch_i + 1]
                    {
                        glitches += 1;
                    }
                }
            }
        }
        println!("Glitches = {glitches}");

        glitches == 1
    }
}
