#include <array>
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

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

    udp::resolver resolver(io_context);
    udp::endpoint receiver_endpoint =
      *resolver.resolve(udp::v4(), argv[1], "13").begin();

    udp::socket socket(io_context);
    socket.open(udp::v4());

    std::array<char, 1> send_buf  = {{ 0 }};
    socket.send_to(boost::asio::buffer(send_buf), receiver_endpoint);

    {
    std::array<char, 128> recv_buf;
    udp::endpoint sender_endpoint;
    size_t len = socket.receive_from(
        boost::asio::buffer(recv_buf), sender_endpoint);

    std::cout.write(recv_buf.data(), len);//niger
    }

    std::array<char, 128> recv_buf_adress;
    udp::endpoint sender_endpoint;
    size_t len = socket.receive_from(
        boost::asio::buffer(recv_buf_adress), sender_endpoint);
    
    std::cout << "Got ip\n";

    auto ip_from_adress = [&recv_buf_adress] -> std::string_view
    {
      auto chPt = std::find(recv_buf_adress.begin(), recv_buf_adress.end(), ':');
      return {recv_buf_adress.data(), std::size_t(chPt-recv_buf_adress.data())};
    }();

    auto port_from_adress = [&recv_buf_adress] -> boost::asio::ip::port_type
    {
      auto chPt = std::find(recv_buf_adress.begin(), recv_buf_adress.end(), ':'); chPt++;
      std::string_view s{chPt, std::size_t(recv_buf_adress.end() - chPt)};

      std::uint_least16_t res = 0;
      res += (s[0]-'0') * 10;
      res += (s[1]-'0');
      return res;
    }();

    udp::endpoint client_second{boost::asio::ip::make_address_v4(ip_from_adress),port_from_adress};
    std::string msg_to_sec = "SUKA!\n";

    udp::socket socket2(io_context);
    socket2.open(udp::v4());

    socket2.send_to(boost::asio::buffer(msg_to_sec),client_second);




    


  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}