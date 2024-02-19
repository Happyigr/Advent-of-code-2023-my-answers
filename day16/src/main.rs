use std::sync::{Arc, Mutex};
use std::thread;
use std::{collections::HashSet, fs};

#[derive(PartialEq, Debug, Clone, Copy)]
enum Dirs {
    Left,
    Right,
    Up,
    Down,
}

#[derive(Clone, PartialEq, Hash, Eq, Debug, Copy)]
struct Pos(usize, usize);

#[derive(Debug)]
struct Beam {
    dir: Dirs,
    pos: Pos,
    route: Vec<(Pos, Dirs)>,
}

impl Beam {
    fn new(pos: Pos, dir: Dirs) -> Beam {
        Beam {
            pos,
            dir,
            route: vec![],
        }
    }

    fn spawn(
        mut self,
        map: &Arc<Mutex<Vec<String>>>,
        route: &Arc<Mutex<HashSet<Pos>>>,
        splitters: &Arc<Mutex<HashSet<Pos>>>,
    ) {
        // getting the ownership of the all values
        let beam_map = Arc::clone(&map);
        let beam_route = Arc::clone(&route);
        let beam_splitters = Arc::clone(&splitters);

        // making the thread with logic of beam
        let handle = thread::spawn(move || loop {
            // Err if the move isn't correct,
            // Ok(None) if there are non new beams spawned and
            // Ok(Some(Beam)) if there a beam was spawned
            let res = self.move_check_new_beam(&beam_map, &beam_splitters);

            // if is Err we add the route of beam to the general route and stop the beam
            if res.is_err() {
                let mut r = beam_route.lock().unwrap();
                for place in self.route {
                    r.insert(place.0);
                }
                break;
            // if there are a beam to spawn, spawn it
            } else if res.as_ref().unwrap().is_some() {
                let beam = res.unwrap().take().unwrap();
                beam.spawn(&beam_map, &beam_route, &beam_splitters);
            }
        });

        // start the thread and making sure that the programm will not stop before the thread is
        // ended
        handle.join().unwrap();
    }

    // the start pos will be written in route
    fn move_check_new_beam(
        &mut self,
        map: &Arc<Mutex<Vec<String>>>,
        splitters: &Arc<Mutex<HashSet<Pos>>>,
    ) -> Result<Option<Beam>, ()> {
        // inserting the current position of beam in the route
        self.route.push((self.pos.clone(), self.dir.clone()));

        // checking if there can be a negative value of usize or the beam had visited
        if self.dir == Dirs::Left && self.pos.0 == 0 || self.dir == Dirs::Up && self.pos.1 == 0 {
            return Err(());
        }

        // changing the pos of beam
        self.pos = match self.dir {
            Dirs::Left => Pos(self.pos.0 - 1, self.pos.1),
            Dirs::Right => Pos(self.pos.0 + 1, self.pos.1),
            Dirs::Up => Pos(self.pos.0, self.pos.1 - 1),
            Dirs::Down => Pos(self.pos.0, self.pos.1 + 1),
        };

        // unlock splitters
        let mut s = splitters.lock().unwrap();

        // checking if the beam had visited this position
        if s.contains(&self.pos) {
            return Err(());
        }

        // locking the map
        let m = map.lock().unwrap();

        // checking if the pos out of the length of map
        if self.pos.1 >= m.len() || self.pos.0 >= m[0].len() {
            return Err(());
        }

        // getting the symbol on which the beam will be
        let sym = &m[self.pos.1][self.pos.0..self.pos.0 + 1];

        // if this is the splitter, then add it to visited spliters, to prevent the infinite loop
        match sym {
            "|" | "-" => s.insert(self.pos),
            _ => true, // do nothing
        };

        // calculating what we should do in dependencies of the direction and char
        match (sym, &self.dir) {
            (".", _) => return Ok(None),
            ("|", Dirs::Down) | ("|", Dirs::Up) => return Ok(None),
            ("-", Dirs::Left) | ("-", Dirs::Right) => return Ok(None),

            ("|", Dirs::Left) | ("|", Dirs::Right) => {
                self.dir = Dirs::Up;
                return Ok(Some(Beam::new(self.pos.clone(), Dirs::Down)));
            }

            ("-", Dirs::Up) | ("-", Dirs::Down) => {
                self.dir = Dirs::Left;
                return Ok(Some(Beam::new(self.pos.clone(), Dirs::Right)));
            }

            ("\\", Dirs::Right) | ("/", Dirs::Left) => {
                self.dir = Dirs::Down;
                return Ok(None);
            }
            ("\\", Dirs::Left) | ("/", Dirs::Right) => {
                self.dir = Dirs::Up;
                return Ok(None);
            }
            ("\\", Dirs::Down) | ("/", Dirs::Up) => {
                self.dir = Dirs::Right;
                return Ok(None);
            }
            ("\\", Dirs::Up) | ("/", Dirs::Down) => {
                self.dir = Dirs::Left;
                return Ok(None);
            }

            (_, _) => unimplemented!("not implemented yet"),
        }
    }
}

fn main() {
    let content = fs::read_to_string("input.txt").unwrap();

    let content: Vec<String> = content
        .split("\n")
        .filter(|l| l != &"")
        .map(|l| l.to_string())
        .collect();

    let ans: Arc<Mutex<usize>> = Arc::new(Mutex::new(0));

    // starting the beams from up and down
    for i in 0..content[0].len() {
        let a = ans.clone();
        let map = content.clone();

        let handle = thread::spawn(move || start_beam(map, Pos(i, 0), Dirs::Down, a));

        let a1 = ans.clone();
        let map1 = content.clone();

        let l = content.len() - 1;
        let handle1 = thread::spawn(move || start_beam(map1, Pos(i, l), Dirs::Up, a1));

        handle.join().unwrap();
        handle1.join().unwrap();
    }

    // starting the beams from left and right
    for i in 0..content.len() {
        let a = ans.clone();
        let map = content.clone();

        let handle = thread::spawn(move || start_beam(map, Pos(0, i), Dirs::Right, a));

        let a1 = ans.clone();
        let map1 = content.clone();

        let l = content[0].len() - 1;
        let handle1 = thread::spawn(move || start_beam(map1, Pos(l, i), Dirs::Left, a1));

        handle.join().unwrap();
        handle1.join().unwrap();
    }

    println!("{}", ans.lock().unwrap());
    // let c = content.lock().unwrap();
    // let r = route.lock().unwrap();
    //
    // for y in 0..c.len() as usize {
    //     for x in 0..c[0].len() as usize {
    //         if r.contains(&Pos(x, y)) {
    //             print!("#");
    //         } else {
    //             print!(".");
    //         }
    //     }
    //     println!("");
    // }
    //
    // println!("{}", r.len());
}

fn start_beam(content: Vec<String>, pos: Pos, dir: Dirs, ans: Arc<Mutex<usize>>) {
    let content: Arc<Mutex<Vec<String>>> = Arc::new(Mutex::new(content));

    let route: Arc<Mutex<HashSet<Pos>>> = Arc::new(Mutex::new(HashSet::new()));

    let splitters: Arc<Mutex<HashSet<Pos>>> = Arc::new(Mutex::new(HashSet::new()));

    let start_beam = Beam::new(pos, dir);

    start_beam.spawn(&content, &route, &splitters);

    let mut a = ans.lock().unwrap();

    *a = a.max(route.lock().unwrap().len());
}
