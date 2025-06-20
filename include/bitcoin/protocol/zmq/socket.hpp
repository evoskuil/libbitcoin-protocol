/**
 * Copyright (c) 2011-2025 libbitcoin developers (see AUTHORS)
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
#ifndef LIBBITCOIN_PROTOCOL_ZMQ_SOCKET_HPP
#define LIBBITCOIN_PROTOCOL_ZMQ_SOCKET_HPP

#include <memory>
#include <bitcoin/protocol/config/sodium.hpp>
#include <bitcoin/protocol/define.hpp>
#include <bitcoin/protocol/settings.hpp>
#include <bitcoin/protocol/zmq/certificate.hpp>
#include <bitcoin/protocol/zmq/context.hpp>
#include <bitcoin/protocol/zmq/error.hpp>
#include <bitcoin/protocol/zmq/identifiers.hpp>

namespace libbitcoin {
namespace protocol {
namespace zmq {

class message;
class authenticator;

/// This class is not thread safe.
/// All calls must be made on the socket thread.
/// Because the socket is only set on construct, sockets are not restartable.
class BCP_API socket
  : public enable_shared_from_base<socket>
{
public:
    DELETE_COPY_MOVE(socket);

    /// The full set of socket roles defined by zeromq.
    enum class role
    {
        pair,
        publisher,
        subscriber,
        requester,
        replier,
        dealer,
        router,
        puller,
        pusher,
        extended_publisher,
        extended_subscriber,
        streamer
    };

    /// A shared socket pointer.
    typedef std::shared_ptr<socket> ptr;

    /// Construct a socket from an existing zeromq socket.
    socket(void* zmq_socket) NOEXCEPT;

    /// Construct a socket of the given context and role and default settings.
    /// Subscribers are automatically subscribed to all messages.
    socket(context& context, role socket_role) NOEXCEPT;

    /// Construct a socket of the given context and role.
    /// Subscribers are automatically subscribed to all messages.
    socket(context& context, role socket_role, const settings& settings) NOEXCEPT;

    /// Close the socket.
    virtual ~socket() NOEXCEPT;

    /// Close the socket (optional, must close or destroy before context stop).
    bool stop() NOEXCEPT;

    /// True if the socket is valid.
    operator bool() const NOEXCEPT;

    /// The underlying zeromq socket.
    void* self() NOEXCEPT;

    /// An opaque locally unique idenfier, valid after stop.
    identifier id() const NOEXCEPT;

    /// Bind the socket to the specified local address.
    error::code bind(const system::config::endpoint& address) NOEXCEPT;

    /// Connect the socket to the specified remote address.
    error::code connect(const system::config::endpoint& address) NOEXCEPT;

    /// Sets the domain for ZAP (ZMQ RFC 27) authentication.
    bool set_authentication_domain(const std::string& domain) NOEXCEPT;

    /// Configure the socket as a curve server (also set the secrety key).
    bool set_curve_server() NOEXCEPT;

    /// Configure the socket as client to the curve server.
    bool set_curve_client(const sodium& server_public_key) NOEXCEPT;

    /// Apply the specified public key to the socket.
    bool set_public_key(const sodium& key) NOEXCEPT;

    /// Apply the specified private key to the socket.
    bool set_private_key(const sodium& key) NOEXCEPT;

    /// Apply the keys of the specified certificate to the socket.
    bool set_certificate(const certificate& certificate) NOEXCEPT;

    /// Configure the socket to connect through the specified socks5 proxy.
    bool set_socks_proxy(const system::config::authority& socks_proxy) NOEXCEPT;

    /// Configure subscriber socket to apply the message filter.
    bool set_subscription(const system::data_chunk& filter) NOEXCEPT;

    /// Configure subscriber socket to remove the message filter.
    bool set_unsubscription(const system::data_chunk& filter) NOEXCEPT;

    /// Send a message on this socket.
    error::code send(message& packet) NOEXCEPT;

    /// Receive a message from this socket.
    error::code receive(message& packet) NOEXCEPT;

protected:
    static int to_socket_type(role socket_role) NOEXCEPT;

    bool set32(int32_t option, int32_t value) NOEXCEPT;
    bool set64(int32_t option, int64_t value) NOEXCEPT;
    bool set(int32_t option, const std::string& value) NOEXCEPT;
    bool set(int32_t option, const system::data_chunk& value) NOEXCEPT;

private:
    void* self_;
    const identifier identifier_;
};

} // namespace zmq
} // namespace protocol
} // namespace libbitcoin

#endif
