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

#include <thread>
#include <iostream>

#include <Worker.h>

Worker::Worker() {
    shallStop = false;
    hasStopped = false;
}

void Worker::getData(Notification &src) const {
    std::lock_guard<std::mutex> lock(mutex);
    src = notification;
}

void Worker::stopWork() {
    std::lock_guard<std::mutex> lock(mutex);
    shallStop = true;
}

bool Worker::isStopped() const {
    std::lock_guard<std::mutex> lock(mutex);
    return hasStopped;
}

void Worker::doWork(INotifiable &caller) {
    {
        std::lock_guard<std::mutex> lock(mutex);
        hasStopped = false;
        notification.fitness = 0.0;
        notification.iteration = 0;
    }

    for (int i = 0;; ++i)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        {
            std::lock_guard<std::mutex> lock(mutex);

            notification.fitness += 0.01;
            notification.iteration++;

            if (notification.fitness >= 1.0) {
                break;
            }

            if (shallStop) {
                break;
            }
        }

        caller.notify();
    }

    {
        std::lock_guard<std::mutex> lock(mutex);
        shallStop = false;
        hasStopped = true;
    }

    caller.notify();

}
