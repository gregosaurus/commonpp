/*
 * File: Metrics.hpp
 * Part of commonpp.
 *
 * Distributed under the 2-clause BSD licence (See LICENCE.TXT file at the
 * project root).
 *
 * Copyright (c) 2015 Thomas Sanchez.  All rights reserved.
 *
 */
#pragma once

#include <chrono>
#include <functional>
#include <memory>
#include <tuple>
#include <vector>
#include <string>
#include <mutex>

#include <boost/signals2.hpp>

#include <commonpp/thread/ThreadPool.hpp>

#include "detail/types.hpp"
#include "MetricTag.hpp"
#include "MetricValue.hpp"
#include "type/Counter.hpp"
#include "type/DescStat.hpp"
#include "type/Gauge.hpp"

namespace commonpp
{
namespace metric
{

// This class and mostly all the components are inspired from:
// http://www.vandenbogaerdt.nl/rrdtool/process.php
// However there is no time serie normalization occuring.
class Metrics
{
private:
    using MetricGenerator = std::function<MetricValue()>;
    using Generator = std::pair<MetricTag, MetricGenerator>;

public:
    using MetricVector =
        std::vector<std::tuple<std::reference_wrapper<const MetricTag>, MetricValue>>;
    using MetricsCallback = boost::signals2::signal<void(const MetricVector&)>;

    using DescStat = type::DescStat;
    using Gauge = type::Gauge<>;
    using Counter = type::Counter<>;
    using SharedCounter = type::SharedCounter;

public:
    Metrics(thread::ThreadPool& pool, std::chrono::seconds period);
    ~Metrics();

    template <typename StatsSummary, typename Reservoir>
    void add(MetricTag tag, const Reservoir& h);

    void add(MetricTag tag, Gauge fn);
    void add(MetricTag tag, Counter fn);
    void add(MetricTag tag, SharedCounter& fn);

    void stop();

    template <typename Callback>
    boost::signals2::connection addMetricsReceiver(Callback&& callback)
    {
        return callbacks_.connect(std::forward<Callback>(callback));
    }

    thread::ThreadPool& getPool()
    {
        return pool_;
    }

private:
    void schedule_timer();
    void calculate_metrics();

private:
    std::chrono::seconds period_;
    thread::ThreadPool& pool_;
    thread::ThreadPool::TimerPtr timer_handle_;
    std::vector<Generator> counters_;
    std::mutex counters_lock_;
    MetricsCallback callbacks_;
};

template <typename StatsSummary, typename Reservoir>
void Metrics::add(MetricTag tag, const Reservoir& r)
{
    std::lock_guard<std::mutex> lock(counters_lock_);
    counters_.emplace_back(std::move(tag), [&r]
                           {
                               return StatsSummary::getMetrics(r);
                           });
}

} // namespace metric
} // namespace commonpp
