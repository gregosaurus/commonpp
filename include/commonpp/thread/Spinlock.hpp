/*
 * File: Spinlock.hpp
 * Part of commonpp.
 *
 * Distributed under the 2-clause BSD licence (See LICENCE.TXT file at the
 * project root).
 *
 * Copyright (c) 2015 Thomas Sanchez.  All rights reserved.
 *
 */
#pragma once

#include <atomic>

namespace commonpp
{
namespace thread
{

// from boost usage of boost::atomic
class Spinlock
{
private:
    enum LockState
    {
        Locked,
        Unlocked,
    };

    mutable std::atomic<LockState> state_;

public:
    Spinlock()
    : state_(Unlocked)
    {
    }

    void lock() const
    {
        while (state_.exchange(Locked, std::memory_order_acquire) == Locked)
        {
            // busy wait
        }
    }

    void unlock() const
    {
        state_.store(Unlocked, std::memory_order_release);
    }
};
} // namespace thread
} // namespace commonpp
