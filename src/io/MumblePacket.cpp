/*
 * MumblePacket.cpp
 *
 *  Created on: 31 déc. 2011
 *      Author: nic0w
 */

#include "MumblePacket.h"

namespace MumbleIO {

MumblePacket::MumblePacket(MumblePacketType type, Message &message) {


	this->packetData = message;

	this->packetHeader.packetType = static_cast<short>(type);
	this->packetHeader.packetLength = message.ByteSize() + 6; //The 6 more bytes are for the header (2 + 4 bytes -> type + size)

	this->rawData = new char[this->packetHeader.packetLength];

	forgeRawData();
}

MumblePacket::~MumblePacket() {
	// TODO Auto-generated destructor stub
}

void MumblePacket::forgeRawData() {

	memcpy(this->rawData, static_cast<char *>(&this->packetHeader), 6); //We copy the header.

	int messageSize = this->packetHeader.packetLength - 6;
	char *dataBuffer = new char[messageSize];

	this->packetData.SerializeToArray(dataBuffer, 6);

	memcpy(this->rawData+6, dataBuffer, messageSize); //We copy the message

	delete dataBuffer;
}


int MumblePacket::getPacketSize() {

	return this->packetHeader.packetLength;
}

char *MumblePacket::operator*(MumblePacket &packet) {
	return packet.rawData;
}

} /* namespace MumbleIO */
