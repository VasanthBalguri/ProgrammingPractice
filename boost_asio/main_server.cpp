#include <ctime>
#include <iostream>
#include <string>
#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

std::string make_daytime_string()
{
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
}

class tcp_connection
  : public boost::enable_shared_from_this<tcp_connection>
{
public:
  typedef boost::shared_ptr<tcp_connection> pointer;

  static pointer create(boost::asio::io_context& io_context)
  {
    return pointer(new tcp_connection(io_context));
  }

  tcp::socket& socket()
  {
    return socket_;
  }

  ~tcp_connection()
  {
       std::cout << "closed connection" << std::endl;
      socket_.close();
  }
  void read_message()
  {
      boost::asio::async_read(socket_,boost::asio::buffer(ret_),
                                  boost::bind(&tcp_connection::handle_read,shared_from_this(),
                                              boost::asio::placeholders::error,
                                              boost::asio::placeholders::bytes_transferred));
  }

  void wait_for_request() {
      // since we capture `this` in the callback, we need to call shared_from_this()
      auto self(shared_from_this());
      // and now call the lambda once data arrives
      // we read a string until the null termination character
      boost::asio::async_read_until(socket_,m_buffer,'\0',
      [this, self](boost::system::error_code ec, std::size_t /*length*/)
      {
          // if there was no error, everything went well and for this demo
          // we print the data to stdout and wait for the next request
          if (!ec)  {
              // we just print the data, you can here call other api's
              // or whatever the server needs to do with the received data
              std::string data{
                  std::istreambuf_iterator<char>(&m_buffer),
                  std::istreambuf_iterator<char>()
              };
              std::cout << data << std::endl;
              wait_for_request();
          } else {
              std::cout << "error: " << ec << std::endl;
          }
      });
  }

  void write_message()
  {
      std::string message = make_daytime_string();
      boost::asio::async_write(socket_, boost::asio::buffer(message),
       [this, self](boost::system::error_code ec, std::size_t /*length*/)
       {
          if(!ec)
          {
              std::cout << "written data to client" << std::endl;
              wait_for_request();
          }
          else
          {
              std::cout << "error: " << ec << std::endl;
          }
      });
  }
  void start()
  {
    //message_ = make_daytime_string();

    //boost::system::error_code error;

    //boost::asio::write(socket_,boost::asio::buffer("connection established\n"),error);
    //boost::asio::read(socket_,boost::asio::buffer(ret_),boost::asio::transfer_all(),error);

    //read_message();
    wait_for_request();
    /*if(error != boost::asio::error::eof)
        std::cout<<"read:"<<ret_<<std::endl;*/


  }

private:
  tcp_connection(boost::asio::io_context& io_context)
    : socket_(io_context)
  {
  }

  void handle_read(const boost::system::error_code& error,
      size_t bytes_transferred);

  void handle_write(const boost::system::error_code& error,
      size_t bytes_transferred);

  tcp::socket socket_;
  std::string message_;
  std::string ret_;
  boost::asio::streambuf m_buffer;
  bool quit = false;
};

void tcp_connection::handle_read(const boost::system::error_code& error,
    size_t bytes_transferred)
{
    if(error != boost::asio::error::eof)
    {
        std::cout<<"bytes read: "<<bytes_transferred<<"bytes: "<<ret_<<std::endl;

        boost::asio::async_write(socket_, boost::asio::buffer("bytes read successfully\n"),
            boost::bind(&tcp_connection::handle_write, shared_from_this(),
              boost::asio::placeholders::error,
              boost::asio::placeholders::bytes_transferred));

        read_message();
    }
}

  void tcp_connection::handle_write(const boost::system::error_code &error, size_t bytes_transferred)
  {
      std::cout<<"bytes written: "<<bytes_transferred<<std::endl;
     /* boost::asio::async_read(socket_,boost::asio::buffer(ret_),
                              boost::bind(&tcp_connection::handle_read,shared_from_this(),
                                          boost::asio::placeholders::error,
                                          boost::asio::placeholders::bytes_transferred));*/
  }

class tcp_server
{
public:
  tcp_server(boost::asio::io_context& io_context)
    : io_context_(io_context),
      acceptor_(io_context, tcp::endpoint(tcp::v4(), 2008))
  {
    start_accept();
  }

private:
  void start_accept()
  {
    tcp_connection::pointer new_connection =
      tcp_connection::create(io_context_);

    acceptor_.async_accept(new_connection->socket(),
        boost::bind(&tcp_server::handle_accept, this, new_connection,
          boost::asio::placeholders::error));
  }

  void handle_accept(tcp_connection::pointer new_connection,
      const boost::system::error_code& error)
  {
    if (!error)
    {
      new_connection->start();
    }

    start_accept();
  }

  boost::asio::io_context& io_context_;
  tcp::acceptor acceptor_;
};

int main()
{
  try
  {
    boost::asio::io_context io_context;
    tcp_server server(io_context);
    io_context.run();
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
