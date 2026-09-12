#include "pch.h"
#include <gtest/gtest.h>
#include "mandelbrot.h"

TEST(CheckValueTest, Zero)
{
    struct Complex c = { 0.0, 0.0 };
    struct Result r {};
    checkValue(c, &r, 100);
    
    EXPECT_TRUE(r.bounded);
    EXPECT_EQ(r.tillInfty, 0);
}

TEST(CheckValueTest, MinusOneIsBounded)
{
    struct Complex c = { -1.0, 0.0 };

    struct Result r {};
    checkValue(c, &r, 100);

    EXPECT_TRUE(r.bounded);
    EXPECT_EQ(r.tillInfty, 0);
}

TEST(CheckValueTest, MinusHalfIsBounded)
{
    struct Complex c = { -0.5, 0.0 };

    struct Result r {};
    checkValue(c, &r, 100);

    EXPECT_TRUE(r.bounded);
    EXPECT_EQ(r.tillInfty, 0);
}

TEST(CheckValueTest, QuarterIsBounded)
{
    // c = 0.25 is on the cusp of the main cardioid.
    struct Complex c = { 0.25, 0.0 };

    struct Result r {};
    checkValue(c, &r, 1000);

    EXPECT_TRUE(r.bounded);
    EXPECT_EQ(r.tillInfty, 0);
}

TEST(CheckValueTest, MinusTwoIsBounded)
{
    // 0 -> -2 -> 2 -> 2 -> ...
    //
    // |z| == 2 is NOT an escape.
    // Escape condition should be |z| > 2.
    struct Complex c = { -2.0, 0.0 };

    struct Result r {};
    checkValue(c, &r, 100);

    EXPECT_TRUE(r.bounded);
    EXPECT_EQ(r.tillInfty, 0);
}

// ------------------------------------------------------------
// Definitely escaping points
// ------------------------------------------------------------

TEST(CheckValueTest, ThreeEscapes)
{
    struct Complex c = { 3.0, 0.0 };

    struct Result r {};
    checkValue(c, &r, 100);

    EXPECT_FALSE(r.bounded);
}

TEST(CheckValueTest, NegativeThreeEscapes)
{
    struct Complex c = { -3.0, 0.0 };

    struct Result r {};
    checkValue(c, &r, 100);

    EXPECT_FALSE(r.bounded);
}

TEST(CheckValueTest, OneEscapes)
{
    struct Complex c = { 1.0, 0.0 };

    struct Result r {};
    checkValue(c, &r, 100);

    EXPECT_FALSE(r.bounded);
}

TEST(CheckValueTest, OnePlusOneIEscapes)
{
    struct Complex c = { 1.0, 1.0 };

    struct Result r {};
    checkValue(c, &r, 100);

    EXPECT_FALSE(r.bounded);
}

// ------------------------------------------------------------
// Exact escape iteration tests
// ------------------------------------------------------------

TEST(CheckValueTest, ThreeEscapesOnFirstIteration)
{
    // z0 = 0
    //
    // z1 = 3
    // |3| > 2
    struct Complex c = { 3.0, 0.0 };

    struct Result r {};
    checkValue(c, &r, 100);

    EXPECT_FALSE(r.bounded);
    EXPECT_EQ(r.tillInfty, 1);
}

TEST(CheckValueTest, NegativeThreeEscapesOnFirstIteration)
{
    // z1 = -3
    // |-3| > 2
    struct Complex c = { -3.0, 0.0 };

    struct Result r {};
    checkValue(c, &r, 100);

    EXPECT_FALSE(r.bounded);
    EXPECT_EQ(r.tillInfty, 1);
}

TEST(CheckValueTest, TwoEscapesOnSecondIteration)
{
    // z0 = 0
    // z1 = 2       |z| == 2, not escaped
    // z2 = 6       escaped
    struct Complex c = { 2.0, 0.0 };

    struct Result r {};
    checkValue(c, &r, 100);

    EXPECT_FALSE(r.bounded);
    EXPECT_EQ(r.tillInfty, 2);
}

TEST(CheckValueTest, OneEscapesOnThirdIteration)
{
    // z0 = 0
    // z1 = 1
    // z2 = 2
    // z3 = 5
    struct Complex c = { 1.0, 0.0 };
    struct Result r {};
    checkValue(c, &r, 100);

    EXPECT_FALSE(r.bounded);
    EXPECT_EQ(r.tillInfty, 3);
}

TEST(CheckValueTest, OnePlusOneIEscapesOnSecondIteration)
{
    // z0 = 0
    //
    // z1 = 1 + i
    // |z1|^2 = 2
    //
    // z2 = (1+i)^2 + (1+i)
    //    = 1 + 3i
    //
    // |z2|^2 = 10 > 4
    struct Complex c = { 1.0, 1.0 };

    struct Result r {};
    checkValue(c, &r, 100);

    EXPECT_FALSE(r.bounded);
    EXPECT_EQ(r.tillInfty, 2);
}


// ------------------------------------------------------------
// maxIterations behavior
// ------------------------------------------------------------

TEST(CheckValueTest, BoundedPointUsesEntireIterationLimit)
{
    const int maxIterations = 500;

    struct Complex c = { 0.0, 0.0 };

    struct Result r {};
    checkValue(c, &r, maxIterations);

    EXPECT_TRUE(r.bounded);
    EXPECT_EQ(r.tillInfty, 0);
}

TEST(CheckValueTest, EscapingPointStopsBeforeIterationLimit)
{
    const int maxIterations = 500;

    struct Complex c = { 1.0, 0.0 };

    struct Result r {};
    checkValue(c, &r, maxIterations);

    EXPECT_FALSE(r.bounded);
    EXPECT_LT(r.tillInfty, maxIterations);
}


// ------------------------------------------------------------
// Important edge case: reaching radius exactly 2
// ------------------------------------------------------------

TEST(CheckValueTest, ReachingExactlyTwoDoesNotImmediatelyEscape)
{
    // For c = 2:
    //
    // z1 = 2
    //
    // At this point |z|^2 == 4.
    // This MUST NOT count as escaped yet.
    //
    // z2 = 6, which does escape.

    struct Complex c = { 2.0, 0.0 };

    struct Result r {};
    checkValue(c, &r, 1);

    EXPECT_TRUE(r.bounded);
    EXPECT_EQ(r.tillInfty, 0);
}


// ------------------------------------------------------------
// Complex conjugate symmetry
//
// Mandelbrot is symmetric about the real axis.
// a + bi and a - bi should behave identically.
// ------------------------------------------------------------

TEST(CheckValueTest, ComplexConjugatesHaveSameResult)
{
    struct Complex upper = { 0.5, 0.5 };
    struct Complex lower = { 0.5, -0.5 };

    struct Result r {};
    checkValue(upper, &r, 100);
    struct Result resultUpper = r;

    checkValue(lower, &r, 100);
    struct Result resultLower = r;

    EXPECT_EQ(resultUpper.bounded, resultLower.bounded);
    EXPECT_EQ(resultUpper.tillInfty, resultLower.tillInfty);
}

TEST(CheckValueTest, EscapingConjugatesHaveSameResult)
{
    struct Complex upper = { 1.0, 1.0 };
    struct Complex lower = { 1.0, -1.0 };

    struct Result r {};
    checkValue(upper, &r, 100);
    struct Result resultUpper = r;

    checkValue(lower, &r, 100);
    struct Result resultLower = r;

    EXPECT_FALSE(resultUpper.bounded);
    EXPECT_FALSE(resultLower.bounded);

    EXPECT_EQ(resultUpper.tillInfty, resultLower.tillInfty);
}