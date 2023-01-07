use gtk::prelude::*;
use gtk::ResponseType;
use std::sync::Arc;

use url::{Position, Url};

use crate::gui::Gui;
use crate::protocols::Gemini;

pub fn info(gui: &Arc<Gui>, message: &str) {
    let dialog = gtk::Dialog::new_with_buttons(
        Some("Info"),
        Some(gui.window()),
        gtk::DialogFlags::MODAL,
        &[("Close", ResponseType::Close)],
    );
    dialog.set_default_response(ResponseType::Close);
    dialog.connect_response(|dialog, _| dialog.destroy());

    let content_area = dialog.get_content_area();
    let message = gtk::Label::new(Some(message));
    content_area.add(&message);

    dialog.show_all();
}

pub fn error(gui: &Arc<Gui>, message: &str) {
    let dialog = gtk::Dialog::new_with_buttons(
        Some("Error"),
        Some(gui.window()),
        gtk::DialogFlags::MODAL,
        &[("Close", ResponseType::Close)],
    );
    dialog.set_default_response(ResponseType::Close);
    dialog.connect_response(|dialog, _| dialog.destroy());

    let content_area = dialog.get_content_area();
    let message = gtk::Label::new(Some(message));
    content_area.add(&message);

    dialog.show_all();
}

pub fn input(gui: &Arc<Gui>, url: Url, message: &str) {
    let dialog = gtk::Dialog::new_with_buttons(
        Some(message),
        Some(gui.window()),
        gtk::DialogFlags::MODAL,
        &[
            ("Close", ResponseType::Close),
            ("Send", ResponseType::Accept),
        ],
    );

    let content_area = dialog.get_content_area();
    let entry = gtk::Entry::new();
    content_area.add(&entry);

    dialog.show_all();

    if dialog.run() == gtk::ResponseType::Accept {
        let response = entry.get_text().expect("get_text failed").to_string();
        let cleaned: &str = &url[..Position::AfterPath];
        let full_url = format!("{}?{}", cleaned.to_string(), response);

        crate::visit_url(&gui, Gemini { source: full_url });
    }

    dialog.destroy();
}
