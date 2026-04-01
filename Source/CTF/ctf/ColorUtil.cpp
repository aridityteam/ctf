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
#include "ColorUtil.h"

namespace CTF::ConColor {
    CTF::OStream& operator<<(CTF::OStream& os, Code code) {
    #ifdef _WIN32
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        WORD color = 0;

        switch (code) {
            case red:         color = FOREGROUND_RED | FOREGROUND_INTENSITY; break;
            case green:       color = FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
            case yellow:      color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
            case blue:        color = FOREGROUND_BLUE | FOREGROUND_INTENSITY; break;
            case magenta:     color = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY; break;
            case cyan:        color = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY; break;
            case white:       color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; break;
            case bright_white:color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY; break;
            case reset:       color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; break;
            default:          color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; break;
        }

        SetConsoleTextAttribute(hConsole, color);
        return os;
    #else
        switch (code) {
            case red:         return os << "\033[31m";
            case green:       return os << "\033[32m";
            case yellow:      return os << "\033[33m";
            case blue:        return os << "\033[34m";
            case magenta:     return os << "\033[35m";
            case cyan:        return os << "\033[36m";
            case white:       return os << "\033[37m";
            case bright_red:  return os << "\033[91m";
            case bright_green:return os << "\033[92m";
            case bright_yellow:return os << "\033[93m";
            case bright_blue: return os << "\033[94m";
            case bright_magenta:return os << "\033[95m";
            case bright_cyan: return os << "\033[96m";
            case bright_white:return os << "\033[97m";
            case reset:       return os << "\033[0m";
            default:          return os;
        }
    #endif
    }
}