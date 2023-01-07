extern crate gdk;
extern crate gio;
extern crate glib;
extern crate gtk;
#[macro_use]
extern crate lazy_static;

use std::env;
use std::str::FromStr;
use std::sync::Arc;
use url::Url;

use gtk::prelude::*;

mod gui;
use gui::Gui;
mod absolute_url;
use absolute_url::AbsoluteUrl;
mod bookmarks;
mod client;
mod colors;
mod dialog;
mod draw;
mod finger;
mod gemini;
mod gopher;
mod history;
mod protocols;
use protocols::{Finger, Gemini, Gopher, Protocol, Scheme};
mod settings;
mod status;
use status::Status;

fn main() {
    // Start up the GTK3 subsystem.
    gtk::init().expect("Unable to start GTK3. Error");

    // Create the main window.
    let gui = Arc::new(Gui::new());

    // Set background color
    let color = settings::get_background_color();
    let provider = gtk::CssProvider::new();
    provider
        .load_from_data(format!("textview text {{ background-color: {}; }}", color).as_bytes())
        .expect("Failed to load CSS");

    gtk::StyleContext::add_provider_for_screen(
        &gdk::Screen::get_default().expect("Error initializing gtk css provider."),
        &provider,
        gtk::STYLE_PROVIDER_PRIORITY_APPLICATION,
    );

    // Bind back button
    {
        let button = gui.back_button();
        let gui = gui.clone();
        button.connect_clicked(move |_| {
            go_back(&gui);
        });
    }

    // Bind forward button
    {
        let button = gui.forward_button();
        let gui = gui.clone();
        button.connect_clicked(move |_| {
            go_forward(&gui);
        });
    }

    // Bind refresh button
    {
        let button = gui.refresh_button();
        let gui = gui.clone();
        button.connect_clicked(move |_| {
            refresh(&gui);
        });
    }

    // Bind add_bookmark button
    {
        let button = gui.add_bookmark_button();
        let gui = gui.clone();
        button.connect_clicked(move |_| {
            add_bookmark(&gui);
        });
    }

    // Bind show_bookmarks button
    {
        let button = gui.show_bookmarks_button();
        let gui = gui.clone();
        button.connect_clicked(move |_| {
            show_bookmarks(&gui);
        });
    }

    // Bind URL bar
    {
        let gui_clone = gui.clone();
        let url_bar = gui.url_bar();
        url_bar.connect_activate(move |b| {
            let url = b.get_text().expect("get_text failed").to_string();
            route_url(&gui_clone, url)
        });
    }

    // Bind Mouse-Back
    {
        let gui_clone = gui.clone();
        let content_view = gui.content_view();
        content_view.connect_button_press_event(move |_, event| {
            if event.get_button() == 8 {
                go_back(&gui_clone);
            }
            Inhibit(false)
        });
    }

    // Use passed URL or settings start_url
    let args: Vec<String> = env::args().collect();
    match args.len() {
        // no argument passed, check settings
        1 => {
            let url = settings::get_start_url();
            route_url(&gui, url)
        }
        // Use argument as initial URL
        _ => route_url(&gui, args[1].to_string()),
    }

    gui.start();
    gtk::main();
}

fn route_url(gui: &Arc<Gui>, url: String) {
    if url == "" {
    } else if url.starts_with("gemini://") {
        visit_url(&gui, Gemini { source: url })
    } else if url.starts_with("gopher://") {
        visit_url(&gui, Gopher { source: url })
    } else if url.starts_with("finger://") {
        visit_url(&gui, Finger { source: url })
    } else {
        visit_url(
            &gui,
            Gemini {
                source: format!("gemini://{}", url),
            },
        )
    };
}

fn go_back(gui: &Arc<Gui>) {
    if let Some(prev) = history::get_previous_url() {
        visit(gui, &prev);
    }
}

fn go_forward(gui: &Arc<Gui>) {
    if let Some(next) = history::get_next_url() {
        visit(gui, &next);
    }
}

fn visit(gui: &Arc<Gui>, url: &Url) {
    match url.scheme() {
        "finger" => visit_url(
            gui,
            Finger {
                source: url.to_string(),
            },
        ),
        "gemini" => visit_url(
            gui,
            Gemini {
                source: url.to_string(),
            },
        ),
        "gopher" => visit_url(
            gui,
            Gopher {
                source: url.to_string(),
            },
        ),
        _ => (),
    }
}

fn refresh(gui: &Arc<Gui>) {
    let url_bar = gui.url_bar();
    let url = url_bar.get_text().expect("get_text failed").to_string();
    route_url(&gui, url)
}

fn update_url_field(gui: &Arc<Gui>, url: &str) {
    let url_bar = gui.url_bar();
    url_bar.get_buffer().set_text(url);
}

fn add_bookmark(gui: &Arc<Gui>) {
    let url_bar = gui.url_bar();
    let current_url = url_bar.get_text();

    if let Some(url) = current_url {
        if bookmarks::is_valid(&url) {
            bookmarks::add(&url);
            dialog::info(&gui, "Bookmark added.");
        } else {
            dialog::error(&gui, "Invalid bookmark URL.");
        }
    }
}

fn show_bookmarks(gui: &Arc<Gui>) {
    let content_view = gui.content_view();

    let bookmarks_list = format!("# Bookmarks\n\n{}", bookmarks::content());
    let parsed_content = gemini::parser::parse(bookmarks_list);

    clear_buffer(&content_view);
    draw::gemini_content(&gui, parsed_content);

    update_url_field(&gui, "::bookmarks");

    content_view.show_all();
}

pub fn visit_url<T: AbsoluteUrl + Protocol>(gui: &Arc<Gui>, url: T) {
    if url.get_source_str() == "gemini://::bookmarks" {
        show_bookmarks(&gui);
        return;
    }

    let content_view = gui.content_view();

    match url.get_scheme() {
        Scheme::Gemini => {
            let absolute_url = url.to_absolute_url();

            match absolute_url {
                Ok(absolute_url) => match gemini::client::get_data(Gemini {
                    source: absolute_url.to_string(),
                }) {
                    Ok((meta, new_content)) => {
                        let meta_str = String::from_utf8_lossy(&meta.unwrap()).to_string();

                        if let Ok(status) = Status::from_str(&meta_str) {
                            match status {
                                Status::Success(meta) => {
                                    if meta.starts_with("text/") {
                                        // display text files.
                                        history::append(absolute_url.as_str());
                                        update_url_field(&gui, absolute_url.as_str());
                                        let content_str =
                                            String::from_utf8_lossy(&new_content).to_string();

                                        clear_buffer(&content_view);
                                        if meta.starts_with("text/gemini") {
                                            let parsed_content = gemini::parser::parse(content_str);
                                            draw::gemini_content(&gui, parsed_content);
                                        } else {
                                            // just a text file
                                            draw::gemini_text_content(&gui, content_str.lines());
                                        }

                                        content_view.show_all();
                                    } else {
                                        // download and try to open the rest.
                                        client::download(new_content);
                                    }
                                }
                                Status::Gone(_meta) => {
                                    dialog::error(&gui, "\nSorry page is gone.\n");
                                }
                                Status::RedirectTemporary(new_url)
                                | Status::RedirectPermanent(new_url) => {
                                    history::append(absolute_url.as_str());
                                    visit_url(&gui, Gemini { source: new_url });
                                }
                                Status::TransientCertificateRequired(_meta)
                                | Status::AuthorisedCertificatedRequired(_meta) => {
                                    dialog::error(
                                        &gui,
                                        "\nYou need a valid certificate to access this page.\n",
                                    );
                                }
                                Status::Input(message) => {
                                    dialog::input(&gui, absolute_url, &message);
                                }
                                _ => (),
                            }
                        }
                    }
                    Err(e) => {
                        dialog::error(&gui, &format!("\n{}\n", e));
                    }
                },
                Err(e) => {
                    dialog::error(&gui, &format!("\n{}\n", e));
                }
            }
        }
        Scheme::Gopher => {
            let absolute_url = url.to_absolute_url();
            match absolute_url {
                Ok(abs_url) => match gopher::client::get_data(url) {
                    Ok((_meta, new_content)) => {
                        history::append(abs_url.as_str());
                        update_url_field(&gui, abs_url.as_str());
                        let content_str = String::from_utf8_lossy(&new_content).to_string();

                        let parsed_content = gopher::parser::parse(content_str);
                        clear_buffer(&content_view);
                        draw::gopher_content(&gui, parsed_content);

                        content_view.show_all();
                    }
                    Err(e) => {
                        dialog::error(&gui, &format!("\n{}\n", e));
                    }
                },
                Err(e) => {
                    dialog::error(&gui, &format!("\n{}\n", e));
                }
            }
        }
        Scheme::Finger => {
            let absolute_url = url.to_absolute_url();
            match absolute_url {
                Ok(abs_url) => match finger::client::get_data(url) {
                    Ok((_meta, new_content)) => {
                        history::append(abs_url.as_str());
                        update_url_field(&gui, abs_url.as_str());
                        let content_str = String::from_utf8_lossy(&new_content).to_string();

                        let parsed_content = finger::parser::parse(content_str);
                        clear_buffer(&content_view);
                        draw::finger_content(&gui, parsed_content);

                        content_view.show_all();
                    }
                    Err(e) => {
                        dialog::error(&gui, &format!("\n{}\n", e));
                    }
                },
                Err(e) => {
                    dialog::error(&gui, &format!("\n{}\n", e));
                }
            }
        }
    }
}

fn clear_buffer(view: &gtk::TextView) {
    if let Some(buffer) = view.get_buffer() {
        let (mut start, mut end) = buffer.get_bounds();
        buffer.delete(&mut start, &mut end);
    }
}
