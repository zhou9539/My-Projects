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
#ifndef INPUTREADER_FIXEDWIDTHINPUTREADER_HPP
#define INPUTREADER_FIXEDWIDTHINPUTREADER_HPP

#include "InputReader.hpp"

#include <array>

// fixed-width field input stream reader
class FixedWidthInputReader : public InputReader {
private:
    // Array to hold the field widths.
    const std::array<size_t, 9> field_widths_ {4, 8, 5, 1, 2, 6, 6, 8, 8};
    // Counter of how many fields from the current row have been read
    size_t current_field_count_;
    // Counter of how many characters from the current field have been read
    size_t current_field_chars_;
protected:
    virtual bool ProcessChar(const char) override;
public:
    // Constructor which delegates to InputReader for handling the stream
    FixedWidthInputReader(std::shared_ptr<std::istream> input_stream)
    : InputReader(input_stream), current_field_count_{0},
      current_field_chars_{0}
    {}
};

#endif //INPUTREADER_FIXEDWIDTHINPUTREADER_HPP
