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

#include <PuzzleMakerImpl.h>
#include <PuzzleMaker.h>

std::optional<output_pair> PuzzleMakerImpl::getPuzzle(
        const std::string &fileName,
        const pairs &distribution,
        const uint &side) const {
    return PuzzleMaker::getPuzzle(fileName, distribution, side);
}
