use url::Url;

pub trait Protocol {
    fn get_source_str(&self) -> &str;
    fn get_source_url(&self) -> Url;
    fn get_scheme(&self) -> Scheme;
}

pub struct Gemini {
    pub source: String,
}
pub struct Gopher {
    pub source: String,
}
pub struct Finger {
    pub source: String,
}

impl Protocol for Finger {
    fn get_source_str(&self) -> &str {
        &self.source
    }

    fn get_source_url(&self) -> Url {
        Url::parse(&self.source).unwrap()
    }

    fn get_scheme(&self) -> Scheme {
        Scheme::Finger
    }
}

impl Protocol for Gemini {
    fn get_source_str(&self) -> &str {
        &self.source
    }

    fn get_source_url(&self) -> Url {
        Url::parse(&self.source).unwrap()
    }

    fn get_scheme(&self) -> Scheme {
        Scheme::Gemini
    }
}

impl Protocol for Gopher {
    fn get_source_str(&self) -> &str {
        &self.source
    }

    fn get_source_url(&self) -> Url {
        Url::parse(&self.source).unwrap()
    }

    fn get_scheme(&self) -> Scheme {
        Scheme::Gopher
    }
}

#[derive(PartialEq)]
pub enum Scheme {
    Finger,
    Gemini,
    Gopher,
}
