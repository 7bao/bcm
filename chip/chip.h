/************************************************************************************
 *  bcm 53134 chip operation headfile
 *
 *  Created on: 2017-08-16
 *
 *  Author: buggy
 */

#include <stdint.h>

/* debug error store */
#define DEBUG_CHIP_ERR

typedef uint64_t BCM_PORT_REG_T;

/* port */
typedef enum _BCM_PORT
{
	PORT_0 = 0,
	PORT_1,
	PORT_2,
	PORT_3,
	PORT_4,
	PORT_5,
	PORT_6,
	PORT_7,
	IMP,
}BCM_PORT_T;

/* chip register page number */
typedef enum _BCM_REG_PAGE
{
	BCM_CHIP_REG_CTRL = 0x0,
	BCM_CHIP_REG_MANA = 0x2,
}BCM_REG_PAGE_T;

/*  chip register address number*/
typedef enum _BCM_REG_ADR
{
	BCM_CHIP_ADR_DELAY = 0x60,
	BCM_CHIP_ADR_CAST = 0x8,
	BCM_CHIP_ADR_SOFTWARE_FORWARD = 0xb,
	
	BCM_CHIP_ADR_FLOW_CTRL = 0xe,
	BCM_CHIP_ADR_MII_OVERRIDE = 0xe,
	
	BCM_CHIP_ADR_RESET_CNT = 0x0,
	BCM_CHIP_ADR_ENABLE_IMP = 0x0,
	BCM_CHIP_ADR_ENABLE_BPDU = 0x0,

	BCM_CHIP_ADR_BRCM_HEADER = 0x3,
}BCM_REG_ADR_T;

/* chip register offset number*/
typedef enum _BCM_REG_OFFSET
{
	BCM_CHIP_OFFSET_UNICAST = 4,
	BCM_CHIP_OFFSET_MULTICAST = 3,
	BCM_CHIP_OFFSET_BROADCAST = 2,
	BCM_CHIP_OFFSET_SOFTWARE_FORWARD_MODE = 0,
	BCM_CHIP_OFFSET_SOFTWARE_FORWARD_ENABLE = 1,
	BCM_CHIP_OFFSET_FLOW_CTRL_TX = 5,
	BCM_CHIP_OFFSET_FLOW_CTRL_RX = 4,
	BCM_CHIP_OFFSET_MII_OVERRIDE = 7,
	BCM_CHIP_OFFSET_RESET_CNT = 0,
	BCM_CHIP_OFFSET_ENABLE_IMP = 6,
	BCM_CHIP_OFFSET_ENABLE_BPDU = 1,
	BCM_CHIP_OFFSET_BRCM_HEADER_PORT_7 = 2,
	BCM_CHIP_OFFSET_BRCM_HEADER_PORT_5 = 1,
	BCM_CHIP_OFFSET_BRCM_HEADER_PORT_8 = 0,
	BCM_CHIP_OFFSET_DELAY_DLL = 1,
	BCM_CHIP_OFFSET_DELAY_RGMII = 0,
}BCM_REG_OFFSET_T;

/*function status*/
enum BCM_CHIP_WORK_STATUS_GENERAL
{
	DISABLE = 0,
	ENABLE = 1
};

/*function status*/
enum BCM_CHIP_WORK_STATUS_SOFTWARE_FORWARD_MODE
{
	UNMANA_MODE = 0, 
	MANA_MODE = 1
};

/*function status*/
enum BCM_CHIP_WORK_STATUS_FLOW_CTRL
{
	NO_PAUSE = 0,
	PAUSE = 1
};

/*function status*/
enum BCM_CHIP_WORK_STATUS_MII_SOFTWARE_OVERRIDE
{
	HARDWARE_PIN = 0,
	CONTENTS = 1
};

/*function status*/
enum IMP_PORT
{
	NO_IMP_PORT = 0,
	ENABLE_IMP_PORT = 2,
};

/*function status*/
enum BCM_CHIP_WORK_STATUS_BRCM_HEADER
{
	NO_HEADER = 0,
	HEADER = 1
};

/*function status*/
enum BCM_CHIP_WORK_STATUS_GEN_DELAY_MODE
{	
	NORMAL_MODE = 0,
	DELAY_MODE = 1
};

/*function status*/
typedef enum BCM_CHIP_CHECK_STATUS
{
	ALL_CHIP_STATUS_OK = 0,
	ALL_CHIP_STATUS_ERR = 1
}BCM_CHIP_CHECK_STATUS_T;

/* type define */
typedef BCM_REG_ADR_T BCM_CHIP_FUNC_STATUS_T;
typedef BCM_REG_ADR_T BCM_CHIP_FUNC_MODE_T;
typedef int32_t BCM_CHIP_FUNC_ERR_STATUS_T;
typedef int32_t BCM_CHIP_FUNC_ERR_NUMBER_T;



/*function status structure*/
typedef struct _BCM_CHIP_WORK_STATUS
{
	BCM_CHIP_FUNC_STATUS_T receive_unicast;
	BCM_CHIP_FUNC_STATUS_T receive_multicast;
	BCM_CHIP_FUNC_STATUS_T receive_broadcast;
	BCM_CHIP_FUNC_STATUS_T software_forwarding_mode;
	BCM_CHIP_FUNC_STATUS_T software_forwarding_enable;
	BCM_CHIP_FUNC_STATUS_T tx_flow_control;
	BCM_CHIP_FUNC_STATUS_T rx_flow_control;
	BCM_CHIP_FUNC_STATUS_T mii_software_override;
	//BCM_CHIP_FUNC_STATUS_T reset_cnt;
	BCM_CHIP_FUNC_STATUS_T imp_port_enable;
	BCM_CHIP_FUNC_STATUS_T receive_bpdu_enable;
	BCM_CHIP_FUNC_STATUS_T brcm_header_port7;
	BCM_CHIP_FUNC_STATUS_T brcm_header_port5;
	BCM_CHIP_FUNC_STATUS_T brcm_header_port8;
	BCM_CHIP_FUNC_STATUS_T dll_clock_delay;
	BCM_CHIP_FUNC_STATUS_T rgmii_tx_clk_delayed;

}BCM_CHIP_WORK_STATUS_T;

/*error store position enum*/
enum _BCM_CHIP_WORK_STATUS_POS
{
	 RECEIVE_UNICAST_POS = 0,
	 RECEIVE_MULTICAST_POS = 1,
	 RECEIVE_BROADCAST_POS,
	 SOFTWARE_FORWARDING_MODE_POS,
	 SOFTWARE_FORWARDING_ENABLE_POS,
	 TX_FLOW_CONTROL_POS,
	 RX_FLOW_CONTROL_POS,
	 MII_SOFTWARE_OVERRIDE_POS,
	 IMP_PORT_ENABLE_POS,
	 RECEIVE_BPDU_ENABLE_POS,
	 BRCM_HEADER_PORT7_POS,
	 BRCM_HEADER_PORT5_POS,
	 BRCM_HEADER_PORT8_POS,
	 DLL_CLOCK_DELAY_POS,
	 RGMII_TX_CLK_DELAYED_POS
};

enum _ERROR_LEVEL
{
	LEVLE_1,
	LEVLE_2,
	LEVLE_3
}ERROR_LEVEL;

/*error store structure*/
typedef struct _BCM_CHIP_ERR_STATUS
{
	BCM_CHIP_FUNC_STATUS_T  error_status[15];
	BCM_CHIP_FUNC_ERR_NUMBER_T error_pos[15];
	BCM_CHIP_FUNC_ERR_NUMBER_T error_cnt;
}BCM_CHIP_ERR_STATUS_T;

//typedef struct _BCM_CHIP_STATUS
//{
//	BCM_CHIP_WORK_STATUS_T bcm_chip_work_status;
//
//}BCM_CHIP_STATUS_T;


