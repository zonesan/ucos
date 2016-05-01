/*
***************************************************************************
*                Copyright (c) 2005 vBridge Microsystem, Inc.  
*                      Unpublished & Not for Publication
*                            All Rights Reserved                   
*   
* File        :     clear.c                 
*                                                                         
* Description :     Initialize
*             
* Date        : 
*	                                                          
***************************************************************************
*/


#include  <includes.h>

#define DMAC_Init_value          0

#define SPRAM_Init_value         0

#define context_hi               0
#define context_lo               0

#define VIDEO_Init_value         0

#define SEC_Init_value           0

#define PPE_Init_value           0

#define LJump_Init_value         0

#define MC_SID_Map_Init_value    0



#define DMAC_CTXT_startAddr    0xa80800c0
#define DMAC_CTXT_endAddr      0xa80800fc

#define DMAC_BUF_startAddr     0xa8080200
#define DMAC_BUF_endAddr       0xa80803fc

#define SPR_startAddr          0xbfa00000
#define SPR_endAddr            0xbfa05000

#define VIDEO_startAddr        0xa80b0080
#define VIDEO_endAddr          0xa80b00bc

#define SEC_startAddr          0xa80e0000
#define SEC_endAddr            0xa8000080
#define SEC_DREG_startAddr     0xa8000100     
#define SEC_DREG_endAddr       0xa8000110

#define PPE_TAGram_startAddr   0xa8090200
#define PPE_TAGram_endAddr     0xa8090600

#define LJump_startAddr        0xa8023020
#define LJump_endAddr          0xa802303c

#define MC_SID_Map_startAddr   0xa8020600
#define MC_SID_Map_endAddr     0xa80206FC





/*
***************************************************************************
*                           DMAC_BUF_clean                                                           
* Description: clear DMAC_CTXT & CMAC_BUF 
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    :  
*                                                        
***************************************************************************
*/


void DMACbuffclear(void)
{
     int DMAC_i, DMAC_j; 
	
     for(DMAC_i = DMAC_CTXT_startAddr; DMAC_i <= DMAC_CTXT_endAddr; DMAC_i += 4) {
     	*(int *)DMAC_i = DMAC_Init_value;
     	}
     for(DMAC_j = DMAC_BUF_startAddr; DMAC_j <= DMAC_BUF_endAddr; DMAC_j += 4) {
     	*(int *)DMAC_j = DMAC_Init_value;
     	}
}




/*
***************************************************************************
*                           SPram_clean                                                           
* Description: clear SPram 
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    :  
*                                                        
***************************************************************************
*/


void SPRAMclear(void)
{
     int SPR_i; 
	
     for(SPR_i = SPR_startAddr; SPR_i <= SPR_endAddr; SPR_i += 4) {
     	*(int *)SPR_i = SPRAM_Init_value;
     	}
}




/*
***************************************************************************
*                           MC_clean                                                           
* Description: clear Memory Context 
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    :  
*                                                        
***************************************************************************
*/

void MCclear(void)
{
     int M_id; 
	
     for(M_id = 0; M_id <= 63; M_id ++) {
     	rMC_CTXT (M_id, W_CONTEXT_HI) = context_hi;
     	rMC_CTXT (M_id, W_CONTEXT_LO) = context_lo;
     	//*(int *)MC_i = MC_Init_value;
     	}
}

/*
***************************************************************************
*                           VIDEO_clean                                                           
* Description: clear VSC and ODE 
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    :  
*                                                        
***************************************************************************
*/

void VIDEOclear(void)
{
     int VIDEO_i; 
	
     for(VIDEO_i = VIDEO_startAddr; VIDEO_i <= VIDEO_endAddr; VIDEO_i += 4) {
     	*(int *)VIDEO_i = VIDEO_Init_value;
     	}
}

/*
***************************************************************************
*                           SEC_clean                                                           
* Description: clear SEC 
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    :  
*                                                        
***************************************************************************
*/

void SECclear(void)
{
     int SEC_i, SEC_j; 
	
     for(SEC_i = SEC_startAddr; SEC_i <= SEC_endAddr; SEC_i += 4) {
     	*(int *)SEC_i = SEC_Init_value;
     	}
     for(SEC_j = SEC_DREG_startAddr; SEC_j <= SEC_DREG_endAddr; SEC_j += 4) {
     	*(int *)SEC_j = SEC_Init_value;
     	}

}


/*
***************************************************************************
*                           PPE_clean                                                           
* Description: clear PPE 
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    :  
*                                                        
***************************************************************************
*/

void PPEclear(void)
{
     int PPE_i; 
     
     for(PPE_i = PPE_TAGram_startAddr; PPE_i <= PPE_TAGram_endAddr; PPE_i += 4) {
     	*(int *)PPE_i = PPE_Init_value;
     	}

}


/*
***************************************************************************
*                           Line Jump_clean                                                           
* Description: clear Line Jump 
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    :  
*                                                        
***************************************************************************
*/

void LJumpclear(void)
{
     int LJump_i; 
	
     for(LJump_i = LJump_startAddr; LJump_i <= LJump_endAddr; LJump_i += 4) {
     	*(int *)LJump_i = LJump_Init_value;
     	}
}

/*
***************************************************************************
*                          MC_SID_Map_clean                                                           
* Description: clear MC_SID_Map 
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    :  
*                                                        
***************************************************************************
*/

void MC_SID_Map_clear(void)
{
     int MC_SID_Map_i; 
	
     for(MC_SID_Map_i = MC_SID_Map_startAddr; MC_SID_Map_i <= MC_SID_Map_endAddr; MC_SID_Map_i += 4) {
     	*(int *)MC_SID_Map_i = MC_SID_Map_Init_value;
     	}
}

void Clr_UDI_xymemory(void)
{
    int i;
    	
    for(i = 0x9fff8000; i < 0x9fff8800; i +=4) {
        *(volatile unsigned int *)i = 0;	
    } 	
}

