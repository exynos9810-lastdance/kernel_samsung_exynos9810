/*
 * Copyright (c) 2016 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com
 *
 * Header file for Exynos DECON driver
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#ifndef ___SAMSUNG_DECON_H__
#define ___SAMSUNG_DECON_H__

#include <linux/fb.h>
#include <linux/kernel.h>
#include <linux/clk.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <linux/wait.h>
#include <linux/kthread.h>
#include <linux/pm_qos.h>
#include <linux/delay.h>
#include <linux/seq_file.h>
#include <linux/platform_device.h>
#include <media/v4l2-device.h>
#include <media/videobuf2-core.h>
#include <soc/samsung/bts.h>
#include <linux/exynos_ion.h>
#include <linux/ion.h>
#include <linux/exynos_iovmm.h>
#ifdef CONFIG_SEC_ABC
#include <linux/sti/abc_common.h>
#endif

#ifdef CONFIG_EXYNOS_COMMON_PANEL
#include "../panel/panel_drv.h"
#endif

/* TODO: SoC dependency will be removed */
#if defined(CONFIG_SOC_EXYNOS9810)
#include "./cal_9810/regs-decon.h"
#include "./cal_9810/decon_cal.h"
#endif

#include "./panels/decon_lcd.h"
#include "dsim.h"
#include "displayport.h"
#if defined(CONFIG_SUPPORT_LEGACY_FENCE)
#include "../../../../dma-buf/sync_debug.h"
#endif
#include "hdr_metadata.h"
#if defined(CONFIG_EXYNOS_COMMON_PANEL)
#include "disp_err.h"
#endif

#define SUCCESS_EXYNOS_SMC	0

#define DISP_RESTRICTION_VER	20180608

extern struct ion_device *ion_exynos;
extern struct decon_device *decon_drvdata[MAX_DECON_CNT];
extern int decon_log_level;
extern int dpu_bts_log_level;
extern int win_update_log_level;
extern int dpu_mres_log_level;
extern int decon_systrace_enable;
extern struct decon_bts_ops decon_bts_control;

#define DECON_MODULE_NAME	"exynos-decon"
#define MAX_NAME_SIZE		32
#define MAX_PLANE_CNT		3
#define MAX_PLANE_ADDR_CNT	4
#define DECON_ENTER_HIBER_CNT	3
#define DECON_ENTER_LPD_CNT	3
#define MIN_BLK_MODE_WIDTH	144
#define MIN_BLK_MODE_HEIGHT	16
#define VSYNC_TIMEOUT_MSEC	200
#define DEFAULT_BPP		32
#define MIN_WIN_BLOCK_WIDTH	8
#define MIN_WIN_BLOCK_HEIGHT	1
#define FD_TRY_CNT		3
#define VALID_FD_VAL		3
#define DECON_TRACE_BUF_SIZE	40

#define DECON_WIN_UPDATE_IDX	(6)

#ifndef KHZ
#define KHZ (1000)
#endif
#ifndef MHZ
#define MHZ (1000*1000)
#endif
#ifndef MSEC
#define MSEC (1000)
#endif

#define FHD (1080 * 1920)

#define SHADOW_UPDATE_TIMEOUT	(300 * 1000) /* 300ms */
#define IDLE_WAIT_TIMEOUT	(50 * 1000) /* 50ms */
#define RUN_WAIT_TIMEOUT	IDLE_WAIT_TIMEOUT
#define CEIL(x)			((x-(u32)(x) > 0 ? (u32)(x+1) : (u32)(x)))
#define DSC_INIT_XMIT_DELAY	0x200

#define EINT_PEND(x)		((x == 0) ? 2 : ((x == 1) ? 4 : 1))

#define MAX_DSC_SLICE_CNT	4

void dpu_debug_printk(const char *function_name, const char *format, ...);

#define decon_err(fmt, ...)							\
	do {									\
		if (decon_log_level >= 3) {					\
			pr_err(pr_fmt("decon: "fmt), ##__VA_ARGS__);			\
		}								\
	} while (0)

#define decon_warn(fmt, ...)							\
	do {									\
		if (decon_log_level >= 4) {					\
			pr_warn(pr_fmt("decon: "fmt), ##__VA_ARGS__);			\
		}								\
	} while (0)

#define decon_info(fmt, ...)							\
	do {									\
		if (decon_log_level >= 6)					\
			pr_info(pr_fmt("decon: "fmt), ##__VA_ARGS__);			\
	} while (0)

#define decon_dbg(fmt, ...)							\
	do {									\
		if (decon_log_level >= 7)					\
			pr_info(pr_fmt("decon: "fmt), ##__VA_ARGS__);			\
	} while (0)

#define DPU_DEBUG_WIN(fmt, args...)						\
	do {									\
		if (win_update_log_level >= 7)					\
			dpu_debug_printk("WIN_UPDATE", fmt,  ##args);		\
	} while (0)

#define DPU_DEBUG_BTS(fmt, args...)						\
	do {									\
		if (dpu_bts_log_level >= 7)					\
			dpu_debug_printk("BTS", fmt,  ##args);			\
	} while (0)

#define DPU_INFO_BTS(fmt, args...)						\
	do {									\
		if (dpu_bts_log_level >= 6)					\
			dpu_debug_printk("BTS", fmt,  ##args);			\
	} while (0)

#define DPU_ERR_BTS(fmt, args...)						\
	do {									\
		if (dpu_bts_log_level >= 3)					\
			dpu_debug_printk("BTS", fmt, ##args);			\
	} while (0)

#define DPU_DEBUG_MRES(fmt, args...)						\
	do {									\
		if (dpu_mres_log_level >= 7)					\
			dpu_debug_printk("MRES", fmt,  ##args);			\
	} while (0)

#define DPU_INFO_MRES(fmt, args...)						\
	do {									\
		if (dpu_mres_log_level >= 6)					\
			dpu_debug_printk("MRES", fmt,  ##args);			\
	} while (0)

#define DPU_ERR_MRES(fmt, args...)						\
	do {									\
		if (dpu_mres_log_level >= 3)					\
			dpu_debug_printk("MRES", fmt, ##args);			\
	} while (0)

/* DECON systrace related */
void decon_tracing_mark_write(struct decon_device *decon, char id, char *str1, int value);
#define decon_systrace(decon, id, str1, value)					\
	do {									\
		if (decon_systrace_enable)					\
			decon_tracing_mark_write(decon, id, str1, value);		\
	} while (0)

enum decon_hold_scheme {
	/*  should be set to this value in case of DSIM video mode */
	DECON_VCLK_HOLD_ONLY		= 0x00,
	/*  should be set to this value in case of DSIM command mode */
	DECON_VCLK_RUNNING_VDEN_DISABLE = 0x01,
	DECON_VCLK_HOLD_VDEN_DISABLE	= 0x02,
	/*  should be set to this value in case of HDMI, eDP */
	DECON_VCLK_NOT_AFFECTED		= 0x03,
};

enum decon_win_alpha_coef {
	BND_COEF_ZERO			= 0x0,
	BND_COEF_ONE			= 0x1,
	BND_COEF_AF			= 0x2,
	BND_COEF_1_M_AF		= 0x3,
	BND_COEF_AB			= 0x4,
	BND_COEF_1_M_AB		= 0x5,
	BND_COEF_PLNAE_ALPHA0		= 0x6,
	BND_COEF_1_M_PLNAE_ALPHA0	= 0x7,
	BND_COEF_PLNAE_ALPHA1		= 0x8,
	BND_COEF_1_M_PLNAE_ALPHA1	= 0x9,
	BND_COEF_ALPHA_MULT		= 0xA,
	BND_COEF_1_M_ALPHA_MULT	= 0xB,
};

enum decon_win_alpha_sel {
	ALPHA_MULT_SRC_SEL_ALPHA0 = 0,
	ALPHA_MULT_SRC_SEL_ALPHA1 = 1,
	ALPHA_MULT_SRC_SEL_AF = 2,
	ALPHA_MULT_SRC_SEL_AB = 3,
};

enum decon_merger_mode {
	DECON_LRM_NO		= 0x0,
	DECON_LRM_NOSWAP_RF	= 0x4,
	DECON_LRM_NOSWAP_LF	= 0x5,
	DECON_LRM_SWAP_RF	= 0x6,
	DECON_LRM_SWAP_LF	= 0x7,
};

enum decon_te_src {
	DECON_TE_FROM_DDI0 = 0,
	DECON_TE_FROM_DDI1,
	DECON_TE_FROM_DDI2,
	DECON_TE_FROM_USB,
};

/*
 * DECON_STATE_ON : disp power on, decon/dsim clock on & lcd on
 * DECON_HIBER : disp power off, decon/dsim clock off & lcd on
 * DECON_STATE_OFF : disp power off, decon/dsim clock off & lcd off
 */
enum decon_state {
	DECON_STATE_INIT = 0,
	DECON_STATE_ON,
	DECON_STATE_DOZE,
	DECON_STATE_HIBER,
	DECON_STATE_DOZE_SUSPEND,
	DECON_STATE_OFF,
	DECON_STATE_TUI,
};

/* To find a proper CLOCK ratio */
enum decon_clk_id {
	CLK_ID_VCLK = 0,
	CLK_ID_ECLK,
	CLK_ID_ACLK,
	CLK_ID_PCLK,
	CLK_ID_DPLL, /* DPU_PLL */
	CLK_ID_RESOLUTION,
	CLK_ID_MIC_RATIO,
	CLK_ID_DSC_RATIO,
	CLK_ID_MAX,
};

enum decon_dsc_id {
	DECON_DSC_ENC0 = 0x0,
	DECON_DSC_ENC1 = 0x1,
	DECON_DSC_ENC2 = 0x2,
};

enum decon_share_path {
	SHAREPATH_DQE_USE		= 0x0,
	SHAREPATH_VG0_USE		= 0x1,
	SHAREPATH_VG1_USE		= 0x2,
	SHAREPATH_VGF1_USE		= 0x3,
	SHAREPATH_VGF0_USE		= 0x4,
};

enum dpp_rotate {
	DPP_ROT_NORMAL = 0x0,
	DPP_ROT_XFLIP,
	DPP_ROT_YFLIP,
	DPP_ROT_180,
	DPP_ROT_90,
	DPP_ROT_90_XFLIP,
	DPP_ROT_90_YFLIP,
	DPP_ROT_270,
};

enum dpp_csc_eq {
	/* eq_mode : 6bits [5:0] */
	CSC_STANDARD_SHIFT = 0,
	CSC_BT_601 = 0,
	CSC_BT_709 = 1,
	CSC_BT_2020 = 2,
	CSC_DCI_P3 = 3,
	CSC_STANDARD_UNSPECIFIED = 63,
	/* eq_mode : 3bits [8:6] */
	CSC_RANGE_SHIFT = 6,
	CSC_RANGE_LIMITED = 0x0,
	CSC_RANGE_FULL = 0x1,
	CSC_RANGE_UNSPECIFIED = 7,
};

enum dpp_comp_src {
	DPP_COMP_SRC_NONE = 0,
	DPP_COMP_SRC_G2D,
	DPP_COMP_SRC_GPU
};

enum dpp_hdr_standard {
	DPP_HDR_OFF = 0,
	DPP_HDR_ST2084,
	DPP_HDR_HLG,
};

enum decon_color_mode {
	HAL_COLOR_MODE_NATIVE = 0,
	HAL_COLOR_MODE_NUM_MAX,
};

struct decon_color_mode_info {
	int index;
	u32 color_mode;
};

struct decon_clocks {
	unsigned long decon[CLK_ID_DPLL + 1];
};

struct decon_dma_buf_data {
	struct ion_handle		*ion_handle;
	struct dma_buf			*dma_buf;
	struct dma_buf_attachment	*attachment;
	struct sg_table			*sg_table;
	dma_addr_t			dma_addr;
#if defined(CONFIG_SUPPORT_LEGACY_FENCE)
	struct sync_file		*fence;
#else
	struct dma_fence		*fence;
#endif
};

struct decon_win_rect {
	int x;
	int y;
	u32 w;
	u32 h;
};

struct decon_rect {
	u32 left;
	u32 top;
	u32 right;
	u32 bottom;
};

struct dpp_params {
	dma_addr_t addr[MAX_PLANE_CNT];
	enum dpp_rotate rot;
	enum dpp_csc_eq eq_mode;
	enum dpp_comp_src comp_src;
	enum dpp_hdr_standard hdr_std;
	u32 min_luminance;
	u32 max_luminance;
};

struct decon_frame {
	int x;
	int y;
	u32 w;
	u32 h;
	u32 f_w;
	u32 f_h;
};

struct decon_win_config {
	enum {
		DECON_WIN_STATE_DISABLED = 0,
		DECON_WIN_STATE_COLOR,
		DECON_WIN_STATE_BUFFER,
		DECON_WIN_STATE_UPDATE,
		DECON_WIN_STATE_CURSOR,
		DECON_WIN_STATE_MRESOL = 0x10000,
		DECON_WIN_STATE_FINGERPRINT = 0x20000,
	} state;

	/* Reusability:This struct is used for IDMA and ODMA */
	union {
		__u32 color;
		struct {
			int				fd_idma[3];
			int				acq_fence;
			int				rel_fence;
			int				plane_alpha;
			enum decon_blending		blending;
			enum decon_idma_type		idma_type;
			enum decon_pixel_format		format;
			struct dpp_params		dpp_parm;
			/* no read area of IDMA */
			struct decon_win_rect		block_area;
			struct decon_win_rect		transparent_area;
			struct decon_win_rect		opaque_area;
			/* source framebuffer coordinates */
			struct decon_frame		src;
		};
	};
	/* destination OSD coordinates */
	struct decon_frame dst;
	bool protection;
	bool compression;
};

struct decon_reg_data {
	u32 num_of_window;
	int plane_cnt[MAX_DECON_WIN + 1];
	struct list_head list;
	struct decon_rect blender_bg;
	struct decon_win_config dpp_config[MAX_DECON_WIN + 1];
	struct decon_win_rect block_rect[MAX_DECON_WIN];
	struct decon_window_regs win_regs[MAX_DECON_WIN];
	struct decon_dma_buf_data dma_buf_data[MAX_DECON_WIN + 1][MAX_PLANE_CNT];
#if !defined(CONFIG_SUPPORT_LEGACY_FENCE)
	struct dma_fence *retire_fence;
#endif

	/*
	 * If window update size is changed, that size has to be applied to
	 * DECON, DSIM and panel in case of below
	 * - full size -> partial size
	 * - partial size -> different partial size
	 * - partial size -> full size
	 *
	 * need_update flag indicates whether changes are applied to hw or not
	 */
	bool need_update;
	/* current update region */
	struct decon_rect up_region;
	/* protected contents playback */
	bool protection[MAX_DECON_WIN + 1];
	/* cursor async */
	bool is_cursor_win[MAX_DECON_WIN];
	int cursor_win;

	bool mres_update;
	u32 lcd_width;
	u32 lcd_height;
	int mres_idx;
};

struct decon_win_config_extra {
	int remained_frames;
	u32 reserved[7];
};

struct decon_win_config_data_old_alt {
	int	retire_fence;
	int	fd_odma;
	u32	fps;
	struct decon_win_config config[MAX_DECON_WIN + 1];
};

struct decon_win_config_data_old {
	int	retire_fence;
	int	fd_odma;
	u32	fps;
	struct decon_win_config config[MAX_DECON_WIN + 2];
};

struct decon_win_config_data_alt {
	int	retire_fence;
	int	fd_odma;
	u32	fps;
	struct decon_win_config config[MAX_DECON_WIN + 1];
	struct decon_win_config_extra extra;
};

struct decon_win_config_data {
	int	retire_fence;
	int	fd_odma;
	u32	fps;
	struct decon_win_config config[MAX_DECON_WIN + 2];
	struct decon_win_config_extra extra;
};

enum hwc_ver {
	HWC_INIT = 0,
	HWC_1_0 = 1,
	HWC_2_0 = 2,
};

struct decon_disp_info {
	enum hwc_ver ver;
	enum decon_psr_mode psr_mode;
	struct lcd_mres_info mres_info;
	u32 chip_ver;
	unsigned char reverved[128];
};

struct dpu_size_info {
	u32 w_in;
	u32 h_in;
	u32 w_out;
	u32 h_out;
};

struct decon_display_mode {
	uint32_t index;
	uint32_t width;
	uint32_t height;
	uint32_t mm_width;
	uint32_t mm_height;
	uint32_t fps;
	uint32_t group;
};

#ifdef CONFIG_DECON_EVENT_LOG
/**
 * Display Subsystem event management status.
 *
 * These status labels are used internally by the DECON to indicate the
 * current status of a device with operations.
 */
typedef enum dpu_event_type {
	/* Related with FB interface */
	DPU_EVT_BLANK = 0,
	DPU_EVT_UNBLANK,
	DPU_EVT_ACT_VSYNC,
	DPU_EVT_DEACT_VSYNC,
	DPU_EVT_WIN_CONFIG,
	DPU_EVT_DISP_INFO,

	/* Related with interrupt */
	DPU_EVT_TE_INTERRUPT,
	DPU_EVT_UNDERRUN,
	DPU_EVT_DECON_FRAMEDONE,
	DPU_EVT_DSIM_FRAMEDONE,
	DPU_EVT_RSC_CONFLICT,

	/* Related with async event */
	DPU_EVT_UPDATE_HANDLER,
	DPU_EVT_DSIM_COMMAND,
	DPU_EVT_TRIG_MASK,
	DPU_EVT_TRIG_UNMASK,
	DPU_EVT_FENCE_RELEASE,
	DPU_EVT_DECON_FRAMEDONE_WAIT,
	DPU_EVT_DECON_SHUTDOWN,
	DPU_EVT_DSIM_SHUTDOWN,
	DPU_EVT_DECON_FRAMESTART,

	/* Related with DPP */
	DPU_EVT_DPP_WINCON,
	DPU_EVT_DPP_FRAMEDONE,
	DPU_EVT_DPP_STOP,
	DPU_EVT_DPP_UPDATE_DONE,
	DPU_EVT_DPP_SHADOW_UPDATE,
	DPU_EVT_DPP_SUSPEND,
	DPU_EVT_DPP_RESUME,

	/* Related with PM */
	DPU_EVT_DECON_SUSPEND,
	DPU_EVT_DECON_RESUME,
	DPU_EVT_ENTER_HIBER,
	DPU_EVT_EXIT_HIBER,
	DPU_EVT_DSIM_SUSPEND,
	DPU_EVT_DSIM_RESUME,
	DPU_EVT_ENTER_ULPS,
	DPU_EVT_EXIT_ULPS,
	DPU_EVT_DSIM_PHY_ON,
	DPU_EVT_DSIM_PHY_OFF,

	DPU_EVT_LINECNT_ZERO,

	/* write-back events */
	DPU_EVT_WB_SET_BUFFER,
	DPU_EVT_WB_SW_TRIGGER,

	DPU_EVT_DMA_FRAMEDONE,
	DPU_EVT_DMA_RECOVERY,

	DPU_EVT_DECON_SET_BUFFER,
	/* cursor async */
	DPU_EVT_CURSOR_POS,
	DPU_EVT_CURSOR_UPDATE,

	/* window update */
	DPU_EVT_WINUP_UPDATE_REGION,
	DPU_EVT_WINUP_FLAGS,
	DPU_EVT_WINUP_APPLY_REGION,
	DPU_EVT_DOZE,
	DPU_EVT_DOZE_SUSPEND,
	DPU_EVT_MAX, /* End of EVENT */
} dpu_event_t;

/* Related with Cursor */
struct disp_log_cursor {
	u32 xpos;
	u32 ypos;
	ktime_t elapsed;	/* End time - Start time */
};

/* Related with Fence */
struct disp_log_fence {
	u32 timeline_value;
	int timeline_max;
};

/* Related with PM */
struct disp_log_pm {
	u32 pm_status;		/* ACTIVE(1) or SUSPENDED(0) */
	ktime_t elapsed;	/* End time - Start time */
};

/* Related with S3CFB_WIN_CONFIG */
struct decon_update_reg_data {
	struct decon_window_regs 	win_regs[MAX_DECON_WIN];
	struct decon_win_config 	win_config[MAX_DECON_WIN + 1];
	struct decon_win_rect 		win;
};

/* Related with MIPI COMMAND read/write */
#define DPU_CALLSTACK_MAX 10
struct dsim_log_cmd_buf {
	u32 id;
	u8 buf;
	void *caller[DPU_CALLSTACK_MAX];
};

/* Related with DPP */
struct disp_log_dpp {
	u32 id;
	u32 start_cnt;
	u32 done_cnt;
	u32 comp_src;
	struct decon_frame src;
	struct decon_frame dst;
};

/* Related with window update information */
struct disp_log_winup {
	struct decon_frame req_region;
	struct decon_frame adj_region;
	struct decon_frame apl_region;
	bool need_update;
	bool reconfigure;
};

/**
 * struct dpu_log - Display Subsystem Log
 * This struct includes DECON/DSIM/DPP
 */
struct dpu_log {
	ktime_t time;
	dpu_event_t type;
	union {
		struct disp_log_dpp dpp;
		struct decon_update_reg_data reg;
		struct dsim_log_cmd_buf cmd_buf;
		struct disp_log_pm pm;
		struct disp_log_fence fence;
		struct disp_log_cursor cursor;
		struct disp_log_winup winup;
	} data;
};

struct dpu_size_err_info {
	ktime_t time;
	struct dpu_size_info info;
};

/* Definitions below are used in the DECON */
#define	DPU_EVENT_LOG_MAX	SZ_512
#define	DPU_EVENT_PRINT_MAX	(DPU_EVENT_LOG_MAX >> 1)
typedef enum dpu_event_log_level_type {
	DPU_EVENT_LEVEL_LOW = 0,
	DPU_EVENT_LEVEL_HIGH,
} dpu_log_level_t;

/* APIs below are used in the DECON/DSIM/DPP driver */
#define DPU_EVENT_START() ktime_t start = ktime_get()
void DPU_EVENT_LOG(dpu_event_t type, struct v4l2_subdev *sd, ktime_t time);
void DPU_EVENT_LOG_WINCON(struct v4l2_subdev *sd, struct decon_reg_data *regs);
void DPU_EVENT_LOG_CMD(struct v4l2_subdev *sd, u32 cmd_id, unsigned long data);
void DPU_EVENT_LOG_CURSOR(struct v4l2_subdev *sd, struct decon_reg_data *regs); /* cursor async */
void DPU_EVENT_LOG_UPDATE_REGION(struct v4l2_subdev *sd,
		struct decon_frame *req_region, struct decon_frame *adj_region);
void DPU_EVENT_LOG_WINUP_FLAGS(struct v4l2_subdev *sd, bool need_update,
		bool reconfigure);
void DPU_EVENT_LOG_APPLY_REGION(struct v4l2_subdev *sd,
		struct decon_rect *apl_rect);
void DPU_EVENT_SHOW(struct seq_file *s, struct decon_device *decon);
int decon_create_debugfs(struct decon_device *decon);
void decon_destroy_debugfs(struct decon_device *decon);
#else /*!*/
#define DPU_EVENT_START(...) do { } while(0)
#define DPU_EVENT_LOG(...) do { } while(0)
#define DPU_EVENT_LOG_WINCON(...) do { } while(0)
#define DPU_EVENT_LOG_CMD(...) do { } while(0)
#define DPU_EVENT_LOG_CURSOR(...) do { } while (0)
#define DPU_EVENT_LOG_UPDATE_REGION(...) do { } while(0)
#define DPU_EVENT_LOG_WINUP_FLAGS(...) do { } while(0)
#define DPU_EVENT_LOG_APPLY_REGION(...) do { } while(0)
#define DPU_EVENT_SHOW(...) do { } while(0)
#define decon_create_debugfs(...) do { } while(0)
#define decon_destroy_debugfs(..) do { } while(0)
#endif

/* HDR information of panel */
enum decon_hdr_type {
	HDR_NONE = 0,
	HDR_DOLBY_VISION = 1,
	HDR_HDR10 = 2,
	HDR_HLG = 3,
};

#define HDR_CAPA_NUM		4

struct decon_hdr_capabilities {
	unsigned int out_types[HDR_CAPA_NUM];
};

struct decon_hdr_capabilities_info {
	int out_num;
	int max_luminance;
	int max_average_luminance;
	int min_luminance;
};

struct decon_resources {
	int irq;
	void __iomem *regs;
	void __iomem *ss_regs;
	struct clk *aclk;
	struct clk *dpll;
	struct clk *pclk;
	struct clk *eclk;
	struct clk *eclk_leaf;
	struct clk *vclk;
	struct clk *vclk_leaf;
	struct clk *busp;
	struct clk *busd;
	struct clk *busc;
	struct clk *core;
	struct pinctrl *pinctrl;
	struct pinctrl_state *hw_te_on;
	struct pinctrl_state *hw_te_off;
};

struct decon_dt_info {
	enum decon_psr_mode psr_mode;
	enum decon_trig_mode trig_mode;
	enum decon_dsi_mode dsi_mode;
	enum decon_out_type out_type;
	int out_idx[MAX_DSIM_CNT];
	int max_win;
	int dft_win;
	int dft_idma;
};

struct decon_win {
	struct decon_device *decon;
	struct fb_info *fbinfo;

	struct fb_videomode videomode;
	struct decon_dma_buf_data dma_buf_data[MAX_PLANE_CNT];
#if defined(CONFIG_FB_TEST)
	struct decon_dma_buf_data fb_buf_data;
#endif
	int plane_cnt;

	int idx;
	int dpp_id;
	u32 pseudo_palette[16];
};
/* cursor async */
struct decon_user_window {
	int x;
	int y;
};

struct dpu_afbc_info {
	dma_addr_t dma_addr[2];
	bool is_afbc[2];
	void *v_addr[2];
	int size[2];
};

struct decon_debug {
	void __iomem *eint_pend;
	struct dentry *debug_root;
#ifdef CONFIG_DECON_EVENT_LOG
	struct dentry *debug_event;
	struct dentry *debug_dump;
	struct dentry *debug_bts;
	struct dentry *debug_win;
	struct dentry *debug_systrace;
#if defined(CONFIG_DSIM_CMD_TEST)
	struct dentry *debug_cmd;
#endif
	struct dentry *debug_recovery_cnt;
	struct dentry *debug_cmd_lp_ref;
	struct dentry *debug_mres;

	struct dpu_log *event_log;
	u32 event_log_cnt;
	atomic_t event_log_idx;
	dpu_log_level_t event_log_level;
#endif
	struct dpu_afbc_info prev_afbc_info;
	struct dpu_afbc_info cur_afbc_info;
	struct ion_handle *handle[MAX_DECON_WIN][MAX_PLANE_CNT];
	int prev_vgf_win_id[2];
};

struct decon_update_regs {
	struct mutex lock;
	struct list_head list;
	struct list_head saved_list;
	struct task_struct *thread;
	struct kthread_worker worker;
	struct kthread_work work;
	atomic_t remaining_frame;
};

struct decon_vsync {
	wait_queue_head_t wait;
	ktime_t timestamp;
	bool active;
	int irq_refcount;
	struct mutex lock;
	struct task_struct *thread;
};

#if defined(CONFIG_EXYNOS_COMMON_PANEL)
struct decon_fsync {
	wait_queue_head_t wait;
	ktime_t timestamp;
	bool active;
	int irq_refcount;
	struct mutex lock;
	struct task_struct *thread;
};
#endif

struct decon_hiber {
#if defined(CONFIG_EXYNOS_HIBERNATION_THREAD)
	wait_queue_head_t wait;
#endif
	ktime_t timestamp;
	struct mutex lock;
	struct task_struct *thread;
	struct kthread_worker worker;
	struct kthread_work work;
	atomic_t trig_cnt;
	atomic_t block_cnt;
	bool init_status;
	void __iomem *cam_status;
	u32 enter_cnt;
	u32 exit_cnt;
};

struct decon_win_update {
	bool enabled;
	u32 rect_w;
	u32 rect_h;
	u32 hori_cnt;
	u32 verti_cnt;
	/* previous update region */
	struct decon_rect prev_up_region;
};

struct decon_bts_ops {
	void (*bts_init)(struct decon_device *decon);
	void (*bts_calc_bw)(struct decon_device *decon,
			struct decon_reg_data *regs);
	void (*bts_update_bw)(struct decon_device *decon,
			struct decon_reg_data *regs, u32 is_after);
	void (*bts_acquire_bw)(struct decon_device *decon);
	void (*bts_release_bw)(struct decon_device *decon);
	void (*bts_deinit)(struct decon_device *decon);
};

struct decon_bts {
	bool enabled;
	u32 resol_clk;
	u32 bw[BTS_DPP_MAX];
	/* each decon must know other decon's BW to get overall BW */
	u32 ch_bw[3][BTS_DPU_MAX];
	u32 peak;
	u32 total_bw;
	u32 prev_total_bw;
	u32 max_disp_freq;
	u32 prev_max_disp_freq;
	enum bts_bw_type type;
	struct decon_bts_ops *ops;
	struct bts_decon_info bts_info;
	struct pm_qos_request mif_qos;
	struct pm_qos_request int_qos;
	struct pm_qos_request disp_qos;
	u32 scen_updated;
	u32 used_cnt;
};

/* cursor async */
struct decon_cursor {
	struct decon_reg_data regs;
	struct mutex lock;
	u32 xpos;
	u32 ypos;
	bool unmask;	/* if true, cursor unmask period */
	bool enabled;
};

/* systrace */
struct decon_systrace_data {
	pid_t pid;
};


#if !defined(CONFIG_SUPPORT_LEGACY_FENCE)
struct decon_fence {
	char name[8];
	u64 context;
	atomic_t timeline;
	spinlock_t lock;
};
#endif

struct vsync_applied_time_data {
	u32 config;
	u64 time;
	u32 reserved[4];
};

struct decon_device {
	int id;
	enum decon_state state;

	unsigned long prev_used_dpp;
	unsigned long cur_using_dpp;
	unsigned long dpp_err_stat;

	struct mutex lock;
	struct mutex pm_lock;
	spinlock_t slock;

	struct ion_client *ion_client;

#if defined(CONFIG_SUPPORT_LEGACY_FENCE)
	struct sync_timeline *timeline;
	int timeline_max;
#endif

	struct v4l2_subdev *out_sd[MAX_DSIM_CNT];
	struct v4l2_subdev *dsim_sd[MAX_DSIM_CNT];
	struct v4l2_subdev *dpp_sd[MAX_DPP_SUBDEV];
	struct v4l2_subdev *displayport_sd;
#ifdef CONFIG_EXYNOS_COMMON_PANEL
	struct v4l2_subdev *panel_sd;
	struct panel_state *panel_state;
#endif
	struct v4l2_device v4l2_dev;
	struct v4l2_subdev sd;

	struct device *dev;
	struct decon_dt_info dt;
	struct decon_win *win[MAX_DECON_WIN];
	struct decon_resources res;
	struct decon_debug d;
	struct decon_update_regs up;
	struct decon_vsync vsync;
#if defined(CONFIG_EXYNOS_COMMON_PANEL)
	struct decon_fsync fsync;
#endif
	struct decon_lcd *lcd_info;
	struct decon_win_update win_up;
	struct decon_hiber hiber;
	struct decon_bts bts;
	struct decon_cursor cursor;
#if !defined(CONFIG_SUPPORT_LEGACY_FENCE)
	struct decon_fence fence;
#endif

	int frame_cnt;
	int frame_cnt_target;
	wait_queue_head_t wait_vstatus;
	int eint_status;
#ifdef CONFIG_LOGGING_BIGDATA_BUG
	int eint_pend;
#endif
	int	update_regs_list_cnt;

	u32 prev_protection_bitmask;
	unsigned long prev_aclk_khz;

	atomic_t is_shutdown;
	bool up_list_saved;
	unsigned long prev_hdr_bits;
	struct exynos_hdr_static_info prev_hdr_info;
	enum hwc_ver ver;
#if defined(CONFIG_EXYNOS_COMMON_PANEL)
	atomic_t bypass;
	struct decon_reg_data last_regs;
#endif
#ifdef CONFIG_DISPLAY_USE_INFO
	struct notifier_block dpui_notif;
#endif
	/* systrace */
	struct decon_systrace_data systrace;

	bool mres_enabled;

	struct dsc_slice *dsw_info;
};

static inline struct decon_device *get_decon_drvdata(u32 id)
{
	return decon_drvdata[id];
}

/* register access subroutines */
static inline u32 decon_read(u32 id, u32 reg_id)
{
	struct decon_device *decon = get_decon_drvdata(id);
	return readl(decon->res.regs + reg_id);
}

static inline u32 decon_read_mask(u32 id, u32 reg_id, u32 mask)
{
	u32 val = decon_read(id, reg_id);
	val &= (mask);
	return val;
}

static inline void decon_write(u32 id, u32 reg_id, u32 val)
{
	struct decon_device *decon = get_decon_drvdata(id);
	writel(val, decon->res.regs + reg_id);
}

static inline void decon_write_mask(u32 id, u32 reg_id, u32 val, u32 mask)
{
	u32 old = decon_read(id, reg_id);

	val = (val & mask) | (old & ~mask);
	decon_write(id, reg_id, val);
}

static inline u32 dsc_read(u32 dsc_id, u32 reg_id)
{
	struct decon_device *decon = get_decon_drvdata(0);
	u32 dsc_offset = dsc_id ? DSC1_OFFSET : DSC0_OFFSET;

	return readl(decon->res.regs + dsc_offset + reg_id);
}

static inline void dsc_write(u32 dsc_id, u32 reg_id, u32 val)
{
	struct decon_device *decon = get_decon_drvdata(0);
	u32 dsc_offset = dsc_id ? DSC1_OFFSET : DSC0_OFFSET;

	writel(val, decon->res.regs + dsc_offset + reg_id);
}

static inline void dsc_write_mask(u32 dsc_id, u32 reg_id, u32 val, u32 mask)
{
	u32 old = dsc_read(dsc_id, reg_id);

	val = (val & mask) | (old & ~mask);
	dsc_write(dsc_id, reg_id, val);
}

static inline u32 sysreg_read(u32 id, u32 reg_id)
{
	struct decon_device *decon = get_decon_drvdata(id);
	return readl(decon->res.ss_regs + reg_id);
}

static inline u32 sysreg_read_mask(u32 id, u32 reg_id, u32 mask)
{
	u32 val = sysreg_read(id, reg_id);
	val &= (mask);
	return val;
}

static inline void sysreg_write(u32 id, u32 reg_id, u32 val)
{
	struct decon_device *decon = get_decon_drvdata(id);
	writel(val, decon->res.ss_regs + reg_id);
}

static inline void sysreg_write_mask(u32 id, u32 reg_id, u32 val, u32 mask)
{
	u32 old = sysreg_read(id, reg_id);

	val = (val & mask) | (old & ~mask);
	sysreg_write(id, reg_id, val);
}

/* common function API */
bool decon_validate_x_alignment(struct decon_device *decon, int x, u32 w,
		u32 bits_per_pixel);
int decon_wait_for_vsync(struct decon_device *decon, u32 timeout);
int decon_check_limitation(struct decon_device *decon, int idx,
		struct decon_win_config *config);

/* DECON to DSI interface functions */
int decon_register_irq(struct decon_device *decon);
int decon_get_clocks(struct decon_device *decon);
void decon_set_clocks(struct decon_device *decon);
int decon_get_out_sd(struct decon_device *decon);
int decon_get_pinctrl(struct decon_device *decon);
int decon_register_ext_irq(struct decon_device *decon);
int decon_create_vsync_thread(struct decon_device *decon);
void decon_destroy_vsync_thread(struct decon_device *decon);
#if defined(CONFIG_EXYNOS_COMMON_PANEL)
int decon_create_fsync_thread(struct decon_device *decon);
void decon_destroy_fsync_thread(struct decon_device *decon);
#endif
int decon_create_psr_info(struct decon_device *decon);
void decon_destroy_psr_info(struct decon_device *decon);

/* DECON to writeback interface functions */
int decon_wb_register_irq(struct decon_device *decon);
void decon_wb_free_irq(struct decon_device *decon);
int decon_wb_get_clocks(struct decon_device *decon);
void decon_wb_set_clocks(struct decon_device *decon);
int decon_wb_get_out_sd(struct decon_device *decon);

/* DECON to DISPLAYPORT interface functions */
int decon_displayport_register_irq(struct decon_device *decon);
void decon_displayport_free_irq(struct decon_device *decon);
int decon_displayport_create_vsync_thread(struct decon_device *decon);
int decon_displayport_get_clocks(struct decon_device *decon);
int decon_displayport_get_out_sd(struct decon_device *decon);
int decon_displayport_get_hdr_capa(struct decon_device *decon,
		struct decon_hdr_capabilities *hdr_capa);
int decon_displayport_get_hdr_capa_info(struct decon_device *decon,
		struct decon_hdr_capabilities_info *hdr_capa_info);
int decon_displayport_get_config(struct decon_device *dex,
		struct exynos_displayport_data *displayport_data);
int decon_displayport_set_config(struct decon_device *dex,
		struct exynos_displayport_data *displayport_data);

/* window update related function */
#define DPU_FULL_RECT(r, lcd)			\
	do {					\
		(r)->left = 0;			\
		(r)->top = 0;			\
		(r)->right = (lcd)->xres - 1;	\
		(r)->bottom = (lcd)->yres - 1;	\
	} while (0)
void dpu_init_win_update(struct decon_device *decon);
void dpu_prepare_win_update_config(struct decon_device *decon,
		struct decon_win_config_data *win_data,
		struct decon_reg_data *regs);
void dpu_set_win_update_config(struct decon_device *decon,
		struct decon_reg_data *regs);
void dpu_set_win_update_partial_size(struct decon_device *decon,
		struct decon_rect *up_region);

/* multi-resolution related function */
void dpu_set_mres_config(struct decon_device *decon, struct decon_reg_data *regs);

/* internal only function API */
int decon_check_var(struct fb_var_screeninfo *var, struct fb_info *info);
int decon_set_par(struct fb_info *info);
int decon_pan_display(struct fb_var_screeninfo *var, struct fb_info *info);
int decon_setcolreg(unsigned regno,
			    unsigned red, unsigned green, unsigned blue,
			    unsigned transp, struct fb_info *info);
int decon_mmap(struct fb_info *info, struct vm_area_struct *vma);

u32 wincon(int idx);

static inline u32 win_start_pos(int x, int y)
{
	return (WIN_STRPTR_Y_F(y) | WIN_STRPTR_X_F(x));
}

static inline u32 win_end_pos(int x, int y,  u32 xres, u32 yres)
{
	return (WIN_ENDPTR_Y_F(y + yres - 1) | WIN_ENDPTR_X_F(x + xres - 1));
}


/* HIBER releated */
int decon_exit_hiber(struct decon_device *decon);
int decon_enter_hiber(struct decon_device *decon);
int decon_lcd_off(struct decon_device *decon);
int decon_register_hiber_work(struct decon_device *decon);
int decon_hiber_block_exit(struct decon_device *decon);
u32 decon_reg_get_cam_status(void __iomem *cam_status);
#ifdef CONFIG_SUPPORT_HMD
bool is_hmd_running(struct decon_device *decon);
#endif

static inline void decon_hiber_block(struct decon_device *decon)
{
	if (decon)
		atomic_inc(&decon->hiber.block_cnt);
}

static inline bool decon_is_hiber_blocked(struct decon_device *decon)
{
	return (atomic_read(&decon->hiber.block_cnt) > 0);
}

static inline int decon_get_hiber_block_cnt(struct decon_device *decon)
{
	return (atomic_read(&decon->hiber.block_cnt));
}

static inline void decon_hiber_unblock(struct decon_device *decon)
{
	if (decon) {
		if (decon_is_hiber_blocked(decon))
			atomic_dec(&decon->hiber.block_cnt);
	}
}

static inline void decon_hiber_block_reset(struct decon_device *decon)
{
	if (decon)
		atomic_set(&decon->hiber.block_cnt, 0);
}

static inline void decon_hiber_trig_reset(struct decon_device *decon)
{
	if (decon)
		atomic_set(&decon->hiber.trig_cnt, 0);
}

static inline bool decon_min_lock_cond(struct decon_device *decon)
{
	return (atomic_read(&decon->hiber.block_cnt) <= 0);
}

static inline bool is_cam_not_running(struct decon_device *decon)
{
	if (!decon->id)
		return (!(decon_reg_get_cam_status(decon->hiber.cam_status) & 0xF));
	else
		return true;
}

static inline bool decon_hiber_enter_cond(struct decon_device *decon)
{
	return ((atomic_read(&decon->hiber.block_cnt) <= 0)
		&& is_cam_not_running(decon)
		&& is_displayport_not_running()
#ifdef CONFIG_SUPPORT_HMD
		&& is_hmd_running(decon)
#endif
		&& (atomic_inc_return(&decon->hiber.trig_cnt) >
			DECON_ENTER_HIBER_CNT));
}

static inline void decon_enter_shutdown(struct decon_device *decon)
{
	atomic_inc(&decon->is_shutdown);
}

static inline bool decon_is_enter_shutdown(struct decon_device *decon)
{
	return atomic_read(&decon->is_shutdown);
}

static inline void decon_enter_shutdown_reset(struct decon_device *decon)
{
	atomic_set(&decon->is_shutdown, 0);
}

#if defined(CONFIG_EXYNOS_COMMON_PANEL)
static inline void decon_set_bypass(struct decon_device *decon, bool on)
{
	atomic_set(&decon->bypass, !!on);
}

static inline void decon_bypass_on(struct decon_device *decon)
{
	atomic_inc(&decon->bypass);
}

static inline void decon_bypass_off(struct decon_device *decon)
{
	atomic_dec(&decon->bypass);
}

/* TODO : remove decon global function */
static inline void decon_set_bypass_global(u32 id, bool on)
{
	struct decon_device *decon = get_decon_drvdata(id);

	decon_set_bypass(decon, on);
}

static inline void decon_bypass_on_global(u32 id)
{
	struct decon_device *decon = get_decon_drvdata(id);

	decon_bypass_on(decon);
}

static inline void decon_bypass_off_global(u32 id)
{
	struct decon_device *decon = get_decon_drvdata(id);

	decon_bypass_off(decon);
}

static inline bool decon_is_bypass(struct decon_device *decon)
{
	return atomic_read(&decon->bypass);
}

int decon_reset_panel(struct decon_device *decon);

static inline int decon_reset_panel_global(u32 id)
{
	struct decon_device *decon = get_decon_drvdata(id);

	return decon_reset_panel(decon);
}
#endif

#ifdef CONFIG_LOGGING_BIGDATA_BUG
void log_decon_bigdata(struct decon_device *decon);
#endif

enum disp_pwr_mode {
	DISP_PWR_OFF = 0,
	DISP_PWR_DOZE,
	DISP_PWR_NORMAL,
	DISP_PWR_DOZE_SUSPEND,
	DISP_PWR_MAX,
};

typedef int (*set_pwr_state_t)(void *);

struct disp_pwr_state {
	u32 state;
	set_pwr_state_t set_pwr_state;
};

static inline bool IS_DECON_ON_STATE(struct decon_device *decon)
{
	return decon->state == DECON_STATE_INIT ||
		decon->state == DECON_STATE_ON ||
		decon->state == DECON_STATE_DOZE ||
		decon->state == DECON_STATE_TUI;
}

static inline bool IS_DECON_OFF_STATE(struct decon_device *decon)
{
	return decon->state == DECON_STATE_HIBER ||
		decon->state == DECON_STATE_DOZE_SUSPEND ||
		decon->state == DECON_STATE_OFF;
}

static inline bool IS_DECON_DOZE_STATE(struct decon_device *decon)
{
	return decon->state == DECON_STATE_DOZE ||
		decon->state == DECON_STATE_DOZE_SUSPEND;
}

static inline bool IS_DECON_HIBER_STATE(struct decon_device *decon)
{
	return decon->state == DECON_STATE_HIBER;
}

/* CAL APIs list */
//void decon_reg_init_probe(u32 id, u32 dsi_idx, struct decon_param *p);
int decon_reg_wait_update_done_timeout(u32 id, unsigned long timeout);
void decon_reg_set_blender_bg_image_size(u32 id, enum decon_dsi_mode dsi_mode,
		struct decon_lcd *lcd_info);
void decon_reg_config_data_path_size(u32 id, u32 width, u32 height,
		u32 overlap_w, struct decon_dsc *p, struct decon_param *param);
u32 dsc_get_dual_slice_mode(struct decon_lcd *lcd_info);
u32 dsc_get_slice_mode_change(struct decon_lcd *lcd_info);
void decon_reg_set_dispif_size(u32 id, u32 width, u32 height);
void decon_reg_clear_int_all(u32 id);
void decon_reg_all_win_shadow_update_req(u32 id);
void decon_reg_update_req_window(u32 id, u32 win_idx);
void decon_reg_set_win_enable(u32 id, u32 win_idx, u32 en);

/* tui feature support external to security driver(gud) */
int decon_tui_protection(bool tui_en);

/* helper functions */
int dpu_get_sd_by_drvname(struct decon_device *decon, char *drvname);
u32 dpu_translate_fmt_to_dpp(u32 format);
u32 dpu_get_bpp(enum decon_pixel_format fmt);
int dpu_get_meta_plane_cnt(enum decon_pixel_format format);
int dpu_get_plane_cnt(enum decon_pixel_format format, bool is_hdr);
u32 dpu_get_alpha_len(int format);
void dpu_unify_rect(struct decon_rect *r1, struct decon_rect *r2,
		struct decon_rect *dst);

void decon_dump(struct decon_device *decon);
void decon_to_psr_info(struct decon_device *decon, struct decon_mode_info *psr);
void decon_to_init_param(struct decon_device *decon, struct decon_param *p);
void decon_create_timeline(struct decon_device *decon, char *name);
void decon_create_release_fences(struct decon_device *decon,
		struct decon_win_config_data *win_data,
		struct sync_file *sync_file);
int decon_create_fence(struct decon_device *decon, struct sync_file **sync_file);
#if defined(CONFIG_SUPPORT_LEGACY_FENCE)
void decon_wait_fence(struct sync_file *fence);
void decon_signal_fence(struct decon_device *decon);
#else
void decon_wait_fence(struct dma_fence *fence);
void decon_signal_fence(struct dma_fence *fence);
#endif

bool decon_intersect(struct decon_rect *r1, struct decon_rect *r2);
int decon_intersection(struct decon_rect *r1,
		struct decon_rect *r2, struct decon_rect *r3);
void dpu_dump_data_to_console(void *v_addr, int buf_size, int id);

bool is_decon_rect_differ(struct decon_rect *r1, struct decon_rect *r2);
bool is_rgb32(int format);
bool is_scaling(struct decon_win_config *config);
bool is_full(struct decon_rect *r, struct decon_lcd *lcd);
bool is_decon_opaque_format(int format);
void __iomem *dpu_get_sysreg_addr(void);
u32 dpu_dma_type_to_channel(enum decon_idma_type type);
void dpu_dump_afbc_info(void);
#if defined(CONFIG_EXYNOS_CONTENT_PATH_PROTECTION)
void decon_set_protected_content(struct decon_device *decon,
		struct decon_reg_data *regs);
#endif

int decon_runtime_suspend(struct device *dev);
int decon_runtime_resume(struct device *dev);
void decon_dpp_stop(struct decon_device *decon, bool do_reset);

int decon_set_out_sd_state(struct decon_device *decon,
		enum decon_state state);
int decon_update_last_regs(struct decon_device *decon,
		struct decon_reg_data *regs);

/* cursor async mode functions */
void decon_set_cursor_reset(struct decon_device *decon,
		struct decon_reg_data *regs);
void decon_set_cursor_unmask(struct decon_device *decon, bool unmask);
void dpu_cursor_win_update_config(struct decon_device *decon,
		struct decon_reg_data *regs);
int decon_set_cursor_win_config(struct decon_device *decon, int x, int y);
void dpu_init_cursor_mode(struct decon_device *decon);

int dpu_sysmmu_fault_handler(struct iommu_domain *domain,
	struct device *dev, unsigned long iova, int flags, void *token);

int _decon_disable(struct decon_device *decon, enum decon_state state);

/* IOCTL commands */
#define S3CFB_SET_VSYNC_INT		_IOW('F', 206, __u32)
#define S3CFB_DECON_SELF_REFRESH	_IOW('F', 207, __u32)
#define S3CFB_WIN_CONFIG_OLD_ALT		_IOW('F', 209, \
						struct decon_win_config_data_old_alt)
#define S3CFB_WIN_CONFIG_OLD		_IOW('F', 209, \
						struct decon_win_config_data_old)
#define S3CFB_WIN_CONFIG_ALT		_IOW('F', 209, \
						struct decon_win_config_data_alt)
#define S3CFB_WIN_CONFIG		_IOW('F', 209, \
						struct decon_win_config_data)
#define EXYNOS_DISP_INFO		_IOW('F', 260, \
						struct decon_disp_info)
#define EXYNOS_DISP_RESTRICTIONS	_IOW('F', 261, \
						struct dpp_restrictions_info)

#define S3CFB_START_CRC			_IOW('F', 270, u32)
#define S3CFB_SEL_CRC_BITS		_IOW('F', 271, u32)
#define S3CFB_GET_CRC_DATA		_IOR('F', 272, u32)

#define EXYNOS_GET_DISPLAYPORT_CONFIG		_IOW('F', 300, \
						struct exynos_displayport_data)
#define EXYNOS_SET_DISPLAYPORT_CONFIG		_IOW('F', 301, \
						struct exynos_displayport_data)

#define EXYNOS_DPU_DUMP_OLD_ALT		_IOW('F', 302, \
						struct decon_win_config_data_old_alt)
#define EXYNOS_DPU_DUMP_OLD		_IOW('F', 302, \
						struct decon_win_config_data_old)
#define EXYNOS_DPU_DUMP_ALT		_IOW('F', 302, \
						struct decon_win_config_data_alt)
#define EXYNOS_DPU_DUMP		_IOW('F', 302, \
						struct decon_win_config_data)

#define S3CFB_POWER_MODE		_IOW('F', 223, __u32)

/* HDR support */
#define S3CFB_GET_HDR_CAPABILITIES _IOW('F', 400, struct decon_hdr_capabilities)
#define S3CFB_GET_HDR_CAPABILITIES_NUM _IOW('F', 401, struct decon_hdr_capabilities_info)

/* Display mode */
#define EXYNOS_GET_DISPLAY_MODE_NUM	_IOW('F', 700, u32)
#define EXYNOS_GET_DISPLAY_MODE		_IOW('F', 701, struct decon_display_mode)
#define EXYNOS_SET_DISPLAY_MODE		_IOW('F', 702, struct decon_display_mode)
#define EXYNOS_GET_DISPLAY_CURRENT_MODE	_IOW('F', 705, u32)

/* cursor async */
#define DECON_WIN_CURSOR_POS		_IOW('F', 222, struct decon_user_window)

/* DPU aclk */
#define EXYNOS_DPU_GET_ACLK		_IOR('F', 500, u32)

/* For HWC2.4 */
#define EXYNOS_GET_VSYNC_CHANGE_TIMELINE	_IOW('F', 850, struct vsync_applied_time_data)

#if defined(CONFIG_EXYNOS_COMMON_PANEL)
#define V4L2_EVENT_DECON                (V4L2_EVENT_PRIVATE_START + 1000)
#define V4L2_EVENT_DECON_FRAME_START    (V4L2_EVENT_DECON + 1)
#define V4L2_EVENT_DECON_FRAME_DONE     (V4L2_EVENT_DECON + 2)
#define V4L2_EVENT_DECON_VSYNC          (V4L2_EVENT_DECON + 3)
#endif

/* COLOR Mode */
#define EXYNOS_GET_COLOR_MODE_NUM	_IOW('F', 600, __u32)
#define EXYNOS_GET_COLOR_MODE		_IOW('F', 601, struct decon_color_mode_info)
#define EXYNOS_SET_COLOR_MODE		_IOW('F', 602, __u32)

#endif /* ___SAMSUNG_DECON_H__ */
