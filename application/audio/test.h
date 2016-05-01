#include "audio_cfg.h"

#ifdef AUDIO_TEST_ENABLE
#ifndef TEST_H
#define TEST_H

#include "def_type.h"

#define INT_S 2
#define INT_MS 1000
#define CTL 0

struct t_sys_info
{
    unsigned int test_1s_cnt;   /* T */
    unsigned int history_difference;    /* # */
    unsigned int task_status;   /* s */
};

struct t_talker_info
{   
    unsigned int ucos_tx;       /* r */
    unsigned int ucos_rx;       /* t */
    unsigned int enc_bytes;     /* E */
    unsigned int dec_bytes;     /* D */
    unsigned int jtbuf_uf_protect;  /* u */
    unsigned int jtbuf_drop_pkg;    /* d */
    unsigned int linear_uf_protect;    /* l */
    unsigned int wr_jtbuf;      /* W */
    unsigned int rd_jtbuf;      /* R */
};
#if 0
struct t_talker_info
{   
    unsigned int ucos_tx[MAX_TALKER_PER_CHANNEL];       /* t */
    unsigned int ucos_rx[MAX_TALKER_PER_CHANNEL];       /* r */
    unsigned int enc_bytes[MAX_TALKER_PER_CHANNEL];     /* e */
    unsigned int dec_bytes[MAX_TALKER_PER_CHANNEL];     /* d */
    unsigned int jtbuf_uf_protect[MAX_TALKER_PER_CHANNEL];  /* u */
    unsigned int jtbuf_drop_pkg[MAX_TALKER_PER_CHANNEL];    /* d */
    unsigned int linear_uf_protect[MAX_TALKER_PER_CHANNEL];    /* l */
    unsigned int wr_jtbuf[MAX_TALKER_PER_CHANNEL];      /* w */
    unsigned int rd_jtbuf[MAX_TALKER_PER_CHANNEL];      /* r */
};
#endif
//char MESSAGE[]="T#tredudwrt";

struct t_test_ctl
{
    unsigned int test_print_en;
    unsigned int test_print_ctl;
    unsigned int test_ms_cnt;
    unsigned int delay_cnt;
    unsigned int cnt_max;
    unsigned int cnt_s;
};

#define CMD_SAVE_MAX 50

struct t_cmd_buf
{
    unsigned int cmd_buf[CMD_SAVE_MAX];
    unsigned int cmd_rd_index;
    unsigned int cmd_wr_index;
};

extern struct t_sys_info sys_info;
extern struct t_talker_info talker_info[MAX_AUDIO_CHANNEL][MAX_TALKER_PER_CHANNEL];
extern struct t_cmd_buf cmd_buf;
extern struct t_test_ctl test_ctl;
inline void test_timer(void);
void test_ctl_clear(void);
void test_message_clear(void);
void test_message_clear_but_time(void);
void test_message_output(void);
void test_cmd_clear(void);
void test_print_cmd(void);
void test_print_all_cmd(void);
inline void test_save_cmd(unsigned int cmd);
void test_init(unsigned int int_s, unsigned int int_ms, unsigned int ctl);
void test_disable(void);
void test_enable(unsigned int ctl);
#endif
#endif

