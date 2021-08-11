/**
*	YM2151 - Chip Controller Software for ARDUINO
*	(C) 2016  Marcel Weiß
*	(C) 2016  Charles Hobbs
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



//TODO Use Most Significant Bits instead of LSBs for Values (Top part of byte instead of bottom part) e.g.
//
//	((value & 0xf) << 4       -->   ((value & 0x78) << 1)
//	((value & 0x3) << 6)	  -->	((value & 0x60) << 1)
//	(value & 0x0F)			  -->	((value & 0x78) >> 3)
//
// ... DONE, but needs Testing



#include "YM2151Driver.h"
#include "SysExHandler.h"
#include "EPROMManager.h"
#include <Arduino.h>

void YM2151DriverClass::init()
{
	for (byte i = 0; i < 8; i++) {
		YM2151Driver.MasterVolume[i] = 63;
	}

	// YM2151Driver.loadTestPatch();
	SysExHandler.testVmem();

	for (byte i = 0; i < 8; i++) {
		YM2151Driver.setPan(i, 0x03);
	}

	YM2151.initLFO();
}

void YM2151DriverClass::setOpVolume(uint8_t channel, uint8_t op, uint8_t value){
	uint8_t adr = getAdr(channel, op);
	TotalLevel[adr] = (TotalLevel[adr] & 0x80) | (value & 0x7F);

	int16_t att, tl;
	tl = map(MasterVolume[channel], 0, 127, 127, -127);

	//see setOpActive
	op = (op == 1) ? 2
		: (op == 2) ? 1
		: op;

	if (OpOn[channel] & (1 << op)) {
		att = TotalLevel[adr] + tl;
	}
	else {
		att = TotalLevel[adr];
	}

	att = constrain(att, 0, 0x7f);

	YM2151.write(0x60 + adr, att);
}

void YM2151DriverClass::setMul(uint8_t channel, uint8_t op, uint8_t value){
	uint8_t adr = getAdr(channel, op);
	DetPhase_Mul[adr] = (DetPhase_Mul[adr] & 0xf0) | (value & 0x0f);
	YM2151.write(0x40 + adr, DetPhase_Mul[adr]);
}
void YM2151DriverClass::setDet1(uint8_t channel, uint8_t op, uint8_t value){
	uint8_t adr = getAdr(channel, op);
	DetPhase_Mul[adr] = (DetPhase_Mul[adr] & 0x8f) | ((value & 0x07) << 4);
	YM2151.write(0x40 + adr, DetPhase_Mul[adr]);
}
void YM2151DriverClass::setDet2(uint8_t channel, uint8_t op, uint8_t value){
	uint8_t adr = getAdr(channel, op);
	EG_Decay2[adr] = (EG_Decay2[adr] & 0x3F) | ((value & 0x03) << 6);
	YM2151.write(0xC0 + adr, EG_Decay2[adr]);
}
void YM2151DriverClass::setKSR(uint8_t channel, uint8_t op, uint8_t value){
	uint8_t adr = getAdr(channel, op);
	EG_Attack[adr] = (EG_Attack[adr] & 0x3F) | ((value & 0x03) << 6);
	YM2151.write(0x80 + adr, EG_Attack[adr]);
}
void YM2151DriverClass::setATR(uint8_t channel, uint8_t op, uint8_t value){
	uint8_t adr = getAdr(channel, op);
	EG_Attack[adr] = (EG_Attack[adr] & 0xE0) | (value & 0x1F);
	YM2151.write(0x80 + adr, EG_Attack[adr]);
}
void YM2151DriverClass::setDec1R(uint8_t channel, uint8_t op, uint8_t value){
	uint8_t adr = getAdr(channel, op);
	EG_Decay1[adr] = (EG_Decay1[adr] & 0xE0) | (value & 0x1F);
	YM2151.write(0xA0 + adr, EG_Decay1[adr]);
}

void YM2151DriverClass::setAMSenseEn(uint8_t channel, uint8_t op, uint8_t value) {
	uint8_t adr = getAdr(channel, op);
	EG_Decay1[adr] = (EG_Decay1[adr] & 0x7F) | ((value & 0x01) << 7);
	YM2151.write(0xA0 + adr, EG_Decay1[adr]);
}

void YM2151DriverClass::setDec1L(uint8_t channel, uint8_t op, uint8_t value){
	uint8_t adr = getAdr(channel, op);
	EGDec_RelRate[adr] = (EGDec_RelRate[adr] & 0x0f) | ((value & 0x0f) << 4);
	YM2151.write(0xE0 + adr, EGDec_RelRate[adr]);
}

void YM2151DriverClass::setDec2R(uint8_t channel, uint8_t op, uint8_t value){
	uint8_t adr = getAdr(channel, op);
	EG_Decay2[adr] = (EG_Decay2[adr] & 0xf0) | (value & 0x0f);
	YM2151.write(0xC0 + adr, EG_Decay2[adr]);
}

void YM2151DriverClass::setRel(uint8_t channel, uint8_t op, uint8_t value){
	uint8_t adr = getAdr(channel, op);
	//uint8_t fix = (EGDec_RelRate[adr] & 0xf0) | (value & 0x0F);
	EGDec_RelRate[adr] = (EGDec_RelRate[adr] & 0xf0) | (value & 0x0f);
	YM2151.write(0xE0 + adr, EGDec_RelRate[adr]);
}

void YM2151DriverClass::setOpActive(uint8_t channel, uint8_t op, bool enable) {
	//Switch the Values because the sequence (M1,M2,C1,C2) is Wrong in Register #0x08, but right for all other Registers
	op = (op == 1) ? 2
		: (op == 2) ? 1
		: op;

	if (enable) {
		OpOn[channel] = OpOn[channel] | (1 << op);
	}
	else {
		OpOn[channel] = OpOn[channel] & (~(1 << op));
	}
}

void YM2151DriverClass::setAlgorithm(uint8_t channel, uint8_t value){
	ChannelControl[channel] = (ChannelControl[channel] & 0xF8) | (value & 0x07);
	YM2151.write(0x20 + channel, ChannelControl[channel]);
}

void YM2151DriverClass::setFeedback(uint8_t channel, uint8_t value){
	ChannelControl[channel] = (ChannelControl[channel] & 0xC7) | ((value & 0x07) << 3);
	YM2151.write(0x20 + channel, ChannelControl[channel]);
}

void YM2151DriverClass::setPMSense(uint8_t channel, uint8_t value) {
	PhAmp_ModSens[channel] = (PhAmp_ModSens[channel] & 0x8F) | ((value & 0x07) << 4);
	YM2151.write(0x38 + channel, PhAmp_ModSens[channel]);
}

void YM2151DriverClass::setAMSense(uint8_t channel, uint8_t value) {
	PhAmp_ModSens[channel] = (PhAmp_ModSens[channel] & 0xFC) | (value & 0x03);
	YM2151.write(0x38 + channel, PhAmp_ModSens[channel]);
}

void YM2151DriverClass::setLFOFreq(uint8_t value) {
	LFOFreq = value;
	YM2151.write(0x18, LFOFreq);
}

void YM2151DriverClass::setWaveForm(uint8_t value) {
	CtrlOut_WF = (CtrlOut_WF & 0xFC) | (value & 0x03);
	YM2151.write(0x1B, CtrlOut_WF);
}

void YM2151DriverClass::setCTRLout(uint8_t value) {
	CtrlOut_WF = (CtrlOut_WF & 0x3F) | ((value & 0x03) << 6);
	YM2151.write(0x1B, CtrlOut_WF);
}

void YM2151DriverClass::setPhaseDepth(uint8_t value) {
	PhAmpMod = (value & 0x7F) | 0x80;
	YM2151.write(0x19, PhAmpMod);
}

void YM2151DriverClass::setAmpDepth(uint8_t value) {
	PhAmpMod = (value & 0x7F);
	YM2151.write(0x19, PhAmpMod);
}

void YM2151DriverClass::setNoiseEnable(uint8_t value) {
	Noize = (Noize & 0x7F) | ((value & 0x01) << 7);
	YM2151.write(0x0F, Noize);
}

void YM2151DriverClass::setNoiseFreq(uint8_t value) {
	Noize = (Noize & 0xE0) | (value & 0x1F);
	YM2151.write(0x0F, Noize);
}

void YM2151DriverClass::setPan(uint8_t channel, uint8_t value){
	//rethink about that (first switches on left, then right, then both, BUT should be left,both,right or so)
	// byte pan[] = { 0x00, 0x01, 0x03, 0x02 };
	ChannelControl[channel] = (ChannelControl[channel] & 0x3F) | ((value & 0x03) << 6);
	YM2151.write(0x20 + channel, ChannelControl[channel]);
}

void YM2151DriverClass::noteOn(uint8_t channel){
	if (this->LFOSync) {
		YM2151.write(0x01, 0x02);
		YM2151.write(0x01, 0x01);
	}
	YM2151.write(0x08, (OpOn[channel]<<3) | (channel & 0x07));
}

void YM2151DriverClass::noteOff(uint8_t channel){
	YM2151.write(0x08, 0x00 | (channel & 0x7));
}

void YM2151DriverClass::setTone(uint8_t ch, uint8_t keycode, int16_t kf)
{
	YM2151.setTone(ch, keycode + (Transpose - 24), kf);
}

void YM2151DriverClass::setTranspose(uint8_t value)
{
	this->Transpose = constrain(value, 0, 48);
}

void YM2151DriverClass::setMasterVolume(uint8_t channel, uint8_t value) {
	YM2151DriverClass::MasterVolume[channel] = value;
	for (uint8_t i = 0; i < 4; i++) {
		uint8_t adr = getAdr(channel, i);
		setOpVolume(channel, i, YM2151DriverClass::TotalLevel[adr]);
	}
	
}

void YM2151DriverClass::setLFOSync(uint8_t value) {
	this->LFOSync = value;
}

extern  PROGMEM const unsigned char initPatch[];
extern  PROGMEM const unsigned char testPatch[];

void YM2151DriverClass::loadInitPatch() {
	Serial.print("loadInitPatch()\r\n\r\n");

	//LFO
	setLFOFreq(pgm_read_byte_near(initPatch));
	setAmpDepth(pgm_read_byte_near(initPatch + 1));
	setPhaseDepth(pgm_read_byte_near(initPatch + 2));
	setWaveForm(pgm_read_byte_near(initPatch + 3));
	setNoiseFreq(pgm_read_byte_near(initPatch + 4));
	setNoiseEnable(pgm_read_byte_near(initPatch + 11));


	for (int i = 0; i < 1; i++) {
		//CH
		setPan(i, pgm_read_byte_near(initPatch + 5));
		setFeedback(i, pgm_read_byte_near(initPatch + 6));
		setAlgorithm(i, pgm_read_byte_near(initPatch + 7));
		setAMSense(i, pgm_read_byte_near(initPatch + 8));
		setPMSense(i, pgm_read_byte_near(initPatch + 9));
		byte a = pgm_read_byte_near(initPatch + 10);
		setOpActive(i, 0, false);
		setOpActive(i, 2, false);
		setOpActive(i, 1, false);
		setOpActive(i, 3, true);

		//OPs
		for (int a = 0; a < 4; a++) {
			setATR(i, a, pgm_read_byte_near(initPatch + 12 + 11*i));
			setDec1R(i, a, pgm_read_byte_near(initPatch + 13 + 11*i));
			setDec2R(i, a, pgm_read_byte_near(initPatch + 14 + 11*i));
			setRel(i, a, pgm_read_byte_near(initPatch + 15 + 11*i));
			setDec1L(i, a, pgm_read_byte_near(initPatch + 16 + 11*i));
			setOpVolume(i, a, pgm_read_byte_near(initPatch + 17 + 11*i));
			setKSR(i, a, pgm_read_byte_near(initPatch + 18 + 11*i));
			setMul(i, a, pgm_read_byte_near(initPatch + 19 + 11*i));
			setDet1(i, a, pgm_read_byte_near(initPatch + 20 + 11*i));
			setDet2(i, a, pgm_read_byte_near(initPatch + 21 + 11*i));
			setAMSenseEn(i, a, pgm_read_byte_near(initPatch + 22 + 11*i));
		}
	}
}

void YM2151DriverClass::loadTestPatch() {
	Serial.print("loadTestPatch()\r\n\r\n");

	//LFO
	setLFOFreq(pgm_read_byte_near(testPatch));
	setAmpDepth(pgm_read_byte_near(testPatch + 1));
	setPhaseDepth(pgm_read_byte_near(testPatch + 2));
	setWaveForm(pgm_read_byte_near(testPatch + 3));
	setNoiseFreq(pgm_read_byte_near(testPatch + 4));
	setNoiseEnable(pgm_read_byte_near(testPatch + 11));


	for (int i = 0; i < 1; i++) {
		//CH
		setPan(i, pgm_read_byte_near(testPatch + 5));
		setFeedback(i, pgm_read_byte_near(testPatch + 6));
		setAlgorithm(i, pgm_read_byte_near(testPatch + 7));
		setAMSense(i, pgm_read_byte_near(testPatch + 8));
		setPMSense(i, pgm_read_byte_near(testPatch + 9));
		byte a = pgm_read_byte_near(testPatch + 10);
		setOpActive(i, 0, true);
		setOpActive(i, 2, true);
		setOpActive(i, 1, true);
		setOpActive(i, 3, true);

		//OPs
		for (int a = 0; a < 4; a++) {
			setATR(i, a, pgm_read_byte_near(testPatch + 12 + 11*i));
			setDec1R(i, a, pgm_read_byte_near(testPatch + 13 + 11*i));
			setDec2R(i, a, pgm_read_byte_near(testPatch + 14 + 11*i));
			setRel(i, a, pgm_read_byte_near(testPatch + 15 + 11*i));
			setDec1L(i, a, pgm_read_byte_near(testPatch + 16 + 11*i));
			setOpVolume(i, a, pgm_read_byte_near(testPatch + 17 + 11*i));
			setKSR(i, a, pgm_read_byte_near(testPatch + 18 + 11*i));
			setMul(i, a, pgm_read_byte_near(testPatch + 19 + 11*i));
			setDet1(i, a, pgm_read_byte_near(testPatch + 20 + 11*i));
			setDet2(i, a, pgm_read_byte_near(testPatch + 21 + 11*i));
			setAMSenseEn(i, a, pgm_read_byte_near(testPatch + 22 + 11*i));
		}
	}
}

uint8_t YM2151DriverClass::getAdr(uint8_t channel, uint8_t op){
	return (op * 8) + channel;
}



//Default Patch
PROGMEM const unsigned char initPatch[] = {
	0,   0,   0,   0,   0, //LFO
	0x03,   0,   0 ,  0,   0,  64 ,  0, //CH
	0x1F ,  0,   0,   0x04,  0 ,  0 ,  0,   0x01,   0,   0,   0, //OPs
	0x1F ,  0,   0,   0x04,  0 ,  0 ,  0,   0x01,   0,   0,   0, //OPs
	0x1F ,  0,   0,   0x04,  0 ,  0 ,  0,   0x01,   0,   0,   0, //OPs
	0x1F ,  0,   0,   0x04,  0 ,  0 ,  0,   0x01,   0,   0,   0 //OPs
};

/*
@:7 Instrument 7
LFO: 0 0 0 0 0
CH: 64 6 5 0 0 120 0
M1: 31 15 5 15 1 12 1 1 0 0 0
C1: 31 0 0 15 1 22 1 2 6 0 0
M2: 31 0 0 15 1 22 1 2 0 0 0
C2: 31 0 0 15 1 22 1 2 6 0 0
*/

PROGMEM const unsigned char testPatch[] = {
	0, 0, 0, 0, 0,
	64, 6, 5, 0, 0, 120, 0,
	31, 15, 5, 15, 1, 12, 1, 1, 0, 0, 0,
	31, 0, 0, 15, 1, 22, 1, 2, 6, 0, 0,
	31, 0, 0, 15, 1, 22, 1, 2, 0, 0, 0,
	31, 0, 0, 15, 1, 22, 1, 2, 6, 0, 0
};

YM2151DriverClass YM2151Driver;

