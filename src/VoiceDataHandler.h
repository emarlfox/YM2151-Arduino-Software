#ifndef _VOICEDATA_h
#define _VOICEDATA_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define NUM_OPERATORS 4
/*
    // OPM operator parameters
    uint8_t attackRate;
    uint8_t decay1Rate;
    uint8_t decay2Rate;
    uint8_t releaseRate;
    uint8_t decay1Level;
    uint8_t keyboardScalingLevel;

    bool amplitudeModEnable;
    uint8_t egBiasSensitivity;
    uint8_t keyVelocity;
    uint8_t outputLevel;
    uint8_t oscFrequency;
    uint8_t keyboardScalingRate;
    uint8_t detune1;

    // OPZ operator parameters
    bool fixedFrequency;
    // 0 = 250 Hz, 7 = 32 kHz
    uint8_t fixedFrequencyRange;
*/

class VoiceDataHandlerClass
{
 public:
	void init();
    void handleVmem(uint8_t* data);
    void handleVced(uint8_t* data);
    void loadVmemBlock(uint8_t* data, uint8_t channel);
private:
    /*
    Operator[4] operators; // Ordering is 4 2 3 1

    bool lfoSync;
    uint8_t feedbackLevel;
    uint8_t algorithm;
    uint8_t lfoSpeed;
    uint8_t lfoDecay;
    uint8_t pitchModDepth;
    uint8_t amplitudeModDepth;
    uint8_t pitchModSensitivity;
    uint8_t amplitudeModSensitivity;
    uint8_t lfoWave;
    uint8_t transpose;
    uint8_t pitchBendRange;
    bool chorusSwitch;
    bool playMode;
    bool sustainFootSwitch;
    bool portamentFootSwitch;
    bool portamentoMode;
    uint8_t portamentoTime;
    uint8_t footVolume;
    uint8_t modulationWheelPitchRange;
    uint8_t modulationWheelAmplitudeRange;
    uint8_t breathControlPitchModRange;
    uint8_t breathControlAmplitudeRange;
    uint8_t breathControlPitchBiasRange;
    uint8_t breathControlEGBiasRange;

    char[10] voiceName;

    uint8_t pitchEGRate1;
    uint8_t pitchEGRate2;
    uint8_t pitchEGRate3;
    uint8_t pitchEGLevel1;
    uint8_t pitchEGLevel2;
    uint8_t pitchEGLevel3;
    */
};

// MUL + DT2 register values based on DX21 operator frequency ratio slots
static uint8_t const frequency_ratio_to_mul_dt2[64][2] =
{
    {0, 0}, {0, 1}, {0, 2}, {0, 3}, {1, 0}, {1, 1}, 
    {1, 2}, {1, 3}, {2, 0}, {2, 1}, {3, 0}, {2, 2}, 
    {2, 3}, {4, 0}, {3, 1}, {3, 2}, {5, 0}, {3, 3}, 
    {4, 1}, {6, 0}, {4, 2}, {4, 3}, {7, 0}, {5, 1}, 
    {5, 2}, {8, 0}, {6, 1}, {5, 3}, {9, 0}, {6, 2}, 
    {7, 1}, {10, 0}, {6, 3}, {7, 2}, {11, 0}, {8, 1}, 
    {12, 0}, {7, 3}, {8, 2}, {9, 1}, {13, 0}, {8, 3}, 
    {14, 0}, {10, 1}, {9, 2}, {15, 0}, {11, 1}, {9, 3}, 
    {10, 2}, {12, 1}, {11, 2}, {10, 3}, {13, 1}, {12, 2}, 
    {11, 3}, {14, 1}, {13, 2}, {12, 3}, {15, 1}, {14, 2}, 
    {13, 3}, {15, 2}, {14, 3}, {15, 3}
};

extern VoiceDataHandlerClass VoiceDataHandler;

#endif
