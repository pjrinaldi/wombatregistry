extern crate linkify;
use linkify::LinkFinder;

use std::str::FromStr;
use url::Url;

#[derive(Debug)]
pub enum Link {
    File(Url, String),
    Ftp(Url, String),
    Finger(Url, String),
    Gemini(Url, String),
    Gopher(Url, String),
    Http(Url, String),
    Image(Url, String),
    Relative(String, String),
    Unknown(Url, String),
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
pub struct ParseError;

impl FromStr for Link {
    type Err = ParseError;

    // Parses a &str into an instance of 'Link'
    fn from_str(line: &str) -> Result<Self, Self::Err> {
        let mut els = line.split('\t');

        if line.starts_with('0') || line.starts_with('1') {
            let label = els.next().expect("no label");
            let path = els.next();
            let host = els.next();
            let port = els.next();

            if let Some(host) = host {
                if let Some(path) = path {
                    let mut text = String::from(label);
                    let selector = text.remove(0);

                    if let Some(port) = port {
                        if path != "" {
                            match Url::parse(&format!(
                                "gopher://{}:{}/{}{}",
                                host, port, selector, path
                            )) {
                                Ok(url) => Ok(Link::Gopher(url, text)),
                                Err(e) => {
                                    println!("ERR {:?}", e);
                                    Err(ParseError)
                                }
                            }
                        } else {
                            match Url::parse(&format!("gopher://{}:{}", host, port)) {
                                Ok(url) => Ok(Link::Gopher(url, text)),
                                Err(e) => {
                                    println!("ERR {:?}", e);
                                    Err(ParseError)
                                }
                            }
                        }
                    } else {
                        Err(ParseError)
                    }
                } else {
                    Err(ParseError)
                }
            } else {
                Err(ParseError)
            }
        } else if line.starts_with('g') || line.starts_with('I') {
            let label = els.next().expect("no label");
            let path = els.next();
            let host = els.next();
            let port = els.next();

            if let Some(host) = host {
                if let Some(p) = path {
                    let mut text = String::from(label);
                    let selector = text.remove(0);

                    let path = if p.starts_with('/') {
                        p.to_string()
                    } else {
                        format!("/{}", p)
                    };

                    if let Some(port) = port {
                        match Url::parse(&format!(
                            "gopher://{}:{}/{}{}",
                            host, port, selector, path
                        )) {
                            Ok(url) => Ok(Link::Image(url, text)),
                            Err(e) => {
                                println!("ERR {:?}", e);
                                Err(ParseError)
                            }
                        }
                    } else {
                        Err(ParseError)
                    }
                } else {
                    Err(ParseError)
                }
            } else {
                Err(ParseError)
            }
        } else if line.starts_with('4') || line.starts_with('5') || line.starts_with('6') || line.starts_with('9') || line.starts_with('d') || line.starts_with('s') {
            let label = els.next().expect("no label");
            let path = els.next();
            let host = els.next();
            let port = els.next();

            if let Some(host) = host {
                if let Some(p) = path {
                    let mut text = String::from(label);
                    let selector = text.remove(0);

                    let path = if p.starts_with('/') {
                        p.to_string()
                    } else {
                        format!("/{}", p)
                    };

                    if let Some(port) = port {
                        match Url::parse(&format!(
                            "gopher://{}:{}/{}{}",
                            host, port, selector, path
                        )) {
                            Ok(url) => Ok(Link::File(url, text)),
                            Err(e) => {
                                println!("ERR {:?}", e);
                                Err(ParseError)
                            }
                        }
                    } else {
                        Err(ParseError)
                    }
                } else {
                    Err(ParseError)
                }
            } else {
                Err(ParseError)
            }
        } else if line.starts_with('[') {
            let url = extract_url(line);
            let label = String::from(line);

            match make_link(String::from(url), label) {
                Some(link) => Ok(link),
                None => Err(ParseError),
            }
        } else if line.starts_with('h') {
            let label = els.next();
            let url = els.next();

            if let Some(label) = label {
                if let Some(url) = url {
                    let mut label = String::from(label);
                    label.remove(0);
                    let mut url = String::from(url);
                    let url = url.split_off(4);
                    match make_link(url, label) {
                        Some(link) => Ok(link),
                        None => Err(ParseError),
                    }
                } else {
                    Err(ParseError)
                }
            } else {
                Err(ParseError)
            }
        } else if line.contains("://") {
            let url = extract_url(line);
            let label = String::from(line);

            match make_link(String::from(url), label) {
                Some(link) => Ok(link),
                None => Err(ParseError),
            }
        } else {
            Err(ParseError)
        }
    }
}

pub fn make_link(url: String, label: String) -> Option<Link> {
    match Url::parse(&url) {
        Ok(url) => match url.scheme() {
            "finger" => Some(Link::Finger(url, label)),
            "ftp" => Some(Link::Ftp(url, label)),
            "gemini" => Some(Link::Gemini(url, label)),
            "gopher" => Some(Link::Gopher(url, label)),
            "http" => Some(Link::Http(url, label)),
            "https" => Some(Link::Http(url, label)),
            _ => Some(Link::Unknown(url, label)),
        },
        Err(url::ParseError::RelativeUrlWithoutBase) => Some(Link::Relative(url, label)),
        _ => None,
    }
}

fn extract_url(line: &str) -> &str {
    let finder = LinkFinder::new();
    let links: Vec<_> = finder.links(line).collect();
    if links.is_empty() {
        line
    } else {
        let link = &links[0];
        link.as_str()
    }
}
