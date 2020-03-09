//
// Created by kurlyana on 12.02.20.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <boost/algorithm/string.hpp>
#include <chrono>
#include <atomic>

void write_to_file(std::string file_out, int );
int count(std::vector<std::string> v_of_words);

inline std::chrono::high_resolution_clock::time_point get_current_time_fenced()
{
    std::atomic_thread_fence(std::memory_order_seq_cst);
    auto res_time = std::chrono::high_resolution_clock::now();
    std::atomic_thread_fence(std::memory_order_seq_cst);
    return res_time;
}

template<class D>
inline long long to_us(const D& d)
{
    return std::chrono::duration_cast<std::chrono::microseconds>(d).count();
}

int method_1(std::string file_in, std::string file_out) {
    auto start_time = get_current_time_fenced();
    std::vector<std::string> v;

    std::ifstream  input(file_in);
    if(!input) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }

    std::string first_word;
    input >> first_word;
    int count = 1;

    std::string word;
    while( input >> word ) {
        if (word == first_word)
        v.push_back(word);
    }

    count += v.size();

    auto finish_time = get_current_time_fenced();
    auto total_time = finish_time - start_time;

    write_to_file(file_out, count);
    return to_us(total_time);
}

int method_2(std::string file_in, std::string file_out) {
    auto start_time = get_current_time_fenced();
    std::ifstream input(file_in);
    if(!input) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }

    std::string str_of_words((std::istreambuf_iterator<char>(input)),
                    std::istreambuf_iterator<char>());

    std::vector<std::string> v_of_words;
    boost::split(v_of_words, str_of_words, boost::is_any_of(" "));

    auto finish_time = get_current_time_fenced();
    auto total_time = finish_time - start_time;

    write_to_file(file_out, count(v_of_words));
    return to_us(total_time);
}

std::vector<std::string> split_method_3(const std::string& str, const std::string& delims = " ", bool skip_empty = true) {
    std::vector<std::string> output;
    auto first = std::cbegin(str);

    while (first != std::cend(str)) {
        const auto second = std::find_first_of(first, std::cend(str),
                                               std::cbegin(delims), std::cend(delims));
        if (first != second || !skip_empty) {
            output.emplace_back(first, second);
        }
        if (second == std::cend(str)) break;
        first =  std::next(second);
    }
    return output;
}

int method_3(std::string file_in, std::string file_out) {
    auto start_time = get_current_time_fenced();
    std::ifstream input(file_in);
    if(!input) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }

    std::string str_of_words((std::istreambuf_iterator<char>(input)),
                             std::istreambuf_iterator<char>());

    std::vector<std::string> v_of_words = split_method_3(str_of_words);

    auto finish_time = get_current_time_fenced();
    auto total_time = finish_time - start_time;

    write_to_file(file_out, count(v_of_words));
    return to_us(total_time);
}

int main(int argc, char** argv)
{
    if (argc < 4) {
        std::cout << "Failed to execute experimentation" << std::endl;
        return 1;
    }

    if (argv[1] == std::string("1")) {
        std::cout << "Counting first word using method 1 (microseconds):\n" << method_1(argv[2], argv[3]) << std::endl;
        return 0;
    }
    if (argv[1] == std::string("2")) {
        std::cout << "Counting first word using method 2 (microseconds):\n" << method_2(argv[2], argv[3]) << std::endl;
        return 0;
    }
    if (argv[1] == std::string("3")) {
        std::cout << "Counting first word using method 3 (microseconds):\n" << method_3(argv[2], argv[3]) << std::endl;
        return 0;
    }
    return 0;
}

void write_to_file(std::string file_out, int count) {
    std::ofstream  output;
    output.open(file_out, std::ios::app);
    output << count;
    output.close();
}

int count(std::vector<std::string> v_of_words) {
    std::string first_word = v_of_words[0];
    int count = 1;
    for (int i = 1; i < v_of_words.size(); i++)
        if (v_of_words[i] == first_word) {
            count += 1;
        }
    return count;
}
