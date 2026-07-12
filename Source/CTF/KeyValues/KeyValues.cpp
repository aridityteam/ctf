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
#include "KeyValues.h"

namespace CTF::KV {
    KeyValues::KeyValues(const String& name)
        : m_name(name), m_value() {}

    const String& KeyValues::GetString() const {
        if (m_value.type != KeyValuesType::String) throw std::runtime_error("Value is not a string");
        return m_value.stringValue;
    }

    int KeyValues::GetInt() const {
        if (m_value.type != KeyValuesType::Int) throw std::runtime_error("Value is not an int");
        return m_value.intValue;
    }

    float KeyValues::GetFloat() const {
        if (m_value.type != KeyValuesType::Float) throw std::runtime_error("Value is not a float");
        return m_value.floatValue;
    }

    bool KeyValues::GetBool() const {
        if (m_value.type != KeyValuesType::Bool) throw std::runtime_error("Value is not a bool");
        return m_value.boolValue;
    }

    void KeyValues::SetValue(const KeyValuesValue& val) {
        m_value = val;
    }
}