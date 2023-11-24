use std::{
    io::{stdin, stdout, BufRead, BufWriter, Write},
    collections::HashSet,
};

fn unique(s: &str) -> bool {
    let mut set = HashSet::new();
    for c in s.chars() {
        if !set.insert(c) {
            return false;
        }
    }
    true
}

fn main() {
    let stdin = stdin();
    let stdout = stdout();
    let mut reader = Reader::from(stdin.lock());
    let mut writer = BufWriter::new(stdout.lock());

    let n: usize = reader.next();
    let mut result = "Neibb".to_string();
    let mut flag = false;
    for _ in 0..n {
        let s: String = reader.next();
        if !unique(&s) || s.len() < 5 || flag && (result.len() < s.len() || (result.len() == s.len() && result > s)) {
            continue;
        }
        result = s;
        flag = true;
    }
    writeln!(writer, "{}", result);
}

pub struct Reader<B> {
    reader: B,
    buffer: Vec<String>,
}

impl<R: BufRead> Reader<R> {
    pub fn next<T: std::str::FromStr>(&mut self) -> T {
        loop {
            if let Some(token) = self.buffer.pop() {
                return token.parse().ok().expect("Failed parse");
            }
            let mut input = String::new();
            self.reader.read_line(&mut input).expect("Failed read");
            self.buffer = input.split_whitespace().rev().map(String::from).collect();
        }
    }
}

impl<R: BufRead> From<R> for Reader<R> {
    fn from(reader: R) -> Self {
        Self { reader, buffer: Vec::new() }
    }
}
