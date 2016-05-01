#include <chip_cfg.h>
#include <audio_cfg.h>

#ifdef AUDIO_TEST_ENABLE
#include "test.h"
#include "comm.h"
#include "../port/os_cpu.h"
#include "../uCOS Configure/os_cfg.h"
#include "../uCOS2.52/uCOS_II.h"
#include "two_risc_audio.h"
#include "../lib/libc_s.h"

struct t_sys_info sys_info;
struct t_talker_info talker_info[MAX_AUDIO_CHANNEL][MAX_TALKER_PER_CHANNEL];
struct t_cmd_buf cmd_buf;
struct t_test_ctl test_ctl;

const char test_sys_info_msg[] = "T#s";
const char test_talker_info_msg[] = "trEDudlWR";

//char MESSAGE[]="T#tredudlwrt";

inline void test_timer(void)
{
    test_ctl.test_ms_cnt++;
    if(test_ctl.test_ms_cnt == test_ctl.cnt_s)
    {
        test_ctl.test_ms_cnt = 0;
        test_ctl.delay_cnt++;
        sys_info.test_1s_cnt++;
    }
}

void test_ctl_clear(void)
{
    int i;
    int tmp;
    unsigned int *p;
    p = (unsigned int *)&test_ctl;
    tmp = sizeof(test_ctl)/sizeof(unsigned int);
    for(i=0; i<tmp; i++)
        *(p + i) = 0;
}

void test_message_clear(void)
{
    memset(&sys_info, 0, sizeof(sys_info));
    memset(&talker_info, 0, sizeof(talker_info));
}

void test_message_output(void)
{
    int i;
    int j;
    int k;
    unsigned int *p;
    extern tAudioCharacter channelAC[MAX_AUDIO_CHANNEL];

    p = (unsigned int *)&sys_info;
    if(!test_ctl.test_print_en)
        return;
    if(test_ctl.delay_cnt < test_ctl.cnt_max)
        return;

    for(i = 0; i < sizeof(sys_info)/sizeof(int); i++)
    {
	    if (*(p+i) != 0)
    	    printc("%c%d ", test_sys_info_msg[i], *(p+i));
    }

    for(i = 0; i < MAX_AUDIO_CHANNEL; i++)
    {
	    if (!channelAC[i].bChannelEnable)
	    {
	    continue;
	    }
	    
	    printc("=%d", i);
	    for (j = 0; j < MAX_TALKER_PER_CHANNEL; j++)
	    {
	        if (((channelAC[i].lwTalkerEnableBitFlag >> j) & 0x1) == 0)
	        {
		        continue;
	        }

	        printc("-%d", j);
	        p = (unsigned int *)&talker_info[i][j];
	        for (k = 0; k < (sizeof(struct t_talker_info)/sizeof(int)); k++)
	        {
		        if (*(p+k) != 0)
		            printc(" %c%d", test_talker_info_msg[k], *(p+k));
	        }
	    }
    }

    printc("\n");
    test_ctl.test_print_en = test_ctl.test_print_ctl;
    test_ctl.delay_cnt = 0;
}

void test_cmd_clear(void)
{
    memset(&cmd_buf, 0, sizeof(cmd_buf));
}

void test_print_cmd(void)
{
    while(cmd_buf.cmd_rd_index != cmd_buf.cmd_wr_index)
    {
        if(cmd_buf.cmd_rd_index >= CMD_SAVE_MAX)
            cmd_buf.cmd_rd_index = 0;
        printc("<%x\n", cmd_buf.cmd_buf[cmd_buf.cmd_rd_index]);
        cmd_buf.cmd_rd_index++;
    }
}

void test_print_all_cmd(void)
{
    int i;
    debug("cmd: ");
    for (i = 0; i < CMD_SAVE_MAX; i++)
    {
	    debug("%x ", cmd_buf.cmd_buf[i]);
    }
    
    debug("rdi=%d ", cmd_buf.cmd_rd_index);
    debug("wdi=%d ", cmd_buf.cmd_wr_index);
}

inline void test_save_cmd(unsigned int cmd)
{
    if(cmd_buf.cmd_wr_index >= CMD_SAVE_MAX)
        cmd_buf.cmd_wr_index = 0;
    cmd_buf.cmd_buf[cmd_buf.cmd_wr_index] = cmd;
    cmd_buf.cmd_wr_index++;
}

void test_init(unsigned int int_s, unsigned int int_ms, unsigned int ctl)
{
    test_message_clear();
    test_ctl_clear();
    test_ctl.cnt_s = 1000/int_ms;
    test_ctl.cnt_max = int_s;
    test_ctl.test_print_ctl = !ctl;
}

void test_disable(void)
{
    test_ctl.test_print_ctl = 0;
    test_ctl.test_print_en = 0;
}

void test_enable(unsigned int ctl)
{
    test_ctl.test_print_ctl = !ctl;
    test_ctl.test_print_en = 1;
}
#endif

