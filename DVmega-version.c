/*
 *   Copyright (C) 2016 by Florian Wolters DF2ET
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>

int main() {
	char *portname = "/dev/ttyUSB0";

	int fd = open (portname, O_RDWR | O_NOCTTY | O_SYNC);
	if (fd < 0)
	{
		printf("error %d opening %s: %s\n", errno, portname, strerror (errno));
		return -1;
	}

	struct termios tty;

	if (tcgetattr(fd, &tty) < 0) {
		printf("Error from tcgetattr: %s\n", strerror(errno));
		return -1;
	}

	cfsetospeed(&tty, (speed_t)B115200);
	cfsetispeed(&tty, (speed_t)B115200);

	tty.c_cflag |= (CLOCAL | CREAD);
	tty.c_cflag &= ~CSIZE;
	tty.c_cflag |= CS8;
	tty.c_cflag &= ~PARENB;
	tty.c_cflag &= ~CSTOPB;
	tty.c_cflag &= ~CRTSCTS;

	tty.c_cc[VMIN] = 1;
	tty.c_cc[VTIME] = 1;

	if (tcsetattr(fd, TCSANOW, &tty) != 0) {
		printf("Error from tcsetattr: %s\n", strerror(errno));
		return -1;
	}

	unsigned char buf[3] = { 0xE0, 0x03, 0x00 };
	int wlen;

	usleep(2000000);

	wlen = write(fd, buf, sizeof(buf));
	if (wlen != sizeof(buf)) {
		printf("Error from write: %d, %d\n", wlen, errno);
	}
	tcdrain(fd);
	usleep(100000);

	unsigned char rbuf[50];
	int rdlen;

	rdlen = read(fd, rbuf, 50);
	if (rdlen > 0) {
		if (rbuf[0] != 0xE0) {
			printf("Error reading response from modem!\n");
			return -1;
		}
		if (rbuf[2] != 0x00) {
			printf("Invalid response from modem!\n");
			return -1;
		}
		int strlen = (int) rbuf[1];
		printf("Version: %u Description: ", rbuf[3]);
		unsigned char *p;
		for (p = rbuf+4; strlen-- > 4; p++)
			printf("%c", *p);
		printf("\n");
	} else if (rdlen < 0) {
		printf("Error from read: %d: %s\n", rdlen, strerror(errno));
	}
}
