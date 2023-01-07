use native_tls::TlsConnector;
use std::io::{Read, Write};
use std::net::{SocketAddr::V4, SocketAddr::V6, TcpStream, ToSocketAddrs};
use std::thread;
use std::time::Duration;

use crate::protocols::*;

pub fn get_data<T: Protocol>(url: T) -> Result<(Option<Vec<u8>>, Vec<u8>), String> {
    let url = url.get_source_url();
    let host = url.host_str().unwrap_or("");
    let port = url.port().unwrap_or(1965);
    let urlf = format!("{}:{}", host, port);

    let mut builder = TlsConnector::builder();
    builder.danger_accept_invalid_hostnames(true);
    builder.danger_accept_invalid_certs(true);

    if let Some(cert) = crate::gemini::certificate::get_certificate(host) {
        let der = cert.to_der().unwrap();
        let identity = native_tls::Identity::from_pkcs12(&der, "").unwrap();
        builder.identity(identity);
    };

    let connector = builder.build().unwrap();

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

                let stream = TcpStream::connect_timeout(&socket_addr, Duration::new(5, 0));

                match stream {
                    Ok(stream) => {
                        let mstream = connector.connect(&host, stream);

                        match mstream {
                            Ok(mut stream) => thread::spawn(move || {
                                let url = format!("{}\r\n", url);
                                stream.write_all(url.as_bytes()).unwrap();
                                let mut res = vec![];
                                stream.read_to_end(&mut res).unwrap();

                                let crlf_idx = find_crlf(&res);

                                if crlf_idx.is_none() {
                                    return Err("Failed to read response (missing crlf)".to_string());
                                }

                                let content = res.split_off(crlf_idx.unwrap() + 2);

                                Ok((Some(res), content))
                            })
                            .join()
                            .unwrap(),
                            Err(e) => Err(format!("Could not connect to {}\n{}", urlf, e)),
                        }
                    }
                    Err(e) => Err(format!("Could not connect to {}\n{}", urlf, e)),
                }
            }
            None => Err(format!("Could not connect to {}", urlf)),
        },
        Err(e) => Err(format!("Could not connect to {}\n{}", urlf, e)),
    }
}

fn find_crlf(data: &[u8]) -> Option<usize> {
    let crlf = b"\r\n";
    data.windows(crlf.len()).position(|window| window == crlf)
}
