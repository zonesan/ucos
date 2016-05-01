/* vb_g729ab API functions prototypes and constants */
#ifndef G729_H
#define G729_H

#define  L_FRAME_COMPRESSED 10  // 10 // HACK!!! 10  162
#define  L_FRAME_SID		2
#define  L_FRAME            80
#define  G729A		0
#define  G729AB		1
#ifdef __cplusplus 
extern "C" {
#endif

//VOID AppAudioG729EncodeInit();
VOID AppAudioG729EncodeInit(short swChannelNum, short swTalkNum);
LONG  AppAudioG729Encode(short swChannelNum, short swTalkNum, PSHORT  pswSpeechInBuf, PBYTE pcwEncoderSpeechOutBuf, SHORT swG729abSwap);
//LONG  AppAudioG729Encode(PSHORT  pswSpeechInBuf, PBYTE pcwEncoderSpeechOutBuf, SHORT swG729abSwap);
VOID AppAudioG729DecodeInit(short swChannelNum, short swTalkNum);
LONG AppAudioG729Decode(short swChannelNum, short swTalkNum, PBYTE  pcwDecodeSpeechInBuf, PSHORT  pswSpeechOutBuf, LONG DecodeFrameLen);

#ifdef __cplusplus
}
#endif
#endif
