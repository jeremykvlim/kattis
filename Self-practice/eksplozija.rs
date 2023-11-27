use std::{
    io::{stdin, BufRead},
    collections::VecDeque,
};

fn main() {
    let mut input = stdin().lock().lines();
    let s: Vec<char> = input.next().unwrap().unwrap().chars().collect();
    let explosion: Vec<char> = input.next().unwrap().unwrap().chars().collect();

    let mut remaining: VecDeque<char> = VecDeque::new();
    let mut state: Vec<usize> = vec![0];

    for c in s {
        remaining.push_back(c);
        if c == explosion[state[state.len() - 1]] {
            state.push(state[state.len() - 1] + 1);
        } else {
            state.push(if c == explosion[0] { 1 } else { 0 });
        }

        if state[state.len() - 1] == explosion.len() {
            state.truncate(state.len() - explosion.len());
            for _ in 0..explosion.len() {
                remaining.pop_back();
            }
        }
    }

    print!("{}", if remaining.is_empty() { "FRULA".to_string() } else { remaining.into_iter().collect() });
}
