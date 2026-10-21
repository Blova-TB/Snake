#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <boost/endian/conversion.hpp>
#include <type_traits>
#include <boost/pfr.hpp>

struct ByteBuffer
{
    const uint8_t *data;
    size_t size;
    size_t offset = 0;
    bool is_left_enough(size_t len) const { return offset + len <= size; }
};

// int
template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
void write_to(std::vector<uint8_t> &buf, T val)
{
    T le_val = boost::endian::native_to_little(val);
    const uint8_t *ptr = reinterpret_cast<const uint8_t *>(&le_val);
    buf.insert(buf.end(), ptr, ptr + sizeof(T));
}

template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
void read_from(ByteBuffer &buf, T &val)
{
    if (!buf.is_left_enough(sizeof(T)))
        throw std::runtime_error("Buffer underflow");
    std::memcpy(&val, buf.data + buf.offset, sizeof(T));
    buf.offset += sizeof(T);
    val = boost::endian::little_to_native(val);
}

// les str
void inline write_to(std::vector<uint8_t> &buf, const std::string &str)
{
    write_to<uint32_t>(buf, static_cast<uint32_t>(str.size()));
    buf.insert(buf.end(), str.begin(), str.end());
}

void inline read_from(ByteBuffer &buf, std::string &str)
{
    uint32_t len;
    read_from(buf, len);
    if (!buf.is_left_enough(len))
        throw std::runtime_error("Buffer underflow on string");
    str.assign(buf.data + buf.offset, buf.data + buf.offset + len);
    buf.offset += len;
}

// tab
template <typename T>
void write_to(std::vector<uint8_t> &buf, const std::vector<T> &vec)
{
    write_to<uint32_t>(buf, static_cast<uint32_t>(vec.size()));
    for (const T &item : vec)
    {
        write_to(buf, item);
    }
}

template <typename T>
void read_from(ByteBuffer &buf, std::vector<T> vec)
{
    uint32_t len;
    read_from(buf, len);
    vec.resize(len);

    for (T &t : vec)
    {
        read_from(buf, t);
    }
}

template <typename T>
std::enable_if_t<std::is_aggregate_v<T>> write_to(std::vector<uint8_t> &buf, const T &aggregate)
{
    boost::pfr::for_each_field(aggregate, [&](const auto &field)
                               { write_to(buf, field); });
}

template <typename T>
std::enable_if_t<std::is_aggregate_v<T>> read_from(ByteBuffer &buf, T &aggregate)
{
    boost::pfr::for_each_field(aggregate, [&](auto &field)
                               { read_from(buf, field); });
}