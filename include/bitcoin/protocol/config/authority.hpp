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

#ifndef LIBBITCOIN_PROTOCOL_CONFIG_AUTHORITY_HPP
#define LIBBITCOIN_PROTOCOL_CONFIG_AUTHORITY_HPP

#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <bitcoin/protocol/boost.hpp>
#include <bitcoin/protocol/define.hpp>

namespace libbitcoin {
namespace protocol {

/// This is a container for a {ip address, port} tuple.
class BCP_API authority
{
public:
    typedef std::shared_ptr<authority> ptr;

    authority() noexcept;

    /// Deserialize a IPv4 or IPv6 address-based hostname[:port].
    /// The port is optional and will be set to zero if not provided.
    /// The host can be in one of two forms:
    /// [2001:db8::2]:port or 1.2.240.1:port.
    authority(const std::string& authority) noexcept (false);

    /// The host can be in one of three forms:
    /// [2001:db8::2] or 2001:db8::2 or 1.2.240.1
    authority(const std::string& host, uint16_t port) noexcept (false);

    /// True if the port is non-zero.
    operator bool() const noexcept;

    /// The ip address of the authority.
    const ipv6& ip() const noexcept;

    /// The tcp port of the authority.
    uint16_t port() const noexcept;

    /// The hostname of the authority as a string.
    /// The form of the return is determined by the type of address, either:
    /// 2001:db8::2 or 1.2.240.1
    std::string to_hostname() const noexcept;

    /// The authority as a string.
    /// The form of the return is determined by the type of address.
    /// The port is optional and not included if zero-valued.
    /// The authority in one of two forms: [2001:db8::2]:port or 1.2.240.1:port
    std::string to_string() const noexcept;

    friend std::istream& operator>>(std::istream& input,
        authority& argument) noexcept(false);
    friend std::ostream& operator<<(std::ostream& output,
        const authority& argument) noexcept;

private:
    // These are not thread safe.

    ipv6 ip_;
    uint16_t port_;
};

typedef std::vector<authority> authorities;

} // namespace protocol
} // namespace libbitcoin

namespace std
{
template<>
struct hash<bc::protocol::authority>
{
    size_t operator()(const bc::protocol::authority& value) const noexcept
    {
        return std::hash<std::string>{}(value.to_string());
    }
};
} // namespace std

#endif
