//=================BY VINCENT 2007.08.30===========================

//#ifdef GPIO_TEST

#define GPIO_TEST_GLOBALS



#include "chip_cfg.h"



#include "risc.h"

#include "Sc.h"

#include "Mc.h"

#include "comm.h"

#include "includes.h"



#include "os_cpu.h"

#include "os_cfg.h"

#include "uCOS_II.h"



#include "bsp.h"



#include "GpioTest.h"



void TaskGpioTest(void *data)

{

    unsigned char err;

    unsigned int  msg = 0 ;   

    unsigned short position;

    unsigned short on_interval,off_interval;
    unsigned short on_tmp,off_tmp;


    on_interval = 0;

    off_interval = 10;       // 1 TIME TICK

    

       

    while(1) {

      msg = (unsigned int )OSQAccept(gptQMsgGpioTest); 

      if(msg !=0 ) {                 //can't not read address 0!!!

   

         position = msg & 0xff;

         on_tmp =( unsigned short) ((msg & 0xfff00) >> 8);        //unit :100ms

         off_tmp = ( unsigned short) ((msg & 0xfff00000) >> 20); //unit :100ms

         on_interval = on_tmp*OS_TICKS_PER_SEC/10;

         off_interval = off_tmp*OS_TICKS_PER_SEC/10;

         if((on_tmp != 0)&& (on_interval == 0) )
            on_interval = 1;
         if((off_tmp != 0)&& (off_interval == 0) )
            off_interval = 1;
        }

              

                 set_gpio_out(position) ;             //gpio is output.                                     

                 if(on_interval != 0)

                 set_gpio(position);

                 //printf("\n***********led on: %d time tick **********GPIO: %d ON**************\n",on_interval,position);

                 OSTimeDly(on_interval); 

                 if(off_interval != 0)

                 clr_gpio(position);

                 //printf("\n***********led off: %d time tick********GPIO: %d OFF**************\n",off_interval,position);

                 OSTimeDly(off_interval);        
                 if((on_interval==0)&&(off_interval==0))
                    OSTimeDly(1);   

        }

}

    

        

//#endif

//=================BY VINCENT 2007.08.30===========================



