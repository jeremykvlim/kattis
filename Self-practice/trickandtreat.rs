use std::{
    io::{stdin, BufRead},
    cmp,
};

fn main() {
    let stdin = stdin();
    let mut reader = Reader::from(stdin.lock());

    let n: i32 = reader.next();
    let m: i32 = reader.next();

    let mut requests = vec![0; n as usize];
    for i in 0..n as usize {
        requests[i] = reader.next();
    }

    let mut fewest = i32::MAX;
    for i in (m - 9)..=m {
        let mut candies = i;
        let mut thrown = 0;
        for j in 0..n as usize {
            if requests[j] > candies {
                thrown += 1;
            } else {
                candies -= requests[j];
            }
        }
        fewest = cmp::min(fewest, thrown);
    }
    print!("{}", fewest);
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
