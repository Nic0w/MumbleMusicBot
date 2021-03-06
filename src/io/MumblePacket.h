/*
 * MumblePacket.h
 *
 *  Created on: 31 déc. 2011
 *      Author: nic0w
 */

#ifndef MUMBLEPACKET_H_
#define MUMBLEPACKET_H_

#include <stdlib.h>
#include "../Mumble.pb.h"

using namespace google::protobuf;

namespace MumbleIO {

enum MumblePacketType {
	Version,
	UDPTunnel,
	Authenticate,
	Ping,
	Reject,
	ServerSync,
	ChannelRemove,
	ChannelState,
	UserRemove,
	UserState,
	BanList,
	TextMessage,
	PermissionDenied,
	ACL,
	QueryUsers,
	CryptSetup,
	ContextActionAdd,
	ContextAction,
	UserList,
	VoiceTarget,
	PermissionQuery,
	CodecVersion,
	UserStats,
	RequestBlob,
	ServerConfig
};

struct MumblePacketHeader {

	short packetType;
	int packetLength;

};

class MumblePacket {
public:
	MumblePacket(MumblePacketType type, const Message &message);
	virtual ~MumblePacket();

	int getPacketSize();

	char *getRawData();
	char *operator*(MumblePacket &packet);

private:
	char *messageData;
	MumblePacketHeader packetHeader;

	char *rawData;
};

} /* namespace MumbleIO */
#endif /* MUMBLEPACKET_H_ */
