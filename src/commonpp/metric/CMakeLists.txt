#
# File: CMakeLists.txt
# Part of commonpp.
#
# Distributed under the 2-clause BSD licence (See LICENCE.TXT file at the
# project root).
#
# Copyright (c) 2015 Thomas Sanchez.  All rights reserved.
#
add_commonpp_library_source(
    SOURCES
        Metrics.cpp
        MetricTag.cpp
        MetricValue.cpp

        sink/GraphiteSink.cpp
        sink/ConsoleSink.cpp
        sink/InfluxDBSink.cpp

        type/Counter.cpp
        )
