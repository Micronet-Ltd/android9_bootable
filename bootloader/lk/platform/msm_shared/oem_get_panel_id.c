/*
 * This file for oem_platform get real panel chip id.
 */
#include <stdlib.h>
#include <debug.h>
#include <err.h>
#include <platform/gpio.h>
#include <platform/timer.h>
#include <target/display.h>

#define PANEL_RESET_IO 61

uint32_t current_id = UNKNOWN_ID;
uint8_t get_id_flag = 0;

extern int mdss_dsi_cmds_tx(struct mipi_panel_info *mipi,
	struct mipi_dsi_cmd *cmds, int count, char dual_dsi);
extern int mipi_dsi_cmds_rx(char **rp, int len);

/* otm1284 read chip_id */
static char otm1284_set_pkt_size[] = {
	0x06,0x00,0x37,0x80};
static struct mipi_dsi_cmd otm1284_set_pkt_size_cmd[] = {
	{sizeof(otm1284_set_pkt_size), otm1284_set_pkt_size,0x0},
};
static char otm1284_panel_manufacture_id[] = {
	0xA1,0x00,0x06,0xA0};
static struct mipi_dsi_cmd otm1284_panel_manufacture_id_cmd[] = {
	{sizeof(otm1284_panel_manufacture_id), otm1284_panel_manufacture_id,0x0},
};

/* nt35512s get chip id select */
static char nt35512s_select_page2_cmds_01[] = {
	0x06, 0x00, 0x39, 0xC0,
	0xF0, 0x55, 0xAA, 0x52,
	0x08, 0x02, 0xFF, 0xFF
};
static char nt35512s_select_page2_cmds_02[] = {
	0x02, 0x00, 0x39, 0xC0,
	0xea, 0x02, 0xFF, 0xFF
};
static char nt35512s_select_page1_cmds_01[] = {
	0x06, 0x00, 0x39, 0xC0,
	0xF0, 0x55, 0xAA, 0x52,
	0x08, 0x01, 0xFF, 0xFF
};
static char nt35512s_panel_manufacture_id[] = {
	0xC5, 0x00, 0x24, 0xA0
};
static struct mipi_dsi_cmd nt35512s_panel_enable_page2_cmd[] = {
	{sizeof(nt35512s_select_page2_cmds_01), nt35512s_select_page2_cmds_01,0x0},
	{sizeof(nt35512s_select_page2_cmds_02), nt35512s_select_page2_cmds_02,0x0},
};
static struct mipi_dsi_cmd nt35512s_panel_enable_page1_cmd[] = {
	{sizeof(nt35512s_select_page1_cmds_01), nt35512s_select_page1_cmds_01,0x0}
};
static struct mipi_dsi_cmd nt35512s_panel_manufacture_id_cmd[] = {
	{sizeof(nt35512s_panel_manufacture_id), nt35512s_panel_manufacture_id,0x0},
};

/* jd9366 get chip id select */
static char jd9366_select_page2_cmds_01[] = {
	0x06, 0x00, 0x39, 0xC0,
	0xF0, 0x55, 0xAA, 0x52,
	0x08, 0x02, 0xFF, 0xFF
};
static char jd9366_select_page2_cmds_02[] = {
	0x02, 0x00, 0x39, 0xC0,
	0xea, 0x02, 0xFF, 0xFF
};
static char jd9366_select_page1_cmds_01[] = {
	0x06, 0x00, 0x39, 0xC0,
	0xF0, 0x55, 0xAA, 0x52,
	0x08, 0x01, 0xFF, 0xFF
};
static char jd9366_panel_manufacture_id[] = {
	0xDC, 0x00, 0x24, 0xA0
};
static struct mipi_dsi_cmd jd9366_panel_enable_page2_cmd[] = {
	{sizeof(jd9366_select_page2_cmds_01), jd9366_select_page2_cmds_01,0x0},
	{sizeof(jd9366_select_page2_cmds_02), jd9366_select_page2_cmds_02,0x0},
};
static struct mipi_dsi_cmd jd9366_panel_enable_page1_cmd[] = {
	{sizeof(jd9366_select_page1_cmds_01), jd9366_select_page1_cmds_01,0x0}
};
static struct mipi_dsi_cmd jd9366_panel_manufacture_id_cmd[] = {
	{sizeof(jd9366_panel_manufacture_id), jd9366_panel_manufacture_id,0x0},
};

/* NT35590 read chip_id */
static char nt35590_panel_set_pkt_size[] = {
        0x06, 0x00, 0x37, 0x80};
static struct mipi_dsi_cmd nt35590_panel_set_pkt_size_cmd[] = {
        {sizeof(nt35590_panel_set_pkt_size), nt35590_panel_set_pkt_size,0x0},
};
static char nt35590_panel_manufacture_id[] = {
        0xF4, 0x00, 0x24, 0xA0};
static struct mipi_dsi_cmd nt35590_panel_manufacture_id_cmd[] = {
        {sizeof(nt35590_panel_manufacture_id), nt35590_panel_manufacture_id,0x0},
};


/* NT35596 read chip_id*/
static char nt35596_select_page1_cmd[] = {
	0x02, 0x00, 0x39, 0xC0,
	0xFF, 0x00, 0xFF, 0xFF};
static struct mipi_dsi_cmd nt35596_panel_enable_page1_cmd[] = {
        {sizeof(nt35596_select_page1_cmd), nt35596_select_page1_cmd,0x0},
};
static char nt35596_panel_set_pkt_size[] = {
	0x04, 0x00, 0x39, 0xC0,
	0x06, 0x00, 0x37, 0x80};
static struct mipi_dsi_cmd nt35596_panel_set_pkt_size_cmd[] = {
        {sizeof(nt35596_panel_set_pkt_size), nt35596_panel_set_pkt_size,0x0},
};
static char nt35596_panel_manufacture_id[] = {
	0xF4, 0x00, 0x24, 0xA0};
static struct mipi_dsi_cmd nt35596_panel_manufacture_id_cmd[] = {
        {sizeof(nt35596_panel_manufacture_id), nt35596_panel_manufacture_id,0x0},
};

/* TD4310 read chip_id*/
static char td4310_select_page1_cmd[] = {
	0x02, 0x00, 0x39, 0xC0,
	0xFF, 0x00, 0xFF, 0xFF};
static struct mipi_dsi_cmd td4310_panel_enable_page1_cmd[] = {
        {sizeof(td4310_select_page1_cmd), td4310_select_page1_cmd,0x0},
};
static char td4310_panel_set_pkt_size[] = {
	0x04, 0x00, 0x39, 0xC0,
	0x06, 0x00, 0x37, 0x80};
static struct mipi_dsi_cmd td4310_panel_set_pkt_size_cmd[] = {
        {sizeof(td4310_panel_set_pkt_size), td4310_panel_set_pkt_size,0x0},
};
static char td4310_panel_manufacture_id[] = {
	0xA0, 0x00, 0x24, 0xA0};
static struct mipi_dsi_cmd td4310_panel_manufacture_id_cmd[] = {
        {sizeof(td4310_panel_manufacture_id), td4310_panel_manufacture_id,0x0},
};

/* HX83112 read chip_id*/
static char hx83112_select_page1_cmd[] = {
	0x02, 0x00, 0x39, 0xC0,
	0xFF, 0x00, 0xFF, 0xFF};
static struct mipi_dsi_cmd hx83112_panel_enable_page1_cmd[] = {
        {sizeof(hx83112_select_page1_cmd), hx83112_select_page1_cmd,0x0},
};
static char hx83112_panel_set_pkt_size[] = {
	0x04, 0x00, 0x39, 0xC0,
	0x06, 0x00, 0x37, 0x80};
static struct mipi_dsi_cmd hx83112_panel_set_pkt_size_cmd[] = {
        {sizeof(hx83112_panel_set_pkt_size), hx83112_panel_set_pkt_size,0x0},
};
static char hx83112_panel_manufacture_id[] = {
	0xA0, 0x00, 0x24, 0xA0};
static struct mipi_dsi_cmd hx83112_panel_manufacture_id_cmd[] = {
        {sizeof(hx83112_panel_manufacture_id), hx83112_panel_manufacture_id,0x0},
};

/* FT8719 read chip_id*/
static char ft8719_select_page1_cmd[] = {
	0x02, 0x00, 0x39, 0xC0,
	0xFF, 0x00, 0xFF, 0xFF};
static struct mipi_dsi_cmd ft8719_panel_enable_page1_cmd[] = {
        {sizeof(ft8719_select_page1_cmd), ft8719_select_page1_cmd,0x0},
};
static char ft8719_panel_set_pkt_size[] = {
	0x04, 0x00, 0x39, 0xC0,
	0x06, 0x00, 0x37, 0x80};
static struct mipi_dsi_cmd ft8719_panel_set_pkt_size_cmd[] = {
        {sizeof(ft8719_panel_set_pkt_size), ft8719_panel_set_pkt_size,0x0},
};
static char ft8719_panel_manufacture_id[] = {
	0xDA, 0x00, 0x24, 0xA0};
static struct mipi_dsi_cmd ft8719_panel_manufacture_id_cmd[] = {
        {sizeof(ft8719_panel_manufacture_id), ft8719_panel_manufacture_id,0x0},
};

/* OTM8018B read chip_id */
static char otm8018b_panel_manufacture_id[4] = {0xDA, 0x00, 0x06, 0xA0};
static struct mipi_dsi_cmd otm8018b_panel_manufacture_id_cmd[] = {
	{ 0x4, otm8018b_panel_manufacture_id,0x0},
};

/* ili9806c read chip_id */
static char ili9806c_set_enable_register[]  = {
        0x04,0x00,0x29,0xC0,
        0xFF,0xFF,0x98,0x16};
static char ili9806c_panel_manufacture_id[] = {
        0xD3,0x00,0x04,0xA0};
static struct mipi_dsi_cmd ili9806c_panel_manufacture_id_cmd[] = {
        { 0x8, ili9806c_set_enable_register,0x0},
        { 0x4, ili9806c_panel_manufacture_id,0x0},
};

/* ili9806e read chip_id */
static char ili9806e_set_page1_cmd1[] = {
	0x02,0x00,0x29,0xC0,
	0x00,0x00,0xff,0xff};
static char ili9806e_set_page1_cmd2[] = {
	0x06,0x00,0x29,0xC0,
	0xFF,0xFF,0x98,0x06,
	0x04,0x01,0xFF,0xFF};
static char ili9806c_set_pkt_size1[] = {
	0x06,0x00,0x37,0x80};
static char ili9806e_panel_manufacture_id1[] = {
	0x00,0x00,0x06,0xA0};
static struct mipi_dsi_cmd ili9806e_panel_manufacture_id1_cmd[] = {
	{sizeof(ili9806e_set_page1_cmd1), ili9806e_set_page1_cmd1,0x0},
	{sizeof(ili9806e_set_page1_cmd2), ili9806e_set_page1_cmd2,0x0},
	{sizeof(ili9806c_set_pkt_size1), ili9806c_set_pkt_size1,0x0},
	{sizeof(ili9806e_panel_manufacture_id1), ili9806e_panel_manufacture_id1,0x0},
};
static char ili9806e_set_page1_cmd3[] = {
	0x02,0x00,0x29,0xC0,
	0x00,0x00,0xff,0xff};
static char ili9806e_set_page1_cmd4[] = {
	0x06,0x00,0x29,0xC0,
	0xFF,0xFF,0x98,0x06,
	0x04,0x01,0xFF,0xFF};
static char ili9806c_set_pkt_size2[] = {
	0x06,0x00,0x37,0x80};
static char ili9806e_panel_manufacture_id2[] = {
	0x01,0x00,0x06,0xA0};
static struct mipi_dsi_cmd ili9806e_panel_manufacture_id2_cmd[] = {
	{sizeof(ili9806e_set_page1_cmd3), ili9806e_set_page1_cmd3,0x0},
	{sizeof(ili9806e_set_page1_cmd4), ili9806e_set_page1_cmd4,0x0},
	{sizeof(ili9806c_set_pkt_size2), ili9806c_set_pkt_size2,0x0},
	{sizeof(ili9806e_panel_manufacture_id2), ili9806e_panel_manufacture_id2,0x0},
};

/*himax 8389b*/
static char hx8389b_id_cmd1[] = {
	0x04, 0x00, 0x39, 0xc0,
	0xB9, 0xFF, 0x83, 0x89
};
static char hx_panel_manufacture_id[] = {0xF4, 0x00, 0x06, 0xA0};
static struct mipi_dsi_cmd hx8389b_panel_manufacture_id_cmd[] = {
	{sizeof(hx8389b_id_cmd1), hx8389b_id_cmd1,0x0},
	{sizeof(hx_panel_manufacture_id), hx_panel_manufacture_id,0x0},
};

static char def_panel_manufacture_id[4] = {0xDA, 0x00, 0x06, 0xA0};
static struct mipi_dsi_cmd def_panel_manufacture_id_cmd[] = {
	{ 0x4, def_panel_manufacture_id,0x0},
};

/*otm9605a read chip_id */
static char otm9605a_set_pkt_size[] = {
	0x06, 0x00, 0x37, 0x80
};
static struct mipi_dsi_cmd otm9605a_panel_set_pkt_size_cmd[] = {
	{sizeof(otm9605a_set_pkt_size), otm9605a_set_pkt_size,0x0},
};

static char otm9605a_panel_manufacture_id[] = {
	0xA1, 0x00, 0x06, 0xA0
};
static struct mipi_dsi_cmd otm9605a_panel_manufacture_id_cmd[] = {
	{sizeof(otm9605a_panel_manufacture_id), otm9605a_panel_manufacture_id,0x0},
};
/* rm68171 get chip id select */
static char rm68171_select_page2_cmds_01[] = {
	0x06, 0x00, 0x39, 0xC0,
	0xF0, 0x55, 0xAA, 0x52,
	0x08, 0x02, 0xFF, 0xFF
};
static char rm68171_select_page2_cmds_02[] = {
	0x02, 0x00, 0x39, 0xC0,
	0xea, 0x02, 0xFF, 0xFF
};
static char rm68171_select_page1_cmds_01[] = {
	0x06, 0x00, 0x39, 0xC0,
	0xF0, 0x55, 0xAA, 0x52,
	0x08, 0x01, 0xFF, 0xFF
};
static char rm68171_panel_manufacture_id[] = {
	0xC5, 0x00, 0x24, 0xA0
};
static struct mipi_dsi_cmd rm68171_panel_enable_page2_cmd[] = {
	{sizeof(rm68171_select_page2_cmds_01), rm68171_select_page2_cmds_01,0x0},
	{sizeof(rm68171_select_page2_cmds_02), rm68171_select_page2_cmds_02,0x0},
};
static struct mipi_dsi_cmd rm68171_panel_enable_page1_cmd[] = {
	{sizeof(rm68171_select_page1_cmds_01), rm68171_select_page1_cmds_01,0x0}
};
static struct mipi_dsi_cmd rm68171_panel_manufacture_id_cmd[] = {
	{sizeof(rm68171_panel_manufacture_id), rm68171_panel_manufacture_id,0x0},
};

/* rm68172 get chip id select */
static char rm68172_select_page1_cmds_01[] = {
	0x06, 0x00, 0x39, 0xC0,
	0xF0, 0x55, 0xAA, 0x52,
	0x08, 0x01, 0xFF, 0xFF
};
static char rm68172_panel_manufacture_id[] = {
	0xC5, 0x00, 0x24, 0xA0
};
static struct mipi_dsi_cmd rm68172_panel_manufacture_id_cmd[] = {
	{sizeof(rm68172_select_page1_cmds_01), rm68172_select_page1_cmds_01,0x0},
	{sizeof(rm68172_panel_manufacture_id), rm68172_panel_manufacture_id,0x0},
};

/* ST7701 read chip_id*/
static char ST7701_select_page1_cmds_01[] = {
	0x06, 0x00, 0x39, 0xC0,
	0xF0, 0x55, 0xAA, 0x52,
	0x08, 0x01, 0xFF, 0xFF
};
static char ST7701_panel_manufacture_id[] = {
	0xA1, 0x00, 0x24, 0xA0
};
static struct mipi_dsi_cmd ST7701_panel_manufacture_id_cmd[] = {
	{sizeof(ST7701_select_page1_cmds_01), ST7701_select_page1_cmds_01,0x0},
	{sizeof(ST7701_panel_manufacture_id), ST7701_panel_manufacture_id,0x0},
};

/* sh1282 read chip_id*/
static char sh1282_select_page1_cmd[] = {
	0x06,0x00,0x29,0xC0,
	0xF0,0x55,0xAA,0x52,
	0x08,0x01,0xFF,0xFF};
static struct mipi_dsi_cmd sh1282_panel_enable_page1_cmd[] = {
	{sizeof(sh1282_select_page1_cmd), sh1282_select_page1_cmd,0x0},
};
static char sh1282_panel_set_pkt_size[] = {
	0x06,0x00,0x37,0x80};
static struct mipi_dsi_cmd sh1282_panel_set_pkt_size_cmd[] = {
	{sizeof(sh1282_panel_set_pkt_size), sh1282_panel_set_pkt_size,0x0},
};
static char sh1282_panel_manufacture_id[] = {
	0xC6, 0x00, 0x24, 0xA0};
static struct mipi_dsi_cmd sh1282_panel_manufacture_id_cmd[] = {
	{sizeof(sh1282_panel_manufacture_id), sh1282_panel_manufacture_id,0x0},
};

/* OTM8019 read chip_id */
static char otm8019_set_pkt_size[] = {
	0x06,0x00,0x37,0x80};
static struct mipi_dsi_cmd otm8019_set_pkt_size_cmd[] = {
	{sizeof(otm8019_set_pkt_size), otm8019_set_pkt_size,0x0},
};
static char otm8019_panel_manufacture_id[] = {
	0xA1,0x00,0x06,0xA0};
static struct mipi_dsi_cmd otm8019_panel_manufacture_id_cmd[] = {
	{sizeof(otm8019_panel_manufacture_id), otm8019_panel_manufacture_id,0x0},
};

/* OTM1901 read chip_id */
static char otm1901_set_pkt_size[] = {
	0x06,0x00,0x37,0x80};
static struct mipi_dsi_cmd otm1901_set_pkt_size_cmd[] = {
	{sizeof(otm1901_set_pkt_size), otm1901_set_pkt_size,0x0},
};
static char otm1901_panel_manufacture_id[] = {
	0xA1,0x00,0x06,0xA0};
static struct mipi_dsi_cmd otm1901_panel_manufacture_id_cmd[] = {
	{sizeof(otm1901_panel_manufacture_id), otm1901_panel_manufacture_id,0x0},
};

/* ili9881 read chip_id */
static char ili9881_set_page1_cmd1[] = {
	0x04,0x00,0x29,0xC0,
	0xFF,0x98,0x81,0x01,
};
static char ili9881_set_pkt_size1[] = {
	0x06,0x00,0x37,0x80};
static char ili9881_panel_manufacture_id1[] = {
	0x00,0x00,0x06,0xA0};
static struct mipi_dsi_cmd ili9881_panel_manufacture_id1_cmd[] = {
	{sizeof(ili9881_set_page1_cmd1), ili9881_set_page1_cmd1,0x0},
	{sizeof(ili9881_set_pkt_size1), ili9881_set_pkt_size1,0x0},
	{sizeof(ili9881_panel_manufacture_id1), ili9881_panel_manufacture_id1,0x0},
};

/* r61318 get chip id select */
static char r61318_access_proctect_cmd1[] = {
	0x02, 0x00, 0x29, 0xC0,
	0xB0, 0x00, 0xFF, 0xFF
};
static char r61318_set_pkt_size1[] = {
	0x04,0x00,0x37,0x80};
static char r61318_panel_manufacture_id[] = {
	0xBF,0x00,0x14,0xA0};
static struct mipi_dsi_cmd r61318_panel_access_proctect_cmd[] = {
	{sizeof(r61318_access_proctect_cmd1), r61318_access_proctect_cmd1,0x0},
};
static struct mipi_dsi_cmd r61318_set_pkt_size_cmd[] = {
	{sizeof(r61318_set_pkt_size1), r61318_set_pkt_size1,0x0}
};
static struct mipi_dsi_cmd r61318_panel_manufacture_id_cmd[] = {
	{sizeof(r61318_panel_manufacture_id), r61318_panel_manufacture_id,0x0},
};
/* r63315 get chip id select */
static char r63315_access_proctect_cmd1[] = {
	0x02, 0x00, 0x29, 0xC0,
	0xB0, 0x00, 0xFF, 0xFF
};
static char r63315_set_pkt_size1[] = {
	0x04,0x00,0x37,0x80};
static char r63315_panel_manufacture_id[] = {
	0xBF,0x00,0x14,0xA0};
static struct mipi_dsi_cmd r63315_panel_access_proctect_cmd[] = {
	{sizeof(r63315_access_proctect_cmd1), r63315_access_proctect_cmd1,0x0},
};
static struct mipi_dsi_cmd r63315_set_pkt_size_cmd[] = {
	{sizeof(r63315_set_pkt_size1), r63315_set_pkt_size1,0x0}
};
static struct mipi_dsi_cmd r63315_panel_manufacture_id_cmd[] = {
	{sizeof(r63315_panel_manufacture_id), r63315_panel_manufacture_id,0x0},
};

static struct gpio_pin reset_gpio = {
  "msmgpio", 61, 3, 1, 0, 1
};

static void panel_reset_gpio(unsigned int t)
{
	dprintf(INFO, "panel_reset_gpio()\n");

	gpio_set_dir(reset_gpio.pin_id, 2);
	mdelay(10);
	gpio_set_dir(reset_gpio.pin_id, 0);
	mdelay(10);
	gpio_set_dir(reset_gpio.pin_id, 2);
	mdelay(t);
}
#if (A406) || (A406_F16)
   static void panel_reset2_gpio(int enable)
   {
       dprintf(INFO, "panel_reset2_gpio()\n"); 
       if(enable){
      				gpio_set_dir(reset_gpio2.pin_id, 2);
       			mdelay(10);
       			gpio_set_dir(reset_gpio2.pin_id, 0);
       			mdelay(10);
       			gpio_set_dir(reset_gpio2.pin_id, 2);
       			mdelay(50);
       }
       else{
       			gpio_set_dir(reset_gpio2.pin_id, 0);
       }
   }
#endif

uint32_t mipi_otm1901_manufacture_id(void)
{
	char rec_buf[24];
	char *rp = rec_buf;
	uint32_t *lp, data, data1, data2;
	//panel_reset_gpio(20);

	mdss_dsi_cmds_tx(NULL,otm1901_set_pkt_size_cmd, ARRAY_SIZE(otm1901_set_pkt_size_cmd),0);
	mdelay(10);
	mdss_dsi_cmds_tx(NULL,otm1901_panel_manufacture_id_cmd, ARRAY_SIZE(otm1901_panel_manufacture_id_cmd),0);
	mdelay(10);
	mipi_dsi_cmds_rx(&rp, 3);
	lp = (uint32_t *)(rp);
	data1 = (uint32_t)(*lp);
	data1 = ntohl(data1);
	data1 = data1 & 0xff;

	lp = (uint32_t *)(rp + 4);
	data2 = (uint32_t)(*lp);
	data2 = data2 & 0xff;

	data = (data1 << 8) | data2;

	dprintf(INFO, "mipi otm1901_lcd manufacture id =0x %x, data1 = 0x %x, data2 = 0x %x \n",data, data1, data2);

	return ((data == OTM1901_ID) ? OTM1901_ID : 0);
}
uint32_t mipi_otm1284_manufacture_id(void)
{
	char rec_buf[24];
	char *rp = rec_buf;
	uint32_t *lp, data, data1, data2;
	//panel_reset_gpio(20);

	mdss_dsi_cmds_tx(NULL,otm1284_set_pkt_size_cmd, ARRAY_SIZE(otm1284_set_pkt_size_cmd),0);
	mdelay(10);
	mdss_dsi_cmds_tx(NULL,otm1284_panel_manufacture_id_cmd, ARRAY_SIZE(otm1284_panel_manufacture_id_cmd),0);
	mdelay(10);
	mipi_dsi_cmds_rx(&rp, 3);
	lp = (uint32_t *)(rp);
	data1 = (uint32_t)(*lp);
	data1 = ntohl(data1);
	data1 = data1 & 0xff;

	lp = (uint32_t *)(rp + 4);
	data2 = (uint32_t)(*lp);
	data2 = data2 & 0xff;

	data = (data1 << 8) | data2;

	dprintf(INFO, "mipi otm1287_lcd manufacture id =0x %x, data1 = 0x %x, data2 = 0x %x \n",data, data1, data2);

	return ((data == OTM1284_ID) ? OTM1284_ID : 0);
}
uint32_t mipi_nt35512s_manufacture_id(void)
{
	char rec_buf[24];
	char *rp = rec_buf;
	uint32_t *lp,data,data1,data2;

	//panel_reset_gpio(20);
	mdss_dsi_cmds_tx(NULL,nt35512s_panel_enable_page2_cmd, ARRAY_SIZE(nt35512s_panel_enable_page2_cmd),0);
	mdelay(50);
	mdss_dsi_cmds_tx(NULL,nt35512s_panel_enable_page1_cmd, ARRAY_SIZE(nt35512s_panel_enable_page1_cmd),0);
	mdelay(50);
	mdss_dsi_cmds_tx(NULL,nt35512s_panel_manufacture_id_cmd, ARRAY_SIZE(nt35512s_panel_manufacture_id_cmd),0);
	mdelay(10);
	mipi_dsi_cmds_rx(&rp,3);
	lp = (uint32_t *) (rp+5);
	data1 = (uint32_t)(*lp);
	data1 = ntohl(data1);
	lp = (uint32_t *) (rp+7);
	data2 = (uint32_t)(*lp);
	data2 = ntohl(data2);
	data = (data1 >> 16) | (data2 >> 24);
	data = data & 0xFFFF;
	dprintf(INFO, "Get nt35512s mipi chip id =0x%x, *lp =0x%x \n", data, *lp);

		return ((data == NT35512S_ID) ? NT35512S_ID: 0);
}
uint32_t mipi_jd9366_manufacture_id(void)
{
	char rec_buf[24];
	char *rp = rec_buf;
	uint32_t *lp,data,data1,data2;

	//panel_reset_gpio(20);
	mdss_dsi_cmds_tx(NULL,jd9366_panel_enable_page2_cmd, ARRAY_SIZE(jd9366_panel_enable_page2_cmd),0);
	mdelay(50);
	mdss_dsi_cmds_tx(NULL,jd9366_panel_enable_page1_cmd, ARRAY_SIZE(jd9366_panel_enable_page1_cmd),0);
	mdelay(50);
	mdss_dsi_cmds_tx(NULL,jd9366_panel_manufacture_id_cmd, ARRAY_SIZE(jd9366_panel_manufacture_id_cmd),0);
	mdelay(10);
	mipi_dsi_cmds_rx(&rp,3);
	lp = (uint32_t *) (rp+5);
	data1 = (uint32_t)(*lp);
	data1 = ntohl(data1);
	lp = (uint32_t *) (rp+7);
	data2 = (uint32_t)(*lp);
	data2 = ntohl(data2);
	data = (data1 >> 16) | (data2 >> 24);
	data = data & 0xFFFF;
	dprintf(INFO, "Get jd9366 mipi chip id =0x%x, *lp =0x%x \n", data, *lp);
	if (data == JD9366_ID)
		return JD9366_ID;
	else if(data == JD9366_NEW_ID)
		return JD9366_NEW_ID;
	else
		return 0;
}
uint32_t mipi_nt35590_manufacture_id(void)
{
	char rec_buf[24];
	char *rp = rec_buf;
	uint32_t *lp, data;

	//panel_reset_gpio(20);

	mdss_dsi_cmds_tx(NULL,nt35590_panel_set_pkt_size_cmd, ARRAY_SIZE(nt35590_panel_set_pkt_size_cmd),0);
	mdelay(10);
	mdss_dsi_cmds_tx(NULL,nt35590_panel_manufacture_id_cmd, ARRAY_SIZE(nt35590_panel_manufacture_id_cmd),0);
	mdelay(10);

	mipi_dsi_cmds_rx(&rp, 4);

	lp = (uint32_t *)rp;
	data = (uint32_t)(*lp);
	data = ntohl(data);
	data = ((data>>24) & 0xff);

	dprintf(INFO, "Get nt35590 id = 0x%x, *lp =0x%x \n",data, *lp);

	return ((data == NT35590_ID) ? NT35590_ID : 0);
}

uint32_t mipi_nt35596_manufacture_id(void)
{
	char rec_buf[24];
	char *rp = rec_buf;
	uint32_t *lp, data;

	//panel_reset_gpio(20);

	mdss_dsi_cmds_tx(NULL,nt35596_panel_enable_page1_cmd, ARRAY_SIZE(nt35596_panel_enable_page1_cmd),0);
	mdelay(50);
	mdss_dsi_cmds_tx(NULL,nt35596_panel_set_pkt_size_cmd, ARRAY_SIZE(nt35596_panel_set_pkt_size_cmd),0);
	mdelay(50);
	mdss_dsi_cmds_tx(NULL,nt35596_panel_manufacture_id_cmd, ARRAY_SIZE(nt35596_panel_manufacture_id_cmd),0);
	mdelay(10);
	mipi_dsi_cmds_rx(&rp, 4);

	lp = (uint32_t *)(rp + 2);
	data = (uint32_t)(*lp);
	data = ntohl(data);
	data = (data & 0xff);

	dprintf(INFO, "mipi nt35596 lcd manufacture id =0x %x, *lp =0x %x \n",data, *lp);
	return ((data == NT35596_ID) ? NT35596_ID : 0);
}

uint32_t mipi_nt35532_manufacture_id(void)
{
	char rec_buf[24];
	char *rp = rec_buf;
	uint32_t *lp, data;

	//panel_reset_gpio(20);

	mdss_dsi_cmds_tx(NULL,nt35596_panel_enable_page1_cmd, ARRAY_SIZE(nt35596_panel_enable_page1_cmd),0);
	mdelay(50);
	mdss_dsi_cmds_tx(NULL,nt35596_panel_set_pkt_size_cmd, ARRAY_SIZE(nt35596_panel_set_pkt_size_cmd),0);
	mdelay(50);
	mdss_dsi_cmds_tx(NULL,nt35596_panel_manufacture_id_cmd, ARRAY_SIZE(nt35596_panel_manufacture_id_cmd),0);
	mdelay(10);
	mipi_dsi_cmds_rx(&rp, 4);

	lp = (uint32_t *)(rp + 2);
	data = (uint32_t)(*lp);
	data = ntohl(data);
	data = (data & 0xff);

	dprintf(INFO, "mipi nt35532 lcd manufacture id =0x %x, *lp =0x %x \n",data, *lp);
	return ((data == NT35532_ID) ? NT35532_ID : 0);
}

uint32_t mipi_td4310_manufacture_id(void)
{
	char rec_buf[24];
	char *rp = rec_buf;
	uint32_t *lp, data;

	//panel_reset_gpio(20);

	mdss_dsi_cmds_tx(NULL,td4310_panel_enable_page1_cmd, ARRAY_SIZE(td4310_panel_enable_page1_cmd),0);
	mdelay(50);
	mdss_dsi_cmds_tx(NULL,td4310_panel_set_pkt_size_cmd, ARRAY_SIZE(td4310_panel_set_pkt_size_cmd),0);
	mdelay(50);
	mdss_dsi_cmds_tx(NULL,td4310_panel_manufacture_id_cmd, ARRAY_SIZE(td4310_panel_manufacture_id_cmd),0);
	mdelay(10);
	mipi_dsi_cmds_rx(&rp, 4);

	lp = (uint32_t *)(rp + 2);
	data = (uint32_t)(*lp);
	data = ntohl(data);
	data = (data & 0xff);

	dprintf(INFO, "mipi td4310 lcd manufacture id =0x %x, *lp =0x %x \n",data, *lp);
	return ((data == TD4310_ID) ? TD4310_ID : 0);
}

uint32_t mipi_hx83112_manufacture_id(void)
{
	char rec_buf[24];
	char *rp = rec_buf;
	uint32_t *lp, data;

	//panel_reset_gpio(20);

	mdss_dsi_cmds_tx(NULL,hx83112_panel_enable_page1_cmd, ARRAY_SIZE(hx83112_panel_enable_page1_cmd),0);
	mdelay(50);
	mdss_dsi_cmds_tx(NULL,hx83112_panel_set_pkt_size_cmd, ARRAY_SIZE(hx83112_panel_set_pkt_size_cmd),0);
	mdelay(50);
	mdss_dsi_cmds_tx(NULL,hx83112_panel_manufacture_id_cmd, ARRAY_SIZE(hx83112_panel_manufacture_id_cmd),0);
	mdelay(10);
	mipi_dsi_cmds_rx(&rp, 4);

	lp = (uint32_t *)(rp + 2);
	data = (uint32_t)(*lp);
	data = ntohl(data);
	data = (data & 0xff);

	dprintf(INFO, "mipi hx83112 lcd manufacture id =0x %x, *lp =0x %x \n",data, *lp);
	return ((data == HX83112_ID) ? HX83112_ID : 0);
}

uint32_t mipi_ft8719_manufacture_id(void)
{
	char rec_buf[24];
	char *rp = rec_buf;
	uint32_t *lp, data;

	//panel_reset_gpio(20);

	mdss_dsi_cmds_tx(NULL,ft8719_panel_enable_page1_cmd, ARRAY_SIZE(ft8719_panel_enable_page1_cmd),0);
	mdelay(50);
	mdss_dsi_cmds_tx(NULL,ft8719_panel_set_pkt_size_cmd, ARRAY_SIZE(ft8719_panel_set_pkt_size_cmd),0);
	mdelay(50);
	mdss_dsi_cmds_tx(NULL,ft8719_panel_manufacture_id_cmd, ARRAY_SIZE(ft8719_panel_manufacture_id_cmd),0);
	mdelay(10);
	mipi_dsi_cmds_rx(&rp, 4);

	lp = (uint32_t *)(rp + 2);
	data = (uint32_t)(*lp);
	data = ntohl(data);
	data = (data & 0xff);

	dprintf(INFO, "mipi ft8719 lcd manufacture id =0x %x, *lp =0x %x \n",data, *lp);
	return ((data == FT8719_ID) ? FT8719_ID : 0);
}

uint32_t mipi_otm8018b_manufacture_id(void)
{
	char ref_buf[24];
	char *rp = ref_buf;
	uint32_t *lp, ret_data;

	//panel_reset_gpio(20);

	mdss_dsi_cmds_tx(NULL,otm8018b_panel_manufacture_id_cmd, ARRAY_SIZE(otm8018b_panel_manufacture_id_cmd),0);
	mdelay(10);
	mipi_dsi_cmds_rx(&rp, 4);
	lp = (uint32_t *)(rp + 2);
	ret_data = (uint32_t)(*lp);
	ret_data = ntohl(ret_data);
	ret_data = (ret_data  & 0xff);

	dprintf(INFO, "Get otm8018 id =0x%x, *lp =0x %x \n",ret_data, *lp);

	return ((ret_data == OTM8018B_ID) ? OTM8018B_ID : 0);
}

uint32_t mipi_hx8389b_manufacture_id(void)
{
	char rec_buf[24];
	char *rp = rec_buf;
	uint32_t *lp, data;
	//int i;
	//panel_reset_gpio(20);

	mdss_dsi_cmds_tx(NULL,hx8389b_panel_manufacture_id_cmd, ARRAY_SIZE(hx8389b_panel_manufacture_id_cmd),0);
	mdelay(10);
	mipi_dsi_cmds_rx(&rp, 3);

	lp = (uint32_t *)(rp + 8);
	data = (uint32_t)(*lp);
	//data = ntohl(data);
	data = ((data >> 8) & 0xff);

	dprintf(INFO, "Get hx8389b id =0x%x, *lp =0x %x \n",data, *lp);

	return  ((data == HX8389B_ID) ? HX8389B_ID : 0);
}

uint32_t mipi_hx8394d_manufacture_id(void)
{
	char ref_buf[24];
	char *rp = ref_buf;
	uint32_t *lp, ret_data;

	//panel_reset_gpio(20);

	mdss_dsi_cmds_tx(NULL,def_panel_manufacture_id_cmd, ARRAY_SIZE(def_panel_manufacture_id_cmd),0);
	mdelay(10);
	mipi_dsi_cmds_rx(&rp, 4);
	lp = (uint32_t *)(rp + 2);
	ret_data = (uint32_t)(*lp);
	ret_data = ntohl(ret_data);
	ret_data = (ret_data  & 0xff);

	dprintf(INFO, "Get 8394d id =0x%x, *lp =0x %x \n",ret_data, *lp);

	return ((ret_data == HX8394D_P800_ID) ? HX8394D_P800_ID : 0);
}

uint32_t mipi_ili9806c_manufacture_id(void)
{
	char ref_buf[24];
	char *rp = ref_buf;
	uint32_t *lp, ret_data;

	//panel_reset_gpio(20);

	mdss_dsi_cmds_tx(NULL,ili9806c_panel_manufacture_id_cmd, ARRAY_SIZE(ili9806c_panel_manufacture_id_cmd),0);

	mdelay(10);
	mipi_dsi_cmds_rx(&rp, 4);

	lp = (uint32_t *)(rp + 8);
	ret_data = (uint32_t)(*lp);
	ret_data = ntohl(ret_data);
	ret_data = ((ret_data >> 16) & 0xff);

	dprintf(INFO, "Get ili9806c id =0x%x, *lp =0x %x \n",ret_data, *lp);

	return ((ret_data == ILI9806C_ID) ? ILI9806C_ID : 0);
}

uint32_t mipi_ili9806e_manufacture_id(void)
{
	char rec_buf[24];
	char *rp = rec_buf;
	uint32_t *lp, data, data1, data2;

	//panel_reset_gpio(20);

	mdss_dsi_cmds_tx(NULL,ili9806e_panel_manufacture_id1_cmd, ARRAY_SIZE(ili9806e_panel_manufacture_id1_cmd),0);
	mdelay(10);
	mipi_dsi_cmds_rx(&rp, 3);

	lp = (uint32_t *)(rp + 4);
	data1 = (uint32_t)(*lp);
	data1 = ntohl(data1);
	data1 = ((data1 >> 16) & 0xff);


	mdss_dsi_cmds_tx(NULL,ili9806e_panel_manufacture_id2_cmd, ARRAY_SIZE(ili9806e_panel_manufacture_id2_cmd),0);
	mdelay(10);
	mipi_dsi_cmds_rx(&rp, 3);

	lp = (uint32_t *)(rp + 4);
	data2 = (uint32_t)(*lp);
	data2 = ntohl(data2);
	data2 = ((data2 >> 16) & 0xff);


	data = (data1 << 8) | data2;
	dprintf(INFO, "Get ili9806e id =0x%x, data1=0x%x, data2=0x%x \n",data, data1, data2);

	return ((data == ILI9806E_ID) ? ILI9806E_ID : 0);
}

uint32_t mipi_ili9881_manufacture_id(void)
{
	char rec_buf[24];
	char *rp = rec_buf;
	uint32_t *lp, data, data1;

	//panel_reset_gpio(20);

	mdss_dsi_cmds_tx(NULL,ili9881_panel_manufacture_id1_cmd, ARRAY_SIZE(ili9881_panel_manufacture_id1_cmd),0);
	mdelay(10);
	mipi_dsi_cmds_rx(&rp, 3);

	lp = (uint32_t *)(rp + 4);
	data1 = (uint32_t)(*lp);
	data1 = ntohl(data1);

	data = data1 & 0xff;
	data1 = ((data1 >> 16) & 0xff);

	data = (data1 << 8) | data;
	dprintf(INFO, "Get ili9881 id =0x%x, data1=0x%x\n",data, data1);

	return ((data == ILI9881_ID) ? ILI9881_ID : 0);
}

uint32_t mipi_rm68171_manufacture_id(void)
{
	char rec_buf[24];
	char *rp = rec_buf;
	uint32_t *lp, data;

	//panel_reset_gpio(20);

	mdss_dsi_cmds_tx(NULL,rm68171_panel_enable_page2_cmd, ARRAY_SIZE(rm68171_panel_enable_page2_cmd),0);
	mdelay(50);
	mdss_dsi_cmds_tx(NULL,rm68171_panel_enable_page1_cmd, ARRAY_SIZE(rm68171_panel_enable_page1_cmd),0);
	mdelay(50);
	mdss_dsi_cmds_tx(NULL,rm68171_panel_manufacture_id_cmd, ARRAY_SIZE(rm68171_panel_manufacture_id_cmd),0);
	mdelay(10);
	mipi_dsi_cmds_rx(&rp, 3);

	lp = (uint32_t *) (rp+4);
	data = (uint32_t)(*lp);
	data = ntohl(data);
	data = (data >> 16) & 0xFFFF;

	dprintf(INFO, "Get rm68171 mipi chip id =0x%x, *lp =0x%x \n", data, *lp);

		return ((data == RM68171_ID) ? RM68171_ID: 0);
}

uint32_t mipi_rm68172_manufacture_id(void)
{
	char rec_buf[24];
	char *rp = rec_buf;
	uint32_t *lp, data;

	//panel_reset_gpio(20);

	mdss_dsi_cmds_tx(NULL,rm68172_panel_manufacture_id_cmd, ARRAY_SIZE(rm68172_panel_manufacture_id_cmd),0);
	mdelay(10);
	mipi_dsi_cmds_rx(&rp, 3);

	lp = (uint32_t *) (rp+1);
	data = (uint32_t)(*lp);
	data = ntohl(data);
	data = data  & 0xFFFF;

	dprintf(INFO, "Get rm68172 mipi chip id =0x%x, *lp =0x%x \n", data, *lp);

		return ((data == RM68172_ID) ? RM68172_ID: 0);
}

uint32_t mipi_ST7701_manufacture_id(void)
   {
       char rec_buf[24];
       char *rp = rec_buf;
       uint32_t *lp, data;
       //panel_reset_gpio(20);
#if (A406) || (A406_F16)
	   panel_reset2_gpio(0);
#endif
       mdss_dsi_cmds_tx(NULL,ST7701_panel_manufacture_id_cmd, ARRAY_SIZE(ST7701_panel_manufacture_id_cmd),0); 
       mdelay(10);
       mipi_dsi_cmds_rx(&rp, 4);
       lp = (uint32_t *)(rp + 4);
       data = (uint32_t)(*lp);
       data = ntohl(data);
       data = ((data >> 16) & 0xffff); 
       dprintf(INFO, "mipi ST7701_lcd manufacture id =0x %x, *lp =0x %x \n",data, *lp);
#if (A406) || (A406_F16)
	panel_reset2_gpio(1); 
#endif
       return ((data == ST7701_ID) ? ST7701_ID : 0);
   } 

uint32_t mipi_otm9605a_manufacture_id(void)
{
	char rec_buf[24];
	char *rp = rec_buf;
	uint32_t *lp1, *lp2, data, data1, data2;

	//panel_reset_gpio(20);

	mdss_dsi_cmds_tx(NULL,otm9605a_panel_set_pkt_size_cmd, ARRAY_SIZE(otm9605a_panel_set_pkt_size_cmd),0);
	mdelay(50);
	mdss_dsi_cmds_tx(NULL,otm9605a_panel_manufacture_id_cmd, ARRAY_SIZE(otm9605a_panel_manufacture_id_cmd),0);
	mdelay(50);

	mipi_dsi_cmds_rx(&rp, 3);
	lp1 = (uint32_t *)(rp);
	data1 = (uint32_t)(*lp1);
	data1 = ntohl(data1);
	data1 &= 0xff;

	lp2 = (uint32_t *)(rp + 1);
	data2 = (uint32_t)(*lp2);
	data2 = ntohl(data2);
	data2 &= 0xff;

	data = ((data1 << 8) | (data2));
	dprintf(INFO, "Get otm9605a id = 0x%x, data1 = 0x%x, data2 = 0x%x \n",data, data1,data2);

	return ((data == OTM9605A_ID) ? OTM9605A_ID : 0);
}
uint32_t mipi_sh1282_manufacture_id(void)
{
	char rec_buf[24];
	char *rp = rec_buf;
	uint32_t *lp, data;

	//panel_reset_gpio(20);

	mdss_dsi_cmds_tx(NULL,sh1282_panel_enable_page1_cmd, ARRAY_SIZE(sh1282_panel_enable_page1_cmd),0);
	mdelay(10);
	mdss_dsi_cmds_tx(NULL,sh1282_panel_set_pkt_size_cmd, ARRAY_SIZE(sh1282_panel_set_pkt_size_cmd),0);
	mdelay(10);
	mdss_dsi_cmds_tx(NULL,sh1282_panel_manufacture_id_cmd, ARRAY_SIZE(sh1282_panel_manufacture_id_cmd),0);
	mdelay(10);
	mipi_dsi_cmds_rx(&rp, 4);

	lp = (uint32_t *)(rp + 4);
	data = (uint32_t)(*lp);
	data = ntohl(data);
	data = ((data >> 16) & 0xffff);

	dprintf(INFO, "mipi sh1282_lcd manufacture id =0x %x, *lp =0x %x \n",data, *lp);

	return ((data == SH1282_ID) ? SH1282_ID : 0);
}

uint32_t mipi_otm8019_manufacture_id(void)
{
	char rec_buf[24];
	char *rp = rec_buf;
	uint32_t *lp, data, data1, data2;
	//panel_reset_gpio(20);
#if A406_F16
       panel_reset2_gpio(0);
#endif
	mdss_dsi_cmds_tx(NULL,otm8019_set_pkt_size_cmd, ARRAY_SIZE(otm8019_set_pkt_size_cmd),0);
	mdelay(10);
	mdss_dsi_cmds_tx(NULL,otm8019_panel_manufacture_id_cmd, ARRAY_SIZE(otm8019_panel_manufacture_id_cmd),0);
	mdelay(10);
	mipi_dsi_cmds_rx(&rp, 3);

	lp = (uint32_t *)(rp);
	data1 = (uint32_t)(*lp);
	data1 = ntohl(data1);
	data1 = data1 & 0xff;

	lp = (uint32_t *)(rp + 4);
	data2 = (uint32_t)(*lp);
	data2 = data2 & 0xff;
	data = (data1 << 8) | data2;
	dprintf(INFO, "mipi otm8019_lcd manufacture id =0x %x, data1 = 0x %x, data2 = 0x %x \n",data, data1, data2);
#if A406_F16
       panel_reset2_gpio(1); 
#endif
	return ((data == OTM8019_ID) ? OTM8019_ID : 0);
}

uint32_t mipi_r61318_manufacture_id(void)
{
	char rec_buf[60];
	char *rp = rec_buf;
	uint32_t *lp, data;

	//panel_reset_gpio(20);

	mdss_dsi_cmds_tx(NULL,r61318_panel_access_proctect_cmd, ARRAY_SIZE(r61318_panel_access_proctect_cmd),0);
	mdelay(50);
	mdss_dsi_cmds_tx(NULL,r61318_set_pkt_size_cmd, ARRAY_SIZE(r61318_set_pkt_size_cmd),0);
	mdelay(50);
	mdss_dsi_cmds_tx(NULL,r61318_panel_manufacture_id_cmd, ARRAY_SIZE(r61318_panel_manufacture_id_cmd),0);
	mdelay(10);
	mipi_dsi_cmds_rx(&rp, 3);
	lp = (uint32_t *) (rp+4);
	data = (uint32_t)(*lp);
	data = ntohl(data);
	data = (data>>16) & 0xFFFF;

	dprintf(INFO, "Get r61318 mipi chip id =0x%x, *lp =0x%x \n", data, *lp);

	return ((data == R61318_ID) ? R61318_ID: 0);
}

uint32_t mipi_r63315_manufacture_id(void)
{
	char rec_buf[60];
	char *rp = rec_buf;
	uint32_t *lp, data;

	//panel_reset_gpio(20);

	mdss_dsi_cmds_tx(NULL,r63315_panel_access_proctect_cmd, ARRAY_SIZE(r63315_panel_access_proctect_cmd),0);
	mdelay(50);
	mdss_dsi_cmds_tx(NULL,r63315_set_pkt_size_cmd, ARRAY_SIZE(r63315_set_pkt_size_cmd),0);
	mdelay(50);
	mdss_dsi_cmds_tx(NULL,r63315_panel_manufacture_id_cmd, ARRAY_SIZE(r63315_panel_manufacture_id_cmd),0);
	mdelay(10);
	mipi_dsi_cmds_rx(&rp, 3);
	lp = (uint32_t *) (rp+4);
	data = (uint32_t)(*lp);
	data = ntohl(data);
	data = (data>>16) & 0xFFFF;

	dprintf(INFO, "Get r63315 mipi chip id =0x%x, *lp =0x%x \n", data, *lp);

	return ((data == R63315_ID) ? R63315_ID: 0);
}


uint32_t mipi_default_manufacture_id(void)
{
	get_id_flag = 1;
	return MIPI_DEFAULT_ID;
}

/*
 * DO NOT change the following function pointers' order optionallya.
 * e.g.
 * The first pointer of mipi_ili9806c_manufacture_id is major used in BOARD_S1_SGC113,
 * and the second one(mipi_otm8018b_manufacture_id) is subordinate.
 * This is a better way to avoid to run reading-id-cammands which is useless for the project.
 */

static uint32_t mipi_get_ic_manufacture_id(void)
{
	uint32_t lcd_chip_id;
	uint8_t retnum = 3;
	uint32_t cnt, NUM = ARRAY_SIZE(mipi_ic_manufacture_id);
	static const  get_ic_id_func  *mipi_manufacture_id;

	dprintf(INFO, "mipi_get_ic_manufacture_id\n");

	while(retnum) {
		for(mipi_manufacture_id = mipi_ic_manufacture_id, cnt = 0; cnt < NUM; ++mipi_manufacture_id, ++cnt) {
			lcd_chip_id = (*mipi_manufacture_id)();

			if(lcd_chip_id) {
				get_id_flag = 1;
				break;
			}
		}

		if(get_id_flag)
			break;
		else
			retnum--;
		panel_reset_gpio(120);
	}

	if(!get_id_flag) {
		lcd_chip_id = UNKNOWN_ID;
		dprintf(INFO,"mipi_get_ic_manufacture_id(), Not found lcd chip id. set default lcd_chip_id = 0x%x.\n",lcd_chip_id);
	}

	dprintf(INFO,"# mipi_get_ic_manufacture_id(). lcd_chip_id =0x%x.\n",lcd_chip_id);
	return lcd_chip_id;
}

unsigned char get_current_chip_id(void)
{

	current_id = mipi_get_ic_manufacture_id();

	dprintf(INFO,"# get_current_chip_id(). current_id = 0x%x \n",current_id);
	return current_id;
}
