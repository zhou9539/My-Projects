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

#include "FixedWidthInputReader.hpp"

bool FixedWidthInputReader::ProcessChar(const char in_char) {
    // Newlines separate rows
    if (in_char == '\n') {
        // Reset position in the current row to character 0, field 0
        current_field_count_ = 0;
        current_field_chars_ = 0;
        /*
         * Will have already signalled the end of the last field when the
         * final character was read.
         */
        return false;
    }

    // There are no separators, all non-newline characters belong to a field
    current_field_ += in_char;

    // Add one to the count of characters read into the current field
    ++current_field_chars_;

    /*
     * If the number of characters read now equals the width of the current
     * then that is the end of this field.
     */
    if (field_widths_.at(current_field_count_) == current_field_chars_) {
        // The next character will belong to the next field
        ++current_field_count_;
        /*
         * ...and will be the first character of that field (we have read 0
         * characters from the next field so far)
         */
        current_field_chars_ = 0;
        return true; // End of field
    }

    return false; // Unless that was the last character, not end of field
}
