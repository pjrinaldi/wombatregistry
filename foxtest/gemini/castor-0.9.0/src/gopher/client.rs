use percent_encoding::percent_decode;
use std::io::{Read, Write};
use std::net::{SocketAddr::V4, SocketAddr::V6, TcpStream, ToSocketAddrs};
use std::thread;
use std::time::Duration;

use crate::Protocol;

pub fn get_data<T: Protocol>(url: T) -> Result<(Option<Vec<u8>>, Vec<u8>), String> {
    let url = url.get_source_url();
    let host = url.host_str().unwrap().to_string();
    let port = url.port().unwrap_or(70);
    let urlf = format!("{}:{}", host, port);

    match urlf.to_socket_addrs() {
        Ok(mut addrs_iter) => match addrs_iter.next() {
            Some(socket_addr) => {
                let socket_addr = match socket_addr {
                    V4(ip) => V4(ip),
                    V6(ip) => match addrs_iter.next() {
                        Some(addr) => addr,
                        None => V6(ip),
                    },
                };

                match TcpStream::connect_timeout(&socket_addr, Duration::new(5, 0)) {
                    Ok(mut stream) => thread::spawn(move || {
                        let path = url.path().to_string();

                        let mut url = match url.query() {
                            Some(query) => format!("{}?{}\r\n", path, query),
                            None => format!("{}\r\n", path),
                        };

                        let url = if url.starts_with("/0")
                            || url.starts_with("/1")
                            || url.starts_with("/g")
                            || url.starts_with("/I")
                            || url.starts_with("/9")
                        {
                            url.split_off(2)
                        } else if url == "/\n" {
                            String::from("\r\n")
                        } else {
                            url
                        };

                        let url = percent_decode(url.as_bytes()).decode_utf8().unwrap();

                        stream.write_all(url.as_bytes()).unwrap();
                        let mut res = vec![];
                        stream.read_to_end(&mut res).unwrap();

                        Ok((None, res))
                    })
                    .join()
                    .unwrap(),
                    Err(e) => Err(format!("Could not connect to {}\n{}", urlf, e)),
                }
            }
            None => Err(format!("Could not connect to {}\n", urlf)),
        },
        Err(e) => Err(format!("Could not connect to {}\n{}", urlf, e)),
    }
}
