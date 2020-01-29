#ifndef _MIPI_ID_REG_H_
#define _MIPI_ID_REG_H_
#include <msm_panel.h>
#include <mipi_dsi.h>
/*---------------------------------------------------------------------------*/
/* static panel selection variable                                           */
/*---------------------------------------------------------------------------*/
typedef uint32_t (*get_ic_id_func)(void);
uint32_t mipi_otm8018b_manufacture_id(void);
uint32_t mipi_hx8389b_manufacture_id(void);
uint32_t mipi_hx8394d_manufacture_id(void);
uint32_t mipi_ili9806c_manufacture_id(void);
uint32_t mipi_ili9806e_manufacture_id(void);
uint32_t mipi_ili9881_manufacture_id(void);
uint32_t mipi_rm68171_manufacture_id(void);
uint32_t mipi_otm9605a_manufacture_id(void);
uint32_t mipi_sh1282_manufacture_id(void);
uint32_t mipi_nt35596_manufacture_id(void);
uint32_t mipi_nt35532_manufacture_id(void);
uint32_t mipi_nt35590_manufacture_id(void);
uint32_t mipi_r61318_manufacture_id(void);
uint32_t mipi_rm68172_manufacture_id(void);
uint32_t mipi_default_manufacture_id(void);
uint32_t mipi_nt35512s_manufacture_id(void);
uint32_t mipi_jd9366_manufacture_id(void);
uint32_t mipi_otm1901_manufacture_id(void);
uint32_t mipi_ts7635dc_manufacture_id(void);
uint32_t mipi_ili7807D_BYD_manufacture_id(void);
uint32_t mipi_nt35521_BYD_manufacture_id(void);
uint32_t mipi_td4310_manufacture_id(void);
uint32_t mipi_hx83112_manufacture_id(void);
uint32_t mipi_ft8719_manufacture_id(void);
uint32_t mipi_otm1284_manufacture_id(void);
uint32_t mipi_r63315_manufacture_id(void);
uint32_t mipi_ST7701_manufacture_id(void);
uint32_t mipi_otm8019_manufacture_id(void);

enum {
	JDI_1080P_VIDEO_PANEL,
	NT35590_720P_VIDEO_PANEL,
	ILI9881_720P_CPT_5INCH_VIDEO_PANEL,
	ILI9881_720P_CPT_5INCH_HLT_VIDEO_PANEL,
	OTM8019A_FWVGA_VIDEO_PANEL,
	RM61318_720P_CPT_VIDEO_PANEL,
	ILI9806E_FWVGA_5INCH_IVO_HUASHI_VIDEO_PANEL,
	OTM1283A_720P_VIDEO_PANEL,
	NT35590_720P_CMD_PANEL,
	INNOLUX_720P_VIDEO_PANEL,
	SH1282_FWVGA_BOE_PANEL,
	ILI9806E_WVGA_BOE_PANEL,
	OTM8019_WVGA_BOE_PANEL,
	RM68191_WVGA_BOE_PANEL,
	SHARP_LS040T3SW01_QHD_VIDEO_PANEL,
	OTM9605A_F551G_WVGA_VIDEO_PANEL,
	OTM1284_720P_VIDEO_PANEL,
	RM68172_TM_WVGA_VIDEO_PANEL,
	ST7701_TM_VIDEO_PANEL,
	ST7701_480P_VIDEO_PANEL,
	RM68172_WANCHANGLONG_WVGA_VIDEO_PANEL,
	HX8394D_720P_VIDEO_PANEL,
	NT35512S_WVGA_VIDEO_PANEL,
	OTM1284A_BOE_HD_VIDEO_PANEL,
	ILL9881C_BOE_HD_VIDEO_PANEL,
	OTM1284A_HD_VIDEO_PANEL,
	NT35596_1080P_VIDEO_PANEL,
	SHARP_WQXGA_DUALDSI_VIDEO_PANEL,
	JDI_FHD_VIDEO_PANEL,
	HX8379A_FWVGA_VIDEO_PANEL,
	R61318_HD_VIDEO_PANEL,
	OTM1284_1080P_VIDEO_PANEL,//wangqing.yu modify for S500 LCM
	NT35532_1080P_VIDEO_PANEL,//s550
	OTM1901_1080P_VIDEO_PANEL,//C600
	TS7635DC_640P_VIDEO_PANEL,//c350
	TD4310_1080P_VIDEO_PANEL,//c551
	HX83112_1080P_VIDEO_PANEL,
	FT8719_1080P_VIDEO_PANEL,//c570
	FT8716U_1080P_VIDEO_PANEL,//c520
	ILI7807D_BYD_1080P_VIDEO_PANEL,//c600
	NT35521_BYD_VIDEO_PANEL,//c800
	JD9366_VIDEO_PANEL,//c802
	JD9366_NEW_VIDEO_PANEL,
	NT35521_WXGA_VIDEO_PANEL,
	R63417_1080P_VIDEO_PANEL,
	JDI_A216_FHD_VIDEO_PANEL,
	R63315_1080P_VIDEO_PANEL,//jijin.wang modify for s500 turly LCM
	HX8394D_A470_VIDEO_PANEL,//wjj modify for a470 lcm
	HUASHI_ILI9881C_VIDEO_PANEL,
	SSD2080M_720P_VIDEO_PANEL,
	SHARP_LQ050T1SX05_VIDEO_PANEL,
	HX8394D_P800_VIDEO_PANEL,
	OTM8019_TM_VIDEO_PANEL,
	NT35512_TM_WVGA_VIDEO,
	UNKNOWN_PANEL_NAME
};

#define OTM9605A_ID 0x9605
#define OTM1284_ID 0x1287
#define OTM1901_ID 0x1901
#define TD4310_ID 0x4310
#define HX83112_ID 0x00
#define OTM8018B_ID 0x40
#define ILI9806C_ID 0x16
#define ILI9806E_ID 0x9806
#define ILI9881_ID 0x9881
#define R61318_ID   0x1318

#define HX8389B_ID  0x89
#define RM68171_ID  0x7181
#define RM68172_ID  0x7281
#define SH1282_ID   0x1202
#define OTM8019_ID  0x8019
#define RM68191_ID  0x8191
#define UNKNOWN_ID  0xFFEF

#define NT35596_ID  0x96
#define FT8719_ID  0x40
#define NT35532_ID  0x32
#define NT35590_ID  0x90
#define NT35512S_ID  0x5521
#define JD9366_ID  0xffff
#define JD9366_NEW_ID  0xa1bf
#define R63315_ID 0x3315

#define MIPI_DEFAULT_ID  1
#define ST7701_ID  0x2177
#define HX8394D_ID  0x8394
#define HX8394D_P800_ID  0x83

#endif


