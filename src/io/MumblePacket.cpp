/*
 * MumblePacket.cpp
 *
 *  Created on: 31 déc. 2011
 *      Author: nic0w
 */

#include "MumblePacket.h"

namespace MumbleIO {

MumblePacket::MumblePacket(MumblePacketType type, const Message &message) {

	int msgSize = message.ByteSize();

	this->messageData = new char[msgSize];
	message.SerializePartialToArray(this->messageData, msgSize);

	this->packetHeader.packetType = static_cast<short>(type);
	this->packetHeader.packetLength = message.ByteSize() + 6; //The 6 more bytes are for the header (2 + 4 bytes -> type + size)

	this->rawData = new char[this->packetHeader.packetLength];

	memcpy(this->rawData, static_cast<void *>(&this->packetHeader), 6); //We copy the header.
	memcpy(this->rawData+6, this->messageData, msgSize); //We copy the message
}

MumblePacket::~MumblePacket() {

	delete this->messageData;
	delete this->rawData;

}


int MumblePacket::getPacketSize() {

	return this->packetHeader.packetLength;
}

char *MumblePacket::getRawData() {

	return this->rawData;
}

char *MumblePacket::operator*(MumblePacket &packet) {
	return packet.rawData;
}

} /* namespace MumbleIO */
