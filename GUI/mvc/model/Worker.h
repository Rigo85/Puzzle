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

#include <mutex>

#include <gtkmm.h>

#include <INotifiable.h>
#include <Notification.h>

class Worker {
private:
    mutable std::mutex mutex;
    bool shallStop;
    bool hasStopped;
    Notification notification;

public:
    Worker();

    virtual ~Worker() = default;

    void doWork(INotifiable& caller);

    void getData(Notification &) const;

    void stopWork();

    bool isStopped() const;
};
