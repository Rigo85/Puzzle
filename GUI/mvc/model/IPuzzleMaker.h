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

#include <iostream>
#include <utility>
#include <vector>
#include <optional>

using output_pair = std::pair <std::string, std::vector<std::pair < uint, uint>>>;
using pairs = std::vector <std::pair<uint, uint>>;

class IPuzzleMaker {
public:
    IPuzzleMaker() = default;

    virtual ~IPuzzleMaker() = default;

    virtual std::optional<output_pair> getPuzzle(const std::string &, const pairs &, const uint &) const = 0;
};
