use gtk4::builders::{ListBoxBuilder, ListBoxRowBuilder};
use gtk4::{prelude::*};
use gtk4::{Application, ApplicationWindow, Button, Entry, builders::BoxBuilder, Label, ListBoxRow};
use async_std::task;
use async_std::fs::{ReadDir};
use futures_lite::stream::NextFuture;
use async_std::prelude::*;

const APP_ID: &str = "org.plok.photomanager";

struct SideEntry {
    label: String,
    path: String
}

fn main() {
    // Create a new application
    let app = Application::builder().application_id(APP_ID).build();

    // Connect to "activate" signal of `app`
    app.connect_activate(build_ui);

    // Run the application
    app.run();
}

async fn get_dir_entries(path: String) {
    let mut dir_stream = async_std::fs::read_dir(path).await.expect("failed reading dir");

    let mut result = Vec::new();

    while let Some(res) = dir_stream.next().await {
        let entry = res.expect("fail");
        println!("{}", entry.path().to_string_lossy());

        result.push(entry.path());
    }
}

fn build_sidebar() -> gtk4::Box {
    let list = ListBoxBuilder::new()
        .build();

        list.connect_row_selected(move |s, row: Option<&ListBoxRow>| {
            let mut path = String::new();
            unsafe {
                path = row.unwrap().data::<String>("path").unwrap().as_ref().to_string();
            }

            task::block_on(async {
                get_dir_entries(path).await;
            })
            
        });

    let opt_home_dir = dirs::home_dir().expect("Failed retreiving home directory");
    let home_dir = opt_home_dir.as_path().to_str().unwrap().clone();

    let mut listmodel: Vec<SideEntry> = Vec::from([
        SideEntry{label: String::from("Home"), path: String::from(home_dir)},
        SideEntry{label: String::from("Downloads"), path: String::from(format!("{}/Downloads", home_dir ))}
        ]);

    for it in listmodel {
        let label = Label::new(Some(&it.label));
        let row = ListBoxRowBuilder::new()
            .child(&label)
            .activatable(true)
            .build();
        unsafe {
            row.set_data::<String>("path", it.path);
        }
        
        list.append(&row);
    }

    let sidebox = BoxBuilder::new()
    .orientation(gtk4::Orientation::Vertical)
    .build();

    sidebox.append(&list);

    return sidebox;
}

fn build_ui(app: &Application) {
    // Create a button with label and margins
    let button = Button::builder()
        .label("Press me!")
        .build();

    // Connect to "clicked" signal of `button`
    button.connect_clicked(move |button| {
        // Set the label to "Hello World!" after the button has been clicked on
        button.set_label("Hello World!");
    });

    let entry = Entry::builder()
        .placeholder_text("Enter path")
        .build();

    let vbox = BoxBuilder::new()
        .orientation(gtk4::Orientation::Vertical)
        .build();

    vbox.append(&entry);
    vbox.append(&button);
    vbox.set_margin_end(16);

    let sidebox = build_sidebar();

    let hbox = BoxBuilder::new()
    .orientation(gtk4::Orientation::Horizontal)
    .build();

    hbox.append(&sidebox);
    hbox.append(&vbox);

    // Create a window and set the title
    let window = ApplicationWindow::builder()
        .application(app)
        .title("Plok PhotoManager")
        .build();

    // hbox.set_margin_start(16);
    hbox.set_margin_end(16);
    hbox.set_margin_top(16);
    hbox.set_margin_bottom(16);
    hbox.set_spacing(16);

    window.set_child(Some(&hbox));

    // Present window
    window.present();
}
