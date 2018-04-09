// Pegasus Frontend
// Copyright (C) 2018  Mátyás Mustoha
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.


#pragma once

#include "GamepadAxisNavigation.h"

#include <QFile>
#include <QGamepadKeyNavigation>
#include <QTextStream>
#include <vector>


namespace backend {

void setup_global();

class Context {
public:
    Context();
    ~Context();
    Context(const Context&) = delete;
    Context& operator=(const Context&) = delete;

private:
    QFile logfile;
    QGamepadKeyNavigation padkeynav;
    GamepadAxisNavigation padaxisnav;

    void setup_logging();
    void setup_gamepad();
};

} // namespace backend