//
// Author Rigoberto Leander Salgado Reyes <rlsalgado2006@gmail.com>
//
// Copyright 2018 by Rigoberto Leander Salgado Reyes.
//
// This program is licensed to you under the terms of version 3 of the
// GNU Affero General Public License. This program is distributed WITHOUT
// ANY EXPRESS OR IMPLIED WARRANTY, INCLUDING THOSE OF NON-INFRINGEMENT,
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE. Please refer to the
// AGPL (http:www.gnu.org/licenses/agpl-3.0.txt) for more details.
//

#include <iostream>

#include <gtkmm/filechooserdialog.h>
#include <gtkmm/messagedialog.h>
#include <gtkmm/aboutdialog.h>

#include <PuzzleSolverWindow.h>
#include <Injector.h>

PuzzleSolverWindow::PuzzleSolverWindow(goatnative::Injector &injector) {
    puzzleMaker = injector.getInstance<IPuzzleMaker>();

    workerThread = nullptr;
    ui = {Gtk::Builder::create_from_file("resources/PuzzleSolverUI.glade")};
    this->set_icon(Gdk::Pixbuf::create_from_file("resources/images/logo.png", -1, 40, true));

    addStyle();
    createAboutDialog();

    if (ui) {
        ui->get_widget<Gtk::Box>("boxLayout1", boxLayout1);

        menuItemOpenImage = Glib::RefPtr<Gtk::MenuItem>::cast_dynamic(ui->get_object("menuItemOpen"));
        menuItemQuitApp = Glib::RefPtr<Gtk::MenuItem>::cast_dynamic(ui->get_object("menuItemQuit"));
        menuItemAbout = Glib::RefPtr<Gtk::MenuItem>::cast_dynamic(ui->get_object("menuItemAbout"));
        imagePuzzle = Glib::RefPtr<Gtk::Image>::cast_dynamic(ui->get_object("imagePuzzle"));
        buttonMakePuzzle = Glib::RefPtr<Gtk::Button>::cast_dynamic(ui->get_object("buttonMakePuzzle"));
        buttonStartStop = Glib::RefPtr<Gtk::ToggleButton>::cast_dynamic(ui->get_object("buttonStartStop"));
        labelFitness = Glib::RefPtr<Gtk::Label>::cast_dynamic(ui->get_object("labelFitness"));
        labelIterationCount = Glib::RefPtr<Gtk::Label>::cast_dynamic(ui->get_object("labelIterationCount"));
        labelPuzzleIteration = Glib::RefPtr<Gtk::Label>::cast_dynamic(ui->get_object("labelPuzzleIteration"));

        if (boxLayout1 && checkWidgets({menuItemOpenImage, menuItemQuitApp, menuItemAbout})) {
            menuItemAbout->signal_activate().connect([this]() {
                aboutDialog.show();
                aboutDialog.present();
            });
            menuItemOpenImage->signal_activate().connect([this]() { onMenuItemOpenImageActivate(); });
            menuItemQuitApp->signal_activate().connect([this] { this->close(); });
            buttonMakePuzzle->signal_clicked().connect([this]() { onButtonMakePuzzleClicked(); });
            buttonStartStop->signal_toggled().connect([this]() { onButtonStartStopToggled(); });
            this->signal_delete_event().connect(sigc::mem_fun(*this, &PuzzleSolverWindow::onAppQuit));

            dispatcher.connect(sigc::mem_fun(*this, &PuzzleSolverWindow::onNotificationFromWorkerThread));

            add(*boxLayout1);
        }
    }
    set_title("Puzzle");
    set_default_size(800, 800);
    show_all();
}

void PuzzleSolverWindow::addStyle() {
    GError *error = nullptr;
    const gchar *home = "resources/style.css";
    GtkCssProvider *cssProvider = gtk_css_provider_new();
    GdkDisplay *display = gdk_display_get_default();
    GdkScreen *screen = gdk_display_get_default_screen(display);
    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER (cssProvider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    gtk_css_provider_load_from_file(cssProvider, g_file_new_for_path(home), &error);
    g_object_unref(cssProvider);
}

void PuzzleSolverWindow::createAboutDialog() {
    aboutDialog.set_transient_for(*this);
    aboutDialog.signal_response().connect(sigc::mem_fun(*this, &PuzzleSolverWindow::onAboutDialogResponse));

    aboutDialog.set_logo(Gdk::Pixbuf::create_from_file("resources/images/logo.png", -1, 40, true));
    aboutDialog.set_program_name("Puzzle");
    aboutDialog.set_version("1.0.0");
    aboutDialog.set_copyright("Rigoberto L. Salgado Reyes");
    aboutDialog.set_comments("Puzzle maker with metaheuristics.");
    aboutDialog.set_license("AGPL");

    aboutDialog.set_website("https://github.com/Rigo85/PuzzleMaker");
    aboutDialog.set_website_label("Puzzle Website");

    aboutDialog.set_authors({"Rigoberto L. Salgado Reyes"});
}

void PuzzleSolverWindow::onMenuItemOpenImageActivate() {
    Gtk::FileChooserDialog dialog("Please choose an image file", Gtk::FILE_CHOOSER_ACTION_OPEN);
    dialog.set_transient_for(*this);

    dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
    dialog.add_button("_Open", Gtk::RESPONSE_OK);

    auto filter_text = Gtk::FileFilter::create();
    filter_text->set_name("Image files");
    filter_text->add_mime_type("image/png");
    filter_text->add_mime_type("image/jpeg");
    filter_text->add_mime_type("image/jpg");
    filter_text->add_mime_type("image/bmp");
    dialog.add_filter(filter_text);

    auto filter_any = Gtk::FileFilter::create();
    filter_any->set_name("Any files");
    filter_any->add_pattern("*");
    dialog.add_filter(filter_any);

    if (dialog.run() == Gtk::RESPONSE_OK) {
        auto response = puzzleMaker->getPuzzle(dialog.get_filename(), {}, 5);
        imagePuzzle->set(Gdk::Pixbuf::create_from_file(response->first, 600, 600, true));
    }
}

bool PuzzleSolverWindow::checkWidgets(std::initializer_list<Glib::RefPtr<Gtk::Widget>> widgets) {
    return std::all_of(widgets.begin(), widgets.end(), [](auto w) { return w; });
}

void PuzzleSolverWindow::onButtonMakePuzzleClicked() {
    Gtk::MessageDialog dialog(*this, "Making puzzle!", false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK);
    dialog.set_secondary_text("And this is the secondary text that explains things.");

    dialog.run();
}

void PuzzleSolverWindow::onButtonStartStopToggled() {
    buttonStartStop->set_label(buttonStartStop->get_active() ? "Stop" : "Start");
    buttonMakePuzzle->set_sensitive(!buttonStartStop->get_active());

    buttonStartStop->get_active() ? onStart() : onStop();
}

void PuzzleSolverWindow::onStart() {
    if (workerThread) {
        std::cout << "Can't start a worker thread while another one is running." << std::endl;
    } else {
        workerThread = std::make_unique<std::thread>([this] { worker.doWork(*this); });
    }
    const bool threadIsRunning = workerThread != nullptr;
    updateButtons(threadIsRunning);
}

void PuzzleSolverWindow::onStop() {
    if (!workerThread) {
        std::cout << "Can't stop a worker thread. None is running." << std::endl;
    } else {
        worker.stopWork();
        const bool threadIsRunning = false;
        updateButtons(threadIsRunning);
    }
}

void PuzzleSolverWindow::notify() {
    dispatcher.emit();
}

void PuzzleSolverWindow::updateButtons(const bool threadIsRunning) {
    buttonMakePuzzle->set_sensitive(!threadIsRunning);
    buttonStartStop->set_active(threadIsRunning);
}

void PuzzleSolverWindow::update_widgets() {
    Notification notification;
    worker.getData(notification);

    labelIterationCount->set_label("iteration Count: " + std::to_string(notification.iteration));
    labelFitness->set_label("Fitness: " + std::to_string(notification.fitness));
}

void PuzzleSolverWindow::onQuit() {
    if (workerThread) {
        worker.stopWork();
        if (workerThread->joinable())
            workerThread->join();
    }
}

void PuzzleSolverWindow::onNotificationFromWorkerThread() {
    if (workerThread && worker.isStopped()) {
        if (workerThread->joinable())
            workerThread->join();
        //delete workerThread;
        workerThread = nullptr;
        updateButtons(false);
    }
    update_widgets();
}

void PuzzleSolverWindow::onAboutDialogResponse(int response_id) {
    switch (response_id) {
        case Gtk::RESPONSE_CLOSE:
        case Gtk::RESPONSE_CANCEL:
        case Gtk::RESPONSE_DELETE_EVENT:
            aboutDialog.hide();
            break;
        default:
            std::cout << "Unexpected response!" << std::endl;
            break;
    }
}

bool PuzzleSolverWindow::onAppQuit(GdkEventAny *any_event) {
    Gtk::MessageDialog msgDialog(*this, "Are you sure you want to close?", false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_OK_CANCEL);
    auto response = msgDialog.run();

    if (response == Gtk::RESPONSE_OK) {
        onQuit();
    }

    return !(response == Gtk::RESPONSE_OK);
}




