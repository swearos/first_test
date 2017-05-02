//
// Created by swearos on 17-5-1.
//
#include "utils.hpp"
#include <boost/filesystem.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace fs = ::boost::filesystem;

std::vector<std::string> &split(const std::string &s, char delim,
                                std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

str_map parse_json_config(const std::string file_path) {
    std::ifstream ifs(file_path.c_str());
    std::map<std::string, std::string> json_map;
    boost::property_tree::ptree pt;
    boost::property_tree::read_json(ifs, pt);

    boost::property_tree::basic_ptree<std::string, std::string>::const_iterator iter = pt.begin();

    for (; iter != pt.end(); ++iter) {
        json_map[iter->first.data()] = iter->second.data();
    }
    return json_map;
}

std::string extract_string(std::string target_key,
                           str_map& default_map) {
    std::string target_str;
    if (default_map.count(target_key) > 0) {
        target_str = default_map[target_key];
    } else {
        LOG(FATAL) << "Can not find target_key : " << target_key;
    }
    return target_str;
}

float extract_float(std::string target_key,
                    str_map& default_map) {
    std::string target_str = extract_string(target_key, default_map);
    return atof(target_str.c_str());
}

int extract_int(std::string target_key,
                str_map& default_map) {
    std::string target_str = extract_string(target_key, default_map);
    return atoi(target_str.c_str());
}

std::vector<float> extract_vector(std::string target_key,
                                  str_map& default_map) {
    std::string target_str = extract_string(target_key, default_map);
    std::vector<float> results;
    std::vector<std::string> elems = split(target_str, ',');

    for (std::vector<std::string>::const_iterator it = elems.begin();
         it != elems.end(); ++it) {
        results.push_back(atof((*it).c_str()));
    }
    return results;
}

std::string float_to_string(const std::vector<float> data) {
    char buff[200];
    std::string ans;
    for (size_t index = 0; index < data.size(); index++) {
        snprintf(buff, sizeof(buff), "%.2f", data[index]);
        if( index == 0 ) ans = std::string(buff);
        else ans += ", " + std::string(buff);
    }
    return ans;
}

std::string float_to_string(const float *data) {
    const int n = sizeof(data) / sizeof(data[0]);
    return float_to_string( std::vector<float>(data, data+n) );
}

std::vector<string> get_file_list(const std::string& path, const string& ext) {
    fs::path fs_path(path);
    vector<string> file_list;

    if(!fs::exists(fs_path) || !fs::is_directory(fs_path))
        return file_list;

    fs::recursive_directory_iterator it(fs_path);
    fs::recursive_directory_iterator endit;

    while (it != endit) {
        if (fs::is_regular_file(*it) && it->path().extension() == ext)
            file_list.push_back(it->path().filename().string());
        ++it;
    }

    return file_list;
}
