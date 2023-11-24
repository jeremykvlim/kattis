use std::{
    io::{stdin, stdout, BufRead, BufWriter, Write},
    collections::HashSet,
};


fn main() {
    let stdin = stdin();
    let stdout = stdout();
    let mut reader = Reader::from(stdin.lock());
    let mut writer = BufWriter::new(stdout.lock());
    loop {
        let n: i32 = reader.next();
        let m: i32 = reader.next();
        if n == 0 && m == 0 { break; }
        let mut set = HashSet::new();
        let mut count = 0;
        for _ in 0..n {
            let num: i32 = reader.next();
            set.insert(num);
        }
        for _ in 0..m {
            let num: i32 = reader.next();
            if set.contains(&num) { count += 1; }
        }
        writeln!(writer, "{}", count);
    }
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
