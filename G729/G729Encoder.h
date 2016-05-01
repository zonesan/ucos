/*
*********************************************************************************************************
*                                Copyright (c) 2005 VBridge Microsystem, Inc.  
*                                      Unpublished & Not for Publication
*                                            All Rights Reserved                   
*   
* File:         UDIInstruction.h             
*                                                                         
* Description :       
*                                                
* By:           WuGang               
*
* Date:         Mar 6, 2006                                                           
**********************************************************************************************************
*/


//void G729Encoder(char *Inspeech, int N, char *outbit);
void G729Encoder(char *Inspeech, int N, char *outbit, char *decodeoutput  );
void G729Decoder(char  *f_serial, int NNN, char *Dec_OutBit);

