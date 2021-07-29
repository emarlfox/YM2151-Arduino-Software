
/**
*	YM2151 - Chip Controller Software for ARDUINO
*	(C) 2016  Marcel Weiß
*
*	This program is free software : you can redistribute it and / or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	This program is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with this program.If not, see <http://www.gnu.org/licenses/>.
*/

#include "SysExHandler.h"
#include "VoiceDataHandler.h"
#include "YM2151Driver.h"

void SysExHandlerClass::init()
{
}

void SysExHandlerClass::handleSysEx(uint8_t* data, uint8_t length) {
	if (data[1] != 0x43) {
		return;
	}

	if (data[1] == 0x43
		&& data[2] == 0x00
		&& data[3] == 0x04
		&& data[4] == 0x20
		&& data[5] == 0x00
	) {
		for (int i = 0; i < 8; i++) {
			VoiceDataHandler.loadVmemBlock(i, &data[6]);
		}
	}
}

SysExHandlerClass SysExHandler;
