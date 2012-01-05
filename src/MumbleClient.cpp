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

	boost::shared_ptr<MumbleIO::MumblePacket> versionPacket = boost::make_shared<MumbleIO::MumblePacket>(MumbleIO::Version, mumbleVersion);

	this->packetQueue.push_back(versionPacket);

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

void MumbleClient::infiniteSendingLoop() {

	bool isRunning = true;
	int bytesSent = 0;

	std::vector<boost::asio::const_buffer> buffers;

	while(isRunning) {
		if(!this->packetQueue.empty()) {

			boost::system::error_code errorCode;

			boost::shared_ptr<MumbleIO::MumblePacket> packet = this->packetQueue.front();
			boost::asio::const_buffer dataBuffer(packet->getRawData(), packet->getPacketSize());
			buffers.push_back(dataBuffer);

			bytesSent = this->sslStream->write_some(buffers, errorCode);
			if(bytesSent != (*packet).getPacketSize() || errorCode) {
				std::cout << "IO Error in infiniteSendingLoop " << std::endl;
				isRunning = false;
			}
			else {
				this->packetQueue.pop_front();
				buffers.pop_back();
			}

		}
	}

}


void MumbleClient::infiniteReceivingLoop() {

	bool isRunning = true;
	int bytesRead = 0;

	std::vector<boost::asio::mutable_buffer> buffers;

	MumbleIO::MumblePacketHeader header;
	char *messageData = NULL;

	boost::asio::mutable_buffer headerBuf(reinterpret_cast<char *>(&header), 6);



	while(isRunning) {
		boost::system::error_code errorCode;

		buffers.push_back(headerBuf);
		bytesRead = this->sslStream->read_some(buffers, errorCode);

		if(bytesRead != 6 || errorCode) {

			//IO Error
		}
		else {

			buffers.pop_back();

			header.packetType = ntohs(header.packetType);
			header.packetLength = ntohl(header.packetLength);

			messageData = new char[header.packetLength];
			boost::asio::mutable_buffer messageBuf(messageData, header.packetLength);

			buffers.push_back(messageBuf);

			bytesRead = this->sslStream->read_some(buffers, errorCode);

		}









	}

}
