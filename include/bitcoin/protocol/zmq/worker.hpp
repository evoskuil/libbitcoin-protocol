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
#ifndef LIBBITCOIN_PROTOCOL_ZMQ_WORKER_HPP
#define LIBBITCOIN_PROTOCOL_ZMQ_WORKER_HPP

#include <atomic>
#include <memory>
#include <future>
#include <shared_mutex>
#include <thread>
#include <bitcoin/system.hpp>
#include <bitcoin/protocol/boost.hpp>
#include <bitcoin/protocol/define.hpp>
#include <bitcoin/protocol/zmq/socket.hpp>

namespace libbitcoin {
namespace protocol {
namespace zmq {

/// This class is thread safe.
class BCP_API worker
{
public:
    DELETE_COPY_MOVE(worker);

    /// A shared worker pointer.
    typedef std::shared_ptr<worker> ptr;

    /// Construct a worker.
    worker(thread_priority priority=thread_priority::normal) NOEXCEPT;

    /// Stop the worker.
    virtual ~worker() NOEXCEPT;

    /// Start the worker.
    virtual bool start() NOEXCEPT;

    /// Stop the worker (optional).
    virtual bool stop() NOEXCEPT;

protected:
    bool stopped() NOEXCEPT;
    bool started(bool result) NOEXCEPT;
    bool finished(bool result) NOEXCEPT;
    bool forward(socket& from, socket& to) NOEXCEPT;
    void relay(socket& left, socket& right) NOEXCEPT;

    virtual void work() = 0;

private:
    // These are protected by mutex.
    std::atomic<bool> stopped_;
    std::promise<bool> started_;
    std::promise<bool> finished_;
    std::shared_ptr<std::thread> thread_;
    const thread_priority priority_;
    mutable std::shared_mutex mutex_;
};

} // namespace zmq
} // namespace protocol
} // namespace libbitcoin

#endif
