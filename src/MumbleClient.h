/*
 * MumbleClient.h
 *
 *  Created on: 28 déc. 2011
 *      Author: nic0w
 */

#ifndef MUMBLECLIENT_H_
#define MUMBLECLIENT_H_

#define MUMBLE_VERSION(x, y, z) ((x << 16) | (y << 8) | (z & 0xFF))

#include <stdlib.h>
#include <stdio.h>

#include <string>
#include <fstream>
#include <iostream>
#include <typeinfo>
#include <deque>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

#include <google/protobuf/io/zero_copy_stream_impl_lite.h>

#include "Mumble.pb.h"

using namespace boost::asio::ip;
using namespace boost::asio::ssl;

using namespace google::protobuf;

class MumbleClient {
public:
	MumbleClient(std::string userName, std::string password);
	virtual ~MumbleClient();

	void connect(std::string address, std::string port) throw(std::exception);

private:
	std::string userName;
	std::string password;
	std::deque<boost::shared_ptr<MumbleIO::MumblePacket>> packetQueue;
	//tcp::socket *tcpSocket;
	udp::socket *udpSocket;
	stream<tcp::socket> *sslStream;
	boost::asio::io_service *ioService;
	boost::thread receiverThread;
	boost::thread senderThread;
	boost::thread pingThread;


	void mumbleConnect();
	void sendPacket(MumbleIO::MumblePacket &packet);

	void infiniteReceivingLoop(); //receiverThread
	void infiniteSendingLoop(); //senderThread
	void pingLoop(); //pingerThread
};

#endif /* MUMBLECLIENT_H_ */
