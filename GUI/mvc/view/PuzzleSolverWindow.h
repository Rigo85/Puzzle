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

#pragma once

#include <gtkmm/applicationwindow.h>
#include <gtkmm/builder.h>
#include <gtkmm/menuitem.h>
#include <gtkmm/box.h>
#include <gtkmm/image.h>
#include <gtkmm/togglebutton.h>
#include <giomm.h>

#include <thread>

#include <Worker.h>
#include <INotifiable.h>

class PuzzleSolverWindow : public Gtk::ApplicationWindow, INotifiable {
private:
    Gtk::Box *boxLayout1;
    Glib::RefPtr<Gtk::Builder> ui;
    Glib::RefPtr<Gtk::MenuItem> menuItemOpenImage;
    Glib::RefPtr<Gtk::MenuItem> menuItemQuitApp;
    Glib::RefPtr<Gtk::MenuItem> menuItemAbout;
    Glib::RefPtr<Gtk::Image> imagePuzzle;
    Glib::RefPtr<Gtk::Button> buttonMakePuzzle;
    Glib::RefPtr<Gtk::ToggleButton> buttonStartStop;
    Glib::RefPtr<Gtk::Label> labelIterationCount;
    Glib::RefPtr<Gtk::Label> labelFitness;
    Glib::RefPtr<Gtk::Label> labelPuzzleIteration;
    Gtk::AboutDialog aboutDialog;

    Glib::Dispatcher dispatcher;
    Worker worker;
    std::unique_ptr<std::thread> workerThread;

public:
    PuzzleSolverWindow();

    virtual ~PuzzleSolverWindow() = default;

private:
    void onMenuItemOpenImageActivate();

    void onButtonMakePuzzleClicked();

    void onButtonStartStopToggled();

    void onStart();

    void onStop();

    void onQuit();

    void onNotificationFromWorkerThread();

    void onAboutDialogResponse(int response_id);

    bool onAppQuit(GdkEventAny*);


    void updateButtons(bool);

    void update_widgets();

    bool checkWidgets(std::initializer_list<Glib::RefPtr<Gtk::Widget>> widgets);

    void notify() override;

    void addStyle();

    void createAboutDialog();
};



