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

#include "VoiceDataHandler.h"
#include "YM2151Driver.h"

void VoiceDataHandlerClass::init()
{
}

/**
 * Handler for VMEM (Voice Bulk Data)
 */
void VoiceDataHandlerClass::handleVmem(uint8_t* data) {
    
}

/**
 * Handler for VCED (Voice Parameters) data
 */
void VoiceDataHandlerClass::handleVced(uint8_t* data) {

}

/**
 * Loads VMEM data into a specified channel.
 */
void VoiceDataHandlerClass::loadVmemBlock(uint8_t channel, uint8_t* data) {
    // Operators 4 - 2 - 3 - 1
    for (uint8_t i = 0; i < 4; i++) {
        uint8_t op = (i == 0) ? 3
                    : (i == 3) ? 0
                    : i;

        // Attack rate
        YM2151Driver.setATR(channel, op, *(data++) & 0x1F);

        // Decay 1 rate
        YM2151Driver.setDec1R(channel, op, *(data++) & 0x1F);
        // Decay 2 rate
        YM2151Driver.setDec2R(channel, op, *(data++) & 0x1F);
        // Release rate
        YM2151Driver.setRel(channel, op, *(data++) & 0x0F);
        // Decay 1 level
        YM2151Driver.setDec1L(channel, op, *(data++) & 0x0F);
        // Keyboard Scaling level
        data++;

        // Amplitude Modulation Enable
        YM2151Driver.setAMSenseEn(channel, op, (*(data) >> 6) & 0x01);
        // EG Bias Sensitivity
        // Key Velocity Sensitivity

        data++;

        // Output Level
        YM2151Driver.setOpVolume(channel, op,
            map(*(data++), 0, 99, 0, 127) & 0x7F
        );

        // Oscillator Frequency
        YM2151Driver.setMul(channel, op, frequency_ratio_to_mul_dt2[*(data) & 0x3F][0]);
        YM2151Driver.setDet2(channel, op, frequency_ratio_to_mul_dt2[*(data) & 0x3F][1]);
        data++;

        // Keyboard Scaling rate
        YM2151Driver.setKSR(channel, op, (*(data) >> 3) & 0x03);
        // Detune 1
        YM2151Driver.setDet1(channel, op, *(data) & 0x07);
        data++;
    }

    // LFO Sync
    YM2151Driver.setLFOSync((*(data) >> 6) & 0x01);
    // Feedback level
    YM2151Driver.setFeedback(channel, (*(data) >> 3) & 0x07);
    // Algorithm
    YM2151Driver.setAlgorithm(channel, *(data) & 0x07);
    data++;

    // LFO Speed
    YM2151Driver.setLFOFreq(*(data));
    // LFO Delay
    data++;

    // Pitch Modulation depth
    YM2151Driver.setPhaseDepth(*(data++));

    // Amplitude Modulation depth
    YM2151Driver.setAmpDepth(*(data++));
    
    // Pitch Modulation sensitivity
    YM2151Driver.setPMSense(channel, (*(data) >> 4) & 0x07);
    // Amplitude Modulation sensitivity
    YM2151Driver.setAMSense(channel, (*(data) >> 2) & 0x02);
    // LFO Wave
    YM2151Driver.setWaveForm(*(data) & 0x02);
    data++;

    // Transpose
    YM2151Driver.setTranspose(*(data++));

    // Pitch Bend Range
    data++;

    // Chorus Switch
    // Play mode (Poly/Monophonic)
    // Sustain Foot Switch
    // Portament Foot Switch
    // Portamento Mode
    data++;

    // Portamento Time
    data++;

    // Foot Volume
    data++;
    
    // Modulation Wheel Pitch Modulation Range
    data++;
    
    // Modulation Wheel Amplitude Modulation Range
    data++;

    // Breath Control Pitch Modulation Range
    data++;

    // Breath Control Amplitude Modulation Range
    data++;

    // Breath Control Pitch Bias Range
    data++;

    // Breath Control EG Bias Range
    data++;

    // Voice Name
    data += 10;

    // Pitch EG Rate 1
    data++;

    // Pitch EG Rate 2
    data++;

    // Pitch EG Rate 3
    data++;

    // Pitch EG Level 1
    data++;

    // Pitch EG Level 2
    data++;

    // Pitch EG Level 3
    data++;
}

PROGMEM const unsigned char deepGrandVmem[] = {
    0xF0, 0x43, 0x00, 0x04, 0x20, 0x00, 
    0x18, 0x01, 0x01, 0x03, 0x00, 0x15, 
    0x00, 0x41, 0x04, 0x0D, 0x16, 0x01, 
    0x01, 0x04, 0x0C, 0x4B, 0x00, 0x52, 
    0x00, 0x13, 0x18, 0x05, 0x01, 0x03, 
    0x00, 0x57, 0x00, 0x32, 0x16, 0x08, 
    0x14, 0x08, 0x01, 0x05, 0x0C, 0x01, 
    0x00, 0x63, 0x04, 0x13, 0x32, 0x23, 
    0x00, 0x00, 0x00, 0x02, 0x18, 0x00, 
    0x04, 0x00, 0x63, 0x32, 0x00, 0x00, 
    0x00, 0x32, 0x00, 0x44, 0x65, 0x65, 
    0x70, 0x20, 0x47, 0x72, 0x61, 0x6E, 
    0x64, 0x63, 0x63, 0x63, 0x32, 0x32, 
    0x32, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0xF7
};

VoiceDataHandlerClass VoiceDataHandler;
