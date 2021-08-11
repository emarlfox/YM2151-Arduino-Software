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

#include "VoiceData.h"
#include "YM2151Driver.h"

void VoiceDataClass::init()
{
}

/**
 * Handler for VMEM (Voice Bulk Data)
 */
void VoiceDataClass::handleVmem(uint8_t* data) {
    
}

/**
 * Handler for VCED (Voice Parameters) data
 */
void VoiceDataClass::handleVced(uint8_t* data) {

}

/**
 * Loads VMEM data into a specified channel.
 */
void VoiceDataClass::loadVmemBlock(uint8_t channel, uint8_t* data) {
    // Operators 4 - 2 - 3 - 1
    for (uint8_t i = 4; i != 0; i--) {
        uint8_t op = i - 1;

        Serial.print("OP");
        Serial.print(((op == 2) ? 1 : (op == 1) ? 2 : op) + 1);
        Serial.println();

        YM2151Driver.setOpActive(channel, op, true);

        // Attack rate
        Serial.print("ATR ");
        Serial.print("\t");
        Serial.print(*data, HEX);
        Serial.println();

        YM2151Driver.setATR(channel, op, *(data++) & 0x1F);

        // Decay 1 rate
        Serial.print("D1R ");
        Serial.print("\t");
        Serial.print(*data, HEX);
        Serial.println();

        YM2151Driver.setDec1R(channel, op, *(data++) & 0x1F);

        // Decay 2 rate
        Serial.print("D2R ");
        Serial.print("\t");
        Serial.print(*data, HEX);
        Serial.println();

        YM2151Driver.setDec2R(channel, op, *(data++) & 0x1F);

        // Release rate
        Serial.print("RR ");
        Serial.print("\t");
        Serial.print(*data, HEX);
        Serial.println();
        YM2151Driver.setRel(channel, op, *(data++) & 0x0F);

        // Decay 1 level
        Serial.print("D1L ");
        Serial.print("\t");
        Serial.print(*data, HEX);
        Serial.println();

        YM2151Driver.setDec1L(channel, op, *(data++) & 0x0F);

        // Keyboard Scaling level
        data++;

        // Amplitude Modulation Enable
        Serial.print("AMS-EN/EGB/KV Sens ");
        Serial.print("\t");
        Serial.print(*data, HEX);
        Serial.print("\t");
        Serial.print(*data, BIN);
        Serial.println();

        YM2151Driver.setAMSenseEn(channel, op, (*(data) >> 6) & 0x01);
        // EG Bias Sensitivity
        // Key Velocity Sensitivity

        data++;

        // Output Level
        Serial.print("TL ");
        Serial.print("\t");
        Serial.print(*data, HEX);
        Serial.println();

        YM2151Driver.setOpVolume(channel, op,
            map(*(data++), 0, 99, 127, 0) & 0x7F
        );

        // Oscillator Frequency
        Serial.print("OSC-F ");
        Serial.print("\t");
        Serial.print(*data, HEX);
        Serial.println();
        Serial.print("MUL ");
        Serial.print("\t");
        Serial.print(frequency_ratio_to_mul_dt2[*(data) & 0x3F][0], HEX);
        Serial.println();
        Serial.print("DT2 ");
        Serial.print("\t");
        Serial.print(frequency_ratio_to_mul_dt2[*(data) & 0x3F][1], HEX);
        Serial.println();

        YM2151Driver.setMul(channel, op, frequency_ratio_to_mul_dt2[*(data) & 0x3F][0]);
        YM2151Driver.setDet2(channel, op, frequency_ratio_to_mul_dt2[*(data) & 0x3F][1]);
        data++;

        // Keyboard Scaling rate
        Serial.print("KSR ");
        Serial.print("\t");
        Serial.print(*data, HEX);
        Serial.println();
        YM2151Driver.setKSR(channel, op, (*(data) >> 3) & 0x03);
        // Detune 1
        Serial.print("DT1 ");
        Serial.print("\t");
        Serial.print(*data, HEX);
        Serial.println();
        YM2151Driver.setDet1(channel, op, *(data) & 0x07);
        data++;
    }

    Serial.print("LFO Sync/Feedback/Algorithm ");
    Serial.print("\t");
    Serial.print(*data, HEX);
    Serial.print("\t");
    Serial.print(*data, BIN);
    Serial.println();
    // LFO Sync
    YM2151Driver.setLFOSync((*(data) >> 6) & 0x01);
    // Feedback level
    YM2151Driver.setFeedback(channel, (*(data) >> 3) & 0x07);
    // Algorithm
    YM2151Driver.setAlgorithm(channel, *(data) & 0x07);
    data++;

    Serial.print("LFO Speed/Delay ");
    Serial.print("\t");
    Serial.print(*data, HEX);
    Serial.print("\t");
    Serial.print(*data, BIN);
    Serial.println();
    // LFO Speed
    YM2151Driver.setLFOFreq(*(data));
    // LFO Delay
    data++;

    Serial.print("PMD ");
    Serial.print("\t");
    Serial.print(*data, HEX);
    Serial.println();
    // Pitch Modulation depth
    YM2151Driver.setPhaseDepth(*(data++));

    Serial.print("AMD ");
    Serial.print("\t");
    Serial.print(*data, HEX);
    Serial.println();
    // Amplitude Modulation depth
    YM2151Driver.setAmpDepth(*(data++));
    
    Serial.print("PMS/AMS/LFO Wave");
    Serial.print("\t");
    Serial.print(*data, HEX);
    Serial.print("\t");
    Serial.print(*data, BIN);
    Serial.println();
    // Pitch Modulation sensitivity
    YM2151Driver.setPMSense(channel, (*(data) >> 4) & 0x07);
    // Amplitude Modulation sensitivity
    YM2151Driver.setAMSense(channel, (*(data) >> 2) & 0x02);
    // LFO Wave
    YM2151Driver.setWaveForm(*(data) & 0x02);
    data++;

    Serial.print("Transpose");
    Serial.print("\t");
    Serial.print(*data, HEX);
    Serial.println();
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

VoiceDataClass VoiceData;
