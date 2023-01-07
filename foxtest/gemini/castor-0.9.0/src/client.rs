use std::io::Write;
use tempfile::NamedTempFile;

pub trait Client {
    fn get_data(&self) -> Result<(Option<Vec<u8>>, Vec<u8>), String>;
}

pub fn download(content: Vec<u8>) {
    let path = write_tmp_file(content);
    open::that(path).unwrap();
}

fn write_tmp_file(content: Vec<u8>) -> std::path::PathBuf {
    let mut tmp_file = NamedTempFile::new().unwrap();
    tmp_file.write_all(&content).unwrap();
    let (_file, path) = tmp_file.keep().unwrap();
    path
}
