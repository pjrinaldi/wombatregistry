extern crate regex;
use regex::Regex;
use std::str::FromStr;
use url::Url;

#[derive(Debug)]
pub enum Link {
    Finger(Url, String),
    Gemini(Url, String),
    Gopher(Url, String),
    Http(Url, String),
    Email(Url, String),
    IRC(Url, String),
    XMPP(Url, String),
    Relative(String, String),
    Unknown(Url, String),
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
pub struct ParseError;

const LINK_REGEX: &str = r"^=>\s*(\S*)\s*(.*)?$";

impl FromStr for Link {
    type Err = ParseError;

    // Parses a &str into an instance of 'Link'
    fn from_str(line: &str) -> Result<Self, Self::Err> {
        let link_regexp = Regex::new(LINK_REGEX).unwrap();

        match link_regexp.captures(&line) {
            Some(caps) => {
                let url_str = caps.get(1).map_or("", |m| m.as_str());
                let label_str = caps.get(2).map_or("", |m| m.as_str());

                let url = url_str.to_string();
                let label = if label_str.is_empty() {
                    url_str.to_string()
                } else {
                    label_str.to_string()
                };

                match make_link(url, label) {
                    Some(link) => Ok(link),
                    None => Err(ParseError),
                }
            }
            None => Err(ParseError),
        }
    }
}

fn make_link(url: String, label: String) -> Option<Link> {
    let urlp = Url::parse(&url);
    match urlp {
        Ok(url) => match url.scheme() {
            "finger" => Some(Link::Finger(url, label)),
            "gemini" => Some(Link::Gemini(url, label)),
            "gopher" => Some(Link::Gopher(url, label)),
            "http" => Some(Link::Http(url, label)),
            "https" => Some(Link::Http(url, label)),
            "mailto" => Some(Link::Email(url, label)),
            "xmpp" => Some(Link::XMPP(url, label)),
            "irc" => Some(Link::IRC(url, label)),
            _ => Some(Link::Unknown(url, label)),
        },
        Err(url::ParseError::RelativeUrlWithoutBase) => Some(Link::Relative(url, label)),
        _ => None,
    }
}
