use std::{
    io::{stdin, stdout, BufRead, BufWriter, Write},
    cmp::min,
};

fn complete(tasks: &Vec<(i64, i64)>, total: i64, resets: &mut i64, attempts: i64, mut c: i64) -> bool {
    let mut remaining = attempts * c;
    let mut total = total;
    for &(t, d) in tasks {
        let spent = min(t / d + (t % d != 0) as i64, attempts);
        if remaining < spent { return false; }
        remaining -= spent;
        total -= min(t, attempts * d);
        if spent == attempts { c -= 1; }
    }
    if total > min(remaining, c) { return false; }
    *resets = attempts;
    true
}

fn main() {
    let stdin = stdin();
    let stdout = stdout();
    let mut reader = Reader::from(stdin.lock());
    let mut writer = BufWriter::new(stdout.lock());

    let n: usize = reader.next();
    let c: i64 = reader.next();

    let mut total = 0;
    let mut tasks = vec![(0, 0); n];
    for task in &mut tasks {
        task.0 = reader.next();
        task.1 = reader.next();
        total += task.0;
    }

    let (mut l, mut r, mut resets) = (1, 1, 0);
    while !complete(&tasks, total, &mut resets, r, c) { r <<= 1; }
    while l < r {
        let mid = l + (r - l) / 2;
        if complete(&tasks, total, &mut resets, mid, c) { r = mid; }
        else { l = mid + 1; }
    }
    writeln!(writer, "{}", resets - 1);
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
