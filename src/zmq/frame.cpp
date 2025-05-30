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
#include <bitcoin/protocol/zmq/frame.hpp>

#include <cstring>
#include <iterator>
#include <bitcoin/system.hpp>
#include <bitcoin/protocol/define.hpp>
#include <bitcoin/protocol/zmq/error.hpp>
#include <bitcoin/protocol/zmq/socket.hpp>
#include <bitcoin/protocol/zmq/zeromq.hpp>

namespace libbitcoin {
namespace protocol {
namespace zmq {

using namespace bc::system;

// Use for receiving.
frame::frame() NOEXCEPT
  : more_(false), valid_(initialize({}))
{
}

// Use for sending.
frame::frame(const system::data_chunk& data) NOEXCEPT
  : more_(false), valid_(initialize(data))
{
}

frame::~frame() NOEXCEPT
{
    destroy();
}

// private
bool frame::initialize(const data_chunk& data) NOEXCEPT
{
    const auto& buffer = pointer_cast<zmq_msg_t>(&message_);

    if (data.empty())
        return (zmq_msg_init(buffer) != zmq_fail);

    if (zmq_msg_init_size(buffer, data.size()) == zmq_fail)
        return false;

    std::memcpy(zmq_msg_data(buffer), data.data(), data.size());
    return true;
}

// private
bool frame::destroy() NOEXCEPT
{
    const auto& buffer = pointer_cast<zmq_msg_t>(&message_);
    return valid_ && (zmq_msg_close(buffer) != zmq_fail);
}

frame::operator bool() const NOEXCEPT
{
    return valid_;
}

bool frame::more() const NOEXCEPT
{
    return more_;
}

// private
bool frame::set_more(socket& socket) NOEXCEPT
{
    int more;
    auto length = sizeof(int);

    if (zmq_getsockopt(socket.self(), ZMQ_RCVMORE, &more, &length) == zmq_fail)
        return false;

    more_ = !is_zero(more);
    return true;
}

data_chunk frame::payload() const NOEXCEPT
{
    const auto& buffer = pointer_cast<zmq_msg_t>(&message_);
    const auto size = zmq_msg_size(buffer);
    const auto data = zmq_msg_data(buffer);
    const auto begin = pointer_cast<uint8_t>(data);
    return { begin, std::next(begin, size) };
}

// Must be called on the socket thread.
error::code frame::receive(socket& socket) NOEXCEPT
{
    if (!valid_)
        return error::invalid_message;

    const auto& buffer = pointer_cast<zmq_msg_t>(&message_);
    const auto result = zmq_msg_recv(buffer, socket.self(), wait_flag)
        != zmq_fail && set_more(socket);
    return result ? error::success : error::get_last_error();
}

// Must be called on the socket thread.
error::code frame::send(socket& socket, bool last) NOEXCEPT
{
    if (!valid_)
        return error::invalid_message;

    const int flags = (last ? 0 : ZMQ_SNDMORE) | wait_flag;
    const auto& buffer = pointer_cast<zmq_msg_t>(&message_);
    const auto result = zmq_msg_send(buffer, socket.self(), flags) != zmq_fail;
    return result ? error::success : error::get_last_error();
}

} // namespace zmq
} // namespace protocol
} // namespace libbitcoin
