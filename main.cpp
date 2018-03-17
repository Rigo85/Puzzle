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

#include <gtkmm.h>

#include <Injector.h>

#include <PuzzleSolverWindow.h>
#include <PuzzleMakerImpl.h>
#include <PuzzleSolverImpl.h>

int main(int argc, char *argv[]) {
    auto app = Gtk::Application::create(argc, argv);

    goatnative::Injector injector;
    injector.registerClass<PuzzleMakerImpl>();
    injector.registerClass<PuzzleSolverImpl>();
    injector.registerInterface<IPuzzleMaker, PuzzleMakerImpl>();
    injector.registerInterface<IPuzzleSolver, PuzzleSolverImpl>();

    PuzzleSolverWindow window(injector);

    return app->run(window);
}
