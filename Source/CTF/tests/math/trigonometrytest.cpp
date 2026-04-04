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

#include <Math/Trigonometry.h>

namespace {
    constexpr double EPS = 1e-8;
    constexpr double EPS_EXP = 1e-9;

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

TEST(TrigonometryTest, Exp_KnownValues) {
    ExpectNear(CTF::Math::Exp(0.0), 1.0, EPS_EXP);
    ExpectNear(CTF::Math::Exp(1.0), std::exp(1.0), EPS_EXP);
    ExpectNear(CTF::Math::Exp(-1.0), std::exp(-1.0), EPS_EXP);
}

TEST(TrigonometryTest, Exp_AgainstStd) {
    for (double x = -5.0; x <= 5.0; x += 0.1) {
        ExpectNear(CTF::Math::Exp(x), std::exp(x), 1e-8);
    }
}

TEST(TrigonometryTest, Exp_Additivity) {
    for (double x = -2.0; x <= 2.0; x += 0.5) {
        for (double y = -2.0; y <= 2.0; y += 0.5) {
            double lhs = CTF::Math::Exp(x + y);
            double rhs = CTF::Math::Exp(x) * CTF::Math::Exp(y);
            EXPECT_NEAR(lhs, rhs, 1e-7);
        }
    }
}

TEST(TrigonometryTest, Exp_Log_Inverse) {
    for (double x = 0.1; x <= 5.0; x += 0.1) {
        double val = CTF::Math::Exp(CTF::Math::Log(x));
        EXPECT_NEAR(val, x, 1e-7);
    }
}

TEST(TrigonometryTest, Exp_LargerValues) {
    for (double x = 5.0; x <= 10.0; x += 1.0) {
        EXPECT_NEAR(CTF::Math::Exp(x), std::exp(x), 1e-5);
    }
}

TEST(TrigonometryTest, Log_KnownValues) {
    ExpectNear(CTF::Math::Log(1.0), 0.0, EPS);
    ExpectNear(CTF::Math::Log(std::exp(1.0)), 1.0, EPS);
}

TEST(TrigonometryTest, Log_AgainstStd) {
    for (double x = 0.1; x <= 5.0; x += 0.1) {
        ExpectNear(CTF::Math::Log(x), std::log(x), 1e-7);
    }
}

TEST(TrigonometryTest, Log_ProductRule) {
    for (double a = 0.5; a <= 3.0; a += 0.5) {
        for (double b = 0.5; b <= 3.0; b += 0.5) {
            double lhs = CTF::Math::Log(a * b);
            double rhs = CTF::Math::Log(a) + CTF::Math::Log(b);
            EXPECT_NEAR(lhs, rhs, 1e-6);
        }
    }
}

TEST(TrigonometryTest, Log_BaseChange) {
    for (double x = 0.5; x <= 5.0; x += 0.5) {
        for (double b = 2.0; b <= 5.0; b += 1.0) {
            double result = CTF::Math::Log(x, b);
            double expected = std::log(x) / std::log(b);
            EXPECT_NEAR(result, expected, 1e-7);
        }
    }
}

TEST(TrigonometryTest, Log_InvalidInput) {
    EXPECT_LT(CTF::Math::Log(0.0), 0.0);
    EXPECT_LT(CTF::Math::Log(-1.0), 0.0);
}

TEST(TrigonometryTest, Log_WideRange) {
    for (double x = 0.1; x <= 10.0; x += 0.5) {
        EXPECT_NEAR(CTF::Math::Log(x), std::log(x), 1e-6);
    }
}
