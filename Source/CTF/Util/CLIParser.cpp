/*
 * Cereon Template Framework, a C++ 23 standard template library.
 * Copyright (c) 2026 The Aridity Team, all rights reserved.
 *
 * This file is part of the Cereon Template Framework project.
 * 
 * Cereon Template Framework is free software: you can redistribute
 * it and/or modify it under the terms of the GNU Lesser General
 * Public License as published by the Free Software Foundation, either
 * version 3 of the License, or any later version.
 * 
 * Cereon Template Framework is distributed in the hope that it will
 * be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Cereon Template Framework. If not, see <https://www.gnu.org/licenses/>. 
 */

#include "CTF.h"
#include "Util/CLIParser.h"

namespace CTF::Utilities {
    /**
     * @brief The constructor for the command-line interface argument parser.
     * @param argc The count of arguments passed into the executable.
     * @param argv The array of arguments passed into the executable.
     */
    CLIParser::CLIParser(const int argc, char* argv[]) : argc_(argc), argv_(argv) {}

    /**
     */
    CLIParser::~CLIParser() = default;

    /**
     * @brief Determines whether the current command-line interface executable has an specific parameter passed.
     * @param option The parameter/option to find.
     * @returns true if the specified parameter has been found; otherwise false.
     */
    bool CLIParser::hasOption(const char* option) const {
        for (int i = 1; i < argc_; ++i) {
            const char* arg = argv_[i];
            if (!A_stricmp(arg, option))
                return true;

            if (const char* eq = A_strchr(arg, '=')) {
                if (size_t len = static_cast<size_t>(eq - arg); !A_strnicmp(arg, option, len) && option[len] == '\0')
                    return true;
            }
        }
        return false;
    }

    /**
     */
    bool CLIParser::hasShortOption(char option) const {
        for (int i = 1; i < argc_; ++i) {
            const char* arg = argv_[i];

            if (arg[0] == '-' && arg[1] == option && arg[2] == '\0')
                return true;
        }

        return false;
    }

    /**
     */
    bool CLIParser::getFlag(const char* option) const {
        return hasOption(option);
    }

    /**
     */
    const char* CLIParser::getPositional(int index) const {
        int pos = 0;

        for (int i = 1; i < argc_; ++i) {
            const char* arg = argv_[i];

            if (arg[0] == '-')
                continue;

            if (pos == index)
                return arg;

            pos++;
        }

        return nullptr;
    }

    /**
     */
    const char* CLIParser::getOptionValue(const char* option) const {
        for (int i = 1; i < argc_; ++i)
        {
            const char* arg = argv_[i];
            if (!A_stricmp(arg, option)) {
                if (i + 1 < argc_)
                    return argv_[i + 1];

                return nullptr;
            }

            if (const char* eq = A_strchr(arg, '=')) {
                size_t len = static_cast<size_t>(eq - arg);
                if (!A_strnicmp(arg, option, len) && option[len] == '\0')
                    return eq + 1;
            }
        }
        return nullptr;
    }

    /**
     */
    int CLIParser::getInt(const char* option, int defaultValue) const {
        const char* val = getOptionValue(option);
        return val ? static_cast<int>(strtol(val, nullptr, 10)) : defaultValue;
    }

    /**
     */
    float CLIParser::getFloat(const char* option, float defaultValue) const {
        const char* val = getOptionValue(option);
        return val ? static_cast<float>(strtod(val, nullptr)) : defaultValue;
    }
}
