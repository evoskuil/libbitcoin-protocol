/**
 * Copyright (c) 2011-2021 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <bitcoin/protocol/config/endpoint.hpp>

#include <cstdint>
#include <string>
#include <boost/regex.hpp>
#include <bitcoin/system.hpp>
#include <bitcoin/protocol/config/authority.hpp>

namespace libbitcoin {
namespace protocol {

using namespace bc::system;

endpoint::endpoint() noexcept
  : endpoint("localhost")
{
}

endpoint::endpoint(const endpoint& other) noexcept
  : scheme_(other.scheme()), host_(other.host()), port_(other.port())
{
}

endpoint::endpoint(const std::string& uri) noexcept(false)
{
    std::stringstream(uri) >> *this;
}

endpoint::endpoint(const authority& authority) noexcept
  : endpoint(authority.to_string())
{
}

endpoint::endpoint(const std::string& host, uint16_t port) noexcept
  : host_(host), port_(port)
{
}

endpoint::endpoint(const std::string& scheme, const std::string& host,
    uint16_t port) noexcept
  : scheme_(scheme), host_(host), port_(port)
{
}

const std::string& endpoint::scheme() const noexcept
{
    return scheme_;
}

const std::string& endpoint::host() const noexcept
{
    return host_;
}

uint16_t endpoint::port() const noexcept
{
    return port_;
}

std::string endpoint::to_string() const noexcept
{
    std::stringstream value;
    value << *this;
    return value.str();
}

endpoint endpoint::to_local() const noexcept
{
    const auto host = host_ == "*" ? "localhost" : host_;
    return endpoint(scheme_, host, port_);
}

endpoint::operator bool() const noexcept
{
    return !scheme_.empty();
}

std::istream& operator>>(std::istream& input,
    endpoint& argument) noexcept(false)
{
    std::string value;
    input >> value;

    // C++11: use std::regex.
    // std::regex requires gcc 4.9, so we are using boost::regex for now.
    static const boost::regex regular("^((tcp|udp|http|https|inproc):\\/\\/)?"
        "(\\[([0-9a-f:\\.]+)]|([^:]+))(:([0-9]{1,5}))?$");

    boost::sregex_iterator it(value.begin(), value.end(), regular), end;
    if (it == end)
        throw istream_exception(value);

    const auto& match = *it;
    argument.scheme_ = match[2];
    argument.host_ = match[3];
    std::string port(match[7]);

    try
    {
        argument.port_ = port.empty() ? 0u : boost::lexical_cast<uint16_t>(port);
    }
    catch (const std::exception&)
    {
        throw istream_exception(value);
    }

    return input;
}

std::ostream& operator<<(std::ostream& output,
    const endpoint& argument) noexcept
{
    if (!argument.scheme().empty())
        output << argument.scheme() << "://";

    output << argument.host();

    if (argument.port() != 0u)
        output << ":" << argument.port();

    return output;
}

} // namespace network
} // namespace libbitcoin
