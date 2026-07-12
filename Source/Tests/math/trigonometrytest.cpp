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

#include <gtest/gtest.h>
#include <cmath>
#include <numbers>

#include <Math/Logarithms.h>
#include <Math/Trigonometry.h>

namespace {
    constexpr double EPS = 1e-8;

    template <typename T>
    void ExpectNear(T a, T b, T eps = EPS) {
        EXPECT_NEAR(a, b, eps);
    }
}

TEST(TrigonometryTest, Sin_KnownValues) {
    using std::numbers::pi;

    ExpectNear(CTF::Math::Sin(0.0), 0.0);
    ExpectNear(CTF::Math::Sin(pi / 2), 1.0);
    ExpectNear(CTF::Math::Sin(pi), 0.0);
    ExpectNear(CTF::Math::Sin(3 * pi / 2), -1.0);
}

TEST(TrigonometryTest, Sin_AgainstStd) {
    for (double x = -10.0; x <= 10.0; x += 0.1) {
        ExpectNear(CTF::Math::Sin(x), std::sin(x));
    }
}

TEST(TrigonometryTest, Sin_Periodicity) {
    using std::numbers::pi;

    for (double x = -5.0; x <= 5.0; x += 0.5) {
        ExpectNear(CTF::Math::Sin(x), CTF::Math::Sin(x + 2 * pi));
    }
}

TEST(TrigonometryTest, Sin_LargeValues) {
    for (double x = -100.0; x <= 100.0; x += 5.0) {
        EXPECT_NEAR(CTF::Math::Sin(x), std::sin(x), 1e-6);
    }
}

TEST(TrigonometryTest, Cos_LargeValues) {
    for (double x = -100.0; x <= 100.0; x += 5.0) {
        EXPECT_NEAR(CTF::Math::Cos(x), std::cos(x), 1e-6);
    }
}

TEST(TrigonometryTest, Cos_KnownValues) {
    using std::numbers::pi;

    ExpectNear(CTF::Math::Cos(0.0), 1.0);
    ExpectNear(CTF::Math::Cos(pi / 2), 0.0);
    ExpectNear(CTF::Math::Cos(pi), -1.0);
    ExpectNear(CTF::Math::Cos(2 * pi), 1.0);
}

TEST(TrigonometryTest, Cos_AgainstStd) {
    for (double x = -10.0; x <= 10.0; x += 0.1) {
        ExpectNear(CTF::Math::Cos(x), std::cos(x));
    }
}

TEST(TrigonometryTest, Cos_Periodicity) {
    using std::numbers::pi;

    for (double x = -5.0; x <= 5.0; x += 0.5) {
        ExpectNear(CTF::Math::Cos(x), CTF::Math::Cos(x + 2 * pi));
    }
}

TEST(TrigonometryTest, Tan_KnownValues) {
    using std::numbers::pi;

    ExpectNear(CTF::Math::Tan(0.0), 0.0);
    ExpectNear(CTF::Math::Tan(pi / 4), 1.0);
    ExpectNear(CTF::Math::Tan(-pi / 4), -1.0);
}

TEST(TrigonometryTest, Tan_AgainstStd) {
    using std::numbers::pi;

    for (double x = -1.4; x <= 1.4; x += 0.05) { // avoid ±pi/2 ≈ 1.57
        ExpectNear(CTF::Math::Tan(x), std::tan(x), 1e-6);
    }
}

TEST(TrigonometryTest, Tan_OddFunction) {
    for (double x = -1.0; x <= 1.0; x += 0.1) {
        ExpectNear(CTF::Math::Tan(-x), -CTF::Math::Tan(x));
    }
}
