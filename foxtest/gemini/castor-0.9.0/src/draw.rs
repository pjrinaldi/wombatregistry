use glib::clone;
use gtk::prelude::*;
use gtk::TextBuffer;
use std::str::FromStr;
use std::sync::Arc;
use url::Url;
use std::convert::TryInto;

extern crate textwrap;
use textwrap::fill;

use crate::absolute_url::AbsoluteUrl;
use crate::colors::*;
use crate::gemini::link::Link as GeminiLink;
use crate::gopher::link::Link as GopherLink;
use crate::gui::Gui;
use crate::protocols::{Finger, Gemini, Gopher};


pub fn gemini_content(
    gui: &Arc<Gui>,
    content: Vec<Result<crate::gemini::parser::TextElement, crate::gemini::parser::ParseError>>,
) -> TextBuffer {
    let content_view = gui.content_view();
    let buffer = content_view.get_buffer().unwrap();

    let mut mono_toggle = false;
    let font_family = crate::settings::get_gemini_text_font_family();

    for el in content {
        match el {
            Ok(crate::gemini::parser::TextElement::MonoText(_text)) => {
                mono_toggle = !mono_toggle;
            }
            Ok(crate::gemini::parser::TextElement::H1(header)) => {
                let mut end_iter = buffer.get_end_iter();
                if mono_toggle {
                    buffer.insert_markup(&mut end_iter, &mono_span(escape_text(&header)));
                } else {
                  buffer.insert_markup(
                      &mut end_iter,
                      &format!(
                          "<span foreground=\"{}\" size=\"{}\" font_family=\"{}\" style=\"{}\">{}{}</span>\n",
                          crate::settings::get_h1_color(),
                          crate::settings::get_gemini_h1_font_size(),
                          crate::settings::get_gemini_h1_font_family(),
                          crate::settings::get_gemini_h1_font_style(),
                          crate::settings::get_h1_character(),
                          wrap_text(&header, &gui)
                      ),
                  );
                }
            }
            Ok(crate::gemini::parser::TextElement::H2(header)) => {
                let mut end_iter = buffer.get_end_iter();
                if mono_toggle {
                    buffer.insert_markup(&mut end_iter, &mono_span(escape_text(&header)));
               } else {
                  buffer.insert_markup(
                      &mut end_iter,
                      &format!(
                          "<span foreground=\"{}\" size=\"{}\" font_family=\"{}\" style=\"{}\">{}{}</span>\n",
                          crate::settings::get_h2_color(),
                          crate::settings::get_gemini_h2_font_size(),
                          crate::settings::get_gemini_h2_font_family(),
                          crate::settings::get_gemini_h2_font_style(),
                          crate::settings::get_h2_character(),
                          wrap_text(&header, &gui)
                      ),
                  );
                }
            }
            Ok(crate::gemini::parser::TextElement::H3(header)) => {
                let mut end_iter = buffer.get_end_iter();
                if mono_toggle {
                    buffer.insert_markup(&mut end_iter, &mono_span(escape_text(&header)));
                } else {
                    buffer.insert_markup(
                        &mut end_iter,
                        &format!(
                            "<span foreground=\"{}\" size=\"{}\" font_family=\"{}\" style=\"{}\">{}{}</span>\n",
                            crate::settings::get_h3_color(),
                            crate::settings::get_gemini_h3_font_size(),
                            crate::settings::get_gemini_h3_font_family(),
                            crate::settings::get_gemini_h3_font_style(),
                            crate::settings::get_h3_character(),
                            wrap_text(&header, &gui)
                        ),
                    );
                }
            }
            Ok(crate::gemini::parser::TextElement::ListItem(item)) => {
                let mut end_iter = buffer.get_end_iter();
                if mono_toggle {
                    buffer.insert_markup(&mut end_iter, &mono_span(item));
                } else {
                    buffer.insert_markup(
                      &mut end_iter,
                      &format!(
                          "<span foreground=\"{}\" size=\"{}\" font_family=\"{}\" style=\"{}\">{}{}</span>\n",
                          crate::settings::get_list_color(),
                          crate::settings::get_gemini_list_font_size(),
                          crate::settings::get_gemini_list_font_family(),
                          crate::settings::get_gemini_list_font_style(),
                          crate::settings::get_list_character(),
                          wrap_text(&item, &gui)
                      ),
                  );
                }
            }
            Ok(crate::gemini::parser::TextElement::Quote(text)) => {
                let mut end_iter = buffer.get_end_iter();
                if mono_toggle {
                    buffer.insert_markup(&mut end_iter, &mono_span(text));
                } else {
                    buffer.insert_markup(
                        &mut end_iter,
                        &format!(
                            "<span foreground=\"{}\" background=\"{}\" font_family=\"{}\" size=\"{}\" style=\"{}\">{}</span>\n",
                            crate::settings::get_gemini_quote_foreground_color(),
                            crate::settings::get_gemini_quote_background_color(),
                            crate::settings::get_gemini_quote_font_family(),
                            crate::settings::get_gemini_quote_font_size(),
                            crate::settings::get_gemini_quote_font_style(),
                            wrap_text(&text, &gui)
                        ),
                    );
                }
            }
            Ok(crate::gemini::parser::TextElement::Text(text)) => {
                let mut end_iter = buffer.get_end_iter();
                if mono_toggle {
                    buffer.insert_markup(&mut end_iter, &mono_span(colors::colorize(&text)));
                } else {
                    buffer.insert_markup(
                        &mut end_iter,
                        &format!(
                            "<span foreground=\"{}\" font_family=\"{}\" size=\"{}\">{}</span>\n",
                            crate::settings::get_text_color(),
                            font_family,
                            crate::settings::get_gemini_text_font_size(),
                            wrap_text(&text, &gui)
                        ),
                    );
                }
            }
            Ok(crate::gemini::parser::TextElement::LinkItem(link_item)) => {
                if mono_toggle {
                    let mut end_iter = buffer.get_end_iter();
                    buffer.insert_markup(&mut end_iter, &mono_span(escape_text(&link_item)));
                } else {
                    gemini_link(&gui, link_item);
                }
            }
            Err(_) => println!("Something failed."),
        }
    }
    buffer
}

pub fn gemini_text_content(gui: &Arc<Gui>, content: std::str::Lines) -> TextBuffer {
    let content_view = gui.content_view();
    let buffer = content_view.get_buffer().unwrap();

    for line in content {
        let mut end_iter = buffer.get_end_iter();
        buffer.insert_markup(
            &mut end_iter,
            &format!(
                "<span foreground=\"{}\" font_family=\"monospace\">{}</span>\n",
                crate::settings::get_text_color(),
                colors::colorize(&line)
            ),
        );
    }
    buffer
}

pub fn gopher_content(
    gui: &Arc<Gui>,
    content: Vec<Result<crate::gopher::parser::TextElement, crate::gopher::parser::ParseError>>,
) -> TextBuffer {
    let content_view = gui.content_view();
    let buffer = content_view.get_buffer().unwrap();

    for el in content {
        match el {
            Ok(crate::gopher::parser::TextElement::Text(text)) => {
                let mut end_iter = buffer.get_end_iter();
                let text = colors::colorize(&text);

                buffer.insert_markup(
                    &mut end_iter,
                    &format!(
                        "<span foreground=\"{}\" font_family=\"{}\" size=\"{}\">{}</span>\n",
                        crate::settings::get_text_color(),
                        crate::settings::get_gopher_font_family(),
                        crate::settings::get_gopher_font_size(),
                        text
                    ),
                );
            }
            Ok(crate::gopher::parser::TextElement::LinkItem(link_item)) => {
                gopher_link(&gui, colors::cleanup(&link_item));
            }
            Ok(crate::gopher::parser::TextElement::ExternalLinkItem(link_item)) => {
                gopher_link(&gui, colors::cleanup(&link_item));
            }
            Ok(crate::gopher::parser::TextElement::Image(link_item)) => {
                gopher_link(&gui, link_item);
            }
            Ok(crate::gopher::parser::TextElement::Binary(link_item)) => {
                gopher_link(&gui, link_item);
            }
            Err(_) => println!("Something failed."),
        }
    }
    buffer
}

pub fn finger_content(
    gui: &Arc<Gui>,
    content: Vec<Result<crate::finger::parser::TextElement, crate::finger::parser::ParseError>>,
) -> TextBuffer {
    let content_view = gui.content_view();
    let buffer = content_view.get_buffer().unwrap();

    for el in content {
        match el {
            Ok(crate::finger::parser::TextElement::Text(text)) => {
                let mut end_iter = buffer.get_end_iter();

                buffer.insert_markup(
                    &mut end_iter,
                    &format!(
                        "<span foreground=\"{}\" font_family=\"{}\" size=\"{}\">{}</span>\n",
                        crate::settings::get_text_color(),
                        crate::settings::get_finger_font_family(),
                        crate::settings::get_finger_font_size(),
                        escape_text(&text)
                    ),
                );
            }
            Err(_) => println!("Something failed."),
        }
    }
    buffer
}

pub fn gemini_link(gui: &Arc<Gui>, link_item: String) {
    match GeminiLink::from_str(&link_item) {
        Ok(GeminiLink::Finger(url, label)) => {
            let button_label = if label.is_empty() {
                url.to_string()
            } else {
                label
            };
            let finger_label = format!("{} [Finger]", button_label);
            insert_button(&gui, url, finger_label);
        }
        Ok(GeminiLink::Gemini(url, label)) => {
            insert_button(&gui, url, label);
        }
        Ok(GeminiLink::Gopher(url, label)) => {
            let button_label = if label.is_empty() {
                url.to_string()
            } else {
                label
            };
            let gopher_label = format!("{} [Gopher]", button_label);
            insert_button(&gui, url, gopher_label);
        }
        Ok(GeminiLink::Http(url, label)) => {
            let button_label = if label.is_empty() {
                url.to_string()
            } else {
                label
            };
            let www_label = format!("{} [WWW]", button_label);

            insert_external_button(&gui, url, &www_label);
        }
        Ok(GeminiLink::Email(url, label)) => {
            let button_label = if label.is_empty() {
                url.to_string()
            } else {
                label
            };
            let email_label = format!("{} [MAILTO]", button_label);

            insert_external_button(&gui, url, &email_label);
        }
        Ok(GeminiLink::XMPP(url, label)) => {
            let button_label = if label.is_empty() {
                url.to_string()
            } else {
                label
            };
            let xmpp_label = format!("{} [XMPP]", button_label);

            insert_external_button(&gui, url, &xmpp_label);
        }
        Ok(GeminiLink::IRC(url, label)) => {
            let button_label = if label.is_empty() {
                url.to_string()
            } else {
                label
            };
            let irc_label = format!("{} [IRC]", button_label);

            insert_external_button(&gui, url, &irc_label);
        }
        Ok(GeminiLink::Relative(url, label)) => {
            let new_url = Gemini { source: url }.to_absolute_url().unwrap();
            insert_button(&gui, new_url, label);
        }
        Ok(GeminiLink::Unknown(_, _)) => (),
        Err(_) => (),
    }
}

pub fn gopher_link(gui: &Arc<Gui>, link_item: String) {
    match GopherLink::from_str(&link_item) {
        Ok(GopherLink::Http(url, label)) => {
            let button_label = if label.is_empty() {
                url.to_string()
            } else {
                label
            };
            let www_label = format!("{} [WWW]", button_label);

            insert_external_button(&gui, url, &www_label);
        }
        Ok(GopherLink::Gopher(url, label)) => {
            let button_label = if label.is_empty() {
                url.to_string()
            } else {
                label
            };
            let gopher_label = format!("{} [Gopher]", button_label);
            insert_button(&gui, url, gopher_label);
        }
        Ok(GopherLink::Image(url, label)) => {
            let button_label = if label.is_empty() {
                url.to_string()
            } else {
                label
            };
            let image_label = format!("{} [Image]", button_label);
            insert_gopher_file_button(&gui, url, image_label);
        }
        Ok(GopherLink::File(url, label)) => {
            let button_label = if label.is_empty() {
                url.to_string()
            } else {
                label
            };
            let file_label = format!("{} [File]", button_label);
            insert_gopher_file_button(&gui, url, file_label);
        }
        Ok(GopherLink::Gemini(url, label)) => {
            insert_button(&gui, url, label);
        }
        Ok(GopherLink::Relative(url, label)) => {
            let new_url = Gopher { source: url }.to_absolute_url().unwrap();
            insert_button(&gui, new_url, label);
        }
        Ok(GopherLink::Ftp(url, label)) => {
            let button_label = if label.is_empty() {
                url.to_string()
            } else {
                label
            };
            let ftp_label = format!("{} [FTP]", button_label);

            insert_external_button(&gui, url, &ftp_label);
        }
        Ok(GopherLink::Finger(url, label)) => {
            let button_label = if label.is_empty() {
                url.to_string()
            } else {
                label
            };
            let finger_label = format!("{} [Finger]", button_label);

            insert_external_button(&gui, url, &finger_label);
        }
        Ok(GopherLink::Unknown(_, _)) => (),
        Err(_) => (),
    }
}

pub fn insert_button(gui: &Arc<Gui>, url: Url, label: String) {
    let content_view = gui.content_view();
    let buffer = content_view.get_buffer().unwrap();

    let button_label = if label.is_empty() {
        url.to_string()
    } else {
        label
    };

    let button = gtk::Button::new_with_label(&button_label);
    button.set_tooltip_text(Some(&url.to_string()));

    button.connect_clicked(clone!(@weak gui => move |_| {
        match url.scheme() {
            "finger" => crate::visit_url(&gui, Finger { source: url.to_string() }),
            "gemini" => crate::visit_url(&gui, Gemini { source: url.to_string() }),
            "gopher" => crate::visit_url(&gui, Gopher { source: url.to_string() }),
            _ => ()
        }
    }));

    let mut start_iter = buffer.get_end_iter();
    let anchor = buffer.create_child_anchor(&mut start_iter).unwrap();
    content_view.add_child_at_anchor(&button, &anchor);
    let mut end_iter = buffer.get_end_iter();
    buffer.insert(&mut end_iter, "\n");
}

pub fn insert_gopher_file_button(gui: &Arc<Gui>, url: Url, label: String) {
    let content_view = gui.content_view();
    let buffer = content_view.get_buffer().unwrap();

    let button_label = if label.is_empty() {
        url.to_string()
    } else {
        label
    };

    let button = gtk::Button::new_with_label(&button_label);
    button.set_tooltip_text(Some(&url.to_string()));

    button.connect_clicked(move |_| {
        let (_meta, content) = crate::gopher::client::get_data(Gopher {
            source: url.to_string(),
        })
        .unwrap();
        crate::client::download(content);
    });

    let mut start_iter = buffer.get_end_iter();
    let anchor = buffer.create_child_anchor(&mut start_iter).unwrap();
    content_view.add_child_at_anchor(&button, &anchor);
    let mut end_iter = buffer.get_end_iter();
    buffer.insert(&mut end_iter, "\n");
}

pub fn insert_external_button(gui: &Arc<Gui>, url: Url, label: &str) {
    let content_view = gui.content_view();
    let buffer = content_view.get_buffer().unwrap();

    let button = gtk::Button::new_with_label(&label);
    button.set_tooltip_text(Some(&url.to_string()));

    button.connect_clicked(move |_| {
        open::that(url.to_string()).unwrap();
    });

    let mut start_iter = buffer.get_end_iter();
    let anchor = buffer.create_child_anchor(&mut start_iter).unwrap();
    content_view.add_child_at_anchor(&button, &anchor);
    let mut end_iter = buffer.get_end_iter();
    buffer.insert(&mut end_iter, "\n");
}

fn wrap_text(str: &str, gui: &Arc<Gui>) -> String {
    fill(&escape_text(str), width(&gui))
}

fn escape_text(str: &str) -> String {
    String::from(glib::markup_escape_text(&str).as_str())
}

fn mono_span(text: String) -> String {
    format!(
        "<span foreground=\"{}\" font_family=\"monospace\" size=\"{}\">{}</span>\n",
        crate::settings::get_text_color(),
        crate::settings::get_gemini_text_font_size(),
        text
    )
}

fn width(gui: &Arc<Gui>) -> usize {
    let (win_width, _) = gtk::ApplicationWindow::get_size(gui.window());
    let calculated_width = (win_width / 10).try_into().unwrap();
    std::cmp::min(calculated_width, crate::settings::max_width().unwrap_or(std::usize::MAX))
}
