use std::{println, collections::HashMap};

use serde::Serialize;
use serde_json::Result;

use clap::Parser;
use rand::prelude::*;


const EARTH_RADIUS: f64 = 6372.8;



#[derive(Parser, Debug)]
#[command(author, version, about, long_about = None)]
struct Args {
    #[arg(short, long)]
    count: u32,

    #[arg(short, long)]
    binary_out: bool,
}


#[derive(Serialize, Debug)]
struct HPoint {
    // From
    x0: f64,
    y0: f64,
    // To
    x1: f64,
    y1: f64
}


fn generate(count: u32, binary_out: bool) {

    for _ in 0..count {
        let r = rand::random::<f64>();
        println!("r: {}", r);
    }
}

fn deg2rad(deg: f64) -> f64 {
    let rad: f64 = 0.01745329251994329577 * deg;
    rad
}

fn square(a: f64) -> f64 {
    a * a
}

fn haversine(p: &HPoint) -> f64 {
    let lat1 = p.y0;
    let lat2 = p.y1;
    let lon1 = p.x0;
    let lon2 = p.x1;

    let d_lat = deg2rad(lat2 - lat1);
    let d_lon = deg2rad(lon2 - lon1);

    let a = square((d_lat / 2.0_f64).sin()) + lat1.cos() * lat2.cos() * square((d_lon / 2.0_f64).sin());
    println!("a: {}", a);
    let c = 2.0 * a.sqrt().asin();
    println!("c: {}", c);

    EARTH_RADIUS * c
}

fn main() {
    let args = Args::parse();
    generate(args.count, args.binary_out);

    println!("args: {:#?}", args);
    let p1 = HPoint { x0: 1.5, y0: -89.7, x1: 165.124, y1: -12.4352 };
    let p2 = HPoint { x0: 23.5, y0: -9.7, x1: 16.124, y1: -12.4352 };

    let h1 = haversine(&p1);
    let h2 = haversine(&p2);
    println!("h1: {}, h2: {}", h1, h2);

    let points = vec![p1, p2];
    let mut hmap = HashMap::new();
    hmap.insert("points", points);
    let ser = serde_json::to_string(&hmap).unwrap();
    println!("Hpoint JSON: {}", ser);
}
