#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;


int main(int argc, char* argv[])
{
  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: client <host>" << std::endl;
      return 1;
    }

    boost::asio::io_context io_context;

    tcp::resolver resolver(io_context);
    tcp::resolver::results_type endpoints =
      resolver.resolve(argv[1], "2008");

    tcp::socket socket(io_context);
    boost::asio::connect(socket, endpoints);
      boost::array<char, 128> buf;
      boost::system::error_code error;

      socket.send(boost::asio::buffer("Read"));
      /*while(error != boost::asio::error::eof)
      {
        size_t len = socket.read_some(boost::asio::buffer(buf), error);
        std::cout.write(buf.data(), len);
      }*/
      std::string test;
      std::cin >> test;


      //socket.write_some(boost::asio::buffer("Read"));
      //if (error == boost::asio::error::eof)
      //  break; // Connection closed cleanly by peer.
      //else if (error)
      //  throw boost::system::system_error(error); // Some other error.
    socket.close();
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
