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

#include <Threading/Task.h>
#include <Threading/CancellationToken.h>

using namespace CTF;
using namespace CTF::Threading;

TEST( TaskTests, ReturnsResult )
{
    Task<int> task(
        []( CancellationToken )
        {
            return 42;
        } );

    EXPECT_EQ( task.get(), 42 );
}

TEST( TaskTests, VoidTaskCompletes )
{
    bool executed = false;

    Task<void> task(
        [ & ]( CancellationToken )
        {
            executed = true;
        } );

    task.get();

    EXPECT_TRUE( executed );
}

TEST( CancellationTokenTests, InitialState )
{
    CancellationTokenSource cts;

    auto token = cts.GetToken();

    EXPECT_TRUE( token.CanBeCanceled() );
    EXPECT_FALSE( token.IsCancellationRequested() );
}

TEST( CancellationTokenTests, CancelSetsState )
{
    CancellationTokenSource cts;

    auto token = cts.GetToken();

    cts.Cancel();

    EXPECT_TRUE( token.IsCancellationRequested() );
}

TEST( CancellationTokenTests, MultipleTokensShareState )
{
    CancellationTokenSource cts;

    auto token1 = cts.GetToken();
    auto token2 = cts.GetToken();

    cts.Cancel();

    EXPECT_TRUE( token1.IsCancellationRequested() );
    EXPECT_TRUE( token2.IsCancellationRequested() );
}

TEST( CancellationTokenTests, ThrowIfCancellationRequested )
{
    CancellationTokenSource cts;

    auto token = cts.GetToken();

    cts.Cancel();

    EXPECT_THROW(
        token.ThrowIfCancellationRequested(),
        OperationCanceledException );
}

TEST( TaskTests, TaskCanObserveCancellation )
{
    CancellationTokenSource cts;

    Task<bool> task(
        []( CancellationToken token )
        {
            while ( !token.IsCancellationRequested() )
            {
                std::this_thread::yield();
            }

            return true;
        },
        cts.GetToken() );

    std::this_thread::sleep_for(
        std::chrono::milliseconds( 25 ) );

    cts.Cancel();

    EXPECT_TRUE( task.get() );
}

TEST( TaskTests, TaskThrowsWhenCanceled )
{
    CancellationTokenSource cts;

    Task<void> task(
        []( CancellationToken token )
        {
            while ( true )
            {
                token.ThrowIfCancellationRequested();

                std::this_thread::yield();
            }
        },
        cts.GetToken() );

    std::this_thread::sleep_for(
        std::chrono::milliseconds( 25 ) );

    cts.Cancel();

    EXPECT_THROW(
        task.get(),
        OperationCanceledException );
}

TEST( TaskTests, ExceptionsPropagate )
{
    Task<int> task(
        []( CancellationToken )
        {
            throw std::runtime_error( "boom" );
            return 0;
        } );

    EXPECT_THROW(
        task.get(),
        std::runtime_error );
}

TEST( TaskTests, CompletionState )
{
    Task<int> task(
        []( CancellationToken )
        {
            return 123;
        } );

    EXPECT_FALSE( task.IsCompleted() );

    EXPECT_EQ( task.get(), 123 );

    EXPECT_TRUE( task.IsCompleted() );
}