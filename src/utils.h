//
// Created by Chris Luttio on 3/24/21.
//

#ifndef P8_WEB_SERVER_UTILS_H
#define P8_WEB_SERVER_UTILS_H

#include <vector>
#include <string>
#include <memory>
#include <string>
#include <stdexcept>


std::vector<std::string> split_string(const std::string& separator, const std::string& str);
std::string join(const std::string& separator, const std::vector<std::string>& arr);
int32_t decode_hex_str(const std::string&);
template<typename ... Args>
std::string string_format( const std::string& format, Args ... args )
{
    int size_s = snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    if( size_s <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
    auto size = static_cast<size_t>( size_s );
    auto buf = std::make_unique<char[]>( size );
    snprintf( buf.get(), size, format.c_str(), args ... );
    return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
}

std::string generate_hash_id(int);

#endif //P8_WEB_SERVER_UTILS_H
