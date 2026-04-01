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

#ifndef CTF_KV_H
#define CTF_KV_H
#pragma once

#include "KeyValuesType.h"
#include "KeyValuesValue.h"
#include "String.h"
#include "Collections/Map.h"
#include <memory>

namespace CTF::KV {
#if defined(__clang__)
#	pragma clang diagnostic push
#	pragma clang diagnostic ignored "-Wattributes"
#elif defined(__GNUC__)
#	pragma GCC diagnostic push
#	pragma GCC diagnostic ignored "-Wattributes"
#else
#	pragma warning(push)
#	pragma warning(disable:4251)
#endif
    class CTF_API KeyValues
    {
    public:
        KeyValues(const String& name = "");

        const String& GetName() const { return m_name; }
        KeyValuesType GetValueType() const { return m_value.type; }

        const String& GetString() const;
        int GetInt() const;
        float GetFloat() const;
        bool GetBool() const;

        void SetValue(const KeyValuesValue& val);

        KeyValues* GetChild(const String& key) const;
        KeyValues* CreateChild(const String& key);
        const Collections::Map<String, std::unique_ptr<KeyValues>>& GetChildren() const { return m_children; }

    private:
        String m_name;
        KeyValuesValue m_value;
        Collections::Map<String, std::unique_ptr<KeyValues>> m_children;

        friend class KeyValuesParser;
    };
#if defined(__clang__)
#	pragma clang diagnostic pop
#elif defined(__GNUC__)
#	pragma GCC diagnostic pop
#else
#	pragma warning(pop)
#endif
}

#endif // !CTF_KV_H