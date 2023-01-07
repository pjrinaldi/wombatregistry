use std::io::{Read, Write};
use std::net::{SocketAddr::V4, SocketAddr::V6, TcpStream, ToSocketAddrs};
use std::thread;
use std::time::Duration;

use crate::Protocol;

pub fn get_data<T: Protocol>(url: T) -> Result<(Option<Vec<u8>>, Vec<u8>), String> {
    let url = url.get_source_url();
    let host = url.host_str().unwrap().to_string();
    let port = url.port().unwrap_or(79);
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
                        let username = if url.username() == "" {
                            url.path().replace("/", "")
                        } else {
                            String::from(url.username())
                        };

                        let request = format!("{}\r\n", username);
                        stream.write_all(request.as_bytes()).unwrap();
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
