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

#include "Hash.h"

namespace CTF::Collections {

template <typename T> int Hash<T>::operator()(const T &key) const {
  const unsigned char *data = reinterpret_cast<const unsigned char *>(&key);
  int Hash = 0;
  for (int i = 0; i < sizeof(T); ++i)
    Hash = Hash * 131 + data[i];
  return Hash;
}

int Hash<String>::operator()(const String &s) const {
  int Hash = 5381;
  const char *str = s.CStr();
  int len = s.Length();
  for (int i = 0; i < len; ++i)
    Hash = ((Hash << 5) + Hash) + static_cast<unsigned char>(str[i]);
  return Hash;
}
} // namespace CTF::Collections