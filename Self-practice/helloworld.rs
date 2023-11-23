use std::io::{self, stdout, BufWriter, Write};

fn main() {
    let stdout = stdout();
    let mut bw = BufWriter::new(stdout.lock());

    write!(bw, "Hello World!\n");
}
