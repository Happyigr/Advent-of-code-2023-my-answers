use std::{collections::HashMap, fs, u128};

fn main() {
    let content = get_content("input.txt");

    let mut boxes: HashMap<u8, Vec<String>> = HashMap::new();
    let mut lens_power: HashMap<String, u8> = HashMap::new();

    for word in content.clone().into_iter() {
        if word.find('-').is_some() {
            let label = &word[..word.len() - 1];
            if let Some(b) = boxes.get_mut(&get_hash(label)) {
                b.retain(|lab| lab != &label);
            }
        } else if word.find('=').is_some() {
            let label = String::from(&word[..word.len() - 2]);
            let power: u8 = word[word.len() - 1..].parse::<u8>().unwrap();

            if let Some(p) = lens_power.get_mut(&label) {
                *p = power;
            } else {
                lens_power.insert(label.clone(), power);
            }

            if let Some(b) = boxes.get_mut(&get_hash(&label)) {
                if b.contains(&label) {
                    b.retain(|lab| lab != &label);
                    b.insert(0, label);
                } else {
                    b.push(label);
                }
            } else {
                boxes.insert(get_hash(&label), vec![label]);
            }
        } else {
            unimplemented!("The - or = not found");
        }
    }

    println!("{:?}", boxes);
    println!("{:?}", lens_power);

    let mut ans = 0;
    for i in 0..=255 {
        if let Some(b) = boxes.get(&i) {
            for (num, lens) in b.iter().enumerate() {
                let box_num = i as u32 + 1;
                let slot = num + 1;
                let power = *lens_power.get(lens).unwrap();

                let res = box_num as u128 * slot as u128 * power as u128;
                println!("{lens} = {res}");
                ans += res;
            }
        }
    }
    println!("{ans}");
}

fn get_hash(word: &str) -> u8 {
    let mut cur_val: u32 = 0;
    for ch in word.chars() {
        cur_val = (cur_val + (ch as u32)) * 17 % 256;
    }

    cur_val as u8
}

fn get_content(file: &str) -> Vec<String> {
    let content = fs::read_to_string(file).unwrap();
    let content = content.replace("\n", "");

    content.split(",").map(|w| w.to_string()).collect()
}
