use std::io::{self, Write};

fn main() {
    let stdout = io::stdout();
    let mut bw = io::BufWriter::new(stdout.lock());

    write!(bw, "Hello World!\n");
}
