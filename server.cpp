#include<array>
#include<funcs/funcs.hpp>
#include<boost/asio.hpp>
#include<iostream>
#include<vector>

int main()
{
    boost::asio::io_service ctx;
    boost::asio::ip::udp::socket socket{ctx, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 13)};
    
    using boost::asio::ip::udp;
    using client = udp::endpoint;
    std::vector<client> cl;
    for (int i = 0; i < 2; ++i)
    {
      std::array<char, 1> recv_buf;
      udp::endpoint remote_endpoint; // filled after
      socket.receive_from(boost::asio::buffer(recv_buf), remote_endpoint);

      std::cout <<"Remote: " <<remote_endpoint.address().to_string() << remote_endpoint.port() << "\n";

      std::string message = "NIGER!\n";

      boost::system::error_code ignored_error;
      socket.send_to(boost::asio::buffer(message),
          remote_endpoint, 0, ignored_error);

      cl.push_back(remote_endpoint);
    }

    //send_to_first_the_endpoint_of_second()
    boost::system::error_code ignored_error;
    std::string endpoint_str = cl[1].address().to_string();
    endpoint_str+=std::to_string(cl[1].port());
    std::cout << "cl[1] adress: " << endpoint_str << "\n";
    socket.send_to(boost::asio::buffer(endpoint_str),
        cl[0],0,ignored_error);

        //server больше не нужен по сути...


    system("pause");



    niger2(3);
}