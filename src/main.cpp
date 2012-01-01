/*
 * main.cpp
 *
 *  Created on: 28 déc. 2011
 *      Author: nic0w
 */

#include <iostream>

#include "MumbleClient.h"

using namespace std;

int main(int argc, char **argv) {


	cout << "Hello World !" << endl;

	MumbleClient client(string("MusicBot"), string(""));

	client.connect(string("jokercraft.fr"), string("64738"));

	return 0;

}


