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
        uint8_t operator = i == 0 ? 3
                         : i == 3 ? 0
                         : i;

        // Attack rate
        YM2151Driver.setATR(channel, operator, data++ & 0x1F);
        // Decay 1 rate
        YM2151Driver.setDec1R(channel, operator, data++ & 0x1F);
        // Decay 2 rate
        YM2151Driver.setDec2R(channel, operator, data++ & 0x1F);
        // Release rate
        YM2151Driver.setRel(channel, operator, data++ & 0x0F);
        // Decay 1 level
        YM2151Driver.setDec1L(channel, operator, data++ & 0x0F);
        // Keyboard Scaling level
        data++;

        // Amplitude Modulation Enable
        YM2151Driver.setAMSenseEn(channel, operator, (data >> 6) & 0x01);
        // EG Bias Sensitivity
        // Key Velocity Sensitivity

        data++;

        // Output Level
        YM2151Driver.setOpVolume(channel, operator,
            map(data++, 0, 99, 0, 127) & 0x7F
        );

        // Oscillator Frequency
        YM2151Driver.setMul(channel, operator, frequency_ratio_to_mul_dt2[data & 0x3F][0]);
        YM2151Driver.setDet2(channel, operator, frequency_ratio_to_mul_dt2[data & 0x3F][1]);
        data++;

        // Keyboard Scaling rate
        YM2151Driver.setKSR(channel, operator, (data >> 3) & 0x03);
        // Detune 1
        YM2151Driver.setKSR(channel, operator, data & 0x07);
    }

    // LFO Sync
    YM2151Driver.setLFOSync((data >> 6) & 0x01);
    // Feedback level
    YM2151Driver.setFeedback(channel, (data >> 3) & 0x07);
    // Algorithm
    YM2151Driver.setAlgorithm(channel, data & 0x07);
    data++;

    // LFO Speed
    YM2151Driver.setLFOFreq(data)
    // LFO Delay
    data++;

    // Pitch Modulation depth
    YM2151Driver.setPhaseDepth(data++);

    // Amplitude Modulation depth
    YM2151Driver.setAmpDepth(data++);
    
    // Pitch Modulation sensitivity
    YM2151Driver.setPMSense(data++);
    // Amplitude Modulation sensitivity
    YM2151Driver.setAMSense(data++);
    // LFO Wave
    YM2151Driver.setWaveForm(data++);

    // Transpose

    // Pitch Bend Range

    // Chorus Switch
    // Play mode (Poly/Mono)
    // Sustain Foot Switch
    // Portament Foot Switch
    // Portamento Mode

    // Portamento Time

    // Foot Volume
    
    // Modulation Wheel Pitch Modulation Range
    
    // Modulation Wheel Amplitude Modulation Range

    // Breath Control Pitch Modulation Range

    // Breath Control Amplitude Modulation Range

    // Breath Control Pitch Bias Range

    // Breath Control EG Bias Range

    // Voice Name

    // Pitch EG Rate 1

    // Pitch EG Rate 2

    // Pitch EG Rate 3

    // Pitch EG Level 1

    // Pitch EG Level 2

    // Pitch EG Level 3

}

VoiceDataHandlerClass VoiceDataHandler;
