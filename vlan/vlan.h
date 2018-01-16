/************************************************************************************
 *  bcm 53134 vlan base on 802.1Q head file
 *
 *  Created on: 2017-11-16
 *
 *  Author: buggy
 */

#include "../phy/phy.h"

#define VTBL_INDEX_MAX 4096
#define TIME_OUT 99
#define NUM_PORT 9

/* vlan table index define */
typedef uint16_t BCM_VTBL_INDEX_T;

/* vlan table address define */
typedef uint16_t BCM_VLAN_VTBL_ADDR_T;

typedef uint16_t BCM_VLAN_PVID_T;
typedef uint32_t BCM_VLAN_T;

typedef BCM_PORT_T BCM_PORT_GROUP_T[NUM_PORT];

enum _BCM_VLAN_NUM
{
	VLAN_0 = 1,
	VLAN_1 = 2,
	VLAN_2 = 3,
	VLAN_3 = 4,
	VLAN_4 = 5,
	VLAN_5 = 6,
	VLAN_6 = 7
};

enum _BCM_PORT_STATUS_T
{
	PORT_UNUSE = 0,
	PORT_USE = 1
};

/*PVID number*/
enum _BCM_VLAN_VID
{
	PVID_0 = 1,
	PVID_1 = 2,
	PVID_2 = 3,
	PVID_3 = 4,
	PVID_4 = 5,
	PVID_5 = 6,
	PVID_6 = 7,
	PVID_7 = 8
};

typedef enum _BCM_VLAN_RRI
{
    VLAN_PRI_0 = 0,
    VLAN_PRI_1 = 1,
    VLAN_PRI_2 = 2,
}BCM_VLAN_PRI_T;

/* vlan read or write enumuner*/
typedef enum _BCM_VLAN_RW
{
	VLAN_WRITE = 0x80,
	VLAN_READ = 0x81,
	VLAN_CLEAR = 0x82
}BCM_VLAN_RW_T;

/* vlan function status */
typedef enum BCM_VLAN_FUNC_STATUS
{
	/*Normal status*/
	VLAN_ENABLE = 1,
	VLAN_DISABLE = 0,

	VLAN_ALL_JOIN_ENABLE = 1,
	VLAN_ALL_JOIN_DISABLE = 0,

	/*VTBL_control*/
	VLAN_TBL_INIT_START = 1,
	VLAN_TBL_INIT_FINISH = 0,

    /*VLAN sign*/
	VLAN_TBL_CLEAR = 2,
	VLAN_TBL_READ = 1,
	VLAN_TBL_WRITE = 0,
	VLAN_TBL_MODE_ARL = 0,
	VLAN_TBL_MODE_MEM = 1,

	/*VTBL status*/
	PORT_UNTAG = 1,
	PORT_TAG = 0,
	PORT_FWD = 1,
	PORT_UNFWD = 0,

	VLAN_TAG_REMOVE = 2,

    /*spinning tree*/
	NO_SPANNING_TREE = 0,
	SPANNING_TREE_DISABLE = 1,
	SPANNING_TREE_BLOCKING = 2,
	SPANNING_TREE_LISTENING = 3,
	SPANNING_TREE_LEARNING = 4,
	SPANNING_TREE_FORWARD = 5,

	/*VLAN multiport address ctrl*/
	MPORT_MAP_TAG = 3,
	MPORT_MAP_FWD = 2,

	MPORT_MAP_UNTAG = 1,
	MPORT_MAP_UNFWD = 0,

	/*VLAN double tag*/
	DOUBLE_TAG_DISABLE = 0,
	DT_MODE_ENABLE = 1,
	IDT_MODE_ENABLE = 2,

	/*ISP port*/
	ISP_PORT = 1,
	CUSTOMER_PORT = 0,

}BCM_VLAN_FUNC_STATUS_T;

/* vlan table entry structure */
typedef struct BCM_VTBL_ENTRY
{
	uint8_t forward_mode;
	uint16_t untag_port;
	uint16_t forward_map;
}BCM_VTBL_ENTRY_T;


/* vlan error status*/
typedef enum _BCM_VLAN_ERROR
{
	VLAN_VTBL_INDEX_BORDER = 0
}BCM_VLAN_WORK_STATE_T;

/*  chip register address number*/
typedef enum _BCM_SPI_PAGE_VLAN_SPI_PAGE
{
	BCM_SPI_PAGE_VTBL = 0x5,
	BCM_SPI_PAGE_VLAN = 0x34,

	BCM_MSTP_REG = 0x43
}BCM_SPI_PAGE_VLAN_T;

/* chip register offset number*/
typedef enum _BCM_VLAN_SPI_OFFSET
{
	/*VLAN table offset*/
	BCM_SPI_OFFSET_VTBL_CTRL = 0x80,
	BCM_SPI_OFFSET_VTBL_INDEX = 0x81,
	BCM_SPI_OFFSET_VTBL_ENTRY = 0x83,

	/*vlan control offset*/
	BCM_SPI_OFFSET_VLAN_CTRL_0 = 0x0,
	BCM_SPI_OFFSET_VLAN_CTRL_1 = 0x1,
	BCM_SPI_OFFSET_VLAN_CTRL_2 = 0x2,
	BCM_SPI_OFFSET_VLAN_CTRL_3 = 0x3,
	BCM_SPI_OFFSET_VLAN_CTRL_4 = 0x5,
	BCM_SPI_OFFSET_VLAN_CTRL_5 = 0x6,
	BCM_SPI_OFFSET_VLAN_CTRL_6 = 0x7,

	/*tag info offset*/
	BCM_SPI_OFFSET_VLAN_TAG_INFO = 0x10,
	BCM_SPI_OFFSET_VLAN_TAG_INFO_IMP = 0x20,

    /*ISP port set*/
	BCM_SPI_OFFSET_VLAN_ISP_PORTMAP = 0x32,

    /*egree vid remark table*/
	BCM_SPI_OFFSET_VLAN_EGRESS_VID_RMK_TBL_ACS = 0x40,
	BCM_SPI_OFFSET_VLAN_EGRESS_VID_RMK_TBL_DAT = 0x44,

    /*mult port address control*/
	BCM_SPI_OFFSET_VLAN_MULTI_PORT_ADDR_CTL = 0xa,

    /*spinning tree set*/
	BCM_SPI_OFFSET_MSPT = 0x10,

    /*join all vlan set*/
	BCM_SPI_OFFSET_JOIN_ALL_VLAN = 0x50,

    /*pvid offset*/
	BCM_SPI_OFFSET_VID_PORT_0 = 0x10,
	BCM_SPI_OFFSET_VID_PORT_1 = 0x12,
	BCM_SPI_OFFSET_VID_PORT_2 = 0x14,
	BCM_SPI_OFFSET_VID_PORT_3 = 0x16,
	BCM_SPI_OFFSET_VID_PORT_4 = 0x18,
	BCM_SPI_OFFSET_VID_PORT_5 = 0x1A,
	BCM_SPI_OFFSET_VID_PORT_7 = 0x1E,
	BCM_SPI_OFFSET_VID_PORT_IMP = 0x20,

}BCM_SPI_OFFSET_VLAN_T;

/* chip register offset number*/
typedef enum _BCM_SPI_PAGE_VLAN_BIT_OFFSET
{
	/*vlan table control*/
	BCM_BIT_OFFSET_VTBL_CTRL_FWD_MODE = 21,
	BCM_BIT_OFFSET_VTBL_CTRL_STDN = 7,
	BCM_BIT_OFFSET_VTBL_CTRL_RWCL = 0,

	/*vlan table index*/
	BCM_BIT_OFFSET_VTBL_INDEX = 0,

	/*vlan table entry*/
	BCM_BIT_OFFSET_VTBL_ENTRY_FWD_MOD = 21,
	BCM_BIT_OFFSET_VTBL_ENTRY_MSTP_INDEX = 18,
	BCM_BIT_OFFSET_VTBL_ENTRY_UNTAG_MAP = 9,
	BCM_BIT_OFFSET_VTBL_ENTRY_FWD_MAP = 0,

	BCM_BIT_OFFSET_VTBL_ENTRY_FWD_MAP_PORT_0 = 0,
	BCM_BIT_OFFSET_VTBL_ENTRY_FWD_MAP_PORT_1 = 1,
	BCM_BIT_OFFSET_VTBL_ENTRY_FWD_MAP_PORT_2 = 2,
	BCM_BIT_OFFSET_VTBL_ENTRY_FWD_MAP_PORT_3 = 3,
	BCM_BIT_OFFSET_VTBL_ENTRY_FWD_MAP_PORT_4 = 4,
	BCM_BIT_OFFSET_VTBL_ENTRY_FWD_MAP_PORT_5 = 5,
	BCM_BIT_OFFSET_VTBL_ENTRY_FWD_MAP_PORT_8 = 8,

	BCM_BIT_OFFSET_VTBL_ENTRY_TAG_MAP_PORT_0 = 9,
	BCM_BIT_OFFSET_VTBL_ENTRY_TAG_MAP_PORT_1 = 10,
	BCM_BIT_OFFSET_VTBL_ENTRY_TAG_MAP_PORT_2 = 11,
	BCM_BIT_OFFSET_VTBL_ENTRY_TAG_MAP_PORT_3 = 12,
	BCM_BIT_OFFSET_VTBL_ENTRY_TAG_MAP_PORT_4 = 13,
	BCM_BIT_OFFSET_VTBL_ENTRY_TAG_MAP_PORT_5 = 14,
	BCM_BIT_OFFSET_VTBL_ENTRY_TAG_MAP_PORT_8 = 17,

	/*CTRL 0 offset*/
	BCM_VLAN_BIT_OFFSET_VLAN_ENABLE = 7,
	BCM_VLAN_BIT_OFFSET_VLAN_1Q_CHANGE = 3,
	BCM_VLAN_BIT_OFFSET_VLAN_1Q_CHANGE_OUT = 1,
	BCM_VLAN_BIT_OFFSET_VLAN_1Q_CHANGE_IN = 0,

	/*CTRL 1 offset*/
	BCM_VLAN_BIT_OFFSET_IPMC_TAG_CTRL = 6,
	BCM_VLAN_BIT_OFFSET_IPMC_FWD_CTRL = 5,
	BCM_VLAN_BIT_OFFSET_MULTICAST_TAG_CTRL = 3,
	BCM_VLAN_BIT_OFFSET_MULTICAST_FWD_CTRL = 2,

	/*CTRL 2 offset*/
	BCM_VLAN_BIT_OFFSET_GG_TAG_CTRL = 6,
	BCM_VLAN_BIT_OFFSET_GG_CHK_FWD_MAP = 5,
	BCM_VLAN_BIT_OFFSET_MII_CHK_FWD_MAP = 2,

	/*CTRL 5 offset*/
	BCM_VLAN_BIT_OFFSET_DOUBLE_TAG = 2,

    /*VLAN forward map port offset*/
	BCM_VLAN_BIT_OFFSET_VLAN_FORWARD_PORT_0 = 0,
	BCM_VLAN_BIT_OFFSET_VLAN_FORWARD_PORT_1 = 1,
	BCM_VLAN_BIT_OFFSET_VLAN_FORWARD_PORT_2 = 2,
	BCM_VLAN_BIT_OFFSET_VLAN_FORWARD_PORT_3 = 3,
	BCM_VLAN_BIT_OFFSET_VLAN_FORWARD_PORT_4 = 4,
	BCM_VLAN_BIT_OFFSET_VLAN_FORWARD_PORT_5 = 5,
	BCM_VLAN_BIT_OFFSET_VLAN_FORWARD_PORT_7 = 7,
	BCM_VLAN_BIT_OFFSET_VLAN_FORWARD_PORT_IMP = 8,

    /*VLAN untag map port offset*/
	BCM_VLAN_BIT_OFFSET_VLAN_UNTAG_PORT_0 = 9,
	BCM_VLAN_BIT_OFFSET_VLAN_UNTAG_PORT_1 = 10,
	BCM_VLAN_BIT_OFFSET_VLAN_UNTAG_PORT_2 = 11,
	BCM_VLAN_BIT_OFFSET_VLAN_UNTAG_PORT_3 = 12,
	BCM_VLAN_BIT_OFFSET_VLAN_UNTAG_PORT_4 = 13,
	BCM_VLAN_BIT_OFFSET_VLAN_UNTAG_PORT_5 = 14,
	BCM_VLAN_BIT_OFFSET_VLAN_UNTAG_PORT_7 = 15,
	BCM_VLAN_BIT_OFFSET_VLAN_UNTAG_PORT_IMP = 17,

	BCM_VLAN_BIT_OFFSET_VLAN_TAG_INFO_VID = 0,
	BCM_VLAN_BIT_OFFSET_VLAN_TAG_INFO_PRI = 13,

    /*egress port VID remark table access offset*/
	BCM_BIT_OFFSET_VLAN_EGRESS_VID_RMK_TBL_ACS_ADR  = 8,
	BCM_BIT_OFFSET_VLAN_EGRESS_VID_RMK_TBL_ACS_EPORT = 4,
	BCM_BIT_OFFSET_VLAN_EGRESS_VID_RMK_TBL_ACS_OPT = 1,
	BCM_BIT_OFFSET_VLAN_EGRESS_VID_RMK_TBL_ACS_START  = 0,

    /*egress port VID remark table data offset*/
	BCM_BIT_OFFSET_VLAN_EGRESS_VID_RMK_TBL_DAT_OUT_TAG  = 28,
	BCM_BIT_OFFSET_VLAN_EGRESS_VID_RMK_TBL_DAT_OUT_VID = 16,
	BCM_BIT_OFFSET_VLAN_EGRESS_VID_RMK_TBL_DAT_IN_TAG = 12,
	BCM_BIT_OFFSET_VLAN_EGRESS_VID_RMK_TBL_DAT_IN_VID = 0,

    /*mst port offset*/
	BCM_BIT_OFFSET_MST_TAB_PORT_7 = 21,
	BCM_BIT_OFFSET_MST_TAB_PORT_5 = 15,
	BCM_BIT_OFFSET_MST_TAB_PORT_4 = 12,
	BCM_BIT_OFFSET_MST_TAB_PORT_3 = 9,
	BCM_BIT_OFFSET_MST_TAB_PORT_2 = 6,
	BCM_BIT_OFFSET_MST_TAB_PORT_1 = 3,
	BCM_BIT_OFFSET_MST_TAB_PORT_0 = 0,

	/*VLAN multi addr */
	BCM_BIT_OFFSET_VLAN_MULITI_ADDR_PORT_5_TAG = 11,
	BCM_BIT_OFFSET_VLAN_MULITI_ADDR_PORT_5_FWD = 10,
	BCM_BIT_OFFSET_VLAN_MULITI_ADDR_PORT_4_TAG = 9,
	BCM_BIT_OFFSET_VLAN_MULITI_ADDR_PORT_4_FWD = 8,
	BCM_BIT_OFFSET_VLAN_MULITI_ADDR_PORT_3_TAG = 7,
	BCM_BIT_OFFSET_VLAN_MULITI_ADDR_PORT_3_FWD = 6,
	BCM_BIT_OFFSET_VLAN_MULITI_ADDR_PORT_2_TAG = 5,
	BCM_BIT_OFFSET_VLAN_MULITI_ADDR_PORT_2_FWD = 4,
	BCM_BIT_OFFSET_VLAN_MULITI_ADDR_PORT_1_TAG = 3,
	BCM_BIT_OFFSET_VLAN_MULITI_ADDR_PORT_1_FWD = 2,
	BCM_BIT_OFFSET_VLAN_MULITI_ADDR_PORT_0_TAG = 1,
	BCM_BIT_OFFSET_VLAN_MULITI_ADDR_PORT_0_FWD = 0,

    /*join all vlan function*/
	BCM_BIT_OFFSET_JOIN_ALL_VLAN_PORT_8 = 8,
	BCM_BIT_OFFSET_JOIN_ALL_VLAN_PORT_7 = 7,
	BCM_BIT_OFFSET_JOIN_ALL_VLAN_PORT_6 = 6,
	BCM_BIT_OFFSET_JOIN_ALL_VLAN_PORT_5 = 5,
	BCM_BIT_OFFSET_JOIN_ALL_VLAN_PORT_4 = 4,
	BCM_BIT_OFFSET_JOIN_ALL_VLAN_PORT_3 = 3,
	BCM_BIT_OFFSET_JOIN_ALL_VLAN_PORT_2 = 2,
	BCM_BIT_OFFSET_JOIN_ALL_VLAN_PORT_1 = 1,
	BCM_BIT_OFFSET_JOIN_ALL_VLAN_PORT_0 = 0,

	/*pvid offset*/
	BCM_BIT_OFFSET_TAG_VID = 0,
	BCM_BIT_OFFSET_TAG_PRI = 13,

}BCM_SPI_PAGE_VLAN_BIT_OFFSET_T;

/* function bit wide */
typedef enum _BCM_SPI_PAGE_VLAN_BIT_WIDE
{
	BCM_VLAN_BIT_1 = 0x1,
	BCM_VLAN_BIT_2 = 0x2,
	BCM_VLAN_BIT_3 = 0x3,
	BCM_VLAN_BIT_8 = 0x8,
}BCM_SPI_PAGE_VLAN_BIT_WIDE_T;

/*tag information structure*/
typedef struct _BCM_VLAN_TAG_INFO
{
	BCM_SPI_PAGE_VLAN_T priority;
	BCM_SPI_PAGE_VLAN_T VLAN_ID;
}BCM_VLAN_TAG_INFO_T;

typedef struct _BCM_VLAN_EGRESS_VID_RMK_TBL_ACS
{
	BCM_SPI_PAGE_VLAN_T table_address;
	BCM_SPI_PAGE_VLAN_T egress_port;
	BCM_SPI_PAGE_VLAN_T operation;
	BCM_SPI_PAGE_VLAN_T start_done;
}BCM_VLAN_EGRESS_VID_RMK_TBL_ACS_T;

/*egress vid remark data access structure*/
typedef struct _BCM_VLAN_EGRESS_VID_RMK_DATA_ACS
{
	BCM_SPI_PAGE_VLAN_T outer_tag_operation;
	BCM_SPI_PAGE_VLAN_T outer_VID;

	BCM_SPI_PAGE_VLAN_T inner_tag_operation;
	BCM_SPI_PAGE_VLAN_T inner_VID;

}BCM_VLAN_EGRESS_VID_RMK_TBL_DAT_T;

/*vlan table control structure*/
typedef struct _BCM_VLAN_VTBL_CTRL_T
{
	BCM_FUNC_T start_done;
	BCM_FUNC_T func;
}BCM_VLAN_VTBL_CTRL_T;

/*vlan table function */
typedef struct _BCM_VLAN_VTBL_FUNC_T
{
	BCM_REG_T fwd_mode;
	BCM_REG_T mstp_index;
	BCM_REG_T untag_map;
	BCM_REG_T fwd_map;
}BCM_VLAN_VTBL_FUNC_T;

typedef struct _BCM_VLAN_VTBL_T
{
	BCM_VLAN_VTBL_CTRL_T ctrl;
	BCM_VLAN_VTBL_ADDR_T vtbl_addr_index;
	BCM_VLAN_VTBL_FUNC_T func;
}BCM_VLAN_VTBL_T;

/******************************************************************************************/
/* vlan status class:
 */
typedef struct
{
	void (* status_single_read)(BCM_VLAN_T );
	BCM_REG_T (* enable_read)();
	BCM_REG_T (* mst_table_read)( BCM_PORT_T );
	BCM_REG_T (* join_all_vlan_en_read)(BCM_PORT_T );
}bcm_vlan_status_op_t;

typedef struct
{

	bcm_vlan_status_op_t operate;
}bcm_vlan_status_t;


/******************************************************************************************/
/* vlan create class:
 */

typedef struct {
	BCM_VLAN_PVID_T pvid;
	BCM_PORT_T      port_group[NUM_PORT];
	BCM_VLAN_T      vlan_index;
	BCM_REG_T       vlan_mode;
	BCM_REG_T       mst_mode;
}bcm_vlan_creat_data_t;

typedef struct {
	BCM_PORT_GROUP_T port_group;
	BCM_VLAN_PVID_T  pvid;
	BCM_REG_T       mst_mode;
};

typedef struct _bcm_vlan_creat
{
	BCM_FUNC_OP_T (* pvid_write)(BCM_PORT_T port_number, BCM_VLAN_PVID_T vid );
	BCM_FUNC_OP_T (* mst_table_write)(BCM_PORT_T port_number, BCM_REG_T mode);
	void          (* table_single_write)( BCM_VLAN_VTBL_FUNC_T *vtbl_entry,BCM_VLAN_T vtbl_index);

}bcm_vlan_creat_op_t;

typedef struct {
	bcm_vlan_creat_data_t   data;
	bcm_vlan_creat_op_t     operate;
}bcm_vlan_creat_t;

/******************************************************************************************/
/* vlan class:
 */
typedef struct {
	bcm_vlan_creat_data_t creat;
	BCM_REG_T enable;
	BCM_VLAN_T num_vlan;
}bcm_vlan_data_t;

typedef struct _bcm_vlan
{
    void      (* create)( bcm_vlan_creat_data_t * );
	BCM_REG_T (* enable)( BCM_REG_T );
	void      (* status)( BCM_VLAN_T );
}bcm_vlan_operat_t;

typedef struct {
	bcm_vlan_data_t   data;
	bcm_vlan_operat_t operate;
}bcm_vlan_t;


