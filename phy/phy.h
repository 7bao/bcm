/************************************************************************************
 *  bcm 53134 mdio operation headfile 
 *
 *  Created on: 2017-08-16
 *
 *  Author: buggy
 */

#include <stdint.h>

/*check count*/
#define CHK_CNT 200
//#define DEBUG

#define PHY_ADR 30


typedef uint64_t BCM_REG_T;					//read or write register value

/*register address member*/
typedef uint16_t BCM_SPI_PAGE_T;
typedef uint16_t BCM_SPI_OFFSET_T;
typedef uint32_t BCM_REG_BIT_OFFSET_T;
typedef uint32_t BCM_REG_BIT_WIDE_T;

/*chip function*/
typedef uint32_t BCM_FUNC_T;

static const BCM_REG_T MASK_SINGLE = 0x1;
static const BCM_REG_T MASK_DOUBLE = 0x3;
static const BCM_REG_T MASK_TRIPLE = 0x7;
static const BCM_REG_T MASK_OCTUPLE = 0x1ff;

/*operationm mode*/
enum W_R_MODE g_operation_mode;

/*general bool*/
enum BOOL
{
	FALSE = 0,
	TRUE = 1
};

/*read status*/
enum W_R_MODE
{
    WRITE_MODE = 0,
	READ_MODE,
	NO_STATUS
};

/*read status*/
enum PHY_OP_STATUS
{
    PHY_READ_ERR = -1,
	PHY_READ_SUCCESS = 1,
	PHY_WRITE_ERR = -1,
	PHY_WRITE_SUCCESS = 1,
};

/* read write return value */
typedef enum _BCM_CHIP_OP_STATUS_T
{
	FUNC_WRITE_ERR = -1,
	FUNC_READ_ERR = -1,
	FUNC_WRITE_CORRECT = 1,
	FUNC_READ_CORRECT = 1,
}BCM_FUNC_OP_T;

/* function bit wide */
typedef enum _BCM_REG_BITS
{
	BCM_CHIP_BIT_1 = 0x1,
	BCM_CHIP_BIT_2 = 0x2,
	BCM_CHIP_BIT_3 = 0x3,
	BCM_CHIP_BIT_8 = 0x8,
}BCM_REG_BITS_T;

/*register */
enum MII_REG_NAME
{
	MII_REG_0 = 0,
	MII_REG_1 = 1,
	MII_REG_2,
	MII_REG_3,
	MII_REG_4,
	MII_REG_5,
	MII_REG_6,
	MII_REG_7,
	MII_REG_8,
	MII_REG_9,
	MII_REG_10,
	MII_REG_11,
	MII_REG_12,
	MII_REG_13,
	MII_REG_14,
	MII_REG_15,
	MII_REG_16,
	MII_REG_17,
	MII_REG_18,
	MII_REG_19,
	MII_REG_20,
	MII_REG_21,
	MII_REG_22,
	MII_REG_23,
	MII_REG_24,
	MII_REG_25,
	MII_REG_26,
	MII_REG_27,
	MII_REG_28,
	MII_REG_29,
	MII_REG_30,
	MII_REG_31,
	MII_REG_32,
};

enum SPI_PAGE
{
	
	PAGE_0 = 0x0,
	PAGE_1 = 0x1,
	PAGE_2 = 0x2,
	PAGE_3 = 0x3,
	PAGE_4 = 0x4,
	PAGE_5 = 0x5,
	PAGE_6 = 0x6,
	PAGE_7 = 0x7,
	PAGE_8 = 0x8,
	PAGE_9 = 0x9,
	//PAGE_10_13 = 0x ,
	//PAGE_ = 0x ,
	PAGE_28 = 0x28,
	PAGE_30 = 0x30, 
	PAGE_31 = 0x31,
	PAGE_32 = 0x32,
	PAGE_33 = 0x33,
	PAGE_34 = 0x34,
	PAGE_35 = 0x35,
	PAGE_36 = 0x36,
	PAGE_37 = 0x37,
	PAGE_38 = 0x38,
	PAGE_39 = 0x39,
	PAGE_40 = 0x40,
	PAGE_41 = 0x41,
	PAGE_42 = 0x42,
	PAGE_43 = 0x43,
	PAGE_44 = 0x44,
	PAGE_45 = 0x45,
	PAGE_46 = 0x46,
	PAGE_47 = 0x47,
	PAGE_48 = 0x48,
	PAGE_49 = 0x49,
	PAGE_4A = 0x4a,
	PAGE_4B = 0x4b,
	PAGE_4C = 0x4c,
	PAGE_4D = 0x4d,
	PAGE_4E = 0x4e,
	PAGE_4F = 0x4f,
	PAGE_70 = 0x70,
	PAGE_71 = 0x71,
	PAGE_72 = 0x72,
	PAGE_85 = 0x85,
	PAGE_88 = 0x88,
	PAGE_91 = 0x91,
	PAGE_92 = 0x92,
	PAGE_93 = 0x93,
	PAGE_94 = 0x94,
	PAGE_95 = 0x95,
	PAGE_A0 = 0xa0,
	PAGE_A1 = 0xa1,
	PAGE_FF = 0xff
};

/*function position structure*/
typedef struct _BCM_REG_ADDR_T
{
	BCM_SPI_PAGE_T   	 spi_page;								//page number
	BCM_SPI_OFFSET_T     spi_offset;							//page address
	BCM_REG_BIT_OFFSET_T bit_offset;							//offset number
	BCM_REG_BIT_WIDE_T   bit_wide;								//bit wide
	BCM_REG_T            func_value;                             //function value
}BCM_REG_ADDR_T;

typedef struct reg
{
	BCM_SPI_PAGE_T spi_page;
	BCM_SPI_OFFSET_T spi_offset;
	BCM_REG_T reg_value;

	BCM_REG_T ( * register_read )(BCM_SPI_PAGE_T, BCM_SPI_OFFSET_T );
	BCM_REG_T ( * register_write)(BCM_SPI_PAGE_T, BCM_SPI_OFFSET_T, BCM_REG_T);
}bcm_chip_t;

typedef struct func
{
	/*operation data*/
	BCM_REG_ADDR_T address;

	/*operation*/
	BCM_REG_T     ( * function_read )( BCM_REG_ADDR_T * );
	BCM_FUNC_OP_T ( * function_write)( BCM_REG_ADDR_T *, BCM_REG_T );
}bcm_func_t;

