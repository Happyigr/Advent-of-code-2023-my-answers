use std::{fs, ops::Deref};

#[derive(Debug)]
struct Lens {
    label: String,
    power: usize,
}

#[derive(Debug)]
struct LensBox {
    lenses: Vec<Lens>,
}

impl LensBox {
    pub fn add_lens(&mut self, label: String, power: usize) {
        for lens in self.lenses.iter_mut() {
            if lens.label == label {
                lens.power = power;
                return;
            }
        }
        self.lenses.push(Lens { label, power });
    }

    pub fn remove_lens(&mut self, label: &str) {
        self.lenses.retain(|lens| lens.label != label);
    }
}

impl Deref for LensBox {
    type Target = Vec<Lens>;

    fn deref(&self) -> &Self::Target {
        &self.lenses
    }
}

fn main() {
    let content = get_content("input.txt");

    let mut boxes: Vec<LensBox> = vec![];

    for _ in 0..=255 {
        boxes.push(LensBox { lenses: vec![] });
    }

    for word in content.iter() {
        if word.contains("=") {
            let lens: Vec<_> = word.split("=").collect();
            let (label, power) = (lens[0], lens[1].parse::<usize>().unwrap());

            boxes[get_hash(label)].add_lens(label.to_string(), power);
        }
        if word.contains("-") {
            let label = &word[..word.len() - 1];
            boxes[get_hash(label)].remove_lens(label);
        }
    }

    // println!("{:?}", boxes);

    let mut ans = 0;
    for (b_i, b) in boxes.iter().enumerate() {
        for (l_i, lens) in b.iter().enumerate() {
            ans += (b_i + 1) * (l_i + 1) * lens.power;
        }
    }

    println!("{ans}");
}

fn get_hash(word: &str) -> usize {
    let mut cur_val = 0;
    for ch in word.chars() {
        cur_val = (cur_val + (ch as usize)) * 17 % 256;
    }

    cur_val
}

fn get_content(file: &str) -> Vec<String> {
    let content = fs::read_to_string(file).unwrap();
    let content = content.replace("\n", "");

    content.split(",").map(|w| w.to_string()).collect()
}
