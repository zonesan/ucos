#include "risc.h"
#include "aic23.h"
#include "comm.h"

void byp_onH_offL(unsigned int sel)
{
    unsigned short int aic23_cmd;
    aic23_cmd = (AAPC << 9)| (DAC)|(sel<<3)|(MICB);
    Send_aic23_cmd(aic23_cmd);
}

void aic_codec_delay(unsigned char delay_cnt)
{
    int i;
    for(i=0;i<delay_cnt*10;i++);
}

void AICInit(void)
{
    unsigned short int aic23_cmd;
    aic_codec_delay(5);
    
    printf("AICInit()\n");
    
    GIOOutput(DM_SCL);          //I2C CLK
    GIOOutput(DM_SDA);          //I2C DATA

    GIOBitSet(DM_SCL);          //I2C CLK
    GIOBitSet(DM_SDA);          //I2C DATA

    /* For DM270 */

    /* LLICVC Left Line Input Channel Volume Control (Address:0000000) */
    aic23_cmd = ((LLICVC << 9) | (LIV4 |LIV3 | LIV2 | LIV1 | LIV0)) ;
    Send_aic23_cmd(aic23_cmd) ;

    /* RLICVC Right Line Input Channel Volume Control (Address:0000001) */
    aic23_cmd = ((RLICVC << 9) | (RIV4 |RIV3 | RIV2 | RIV1 | RIV0));
    Send_aic23_cmd(aic23_cmd);

    /* LCHVC Left Channel Headphone Volume Control    (Address:0000010) */
    aic23_cmd = ((LCHVC << 9) | (LZC | LHV6 | LHV5 | LHV4 | LHV3 | LHV2 | LHV1 | LHV0));
    Send_aic23_cmd(aic23_cmd);

    /* RCHVC Right Channel Headphone Volume Control    (Address:0000011) */
    aic23_cmd = ((RCHVC << 9) | (RZC | RHV6 | RHV5 | RHV4 | RHV3 | RHV2 | RHV1 | RHV0));
    Send_aic23_cmd(aic23_cmd);

    /* AAPC Analog Audio Path Control                 (Address:0000100) */
    aic23_cmd = ((AAPC << 9) | (DAC | MICB | INSEL)) ;
    Send_aic23_cmd(aic23_cmd);
    
    /* Digital Audio Path Control                     (Address:0000101) */
    
    /* Power Down Control                             (Address:0000110) */
    aic23_cmd = (PDC << 9);
    Send_aic23_cmd(aic23_cmd) ;

    /* Digital Audio Interface Format                 (Address:0000111) */
    /* Initialization for EVM vs DMAB */
    aic23_cmd = ((DAIF << 9) | (FOR1 | FOR0)); //To set in slave/DSP/16Bit mode
    //aic23_cmd = ((DAIF << 9) | (MS | FOR1 | FOR0)); //To set in slave/DSP/16Bit mode
    Send_aic23_cmd(aic23_cmd);

    /* Sample Rate Control                            (Address:0001000) */
    
    /* Digital Interface Activation                   (Address:0001001) */
    aic23_cmd = ((DIA<<9) | ACT ) ;
    Send_aic23_cmd(aic23_cmd);

}

int Send_aic23_cmd(int cmd )
{
    unsigned char I2c_write_cmd;
    unsigned char nI2c_Data, ack_or_nak = 0;
    int i, j, ret;

    ret = I2C_ACK;
    aic_codec_delay(1);
    //I2C Start Condition
    aic_codec_data_low_output();        //start signal
    aic_codec_clk_low_output(); //clock start

    //send iic address
    I2c_write_cmd = AIC23_i2cDevAddr;
    I2c_write_cmd <<=1;
    //printf("iic addr = %x, data = %x\n", (cmd >> 9), (cmd & 0x1ff));
    for(i = 0; i<8; i++) {                                     //initial cmd start loop
        if ((I2c_write_cmd & 0x80) == 0x80 )
            aic_codec_data_high_output();
        else
            aic_codec_data_low_output();

        I2c_write_cmd = I2c_write_cmd << 1 ;
        aic_codec_delay(1);
        aic_codec_clk_high_output();
        aic_codec_clk_low_output();                          //clock low end
    }

    //============================================
    // wait ack bit
    //============================================
    GIOBitSet(DM_SDA);
    GIOInput(DM_SDA);
    aic_codec_delay(1);
    aic_codec_clk_high_output();
    // check here
    ack_or_nak = GIOGetBit(DM_SDA);

    if(ack_or_nak == 1) {
        printf("AIC I2C No ACK1\r\n");
        ret = I2C_NCK;
        goto end_proc;
    }

    aic_codec_clk_low_output();

    GIOOutput(DM_SDA);

    // Send Command
    for(i = 0; i<2; i++){                                    //address and data start
        if( i == 0)
            nI2c_Data = (cmd>>8);
        else if(i==1)
            nI2c_Data = (cmd&0xFF);

        for(j = 0; j<8; j++){                                 //address and data start
            if ((nI2c_Data & 0x80) == 0x80)
                aic_codec_data_high_output();
            else
                aic_codec_data_low_output();

            nI2c_Data = nI2c_Data << 1 ;
            aic_codec_delay(1);
            aic_codec_clk_high_output();
            aic_codec_clk_low_output();
        }

        //============================================
        // wait ack bit
        //============================================
        GIOBitSet(DM_SDA);
        GIOInput(DM_SDA);                    //SDA
        aic_codec_delay(1);

        aic_codec_clk_high_output();
        //check here
        ack_or_nak = GIOGetBit(DM_SDA);

        if(ack_or_nak == 1) {
            printf("AIC I2C No ACK2\r\n");
            ret = I2C_NCK;
            goto end_proc;
        }

        aic_codec_clk_low_output();

        GIOOutput(DM_SDA);
    }

end_proc:
    //I2C Stop Condition
    aic_codec_delay(1);
    aic_codec_clk_high_output();
    aic_codec_data_high_output();
    //GIOBitSet(AIC_CS);            //AIC23_CE
    aic_codec_delay(1);

    return ret;
}

int Send_aic23_cmd_test(unsigned char addr, unsigned int cmd)
{

    unsigned char I2c_write_cmd;
    unsigned char nI2c_Data, ack_or_nak = 0;
    int i, j, ret;

    ret = I2C_ACK;
    //GIOBitClear(AIC_CS);            //AIC23_CE
    aic_codec_delay(5);
    //I2C Start Condition
    aic_codec_data_low_output();                          //start signal
    aic_codec_clk_low_output();                              //clock start

    I2c_write_cmd = addr;
    printf("mad=%x;", I2c_write_cmd);
    I2c_write_cmd <<=1;

    for(i = 0; i<8; i++) {                                     //initial cmd start loop
        if ((I2c_write_cmd & 0x80) == 0x80 )
            aic_codec_data_high_output();
        else
            aic_codec_data_low_output();

        I2c_write_cmd = I2c_write_cmd << 1 ;
        aic_codec_delay(5);
        aic_codec_clk_high_output();
        aic_codec_clk_low_output();                          //clock low end
    }

    //============================================
    // wait ack bit
    //============================================
    GIOBitSet(DM_SDA);
    GIOInput(DM_SDA);
    aic_codec_delay(5);
    aic_codec_clk_high_output();
    // check here
    ack_or_nak = GIOGetBit(DM_SDA);

    if(ack_or_nak == 1) {
        printf("AIC I2C No ACK1\r\n");
        ret = I2C_NCK;
        goto end_proc;
    }

    aic_codec_clk_low_output();

    GIOOutput(DM_SDA);

    // Send Command
    for(i = 0; i<2; i++){                                    //address and data start

        if( i == 0)
        {
            nI2c_Data = (cmd>>8);
            printf("sad=%x;", nI2c_Data);
        }
        else if(i==1)
        {
            nI2c_Data = (cmd&0xFF);
            printf("da=%x\n", nI2c_Data);
        }
        for(j = 0; j<8; j++){                                 //address and data start
            if ((nI2c_Data & 0x80) == 0x80)
                aic_codec_data_high_output();
            else
                aic_codec_data_low_output();

            nI2c_Data = nI2c_Data << 1 ;
            aic_codec_delay(5);
            aic_codec_clk_high_output();
            aic_codec_clk_low_output();
        }

        //============================================
        // wait ack bit
        //============================================
        GIOBitSet(DM_SDA);
        GIOInput(DM_SDA);                    //SDA
        aic_codec_delay(5);

        aic_codec_clk_high_output();
        //check here
        ack_or_nak = GIOGetBit(DM_SDA);

        if(ack_or_nak == 1) {
            printf("AIC I2C No ACK2\r\n");
            ret = I2C_NCK;
            goto end_proc;
        }

        aic_codec_clk_low_output();

        GIOOutput(DM_SDA);
    }

end_proc:
    //I2C Stop Condition
    aic_codec_delay(5);
    aic_codec_clk_high_output();
    aic_codec_data_high_output();
    //GIOBitSet(AIC_CS);            //AIC23_CE
    aic_codec_delay(5);

    return 1;
}

void WAIT(void)
{
    int i ;
    for (i=0 ; i<25 ; i++ )
    {
    }
}

void ConfigureAIC(unsigned int Oscillator_freq,unsigned int Sampling_freq)
{
    /* DAPC Digital Audio Path Control                (Address:0000101) */
    /* Sample Rate Control                            (Address:0001000) */
    unsigned short int aic23_cmd;
    
    printf("ConfigureAIC()\n");
    
    switch(Oscillator_freq)
    {
    case 12288000:
        switch(Sampling_freq)
        {
        case 48000:
            aic23_cmd = ((DAPC << 9)| (DEEMP1) | (DEEMP0)) ;
            Send_aic23_cmd(aic23_cmd);
            aic23_cmd = (SRC<<9);
            Send_aic23_cmd(aic23_cmd) ;
            break;
        case 32000:
            aic23_cmd = ((DAPC << 9)| (DEEMP0));
            Send_aic23_cmd(aic23_cmd);
            aic23_cmd = ((SRC << 9) | (SR2SR )|(SR1SR)) ;
            Send_aic23_cmd(aic23_cmd);
            break;
        case 24000:
            aic23_cmd = ((DAPC << 9)| (DEEMP1) | (DEEMP0)) ;
            Send_aic23_cmd(aic23_cmd);
            aic23_cmd    = (SRC << 9);
            Send_aic23_cmd(aic23_cmd) ;
            break;
        case 16000:
            aic23_cmd = ((DAPC << 9)| (DEEMP0)) ;
            Send_aic23_cmd(aic23_cmd);
            aic23_cmd = ((SRC << 9) | (SR2SR )|(SR1SR)) ;
            Send_aic23_cmd(aic23_cmd) ;
            break;
        case 8000:
            aic23_cmd = ((DAPC << 9)| (DEEMP0)) ;
            Send_aic23_cmd(aic23_cmd);
            aic23_cmd    = ((SRC << 9) | (SR1SR )|(SR0SR)) ;
            Send_aic23_cmd(aic23_cmd) ;
            break;
        }

        break;
    case 11289600:
        switch(Sampling_freq)
        {
        case 44100:
            aic23_cmd = ((DAPC << 9)| (DEEMP1)) ;
            Send_aic23_cmd(aic23_cmd);
            aic23_cmd    = ((SRC <<9) |SR3SR);
            Send_aic23_cmd(aic23_cmd) ;
            break;
        case 22050:
            aic23_cmd = ((DAPC << 9)| (DEEMP1)) ;
            Send_aic23_cmd(aic23_cmd);
            aic23_cmd    = ((SRC <<9) |SR3SR);
            Send_aic23_cmd(aic23_cmd) ;
            break;
        }
        break;
    case 2048000:
        switch(Sampling_freq)
        {
        case 48000:
            aic23_cmd = ((DAPC << 9)| (DEEMP1) | (DEEMP0) ) ;
            Send_aic23_cmd(aic23_cmd);
            aic23_cmd    = ((SRC <<9) | (SR2SR )|(SR1SR)| (SR0SR));
            Send_aic23_cmd(aic23_cmd) ;
            break;
        case 32000:
            aic23_cmd = ((DAPC << 9)| (DEEMP0)) ;
            Send_aic23_cmd(aic23_cmd);
            aic23_cmd    = ((SRC <<9) | (SR2SR )|(SR1SR)| (SR0SR));
            Send_aic23_cmd(aic23_cmd);
            break;
        case 24000:
            aic23_cmd = ((DAPC << 9)| (DEEMP1) | (DEEMP0));
            Send_aic23_cmd(aic23_cmd);
            aic23_cmd    = ((SRC << 9) | (SR2SR )|(SR1SR)| (SR0SR));
            Send_aic23_cmd(aic23_cmd);
            break;
        case 16000:
            aic23_cmd = ((DAPC << 9)| (DEEMP1) | (DEEMP0) ) ;
            Send_aic23_cmd(aic23_cmd);
            aic23_cmd    = ((SRC <<9) | (SR2SR )|(SR1SR)| (SR0SR));
            Send_aic23_cmd(aic23_cmd) ;
            break;
        }
        break;

    case 12000000: // This applies to DMA PCB
        switch(Sampling_freq)
        {
        case 48000:
            aic23_cmd = ((DAPC << 9)| (DEEMP1) | (DEEMP0)) ;
            Send_aic23_cmd(aic23_cmd);
            aic23_cmd    =  (SRC<<9 | (MSSR));
            Send_aic23_cmd(aic23_cmd) ;
            break;
        case 32000:
            aic23_cmd = ((DAPC << 9)| (DEEMP0));
            Send_aic23_cmd(aic23_cmd);
            aic23_cmd    = ((SRC << 9) | (SR2SR )|(SR1SR)|(MSSR)) ;
            Send_aic23_cmd(aic23_cmd);
            break;
        case 24000:
            aic23_cmd = ((DAPC << 9)| (DEEMP1) | (DEEMP0)) ;
            Send_aic23_cmd(aic23_cmd);
            aic23_cmd    = (SRC << 9 |(CLKOUT) | (CLKIN) |(MSSR));
            Send_aic23_cmd(aic23_cmd) ;
            break;
        case 16000:
            aic23_cmd = ((DAPC << 9)| (DEEMP0)) ;
            Send_aic23_cmd(aic23_cmd);
            aic23_cmd    = ((SRC << 9) | (CLKOUT) | (CLKIN) | (SR2SR )|(SR1SR)|(MSSR)) ;
            Send_aic23_cmd(aic23_cmd) ;
            break;
        case 8000:
            printf("8k\n");
            aic23_cmd = ((DAPC << 9)|(DEEMP0)) ;
            Send_aic23_cmd(aic23_cmd);
            aic23_cmd    =  (SRC<<9 | ((MSSR) |(SR1SR) |(SR0SR)));
            //aic23_cmd    =  (SRC<<9 | (CLKOUT | MSSR | SR1SR | SR0SR));
            Send_aic23_cmd(aic23_cmd) ;
            break;
        case 44100:
            aic23_cmd = ((DAPC << 9)| (DEEMP1) ) ;
            Send_aic23_cmd(aic23_cmd);
            aic23_cmd    = ((SRC <<9) |(SR3SR) | (BOSR)|(MSSR));
            Send_aic23_cmd(aic23_cmd) ;
            break;
        case 22050:
            aic23_cmd = ((DAPC << 9)| (DEEMP1)) ;
            Send_aic23_cmd(aic23_cmd);
            aic23_cmd    = ((SRC <<9) |(CLKOUT) | (CLKIN) | (SR3SR) | (BOSR) |(MSSR));
            Send_aic23_cmd(aic23_cmd) ;
            break;
        }
    }
    //aic23_cmd[RESET] = RES;      //2003-12-22 jangwy
    //Send_aic23_cmd(aic23_cmd[RESET]);    //2003-12-22 jangwy
}

/* input 'vol' ranges from 1-15. 0=mute. rest=do not care. */
#define AIC23_VOL_MAX       0x7f
#define AIC23_VOL_MUTE      0x30

void aic23_set_volume(int volL, int volR)
{
    unsigned short int cmd, vol;

    /* LCHVC Left Channel Headphone Volume Control    (Address:0000010) */
    if (volL == 0)
    {
        cmd = AIC23_VOL_MUTE;
        cmd |= ((LCHVC << 9) |  (LZC) );
        Send_aic23_cmd(cmd);
    }
    else if (volL > 0 && volL < 16)
    {
        cmd = AIC23_VOL_MAX - 2*(15 - volL);
        cmd |= ((LCHVC << 9) |  (LZC) );
        Send_aic23_cmd(cmd);
    }

    /* RCHVC Right Channel Headphone Volume Control    (Address:0000011) */
    if (volR == 0)
    {
        cmd = AIC23_VOL_MUTE;
        cmd |= ((RCHVC << 9) |  (RZC) );
        Send_aic23_cmd(cmd);
    }
    else if (volL > 0 && volL < 16)
    {
        cmd = AIC23_VOL_MAX - 2*(15 - volR);
        cmd |= ((RCHVC << 9) |  (RZC) );
        Send_aic23_cmd(cmd);
    }
}



static void aic_codec_data_high_output(void)
{
    GIOOutput(DM_SDA);
    GIOBitSet(DM_SDA);
    aic_codec_delay(5);
}

static void aic_codec_data_low_output(void)
{
    GIOOutput(DM_SDA);
    GIOBitClear(DM_SDA);
    aic_codec_delay(5);
}

static void aic_codec_clk_high_output(void)
{
    GIOOutput(DM_SCL);
    GIOBitSet(DM_SCL);
    aic_codec_delay(5);
}


static void aic_codec_clk_low_output(void)
{
    GIOOutput(DM_SCL);
    GIOBitClear(DM_SCL);
    aic_codec_delay(5);
}

inline void GIOOutput(unsigned int bit)
{
    rRISC_GPIO_DIR &= ~(1 << bit);
}

inline void GIOInput(unsigned int bit)
{
    rRISC_GPIO_DIR |= (1 << bit);
}

inline void GIOBitSet(unsigned int bit)
{
    rRISC_GPIO |= (1 << bit);
}

inline void GIOBitClear(unsigned int bit)
{
    rRISC_GPIO &= ~(1 << bit);
}

inline unsigned int GIOGetBit(unsigned int bit)
{
    return (rRISC_GPIO & (1 << bit));
}
