// use gtk4::builders::{ListBoxBuilder, ListBoxRowBuilder};
// use gtk4::{prelude::*};
// use gtk4::{Application, ApplicationWindow, Button, Entry, builders::BoxBuilder, Label, ListBoxRow};
use async_std::task;
use async_std::fs::{ReadDir};
use futures_lite::stream::NextFuture;
use async_std::prelude::*;
use std::collections::hash_map;
use std::ffi::OsStr;
use std::path::Path;

use jpeg_decoder::Decoder;
use std::fs::File;
use std::io::BufReader;

use rexif::{parse_buffer, ExifEntry, TagValue};
use rexif::ExifTag;

use std::collections::HashMap;
use std::collections::BTreeMap;

const APP_ID: &str = "org.plok.photomanager";

struct SideEntry {
    label: String,
    path: String
}

fn main() {

    task::block_on(async {
        let dir = "/home/simon/Pictures/202207-Liam/";
        let files = get_dir_entries(dir).await; // TODO make pipeline

        let mut counter: i32 = 0;

        let mut rename_list: BTreeMap::<String, String> = BTreeMap::new();
        for filename in files {
            let path = Path::new(&filename);
            let ext = path
                .extension()
                .and_then(OsStr::to_str).expect("No extension found");
                
            let lower_ext = ext.to_ascii_lowercase();

            if lower_ext != "jpg" {
                continue;
            }
            
            counter += 1;

            let file = File::open(filename.clone()).expect("failed to open file");
            let mut decoder = Decoder::new(BufReader::new(file));
            let read_info_result = decoder.read_info();
            if (read_info_result.is_err()) {
                println!("failed to read metadata: {}", filename);
                continue;
            }
            let metadata = decoder.info().unwrap();

            // println!("{}x{}", metadata.width, metadata.height);

            let exif_result = decoder.exif_data();
            if (exif_result.is_none()) {
                println!("failed to read exif");
                continue;
            }

            let exif_parse_result = parse_buffer(exif_result.unwrap());
            if (exif_parse_result.is_err()) {
                println!("Failed parsing exif: {}", exif_parse_result.err().unwrap());
                continue;
            }

            let exif = exif_parse_result.unwrap();
            let mut map = HashMap::<ExifTag, ExifEntry>::new();
            for e in exif.entries.clone() {
                // println!("Exif: {}: {}", e.tag.to_string(), e.value_more_readable);
                map.insert(e.tag, e);
            }

            if !map.contains_key(&ExifTag::DateTime) {
                println!("No datetime");
                for e in exif.entries {
                    println!("Exif: {}: {}", e.tag.to_string(), e.value_more_readable);
                }
                continue;
            }

            let date = map[&ExifTag::DateTime].clone();
            let mut new_file_name_opt : Option<String> = None;
            if let TagValue::Ascii(v) = date.value {
                new_file_name_opt = Some(format!("{}/{}-{:04}.jpg", dir, v.replace(":", "").replace(" ", "-"), counter));
            }

            rename_list.insert(new_file_name_opt.clone().expect("fail!"), filename.clone());

            println!("found jpeg: {}: {} -> {}", date.value_more_readable, path.to_string_lossy(), new_file_name_opt.unwrap());
        }

        for (new_file, old_file) in rename_list {
            if (Path::new(new_file.as_str()).exists()) {
                println!("Destination already exist: {}", new_file.as_str());
                continue;
            }

            let result = std::fs::rename(old_file.clone(), new_file);
            if let Err(e) = result {
                println!("Error renaming: {}\n {}", old_file, e.to_string());
            }
        }
    })
    

    // // Create a new application
    // let app = Application::builder().application_id(APP_ID).build();

    // // Connect to "activate" signal of `app`
    // app.connect_activate(build_ui);

    // // Run the application
    // app.run();
}

async fn get_dir_entries(path: &str) -> Vec<String> {
    let mut dir_stream = async_std::fs::read_dir(path).await.expect("failed reading dir");

    let mut result = Vec::<String>::new();

    while let Some(res) = dir_stream.next().await {
        let entry = res.expect("fail");
        println!("{}", entry.path().to_string_lossy());

        result.push(entry.path().to_string_lossy().as_ref().to_string());
    }

    return result;
}

// fn build_sidebar() -> gtk4::Box {
//     let list = ListBoxBuilder::new()
//         .build();

//         list.connect_row_selected(move |s, row: Option<&ListBoxRow>| {
//             let mut path = String::new();
//             unsafe {
//                 path = row.unwrap().data::<String>("path").unwrap().as_ref().to_string();
//             }

//             task::block_on(async {
//                 get_dir_entries(path).await;
//             })
            
//         });

//     let opt_home_dir = dirs::home_dir().expect("Failed retreiving home directory");
//     let home_dir = opt_home_dir.as_path().to_str().unwrap().clone();

//     let mut listmodel: Vec<SideEntry> = Vec::from([
//         SideEntry{label: String::from("Home"), path: String::from(home_dir)},
//         SideEntry{label: String::from("Downloads"), path: String::from(format!("{}/Downloads", home_dir ))}
//         ]);

//     for it in listmodel {
//         let label = Label::new(Some(&it.label));
//         let row = ListBoxRowBuilder::new()
//             .child(&label)
//             .activatable(true)
//             .build();
//         unsafe {
//             row.set_data::<String>("path", it.path);
//         }
        
//         list.append(&row);
//     }

//     let sidebox = BoxBuilder::new()
//     .orientation(gtk4::Orientation::Vertical)
//     .build();

//     sidebox.append(&list);

//     return sidebox;
// }

// fn build_ui(app: &Application) {
//     // Create a button with label and margins
//     let button = Button::builder()
//         .label("Press me!")
//         .build();

//     // Connect to "clicked" signal of `button`
//     button.connect_clicked(move |button| {
//         // Set the label to "Hello World!" after the button has been clicked on
//         button.set_label("Hello World!");
//     });

//     let entry = Entry::builder()
//         .placeholder_text("Enter path")
//         .build();

//     let vbox = BoxBuilder::new()
//         .orientation(gtk4::Orientation::Vertical)
//         .build();

//     vbox.append(&entry);
//     vbox.append(&button);
//     vbox.set_margin_end(16);

//     let sidebox = build_sidebar();

//     let hbox = BoxBuilder::new()
//     .orientation(gtk4::Orientation::Horizontal)
//     .build();

//     hbox.append(&sidebox);
//     hbox.append(&vbox);

//     // Create a window and set the title
//     let window = ApplicationWindow::builder()
//         .application(app)
//         .title("Plok PhotoManager")
//         .build();

//     // hbox.set_margin_start(16);
//     hbox.set_margin_end(16);
//     hbox.set_margin_top(16);
//     hbox.set_margin_bottom(16);
//     hbox.set_spacing(16);

//     window.set_child(Some(&hbox));

//     // Present window
//     window.present();
// }
