/*******************************************************************************
 * DFPlayer_Mini_Mp3, This library provides a quite complete function for      * 
 * DFPlayer mini mp3 module.                                                   *
 * www.github.com/dfrobot/DFPlayer_Mini_Mp3 (github as default source provider)*
 *  DFRobot-A great source for opensource hardware and robot.                  *
 *                                                                             *
 * This file is part of the DFplayer_Mini_Mp3 library.                         *
 *                                                                             *
 * DFPlayer_Mini_Mp3 is free software: you can redistribute it and/or          *
 * modify it under the terms of the GNU Lesser General Public License as       *
 * published by the Free Software Foundation, either version 3 of              *
 * the License, or any later version.                                          *
 *                                                                             *
 * DFPlayer_Mini_Mp3 is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of              *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               *
 * GNU Lesser General Public License for more details.                         *
 *                                                                             *
 * DFPlayer_Mini_Mp3 is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of              *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               *
 * GNU Lesser General Public License for more details.                         *
 *                                                                             *
 * You should have received a copy of the GNU Lesser General Public            *
 * License along with DFPlayer_Mini_Mp3. If not, see                           *
 * <http://www.gnu.org/licenses/>.                                             *
 *									       *
 ******************************************************************************/

/*
 *	Copyright:	DFRobot
 *	name:		DFPlayer_Mini_Mp3
 *	version:	1.0
 *	Author:		lisper <lisper.li@dfrobot.com>
 *	Date:		2014-05-22
 *	Description:	mp3 library for DFPlayer mini board
 */


#include <SoftwareSerial.h>
#include <Arduino.h>

extern uint8_t send_buf[10];
extern uint8_t recv_buf[10];

static void(*send_func)() = NULL;
static HardwareSerial *hserial = NULL;
static SoftwareSerial *sserial = NULL;
static boolean is_reply = false;

//
void mp3_set_reply (boolean state) {
	is_reply = state;
	send_buf[4] = is_reply;
}

//
static void fill_uint16_bigend (uint8_t *thebuf, uint16_t data) {
	*thebuf =	(uint8_t)(data>>8);
	*(thebuf+1) =	(uint8_t)data;
}


//
uint16_t mp3_get_checksum (uint8_t *thebuf) {
	uint16_t sum = 0;
	for (int i=1; i<7; i++) {
		sum += thebuf[i];
	}
	return -sum;
}

//
void mp3_fill_checksum () {
	uint16_t checksum = mp3_get_checksum (send_buf);
	send_buf[7] = (uint8_t)(checksum>>8);
	send_buf[8] = (uint8_t)checksum;
}

//
void h_send_func () {
	for (int i=0; i<10; i++) {
		hserial->write (send_buf[i]);
	}
}

//
void s_send_func () {
	for (int i=0; i<10; i++) {
		sserial->write (send_buf[i]);
	}
}

//
void mp3_set_serial (HardwareSerial *theSerial) {
	hserial = theSerial;
	send_func = h_send_func;
}

//
void mp3_set_serial (SoftwareSerial *theSerial) {
	sserial = theSerial;
	send_func = s_send_func;
}

//
void mp3_send_cmd (uint8_t cmd, uint16_t arg) {
	send_buf[3] = cmd;
	fill_uint16_bigend ((send_buf+5), arg);
	mp3_fill_checksum ();
	send_func ();
}


//
void mp3_play (uint16_t num) {
	mp3_send_cmd (0x03, num);
}

//
void mp3_next () {
	mp3_send_cmd (0x01, 0);
}

//
void mp3_prev () {
	mp3_send_cmd (0x02, 0);
}

//0x06 set volume 0-30
void mp3_set_volume (uint16_t volume) {
	mp3_send_cmd (0x06, volume);
}

//0x07 set EQ0/1/2/3/4/5    Normal/Pop/Rock/Jazz/Classic/Bass
void mp3_set_EQ (uint16_t eq) {
	mp3_send_cmd (0x07, eq);
}

//0x09 set device 1/2/3/4/5 U/SD/AUX/SLEEP/FLASH
void mp3_set_device (uint16_t device) {
	mp3_send_cmd (0x09, device);
}

//
void mp3_sleep () {
	mp3_send_cmd (0x0a, 0);
}

//
void mp3_reset () {
	mp3_send_cmd (0x0c, 0);
}

//
void mp3_play () {
	mp3_send_cmd (0x0d, 0);
}

//
void mp3_pause () {
	mp3_send_cmd (0x0e, 0);
}

//
void mp3_stop () {
	mp3_send_cmd (0x16, 0);
}

//
void mp3_play_mp3 (uint16_t num) {
	mp3_send_cmd (0x12, num);
}

//
void mp3_get_state () {
	mp3_send_cmd (0x42, 0);
}

//
void mp3_get_volume () {
	mp3_send_cmd (0x43, 0);
}


//
void mp3_get_u_sum () {
	mp3_send_cmd (0x47, 0);
}

//
void mp3_get_tf_sum () {
	mp3_send_cmd (0x48, 0);
}

//
void mp3_get_flash_sum () {
	mp3_send_cmd (0x49, 0);
}


//
void mp3_get_tf_current () {
	mp3_send_cmd (0x4c, 0);
}

//
void mp3_get_u_current () {
	mp3_send_cmd (0x4b, 0);
}

//
void mp3_get_flash_current () {
	mp3_send_cmd (0x4d, 0);
}

//
void mp3_single_loop (boolean state) {
	mp3_send_cmd (0x19, !state);
}

//
void mp3_DAC (boolean state) {
	mp3_send_cmd (0x1a, !state);
}

//
void mp3_random_play () {
	mp3_send_cmd (0x18, 0);
}

