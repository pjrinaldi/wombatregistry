use crate::Finger;
use crate::Gemini;
use crate::Gopher;
use crate::Protocol;
use url::Url;

pub trait AbsoluteUrl {
    fn to_absolute_url(&self) -> Result<url::Url, url::ParseError>;
}

impl AbsoluteUrl for Finger {
    fn to_absolute_url(&self) -> Result<url::Url, url::ParseError> {
        Ok(self.get_source_url())
    }
}

impl AbsoluteUrl for Gemini {
    fn to_absolute_url(&self) -> Result<url::Url, url::ParseError> {
        let url = self.get_source_str();
        // Creates an absolute link if needed
        match crate::history::get_current_host() {
            Some(host) => {
                if url.starts_with("gemini://") {
                    Url::parse(&url)
                } else if url.starts_with("//") {
                    Url::parse(&format!("gemini:{}", url))
                } else if url.starts_with('/') {
                    Url::parse(&format!("gemini://{}{}", host, url))
                } else {
                    let current_host_path = crate::history::get_current_url().unwrap();
                    Url::parse(&format!("{}{}", current_host_path, url))
                }
            }
            None => {
                if url.starts_with("gemini://") {
                    Url::parse(&url)
                } else if url.starts_with("//") {
                    Url::parse(&format!("gemini:{}", url))
                } else {
                    Url::parse(url)
                }
            }
        }
    }
}

impl AbsoluteUrl for Gopher {
    fn to_absolute_url(&self) -> Result<url::Url, url::ParseError> {
        let url = self.get_source_str();
        // Creates an absolute link if needed
        match crate::history::get_current_host() {
            Some(host) => {
                if url.starts_with("gopher://") {
                    Url::parse(&url)
                } else if url.starts_with("//") {
                    Url::parse(&format!("gopher:{}", url))
                } else if url.starts_with('/') {
                    Url::parse(&format!("gopher://{}{}", host, url))
                } else {
                    let current_host_path = crate::history::get_current_url().unwrap();
                    Url::parse(&format!("{}{}", current_host_path, url))
                }
            }
            None => {
                if url.starts_with("gopher://") {
                    Url::parse(&url)
                } else if url.starts_with("//") {
                    Url::parse(&format!("gopher:{}", url))
                } else {
                    Url::parse(&format!("gopher://{}", url))
                }
            }
        }
    }
}

#[test]
fn test_make_absolute_slash_path_no_current_host() {
    crate::history::clear();

    let url = "/foo";
    let absolute_url = Gemini {
        source: String::from(url),
    }
    .to_absolute_url();
    assert_eq!(absolute_url, Err(url::ParseError::RelativeUrlWithoutBase));
}
#[test]
fn test_make_absolute_just_path_no_current_host() {
    crate::history::clear();

    let url = "foo";
    let absolute_url = Gemini {
        source: String::from(url),
    }
    .to_absolute_url();
    assert_eq!(absolute_url, Err(url::ParseError::RelativeUrlWithoutBase));
}
#[test]
fn test_make_absolute_full_url() {
    crate::history::clear();

    crate::history::append("gemini://typed-hole.org");
    let url = "gemini://typed-hole.org/foo";
    let expected_url = Url::parse("gemini://typed-hole.org/foo").unwrap();
    let absolute_url = Gemini {
        source: String::from(url),
    }
    .to_absolute_url()
    .unwrap();
    assert_eq!(expected_url, absolute_url);
}
#[test]
fn test_make_absolute_full_url_no_protocol() {
    crate::history::clear();

    crate::history::append("gemini://typed-hole.org");
    let url = "//typed-hole.org/foo";
    let expected_url = Url::parse("gemini://typed-hole.org/foo").unwrap();
    let absolute_url = Gemini {
        source: String::from(url),
    }
    .to_absolute_url()
    .unwrap();
    assert_eq!(expected_url, absolute_url);
}
#[test]
fn test_make_absolute_slash_path() {
    crate::history::clear();

    crate::history::append("gemini://typed-hole.org");
    let url = "/foo";
    let expected_url = Url::parse("gemini://typed-hole.org/foo").unwrap();
    let absolute_url = Gemini {
        source: String::from(url),
    }
    .to_absolute_url()
    .unwrap();
    assert_eq!(expected_url, absolute_url);
}
#[test]
fn test_make_absolute_just_path() {
    crate::history::clear();

    crate::history::append("gemini://typed-hole.org");
    let url = "foo";
    let expected_url = Url::parse("gemini://typed-hole.org/foo").unwrap();
    let absolute_url = Gemini {
        source: String::from(url),
    }
    .to_absolute_url()
    .unwrap();
    assert_eq!(expected_url, absolute_url);
}
#[test]
fn test_make_absolute_full_url_no_current_host() {
    crate::history::clear();

    let url = "gemini://typed-hole.org/foo";
    let expected_url = Url::parse("gemini://typed-hole.org/foo").unwrap();
    let absolute_url = Gemini {
        source: String::from(url),
    }
    .to_absolute_url()
    .unwrap();
    assert_eq!(expected_url, absolute_url);
}
#[test]
fn test_make_absolute_full_url_no_protocol_no_current_host() {
    crate::history::clear();

    let url = "//typed-hole.org/foo";
    let expected_url = Url::parse("gemini://typed-hole.org/foo").unwrap();
    let absolute_url = Gemini {
        source: String::from(url),
    }
    .to_absolute_url()
    .unwrap();
    assert_eq!(expected_url, absolute_url);
}
