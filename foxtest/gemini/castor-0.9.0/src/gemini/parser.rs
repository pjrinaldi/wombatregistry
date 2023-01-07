use std::str::FromStr;

#[derive(Debug)]
pub enum TextElement {
    H1(String),
    H2(String),
    H3(String),
    ListItem(String),
    LinkItem(String),
    Text(String),
    MonoText(String),
    Quote(String),
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
pub struct ParseError;

impl FromStr for TextElement {
    type Err = ParseError;

    // Parses a &str into an instance of 'TextElement'
    fn from_str(line: &str) -> Result<TextElement, ParseError> {
        if line.starts_with("###") {
            let text = line.split_at(3).1.trim();
            Ok(TextElement::H3(String::from(text)))
        } else if line.starts_with("##") {
            let text = line.split_at(2).1.trim();
            Ok(TextElement::H2(String::from(text)))
        } else if line.starts_with('#') {
            let text = line.split_at(1).1.trim();
            Ok(TextElement::H1(String::from(text)))
        } else if line.starts_with('*') {
            let text = line.split_at(1).1.trim();
            Ok(TextElement::ListItem(String::from(text)))
        } else if line.starts_with("=>") {
            Ok(TextElement::LinkItem(String::from(line)))
        } else if line.starts_with("```") {
            let text = line.split_at(3).1.trim();
            Ok(TextElement::MonoText(String::from(text)))
        } else if line.starts_with('>') {
            let text = line.split_at(1).1.trim();
            Ok(TextElement::Quote(String::from(text)))
        } else {
            Ok(TextElement::Text(String::from(line)))
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
