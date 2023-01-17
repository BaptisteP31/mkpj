#pragma once

#ifndef DOWNLOAD_HPP
#define DOWNLOAD_HPP

#include <stdio.h>
#include <curl/curl.h>
#include <filesystem>
#include <string>

void download_from_internet(std::string url, std::filesystem::path path);

#endif