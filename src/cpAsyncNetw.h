#pragma once

#include <string>
#include <deque>
#include <mutex>
#include <thread>

#include "asio.hpp"

#include <RED4ext/RED4ext.hpp>
#include <RedLib.hpp>

class cpAsyncNetw {
public:
    cpAsyncNetw();
    ~cpAsyncNetw();

    // Connect to the server at the given IP and port.
    bool Connect(const Red::CString& ip, int port);

    // Disconnect from the server.
    void Disconnect();

    // Send a string to the server.
    bool SendString(const Red::CString& msg);

    // Retrieve the first received message (if available) and remove it from the buffer.
    // Returns an empty string if no messages are available.
    std::string GetReceivedString();

private:
    // Start an asynchronous read operation.
    void DoRead();

    // Run the ASIO io_context.
    void RunIO();

    asio::io_context io_context_;
    asio::ip::tcp::socket socket_;
    asio::streambuf read_buffer_;
    std::thread io_thread_;

    std::mutex messages_mutex_;
    std::deque<std::string> messages_;
    static const size_t MAX_MESSAGES = 16;
    bool connected_;
};


RTTI_DEFINE_CLASS(cpAsyncNetw, {
    RTTI_ALIAS("WonTeu.cpAsyncNetw");
  
    RTTI_ABSTRACT();
  
    RTTI_METHOD(Connect);
    RTTI_METHOD(Disconnect);
    RTTI_METHOD(SendString);
    RTTI_METHOD(GetReceivedString);
});