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
#ifndef LIBBITCOIN_PROTOCOL_CONFIG_ENDPOINT_HPP
#define LIBBITCOIN_PROTOCOL_CONFIG_ENDPOINT_HPP

#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <bitcoin/system.hpp>
#include <bitcoin/protocol/config/authority.hpp>
#include <bitcoin/protocol/define.hpp>

namespace libbitcoin {
namespace protocol {

/// Serialization helper for a network endpoint in URI format.
/// This is a container for a {scheme, host, port} tuple.
class BCP_API endpoint
{
public:
    typedef std::shared_ptr<endpoint> ptr;

    endpoint() noexcept;
    endpoint(const endpoint& other) noexcept;

    /// The scheme and port may be undefined, in which case the port is
    /// reported as zero and the scheme is reported as an empty string.
    /// The value is of the form: [scheme://]host[:port]
    endpoint(const std::string& uri) noexcept(false);
    endpoint(const authority& authority) noexcept;

    /// host may be host name or ip address.
    endpoint(const std::string& host, uint16_t port) noexcept;
    endpoint(const std::string& scheme, const std::string& host,
        uint16_t port) noexcept;

    /// True if the endpoint is initialized.
    operator bool() const noexcept;

    /// The scheme of the endpoint or empty string.
    const std::string& scheme() const noexcept;

    /// The host name or ip address of the endpoint.
    const std::string& host() const noexcept;

    /// The tcp port of the endpoint.
    uint16_t port() const noexcept;

    /// An empty scheme and/or empty port is omitted.
    /// The endpoint is of the form: [scheme://]host[:port]
    std::string to_string() const noexcept;

    /// Return a new endpoint that replaces host instances of "*" with
    /// "localhost". This is intended for clients that wish to connect
    /// to a service that has been configured to bind to all interfaces.
    /// The endpoint is of the form: [scheme://]host[:port]
    endpoint to_local() const noexcept;

    friend std::istream& operator>>(std::istream& input,
        endpoint& argument) noexcept(false);
    friend std::ostream& operator<<(std::ostream& output,
        const endpoint& argument) noexcept;

private:
    // These are not thread safe.

    std::string scheme_;
    std::string host_;
    uint16_t port_;
};

typedef std::vector<endpoint> endpoints;

} // namespace protocol
} // namespace libbitcoin

namespace std
{
template<>
struct hash<bc::protocol::endpoint>
{
    size_t operator()(const bc::protocol::endpoint& value) const noexcept
    {
        return std::hash<std::string>{}(value.to_string());
    }
};
} // namespace std

#endif
