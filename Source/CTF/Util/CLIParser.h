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

#ifndef CTF_CLIPARSER_H
#define CTF_CLIPARSER_H
#pragma once

namespace CTF::Utilities {

    /******************************************************************************
    * A command-line utility for parsing arguments and parameters.
    ******************************************************************************/
    class CTF_API CLIParser {
    public:
        /******************************************************************************
        ******************************************************************************/
        CLIParser(int argc, char* argv[]);

        /******************************************************************************
        ******************************************************************************/
        ~CLIParser();

        /******************************************************************************
        ******************************************************************************/
        bool hasOption(const char* option) const;

        /******************************************************************************
        ******************************************************************************/
        bool hasShortOption(char option) const;

        /******************************************************************************
        ******************************************************************************/
        bool getFlag(const char* option) const;

        /******************************************************************************
        ******************************************************************************/
        const char *getPositional(int index) const;

        /******************************************************************************
        ******************************************************************************/
        const char *getOptionValue(const char* option) const;

        /******************************************************************************
        ******************************************************************************/
        int getInt(const char* option, int iDefault = 0) const;

        /******************************************************************************
        ******************************************************************************/
        float getFloat(const char* option, float iDefault = 0.0f) const;

    private:
        int argc_;
        char** argv_;
    };

}

#endif // CTF_CLIPARSER_H
