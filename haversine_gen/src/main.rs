use std::{collections::HashMap, fs::File, io::Write, mem};

use serde::Serialize;

use clap::Parser;
use rand::prelude::*;
use rand::rngs::StdRng;

// approximate radius, in km.
const EARTH_RADIUS: f64 = 6372.8;

#[derive(Parser, Debug)]
#[command(author, version, about, long_about = None)]
struct Args {
    #[arg(short, long)]
    count: u32,

    #[arg(short, long)]
    seed: u64,

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
    y1: f64,
}

fn generate(count: u32, seed: u64, binary_out: bool) {
    let mut points: Vec<HPoint> = Vec::new();

    let mut file: Option<File> = if binary_out {
        let filename = format!("haversine_c{}_s{}.bin", count, seed);
        Some(
            File::options()
                .write(true)
                .append(false)
                .create(true)
                .open(filename)
                .expect("Failed to open binary file"),
        )
    } else {
        None
    };

    let mut rng = if seed != 0 {
        StdRng::seed_from_u64(seed)
    } else {
        StdRng::from_entropy()
    };

    let mut haversines: Vec<f64> = Vec::new();

    for _ in 0..count {
        let x0 = rng.gen::<f64>() * 360.0 - 180.0;
        let x1 = rng.gen::<f64>() * 360.0 - 180.0;
        let y0 = rng.gen::<f64>() * 180.0 - 90.0;
        let y1 = rng.gen::<f64>() * 180.0 - 90.0;

        let p = HPoint { x0, y0, x1, y1 };

        let hav = haversine(&p);
        haversines.push(hav);

        points.push(p);
    }

    match file {
        Some(ref mut f) => {
            // we have to convert the f64 vector into an array of pure bytes
            // thus, we convert each f64 to a stream of 8 bytes (in LE)
            let mut hav_bytes = Vec::<u8>::with_capacity(haversines.len() * mem::size_of::<f64>());
            for hav in haversines.iter() {
                hav_bytes.extend_from_slice(&hav.to_le_bytes());
            }

            f.write_all(&hav_bytes).expect("write to file failed")
        }
        None => (),
    }

    // In order to have a correct JSON file, we need an outer map
    let mut hmap = HashMap::new();
    hmap.insert("points", points);

    // Create the JSON string
    let ser = serde_json::to_string_pretty(&hmap).unwrap();

    let json_filename = format!("haversine_c{}_s{}.json", count, seed);
    let mut jsonfile = File::create(json_filename).expect("Failed to create JSON file");
    write!(&mut jsonfile, "{}", ser).expect("Failed to write JSON");
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

    let a =
        square((d_lat / 2.0_f64).sin()) + lat1.cos() * lat2.cos() * square((d_lon / 2.0_f64).sin());
    let c = 2.0 * a.sqrt().asin();

    EARTH_RADIUS * c
}

fn main() {
    let args = Args::parse();
    generate(args.count, args.seed, args.binary_out);
}
