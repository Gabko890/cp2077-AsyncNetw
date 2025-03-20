#include "cpAsyncNetw.h"
#include <iostream>
#include <sstream>

cpAsyncNetw::cpAsyncNetw()
    : socket_(io_context_), connected_(false)
{
}

cpAsyncNetw::~cpAsyncNetw() {
    Disconnect();
}

bool cpAsyncNetw::Connect(const Red::CString& ip, int port) {
    try {
        asio::ip::tcp::endpoint endpoint(asio::ip::address::from_string(ip.c_str()), port);
        socket_.connect(endpoint);
        connected_ = true;
        // Start asynchronous reading.
        DoRead();
        // Run the io_context in a separate thread.
        io_thread_ = std::thread([this]() { RunIO(); });
    }
    catch (std::exception &e) {
        std::cerr << "Connect exception: " << e.what() << "\n";
        return false;
    }
    return true;
}

void cpAsyncNetw::Disconnect() {
    if (connected_) {
        try {
            socket_.close();
        }
        catch (...) {
        }
        connected_ = false;
    }
    io_context_.stop();
    if (io_thread_.joinable())
        io_thread_.join();
}

bool cpAsyncNetw::SendString(const Red::CString& msg) {
    if (!connected_)
        return false;
    try {
        // Append newline as delimiter.
        std::string send_msg = std::string(msg.c_str()) + "\n";
        asio::write(socket_, asio::buffer(send_msg));
    }
    catch (std::exception &e) {
        std::cerr << "SendString exception: " << e.what() << "\n";
        return false;
    }
    return true;
}

std::string cpAsyncNetw::GetReceivedString() {
    std::lock_guard<std::mutex> lock(messages_mutex_);
    if (!messages_.empty()) {
        std::string msg = messages_.front();
        messages_.pop_front();
        return msg;
    }
    return "";
}

void cpAsyncNetw::DoRead() {
    // Use async_read_until to read data until a newline is encountered.
    asio::async_read_until(socket_, read_buffer_, "\n",
        [this](std::error_code ec, std::size_t bytes_transferred) {
            if (!ec) {
                std::istream is(&read_buffer_);
                std::string line;
                std::getline(is, line);
                {
                    std::lock_guard<std::mutex> lock(messages_mutex_);
                    if (messages_.size() >= MAX_MESSAGES) {
                        messages_.pop_front();
                    }
                    messages_.push_back(line);
                }
                // Continue reading for the next message.
                DoRead();
            }
            else {
                if (ec != asio::error::operation_aborted)
                    std::cerr << "DoRead error: " << ec.message() << "\n";
            }
        });
}

void cpAsyncNetw::RunIO() {
    try {
        io_context_.run();
    }
    catch (std::exception &e) {
        std::cerr << "RunIO exception: " << e.what() << "\n";
    }
}