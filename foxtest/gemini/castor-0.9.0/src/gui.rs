use gtk::prelude::*;
use gtk::{ApplicationWindow, Button, Entry, TextView};

use gdk::WindowExt;

pub struct Gui {
    window: ApplicationWindow,
    url_bar: Entry,
    content_view: TextView,
    back_button: Button,
    forward_button: Button,
    refresh_button: Button,
    add_bookmark_button: Button,
    show_bookmarks_button: Button,
}

impl Default for Gui {
    fn default() -> Self {
        Self::new()
    }
}

impl Gui {
    pub fn new() -> Gui {
        // Initialize the UI from the Glade XML.
        let glade_src = include_str!("castor.glade");
        let builder = gtk::Builder::new_from_string(glade_src);

        // Get handles for the various controls we need to use.
        let window: ApplicationWindow = builder.get_object("window").expect("Couldn't get window");
        let url_bar: Entry = builder.get_object("url_bar").expect("Couldn't get url_bar");
        let content_view: TextView = builder
            .get_object("content_view")
            .expect("Couldn't get content_view");
        let back_button: Button = builder
            .get_object("back_button")
            .expect("Couldn't get back_button");
        let forward_button: Button = builder
            .get_object("forward_button")
            .expect("Couldn't get forward_button");
        let refresh_button: Button = builder
            .get_object("refresh_button")
            .expect("Couldn't get refresh_button");
        let add_bookmark_button: Button = builder
            .get_object("add_bookmark_button")
            .expect("Couldn't get add_bookmark_button");
        let show_bookmarks_button: Button = builder
            .get_object("show_bookmarks_button")
            .expect("Couldn't get show_bookmarks_button");

        Gui {
            window,
            url_bar,
            content_view,
            back_button,
            forward_button,
            refresh_button,
            add_bookmark_button,
            show_bookmarks_button,
        }
    }

    pub fn start(&self) {
        glib::set_application_name("Castor Browser");
        self.window.set_role("Castor Browser");
        self.window.connect_delete_event(|_, _| {
            gtk::main_quit();
            Inhibit(false)
        });
        self.content_view.connect_motion_notify_event(|win, _| {
            let w = gtk::TextViewExt::get_window(win, gtk::TextWindowType::Text).unwrap();
            w.set_cursor(gdk::Cursor::new_from_name(&w.get_display(), "default").as_ref());
            Inhibit(false)
        });
        self.window.show_all();
    }

    pub fn window(&self) -> &ApplicationWindow {
        &self.window
    }

    pub fn url_bar(&self) -> &Entry {
        &self.url_bar
    }

    pub fn content_view(&self) -> &TextView {
        &self.content_view
    }

    pub fn back_button(&self) -> &Button {
        &self.back_button
    }

    pub fn forward_button(&self) -> &Button {
        &self.forward_button
    }

    pub fn refresh_button(&self) -> &Button {
        &self.refresh_button
    }

    pub fn add_bookmark_button(&self) -> &Button {
        &self.add_bookmark_button
    }

    pub fn show_bookmarks_button(&self) -> &Button {
        &self.show_bookmarks_button
    }
}
