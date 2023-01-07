use std::str::FromStr;

#[derive(Debug)]
pub enum TextElement {
    Binary(String),
    ExternalLinkItem(String),
    LinkItem(String),
    Image(String),
    Text(String),
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
pub struct ParseError;

impl FromStr for TextElement {
    type Err = ParseError;

    // Parses a &str into an instance of 'TextElement'
    fn from_str(line: &str) -> Result<TextElement, ParseError> {
        let els = line.split('\t');
        if els.count() >= 2 {
            // Gophermap line
            if line.starts_with('0') || line.starts_with('1') {
                Ok(TextElement::LinkItem(String::from(line)))
            } else if line.starts_with('i') {
                let mut els = line.split('\t');
                let text = els.next().expect("");
                let mut text = String::from(text);
                text.remove(0);
                Ok(TextElement::Text(text))
            } else if line.starts_with('h') {
                Ok(TextElement::ExternalLinkItem(String::from(line)))
            } else if line.starts_with('I') || line.starts_with('g') {
                Ok(TextElement::Image(String::from(line)))
            } else if line.starts_with('4') || line.starts_with('5') || line.starts_with('6') || line.starts_with('9') || line.starts_with('d') || line.starts_with('s') {
                Ok(TextElement::Binary(String::from(line)))
            } else {
                Ok(TextElement::Text(String::from(line)))
            }
        } else {
            // Text line
            if line.contains("gopher://") {
                Ok(TextElement::LinkItem(String::from(line)))
            } else if line.contains("http://")
                || line.contains("https://")
                || line.contains("gemini://")
                || line.contains("finger://")
                || line.contains("ftp://")
            {
                Ok(TextElement::ExternalLinkItem(String::from(line)))
            } else {
                Ok(TextElement::Text(String::from(line)))
            }
        }
    }
}

pub fn parse(content: String) -> Vec<Result<TextElement, ParseError>> {
    let mut parsed = Vec::new();

    for line in content.lines() {
        parsed.push(TextElement::from_str(line));
    }
    parsed
}
