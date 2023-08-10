use std::{println, collections::HashMap};

use serde::Serialize;
use serde_json::Result;

use clap::Parser;
use rand::prelude::*;

#[derive(Serialize, Debug)]
struct HPoint {
    // From
    x0: f32,
    y0: f32,
    // To
    x1: f32,
    y1: f32
}

#[derive(Parser, Debug)]
#[command(author, version, about, long_about = None)]
struct Args {
    #[arg(short, long)]
    count: u32,

    #[arg(short, long)]
    binary_out: bool,
}

fn generate(count: u32, binary_out: bool) {

    for _ in 0..count {
        let r = rand::random::<f64>();
        println!("r: {}", r);
    }
}

fn main() {
    let args = Args::parse();
    generate(args.count, args.binary_out);

    println!("args: {:#?}", args);
    let p1 = HPoint { x0: 1.5, y0: -89.7, x1: 165.124, y1: -12.4352 };
    let p2 = HPoint { x0: 1.5, y0: -89.7, x1: 165.124, y1: -12.4352 };
    let points = vec![p1, p2];
    let mut hmap = HashMap::new();
    hmap.insert("points", points);
    let ser = serde_json::to_string(&hmap).unwrap();
    println!("Hpoint JSON: {}", ser);
}
