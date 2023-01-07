extern crate dirs;

use std::fs;
use std::fs::File;
use std::fs::OpenOptions;
use std::io::{Read, Write};
use url::Url;

pub fn is_valid(url: &str) -> bool {
    Url::parse(&url).is_ok()
}

pub fn add(url: &str) {
    let mut file = bookmarks_file();
    let entry = format!("=> {}\n", url);
    file.write_all(entry.as_bytes())
        .expect("Unable to write file");
}

pub fn content() -> String {
    let mut file = bookmarks_file();
    let mut content = String::new();
    file.read_to_string(&mut content)
        .expect("Unable to read file");

    content
}

fn bookmarks_file() -> File {
    let mut bookmarks = dirs::data_local_dir().unwrap();
    bookmarks.push("castor");
    fs::create_dir_all(&bookmarks).unwrap();
    bookmarks.push("bookmarks");
    let file_path = bookmarks.into_os_string();

    OpenOptions::new()
        .create(true)
        .append(true)
        .read(true)
        .open(file_path)
        .expect("file not found")
}
