// Copyright 2018 Your Name <your_email>

#ifndef INCLUDE_HEADER_HPP_
#define INCLUDE_HEADER_HPP_
#include <iostream>
#include <thread>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <picosha2.h>
#include <cstring>
#include <string>
#include <stdlib.h>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/sources/severity_logger.hpp>

std::atomic_uint64_t a = 0;
using std::cout;
using std::endl;

void init() {
    boost::log::register_simple_formatter_factory
            <boost::log::trivial::severity_level, char>("Severity");
    boost::log::add_file_log
            (
                    boost::log::keywords::file_name = "/tmp/my_file%N.log",
                    boost::log::keywords::rotation_size = 10 * 1024 * 1024,
                    boost::log::keywords::time_based_rotation =
                    boost::log::sinks::file::rotation_at_time_point(0, 0, 0),
                    boost::log::keywords::format =
                    "[%TimeStamp%] [%ThreadID%] [%Severity%] %Message%");
    boost::log::add_console_log
            (
                    std::cout,
                    boost::log::keywords::format =
                    "[%TimeStamp%] [%ThreadID%] [%Severity%] %Message%");
    boost::log::add_common_attributes();
}

std::string rand_s(int length)
{
    std::string result;
    for (int i = 0; i < length; i++) {
        unsigned r_time = time(nullptr);
        result.push_back(rand_r(&r_time) % 255);
    }
    return result;
}

std::string sha256(std::string data_sha) {
    std::string hash = picosha2::hash256_hex_string(data_sha);
    return hash;
}

void stream() {
    while (true) {
        unsigned r_time = time(nullptr);
        std::string random_data = rand_s(rand_r(&r_time) % 9);
        std::string hash_result = sha256(random_data);

        if (hash_result.find("0000") == hash_result.size()-4) {
            BOOST_LOG_TRIVIAL(info) << hash_result << " id: "
            << std::this_thread::get_id();
            break;
        } else {
            BOOST_LOG_TRIVIAL(trace) << hash_result << " id: "
            << std::this_thread::get_id();
        }
    }
}
#endif // INCLUDE_HEADER_HPP_
