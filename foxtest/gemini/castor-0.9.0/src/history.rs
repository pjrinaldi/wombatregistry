use std::sync::Mutex;
use url::Url;

lazy_static! {
    static ref HISTORY: Mutex<History> = Mutex::new(History::new());
}

#[derive(Debug, Clone, PartialEq, Eq, Hash)]
struct History {
    past: Vec<Url>,
    current: Option<Url>,
    future: Vec<Url>,
}

impl History {
    fn new() -> Self {
        History {
            past: vec![],
            current: None,
            future: vec![],
        }
    }

    fn get_previous_url(&mut self) -> Option<Url> {
        let p = self.past.pop();
        if p.is_some() {
            if let Some(c) = self.current.take() {
                self.future.push(c);
            }
            self.current = None;
        };
        p
    }

    fn get_next_url(&mut self) -> Option<Url> {
        let f = self.future.pop();
        if f.is_some() {
            if let Some(c) = self.current.take() {
                self.past.push(c);
            }
            self.current = None;
        };
        f
    }

    fn append(&mut self, url: &str) {
        let url = Url::parse(url).unwrap();
        if let Some(c) = self.current.replace(url) {
            self.past.push(c);
            self.future = vec![]
        };
    }

    fn current(&self) -> Option<&Url> {
        self.current.as_ref()
    }
}

pub fn append(url: &str) {
    HISTORY.lock().unwrap().append(url)
}

pub fn get_current_host() -> Option<String> {
    HISTORY
        .lock()
        .unwrap()
        .current()
        .and_then(|u| u.host_str())
        .map(String::from)
}

pub fn get_current_url() -> Option<String> {
    HISTORY
        .lock()
        .unwrap()
        .current()
        .and_then(|u| u.join("./").ok())
        .map(|p| p.to_string())
}

pub fn get_previous_url() -> Option<Url> {
    let mut history = HISTORY.lock().unwrap();

    history.get_previous_url()
}

pub fn get_next_url() -> Option<Url> {
    let mut history = HISTORY.lock().unwrap();

    history.get_next_url()
}

#[cfg(test)]
pub(crate) fn clear() -> () {
    *HISTORY.lock().unwrap() = History::new();
}

#[test]
fn test_append_simple() {
    crate::history::clear();

    *HISTORY.lock().unwrap() = History {
        past: vec![],
        current: Some(Url::parse("gemini://typed-hole.org/foo").unwrap()),
        future: vec![],
    };

    append("gemini://typed-hole.org");

    assert_eq!(
        *HISTORY.lock().unwrap(),
        History {
            past: vec![Url::parse("gemini://typed-hole.org/foo").unwrap()],
            current: Some(Url::parse("gemini://typed-hole.org").unwrap()),
            future: vec![],
        },
    );
}

#[test]
fn test_append_clear_future() {
    crate::history::clear();

    *HISTORY.lock().unwrap() = History {
        past: vec![],
        current: Some(Url::parse("gemini://typed-hole.org").unwrap()),
        future: vec![Url::parse("gemini://typed-hole.org/foo").unwrap()],
    };

    append("gemini://typed-hole.org/bar");

    assert_eq!(
        *HISTORY.lock().unwrap(),
        History {
            past: vec![Url::parse("gemini://typed-hole.org").unwrap()],
            current: Some(Url::parse("gemini://typed-hole.org/bar").unwrap()),
            future: vec![],
        },
    );
}

#[test]
fn test_append_no_current() {
    crate::history::clear();

    *HISTORY.lock().unwrap() = History {
        past: vec![],
        current: None,
        future: vec![Url::parse("gemini://typed-hole.org/foo").unwrap()],
    };

    append("gemini://typed-hole.org");

    assert_eq!(
        *HISTORY.lock().unwrap(),
        History {
            past: vec![],
            current: Some(Url::parse("gemini://typed-hole.org").unwrap()),
            future: vec![Url::parse("gemini://typed-hole.org/foo").unwrap()],
        },
    );
}

#[test]
fn test_get_previous_url_simple() {
    crate::history::clear();

    *HISTORY.lock().unwrap() = History {
        past: vec![Url::parse("gemini://typed-hole.org").unwrap()],
        current: Some(Url::parse("gemini://typed-hole.org/foo").unwrap()),
        future: vec![],
    };

    let previous = get_previous_url();

    assert_eq!(
        previous,
        Some(Url::parse("gemini://typed-hole.org").unwrap())
    );
    assert_eq!(
        *HISTORY.lock().unwrap(),
        History {
            past: vec![],
            current: None,
            future: vec![Url::parse("gemini://typed-hole.org/foo").unwrap()]
        },
    );
}

#[test]
fn test_get_previous_url_no_past() {
    crate::history::clear();

    let simple = History {
        past: vec![],
        current: Some(Url::parse("gemini://typed-hole.org/foo").unwrap()),
        future: vec![],
    };
    *HISTORY.lock().unwrap() = simple.clone();

    let previous = get_previous_url();

    assert_eq!(previous, None);
    assert_eq!(*HISTORY.lock().unwrap(), simple);
}
