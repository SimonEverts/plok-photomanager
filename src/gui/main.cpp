#include "katla/core/core-application.h"

#include "katla/gtk4/gtk-application.h"

#include "katla/gtk4/gtk-text-impl.h"
#include "katla/gtk4/gtk-button-impl.h"
#include "katla/gtk4/gtk-column.h"
#include "katla/gtk4/gtk-row.h"
#include "katla/gtk4/gtk-list-view.h"

#include "string.h"
#include <iostream>
#include <vector>

int main(int argc, char* argv[])
{
    std::vector<std::string> args;
    for (int i=0; i<argc; i++) {
        std::string arg = argv[i];
        args.push_back(arg);
    }

    katla::GtkApplication gtkApplication;
    gtkApplication.init(argc, argv, "Plok Photomanager");

    std::shared_ptr<katla::Window> window;

    auto listview = std::make_shared<katla::GtkListView>();

//     auto genListEntry = [](const backer::FileSystemEntry& file) {
//         auto entryLabel = std::make_shared<katla::GtkTextImpl>();
//         entryLabel->updateText({katla::format("{}", file.absolutePath)});
//         entryLabel->init();

//         auto row = std::make_shared<katla::GtkRow>();
//         row->updateContainer({
//                                      .children = {
//                                              katla::ContainerChild{.child = entryLabel, true, true, 16},
// //                                             katla::ContainerChild{.child = entryButton, false, true, 0}
//                                      }
//                              });
//         row->init();
//         row->show();
//         return row;
//     };

    gtkApplication.onStartup([&gtkApplication, &window, &listview]() {
        katla::printInfo("startup!");

        window = gtkApplication.createWindow("Plok Photomanager", {800,600});

        auto textInput = 

        auto label = std::make_shared<katla::GtkTextImpl>();
        label->updateText({"File list:"});
        label->init();

        listview = std::make_shared<katla::GtkListView>();
        listview->updateContainer({
            .children = {}
        });

        listview->init();
        listview->show();

        auto column = std::make_shared<katla::GtkColumn>();
        column->init();

        column->append(label, {});
        column->append(listview, {.expand = true, .fill = true});

        window->updateContainer({
                .children = {katla::ContainerChild{.child = std::static_pointer_cast<katla::Widget>(column)}}
        });
    });

   

    gtkApplication.onActivate([&window, &listview]() {
        window->show();

        listview->onRowSelected([](int index) {
            katla::printInfo("Row selected {}", index);

            // duplicateListView->clear();
            // for (auto& file : fileMap[keys[index]]) {
            //     duplicateListView->addWidget(genListEntry(*file));
            // }
        });

    });

    return gtkApplication.run();
}
