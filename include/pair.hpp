#pragma once
#ifndef PAIR_HPP
#define PAIR_HPP

#include "../include/colors.hpp"
#include "../include/download.hpp"
#include <bits/stdc++.h>

#define PAIR_FILE ".pairs.conf"

struct Pair {
    std::string name;
    std::string version;
    std::string description;
    std::string include;
    std::string source;
    std::string example;
    std::string library;
};

using collection = std::vector<Pair>;

collection      get_pairs       ();
void            display_pairs   (collection pairs);
void            download_pair   (std::string pair_name);

#endif