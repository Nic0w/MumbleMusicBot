/*
 * MumbleClient.cpp
 *
 *  Created on: 28 déc. 2011
 *      Author: nic0w
 */

#include "MumbleClient.h"

MumbleClient::MumbleClient(std::string userName, std::string password) {


	this->userName = userName;
	this->password = password;

	this->ioService = new boost::asio::io_service;
}

MumbleClient::~MumbleClient() {
	// TODO Auto-generated destructor stub
}


void MumbleClient::mumbleConnect() {

	MumbleProto::Version mumbleVersion;
	mumbleVersion.set_version(MUMBLE_VERSION(1, 2, 2));
	mumbleVersion.set_release("MumbleMusicBot 0.0.1-dev");

	MumbleIO::MumblePacket versionPacket(MumbleIO::Version, mumbleVersion);

	sendPacket(versionPacket);




}


void MumbleClient::connect(std::string address, std::string port) throw(std::exception) {

	//First, we resolve the address, in case it's not an IP.
	tcp::resolver addressResolver(*this->ioService);
	tcp::resolver::query query(address, port);
	tcp::resolver::iterator addressesIterator = addressResolver.resolve(query); //A hostname may points to many IP addresses.
	tcp::resolver::iterator iteratorEnd;

	//We create the SSL socket
	context sslContext(*this->ioService, boost::asio::ssl::context::tlsv1);
	this->sslStream = new stream<tcp::socket>(*this->ioService, sslContext);
	tcp::socket::lowest_layer_type &tcpSocket = this->sslStream->lowest_layer();

	boost::system::error_code error = boost::asio::error::host_not_found;

	while(addressesIterator != iteratorEnd && error) {

		std::cout << "Trying to connect to " << (*addressesIterator).endpoint().address() << std::endl;

		tcpSocket.close(); //We close the last connection.
		tcpSocket.connect(*addressesIterator++, error); //And we try a new one.
	}

	if(error) { // If we didn't found any connection working.
		std::cout << "Connection error : " << error.message() << std::endl;
        throw std::exception();
	}
	else { std::cout << "Connected to : " << tcpSocket.remote_endpoint().address().to_string() << std::endl; }

	this->sslStream->handshake(boost::asio::ssl::stream_base::client, error);

	if(error) { // Handshake failed
		std::cout << "Handshake error : " << error.message() << std::endl;
        throw std::exception();
	}
	else { std::cout << "Handshake successful ! " << std::endl; }

	//tcp::no_delay noDelay(true);
    //boost::asio::socket_base::non_blocking_io nonBlockingIO(true);
    //tcpSocket.io_control(noDelay);
    //tcpSocket.set_option(nonBlockingIO);

    //Now, we connecthe UDP Stream :
	udp::endpoint udpEndpoint(tcpSocket.remote_endpoint().address(), tcpSocket.remote_endpoint().port());
	this->udpSocket = new udp::socket(*this->ioService);
	this->udpSocket->connect(udpEndpoint, error);

	mumbleConnect();

}

void MumbleClient::sendPacket(MumbleIO::MumblePacket &packet) {

	boost::system::error_code error;

	boost::asio::const_buffer data(*packet, packet.getPacketSize());

	int bytesWritten = this->sslStream->write_some(data, error);

}


void MumbleClient::infiniteReceivingLoop() {

	bool isRunning = true;

	while(isRunning) {


		this->sslStream->
	}

}
