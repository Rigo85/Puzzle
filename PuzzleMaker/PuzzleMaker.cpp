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

#include <PuzzleMaker.h>

#include <opencv2/opencv.hpp>

#include <random>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

std::optional<output_pair> PuzzleMaker::getPuzzle(
        const std::string &fileName,
        const pairs &distribution,
        const uint &side) {
    cv::Mat inputImage = cv::imread(fileName);

    if (inputImage.empty()) {
        return std::nullopt;
    }

    auto imageWidth = inputImage.cols;
    auto imageHeight = inputImage.rows;
    auto roiCount = side;

    cv::Mat outputImage;
    inputImage.copyTo(outputImage);

    auto roiWidth = imageWidth / roiCount;
    auto roiHeight = imageHeight / roiCount;

    std::random_device rd;
    std::mt19937 g(rd());

    pairs _distribution = distribution;
    if (_distribution.empty()) {
        for (auto i = 0u; i < imageWidth / roiWidth; ++i) {
            for (auto j = 0u; j < imageHeight / roiHeight; ++j) {
                _distribution.emplace_back(i, j);
            }
        }

        std::shuffle(_distribution.begin(), _distribution.end(), g);
    }

    auto pos = 0u;

    for (auto i = 0u; i < imageWidth / roiWidth; ++i) {
        for (auto j = 0u; j < imageHeight / roiHeight; ++j) {
            cv::Rect roi(i * roiWidth, j * roiHeight, roiWidth, roiHeight);

            auto pair = _distribution[pos++];
            cv::Rect roi2(pair.first * roiWidth, pair.second * roiHeight, roiWidth, roiHeight);

            inputImage(roi2).copyTo(outputImage(roi));
        }
    }

    auto path = fs::temp_directory_path() / fs::path((std::string) "PuzzleImage" + std::to_string(g()) + ".png");

    cv::imwrite(path.string(), outputImage);

    return std::make_optional(std::make_pair(path.string(), _distribution));
}
