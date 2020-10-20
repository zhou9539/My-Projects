/*
 * Copyright 2020 Laurence Alexander Hurst
 *
 * This file is part of C++ for Finance.
 *
 * C++ for Finance is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * C++ for Finance is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with C++ for Finance.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <memory>
#include <string>
#include <exception>
#include <istream>
#include "InputReader.hpp"

std::unique_ptr<std::string> InputReader::GetNextField() {
    bool finished {false}; // Have we reached the end of the string?

    while(!finished) {
        // Test if stream is safe to use
        if (!input_stream_->good())
            throw std::runtime_error {"Problem with input stream in GetNextField"};

        // Get next character from file
        char in_char{'\0'};
        input_stream_->get(in_char);

        // If hit end of file, won't have read any char
        if (input_stream_->eof())
            finished = true;
        else
            finished = ProcessChar(in_char);
    }

    // Return a unique pointer to the string (to avoid creating unnecessary copies)
    std::unique_ptr<std::string> result {std::make_unique<std::string>(current_field_)};
    // Clear the field read for the next input
    current_field_ = "";
    // Return the pointer
    return result;
}

