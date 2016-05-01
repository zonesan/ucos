#include  "def_type.h"
#include  "BLMS_ec.h"

#define df 1				// Used only for Waleed's test vecotrs
#define ECHO_TAIL_LENGTH  64  // in ms, 10-512, was 512

BLMS_EC  *pBLMSEC;		// Stand Alone BLMS EC

void AppAudioBLMSEcInit();

void AppAudioBLMSEc(PSHORT pswLECNearSpeechInputData, PSHORT pswLECFarSpeechInputData, PBYTE pswLECOutputData);
//void AppAudioBLMSEc(Word16 * pswLECNearSpeechInputData, Word16 * pswLECFarSpeechInputData, unsigned char * pswLECOutputData);



