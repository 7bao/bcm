/************************************************************************************
 *  bcm 53134 port operation headfile
 *
 *  Created on: 2017-08-16
 *
 *  Author: buggy
 */

#include <stdint.h>

/* port count */
#define PORT_COUNT 4

/* debug switch */
#define DEBUG_PORT_ERR
#define DEBUG_SPD
//#define DEBUG_DUP

/* chip register page number */
enum BCM_REG_PAGE
{
	BCM_CHIP_REG_IMP = 0x0,
	BCM_CHIP_REG_STATUS = 0x1,
};

/*  chip register address number*/
enum BCM_REG_ADR
{
	BCM_CHIP_ADR_LINK = 0x0,
	BCM_CHIP_ADR_SPD = 0x4,
	BCM_CHIP_ADR_DUP = 0x8,
	BCM_CHIP_ADR_IMP = 0xe,
};

/* chip register offset number*/
enum BCM_PORT_OFFSET_SPD
{
	PORT_0_OFFSET_SPD = 0,
	PORT_1_OFFSET_SPD = 2,
	PORT_2_OFFSET_SPD = 4,
	PORT_3_OFFSET_SPD = 6,
	PORT_4_OFFSET_SPD = 8,
	PORT_5_OFFSET_SPD = 10,

	PORT_7_OFFSET_SPD = 14,
	PORT_8_OFFSET_SPD = 16,
};

/* chip register offset number*/
enum BCM_PORT_OFFSET_DUP
{
	PORT_0_OFFSET_DUP = 0,
	PORT_1_OFFSET_DUP = 1,
	PORT_2_OFFSET_DUP = 2,
	PORT_3_OFFSET_DUP = 3,
	PORT_4_OFFSET_DUP = 4,
	PORT_5_OFFSET_DUP = 5,

	PORT_7_OFFSET_DUP = 7,
	PORT_8_OFFSET_DUP = 8,

};

/* chip register offset number*/
enum BCM_PORT_OFFSET_LINK
{
	PORT_0_OFFSET_LINK = 0,
	PORT_1_OFFSET_LINK = 1,
	PORT_2_OFFSET_LINK = 2,
	PORT_3_OFFSET_LINK = 3,
	PORT_4_OFFSET_LINK = 4,
	PORT_5_OFFSET_LINK = 5,

	PORT_7_OFFSET_LINK = 7,
	PORT_8_OFFSET_LINK = 8,
};

/* chip register offset number*/
enum BCM_PORT_IMP_STATUS
{
	IMP_OFFSET_LINK = 0,
	IMP_OFFSET_DUP = 1,
	IMP_OFFSET_SPD = 2,
};

/*error store position enum*/
enum _BCM_PORT_WORK_STATUS_POS
{
	BCM_PORT_STATUS_LINK_POS = 0,
	BCM_PORT_STATUS_DUPLEX_POS = 1,
	BCM_PORT_STATUS_SPEED_POS = 2,
}BCM_PORT_WORK_STATUS_POS;

/*port function status*/
typedef enum _BCM_PORT_STATUS_LINK
{
	LINK_FAIL = 0,
	LINK_PASS = 1
}BCM_PORT_STATUS_LINK_T;

/*port function status*/
typedef enum _BCM_PORT_STATUS_DUPLEX
{
	HALF_DUPLEX = 0,
	FULL_DUPLEX = 1,
}BCM_PORT_STATUS_DUPLEX_T;

/*port function status*/
typedef enum _BCM_PORT_STATUS_SPEED
{
	TEN_Mbps = 0,
	HUNDRED_Mbps = 1,
	THOUSAND_Mbps = 2
}BCM_PORT_STATUS_SPEED_T;

/*port write read status*/
typedef enum _BCM_PORT_OP_STATUS_T
{
	BCM_PORT_WRITE_ERR = -1,
	BCM_PORT_READ_ERR = -1,
	BCM_PORT_WRITE_SUCCESS = 1,
	BCM_PORT_READ_SUCCESS = 1,
}BCM_PORT_OP_STATUS_T;

typedef enum BCM_PORT_CHECK_STATUS
{
	ALL_PORT_STATUS_OK = 0,
	ALL_PORT_STATUS_ERR = 1
}BCM_PORT_CHECK_STATUS_T;

/*type define*/
typedef uint8_t R_FLAG_T;

typedef enum BCM_REG_PAGE BCM_PORT_STATUS_T;

typedef uint32_t BCM_CHIP_PORT_ERR_NUMBER_T;

typedef BCM_PORT_STATUS_LINK_T BCM_PORT_FUNC_STATUS_T;

/*function status structure*/
typedef struct _BCM_PORT_WORK_STATUS
{
	BCM_PORT_STATUS_LINK_T link_status;
	BCM_PORT_STATUS_DUPLEX_T duplex_status;
	BCM_PORT_STATUS_SPEED_T speed_status;
}BCM_PORT_WORK_STATUS_T;

/*error store structure*/
typedef struct _BCM_ERR_PORT_STATUS
{
	BCM_PORT_FUNC_STATUS_T error_status[3];
	BCM_CHIP_PORT_ERR_NUMBER_T error_pos[3];
	BCM_CHIP_PORT_ERR_NUMBER_T error_cnt;
}BCM_PORT_ERR_STATUS_T;


typedef struct _BCM_ALL_PORT_STATUS
{
	BCM_PORT_WORK_STATUS_T bcm_port_work_status;
	BCM_PORT_ERR_STATUS_T bcm_port_err_status;
}BCM_ALL_PORT_STATUS_T;
