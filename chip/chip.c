/************************************************************************************
 * (c) Copyright 2017 CRRC.  All rights reserved.
 *
 *  bcm 53134 chip operation
 *
 *  Created on: 2017-08-16
 *
 *  Author: buggy
 */

#include <stdio.h>
#include <string.h>

#include <time.h>

#include "chip.h"
#include "../port/port.h"
#include "../phy/phy.h"

/****************************************************************************************************/
/*description: global variables
/****************************************************************************************************/

/*current chip status*/
BCM_CHIP_WORK_STATUS_T * g_p_bcm_chip_work_status;

/*default chip status*/
const BCM_CHIP_WORK_STATUS_T g_bcm_chip_work_default_status = {ENABLE,ENABLE,ENABLE,MANA_MODE,ENABLE,NO_PAUSE,NO_PAUSE,CONTENTS,
															   ENABLE_IMP_PORT,ENABLE,NO_HEADER,NO_HEADER,NO_HEADER,DELAY_MODE,DELAY_MODE};

/****************************************************************************************************/
/*description: ex function
/****************************************************************************************************/

/****************************************************************************************************/
/*description: read chip all status
 * para 0: chip status pointer
*/
BCM_CHIP_WORK_STATUS_T * bcm_chip_status_read( BCM_CHIP_WORK_STATUS_T * bcm_chip_current_status );

/****************************************************************************************************/
/*description: initialization chip default status
 * para 0: chip default status
*/
BCM_CHIP_WORK_STATUS_T * bcm_chip_status_init_write( BCM_CHIP_WORK_STATUS_T * chip_current_status );

/****************************************************************************************************/
/*description: store chip error status
 * para 0: chip error status
*/
void bcm_chip_err_status_store( BCM_CHIP_ERR_STATUS_T * err_status );

/****************************************************************************************************/
/*description: check chip status
 * para 0: chip error status
*/
BCM_CHIP_ERR_STATUS_T  * bcm_chip_status_check( BCM_CHIP_ERR_STATUS_T * chip_err_status );


/****************************************************************************************************/
/*description: static function
/****************************************************************************************************/

/****************************************************************************************************/
/*description: read chip unicast status
*/
static BCM_REG_T bcm_chip_receive_unicast_read();

/****************************************************************************************************/
/*description: read chip multicast status
*/
static BCM_REG_T bcm_chip_receive_multicast_read();

/****************************************************************************************************/
/*description: read chip broadcast status
*/BCM_CHIP_WORK_STATUS_T bcm_chip_current_status;
static BCM_REG_T bcm_chip_receive_broadcast_read();

/****************************************************************************************************/
/*description: read chip software forwarding status
*/
static BCM_REG_T bcm_chip_software_forward_mode_read();

/*****************************BCM_CHIP_WORK_STATUS_T bcm_chip_current_status;***********************************************************************/
/*description: read chip software forward enable status
*/
static BCM_REG_T bcm_chip_software_forward_enable_read();

/****************************************************************************************************/
/*description: read chip tx flow control status
*/
static BCM_REG_T bcm_chip_flow_ctrl_tx_read();

/****************************************************************************************************/
/*description: read chip rx flow control status
*/
static BCM_REG_T bcm_chip_flow_ctrl_rx_read();

/****************************************************************************************************/
/*description: read chip mii override status
*/
static BCM_REG_T bcm_chip_mii_software_override_read();

/****************************************************************************************************/
/*description: read chip reset status
*/
//static BCM_CHIP_FUNC_STATUS_BCM_CHIP_WORK_STATUS_T bcm_chip_current_status;T bcm_chip_reset_cnt_read();

/****************************************************************************************************/
/*description: read chip imp port enable status
*/
static BCM_REG_T bcm_chip_imp_port_enable_read();

/****************************************************************************************************/
/*description: read chip bpdu enable status
*/
static BCM_REG_T bcm_chip_receive_bpdu_enable_read();

/****************************************************************************************************/
/*description: read chip port 7 brcm header status
*/
static BCM_REG_T bcm_chip_brcm_header_port7_read();

/****************************************************************************************************/
/*description: read chip port 5 brcm header status
*/
static BCM_REG_T bcm_chip_brcm_header_port5_read();

/****************************************************************************************************/
/*description: read chip port 8 brcm header status
*/
static BCM_REG_T bcm_chip_brcm_header_port8_read();

/****************************************************************************************************/
/*description: read chip dll delay status
*/
static BCM_REG_T bcm_chip_dll_delay_read();

/****************************************************************************************************/
/*description: read chip rgmii tx clock delay status
*/
static BCM_REG_T bcm_chip_rgmii_tx_clk_delay_read();

/****************************************************************************************************/
/*description: read chip unicast status
/*param 0: BCM_REG_T  function mode
*/
static BCM_REG_T bcm_chip_receive_unicast_write( BCM_REG_T mode );

/****************************************************************************************************/
/*description: read chip multicast status
/*param 0: BCM_REG_T  function mode
*/
static BCM_REG_T bcm_chip_receive_multicast_write(  BCM_REG_T mode );

/****************************************************************************************************/
/*description: read chip broadcast status
/*param 0: BCM_REG_T  function mode
*/
static BCM_REG_T bcm_chip_receive_broadcast_write(  BCM_REG_T mode );

/****************************************************************************************************/
/*description: read chip software forwarding status
/*param 0: BCM_REG_T  function mode
*/
static BCM_REG_T bcm_chip_software_forward_mode_write( BCM_REG_T mode );

/****************************************************************************************************/
/*description: read chip software forward enable status
/*param 0: BCM_REG_T  function mode
*/
static BCM_REG_T bcm_chip_software_forward_enable_write( BCM_REG_T mode );

/****************************************************************************************************/
/*description: read chip tx flow control status
/*param 0: BCM_REG_T  function mode
*/
static BCM_REG_T bcm_chip_tx_flow_control_write( BCM_REG_T mode );

/****************************************************************************************************/
/*description: read chip rx flow control status
/*param 0: BCM_REG_T  function mode
*/
static BCM_REG_T bcm_chip_rx_flow_control_write( BCM_REG_T mode );


/****************************************************************************************************/
/*description: read chip mii override status
/*param 0: BCM_REG_T  function mode
*/
static BCM_REG_T bcm_chip_mii_software_override_write( BCM_REG_T mode );


//static BCM_REG_T bcm_chip_reset_cnt_write();

/****************************************************************************************************/
/*description: read chip imp port enable status
/*param 0: BCM_REG_T  function mode
*/
static BCM_REG_T bcm_chip_imp_port_enable_write( BCM_REG_T mode );

/****************************************************************************************************/
/*description: read chip bpdu enable status
/*param 0: BCM_REG_T  function mode
*/
static BCM_REG_T bcm_chip_receive_bpdu_enable_write( BCM_REG_T mode );

/****************************************************************************************************/
/*description: read chip port 7 brcm header status
/*param 0: BCM_REG_T  function mode
*/
static BCM_REG_T bcm_chip_brcm_header_port7_write( BCM_REG_T mode );

/****************************************************************************************************/
/*description: read chip port 5 brcm header status
/*param 0: BCM_REG_T  function mode
*/
static BCM_REG_T bcm_chip_brcm_header_port5_write( BCM_REG_T mode );

/****************************************************************************************************/
/*description: read chip port 8 brcm header status
/*param 0: BCM_REG_T  function mode
*/
static BCM_REG_T bcm_chip_brcm_header_port8_write( BCM_REG_T mode );

/****************************************************************************************************/
/*description: read chip dll delay status
/*param 0: BCM_REG_T  function mode
*/
static BCM_REG_T bcm_chip_clock_dll_delay_write( BCM_REG_T mode );

/****************************************************************************************************/
/*description: read chip rgmii tx clock delay status
/*param 0: BCM_REG_T  function mode
*/
static BCM_REG_T bcm_chip_rgmii_tx_clk_delay_write( BCM_REG_T mode );



BCM_CHIP_WORK_STATUS_T * bcm_chip_status_read(BCM_CHIP_WORK_STATUS_T * bcm_chip_current_status)
{

	bcm_chip_current_status->receive_unicast = bcm_chip_receive_unicast_read();

	bcm_chip_current_status->receive_multicast = bcm_chip_receive_multicast_read();

	bcm_chip_current_status->receive_broadcast = bcm_chip_receive_broadcast_read();

	bcm_chip_current_status->software_forwarding_mode =	bcm_chip_software_forward_mode_read();

	/****************************************************************************************************/
	/*description: read chip software forward enable status
	*/
	bcm_chip_current_status->software_forwarding_enable = bcm_chip_software_forward_enable_read();

	/****************************************************************************************************/
	/*description: read chip tx flow control status
	*/
	bcm_chip_current_status->tx_flow_control = bcm_chip_flow_ctrl_tx_read();

	/****************************************************************************************************/
	/*description: read chip rx flow control status
	*/
	bcm_chip_current_status->tx_flow_control = bcm_chip_flow_ctrl_rx_read();

	/****************************************************************************************************/
	/*description: read chip mii override status
	*/
	bcm_chip_current_status->mii_software_override = bcm_chip_mii_software_override_read();

	/****************************************************************************************************/
	/*description: read chip reset status
	*/
	//bcm_chip_reset_cnt_read();

	/****************************************************************************************************/
	/*description: read chip imp port enable status
	*/
	bcm_chip_current_status->imp_port_enable = bcm_chip_imp_port_enable_read();

	/****************************************************************************************************/
	/*description: read chip bpdu enable status
	*/
	bcm_chip_current_status->receive_bpdu_enable = bcm_chip_receive_bpdu_enable_read();

	/****************************************************************************************************/
	/*description: read chip port 7 brcm header status
	*/
	bcm_chip_current_status->brcm_header_port7 = bcm_chip_brcm_header_port7_read();

	/****************************************************************************************************/
	/*description: read chip port 5 brcm header status
	*/
	bcm_chip_current_status->brcm_header_port5 = bcm_chip_brcm_header_port5_read();

	/****************************************************************************************************/
	/*description: read chip port 8 brcm header status
	*/
	bcm_chip_current_status->brcm_header_port8 = bcm_chip_brcm_header_port8_read();

	/****************************************************************************************************/
	/*description: read chip dll delay status
	*/
	bcm_chip_current_status->dll_clock_delay = bcm_chip_dll_delay_read();

	/****************************************************************************************************/
	/*description: read chip rgmii tx clock delay status
	*/
	bcm_chip_current_status->rgmii_tx_clk_delayed = bcm_chip_rgmii_tx_clk_delay_read();

	return bcm_chip_current_status;
}


BCM_CHIP_ERR_STATUS_T * bcm_chip_status_check( BCM_CHIP_ERR_STATUS_T  * chip_err_status )
{

	uint32_t i = 0;

	memset(chip_err_status,0,sizeof( BCM_CHIP_ERR_STATUS_T ));
	chip_err_status->error_cnt = 0;

	for(; i < sizeof( BCM_CHIP_WORK_STATUS_T ) / sizeof( BCM_REG_T ); i++)
	{
		if( *(( BCM_REG_T *)g_p_bcm_chip_work_status + i ) != *(( BCM_REG_T *)&g_bcm_chip_work_default_status + i ))
		{
			chip_err_status->error_status[ chip_err_status->error_cnt ] = *(( BCM_REG_T *)g_p_bcm_chip_work_status + i);
			chip_err_status->error_pos[ chip_err_status->error_cnt ] = i;
			chip_err_status->error_cnt++;
		}
		#ifdef DEBUG_CHECK
		printf("read %d %d",i,*((( BCM_REG_T *)g_p_bcm_chip_work_status ) + i));
		printf(":%d\n",*((( BCM_REG_T *)&g_bcm_chip_work_default_status ) + i ));
		#endif
	}

	#ifdef DEBUG_CHIP_ERR
	for(i = 0; i < chip_err_status->error_cnt; i++)
	{
		printf("Chip error cnt:%d error pos:%d error sts:%d\n",chip_err_status->error_cnt, chip_err_status->error_pos[ i ], chip_err_status->error_status[ i ]);
	}
	#endif
	return ( BCM_CHIP_ERR_STATUS_T *)chip_err_status;
}

void bcm_chip_err_status_store( BCM_CHIP_ERR_STATUS_T * err_status)
{
	uint32_t i;
	FILE * file;
	time_t timer;
	time( &timer );
	char time[50];
	char chip[5];
	char fact[100];
	char *s_time;
	uint32_t s_time_len;

	file = fopen("/mnt/BCM_ERR.csv","a");
	if (file == NULL)
	{
		printf("open csv error\n");
	}

	s_time = ctime(&timer);

	s_time_len = strlen(s_time);
	sprintf(chip,",%s","Chip");

	/*clear "\n"*/
	memcpy(time, s_time, s_time_len - 1);

	for(i = 0; i < err_status->error_cnt; i++)
	{
		memset(fact,0,sizeof(fact));
		switch(err_status->error_pos[i])
		{
			case RECEIVE_UNICAST_POS:
				strcpy(fact,",Unicast error\n");
				fputs(time,file);
				fputs(chip,file);
				fputs(fact,file);
				break;
			case RECEIVE_MULTICAST_POS:
				strcpy(fact, ",Multicast error\n");
				fputs(time,file);
				fputs(chip,file);
				fputs(fact,file);
				break;
			case RECEIVE_BROADCAST_POS:
				strcpy(fact,",Broadcast error\n");
				fputs(time,file);
				fputs(chip,file);
				fputs(fact,file);
				break;
			case SOFTWARE_FORWARDING_MODE_POS:
				strcpy(fact, ",Software forward mode error\n ");
				fputs(time,file);
				fputs(chip,file);
				fputs(fact,file);
				break;
			case SOFTWARE_FORWARDING_ENABLE_POS:
				strcpy(fact, ",Software forward enable error\n ");
				fputs(time,file);
				fputs(chip,file);
				fputs(fact,file);
				break;
			case TX_FLOW_CONTROL_POS:
				strcpy(fact, ",Tx flow control error\n ");
				fputs(time,file);
				fputs(chip,file);
				fputs(fact,file);
				break;
			case RX_FLOW_CONTROL_POS:
				strcpy(fact, ",Rx flow control error\n ");
				fputs(time,file);
				fputs(chip,file);
				fputs(fact,file);
				break;
			case MII_SOFTWARE_OVERRIDE_POS:
				strcpy(fact, ",Mii software override error\n ");
				fputs(time,file);
				fputs(chip,file);
				fputs(fact,file);
				break;
			case IMP_PORT_ENABLE_POS:
				strcpy(fact, ",Imp port enable error\n ");
				fputs(time,file);
				fputs(chip,file);
				fputs(fact,file);
				break;
			case RECEIVE_BPDU_ENABLE_POS:
				strcpy(fact, ",Receive bpdu enable error\n ");
				fputs(time,file);
				fputs(chip,file);
				fputs(fact,file);
				break;
			case BRCM_HEADER_PORT7_POS:
				strcpy(fact, ",Brcm header port7 error\n ");
				fputs(time,file);
				fputs(chip,file);
				fputs(fact,file);
				break;
			case BRCM_HEADER_PORT5_POS:
				strcpy(fact, ",Brcm header port5 error\n ");
				fputs(time,file);
				fputs(chip,file);
				fputs(fact,file);
				break;
			case BRCM_HEADER_PORT8_POS:
				strcpy(fact, ",Brcm header port8 error\n ");
				fputs(time,file);
				fputs(chip,file);
				fputs(fact,file);
				break;
			case DLL_CLOCK_DELAY_POS:
				strcpy(fact, ",Dll clock delay error\n ");
				fputs(time,file);
				fputs(chip,file);
				fputs(fact,file);
				break;
			case RGMII_TX_CLK_DELAYED_POS:
				strcpy(fact, ",Rgmii tx clk delay error\n ");
				fputs(time,file);
				fputs(chip,file);
				fputs(fact,file);
				break;
		}

	}
	fclose(file);
}

static BCM_REG_T bcm_chip_receive_unicast_read()
{
	BCM_REG_T mode;

	BCM_REG_ADDR_T receive_unicast_reg_value;

	receive_unicast_reg_value.spi_page = BCM_CHIP_REG_CTRL;
	receive_unicast_reg_value.spi_offset = BCM_CHIP_ADR_CAST;
	receive_unicast_reg_value.bit_offset = BCM_CHIP_OFFSET_UNICAST;
	receive_unicast_reg_value.bit_wide = BCM_CHIP_BIT_1;

	mode = chip_func_read( &receive_unicast_reg_value );


	switch ( mode )
	{
		case ENABLE:
			printf("Unicast Enable\n");
			break;
		case DISABLE:
			printf("Unicast Disable\n");
			break;
		default:
			printf("error\n");
	}

	return mode;
}

static BCM_REG_T bcm_chip_receive_multicast_read()
{
	BCM_REG_T mode;

	BCM_REG_ADDR_T receive_unicast_reg_value;

	receive_unicast_reg_value.spi_page = BCM_CHIP_REG_CTRL;
	receive_unicast_reg_value.spi_offset = BCM_CHIP_ADR_CAST;
	receive_unicast_reg_value.bit_offset = BCM_CHIP_OFFSET_MULTICAST;
	receive_unicast_reg_value.bit_wide = BCM_CHIP_BIT_1;

	mode = chip_func_read( &receive_unicast_reg_value );

	switch ( mode )
	{
		case ENABLE:
			printf("Multicast Enable\n");
			break;
		case DISABLE:
			printf("Multicast Disable\n");
			break;
		default:
			printf("error\n");
	}

	return mode;
}

static BCM_REG_T bcm_chip_receive_broadcast_read()
{
	BCM_REG_T mode;

	BCM_REG_ADDR_T receive_broadcast_reg_value;

	receive_broadcast_reg_value.spi_page = BCM_CHIP_REG_CTRL;
	receive_broadcast_reg_value.spi_offset = BCM_CHIP_ADR_CAST;
	receive_broadcast_reg_value.bit_offset = BCM_CHIP_OFFSET_BROADCAST;
	receive_broadcast_reg_value.bit_wide = BCM_CHIP_BIT_1;

	mode = chip_func_read( &receive_broadcast_reg_value );

	switch ( mode )
	{
		case ENABLE:
			printf("Broadcast Enable\n");
			break;
		case DISABLE:
			printf("Broadcast Disable\n");
			break;
		default:
			printf("error\n");
	}

	return mode;
}

static BCM_REG_T bcm_chip_software_forward_mode_read()
{
	BCM_REG_T mode;

	BCM_REG_ADDR_T software_forward_reg_value;

	software_forward_reg_value.spi_page = BCM_CHIP_REG_CTRL;
	software_forward_reg_value.spi_offset = BCM_CHIP_ADR_SOFTWARE_FORWARD;
	software_forward_reg_value.bit_offset = BCM_CHIP_OFFSET_SOFTWARE_FORWARD_MODE;
	software_forward_reg_value.bit_wide = BCM_CHIP_BIT_1;

	mode = chip_func_read( &software_forward_reg_value );

	switch ( mode )
	{
		case UNMANA_MODE:
			printf("Unmanage Mode\n");
			break;
		case MANA_MODE:
			printf("Manage Mode\n");
			break;
		default:
			printf("Forward error\n");
	}

	return mode;
}

static BCM_REG_T bcm_chip_software_forward_enable_read()
{
	BCM_REG_T mode;

	BCM_REG_ADDR_T software_forward_reg_value;

	software_forward_reg_value.spi_page = BCM_CHIP_REG_CTRL;
	software_forward_reg_value.spi_offset = BCM_CHIP_ADR_SOFTWARE_FORWARD;
	software_forward_reg_value.bit_offset = BCM_CHIP_OFFSET_SOFTWARE_FORWARD_ENABLE;
	software_forward_reg_value.bit_wide = BCM_CHIP_BIT_1;

	mode = chip_func_read( &software_forward_reg_value );

	switch ( mode )
	{
		case ENABLE:
			printf("Forward Enable\n");
			break;
		case DISABLE:
			printf("Forward Disable\n");
			break;
		default:
			printf("Forward error\n");
	}

	return mode;
}

static BCM_REG_T bcm_chip_flow_ctrl_rx_read()
{
	BCM_REG_T mode;

	BCM_REG_ADDR_T flow_ctrl_rx_reg_value;

	flow_ctrl_rx_reg_value.spi_page = BCM_CHIP_REG_CTRL;
	flow_ctrl_rx_reg_value.spi_offset = BCM_CHIP_ADR_FLOW_CTRL;
	flow_ctrl_rx_reg_value.bit_offset = BCM_CHIP_OFFSET_FLOW_CTRL_RX;
	flow_ctrl_rx_reg_value.bit_wide = BCM_CHIP_BIT_1;

	mode = chip_func_read( &flow_ctrl_rx_reg_value );

	switch ( mode )
	{
		case NO_PAUSE:
			printf("Rx flow control No Pause\n");
			break;
		case PAUSE:
			printf("Rx flow control Pause\n");
			break;
		default:
			printf("Rx flow error\n");
	}
	return mode;
}

static BCM_REG_T bcm_chip_flow_ctrl_tx_read()
{
	BCM_REG_T mode;

	BCM_REG_ADDR_T receive_unicast_reg_value;

	receive_unicast_reg_value.spi_page = BCM_CHIP_REG_CTRL;
	receive_unicast_reg_value.spi_offset = BCM_CHIP_ADR_FLOW_CTRL;
	receive_unicast_reg_value.bit_offset = BCM_CHIP_OFFSET_FLOW_CTRL_TX;
	receive_unicast_reg_value.bit_wide = BCM_CHIP_BIT_1;

	mode = chip_func_read( &receive_unicast_reg_value );

	switch ( mode )
	{
		case NO_PAUSE:
			printf("Tx flow control No Pause\n");
			break;
		case PAUSE:
			printf("Tx flow control Pause\n");
			break;
		default:
			printf("Tx flow error\n");
	}

	return mode;
}

static BCM_REG_T bcm_chip_mii_software_override_read()
{
	BCM_REG_T mode;

	BCM_REG_ADDR_T mii_software_override_reg_value;

	mii_software_override_reg_value.spi_page = BCM_CHIP_REG_CTRL;
	mii_software_override_reg_value.spi_offset = BCM_CHIP_ADR_MII_OVERRIDE;
	mii_software_override_reg_value.bit_offset = BCM_CHIP_OFFSET_MII_OVERRIDE;
	mii_software_override_reg_value.bit_wide = BCM_CHIP_BIT_1;

	mode = chip_func_read( &mii_software_override_reg_value );

	switch ( mode )
	{
		case HARDWARE_PIN:
			printf("Use hardware pin\n");
			break;
		case CONTENTS:
			printf("Use contents\n");
			break;
		default:
			printf("mii software override error\n");
	}
	return mode;
}

//static BCM_REG_T bcm_chip_reset_cnt_read()
//{
//	BCM_REG_T mode;
//
//	BCM_REG_ADDR_T receive_unicast_reg_value;
//
//	receive_unicast_reg_value.spi_page = BCM_CHIP_REG_CTRL;
//	receive_unicast_reg_value.spi_offset = BCM_CHIP_ADR_RESET_CNT;
//	receive_unicast_reg_value.bit_offset = BCM_CHIP_OFFSET_RESET_CNT;
//	receive_unicast_reg_value.bit_wide = BCM_CHIP_BIT_1;
//
//	mode = chip_func_read( &receive_unicast_reg_value );
//
//	switch ( mode )
//	{
//		case :
//			mode = ENABLE;
//			break;
//		case DISABLE:
//			mode = DISABLE;
//			break;
//		default:
//			printf("error\n");
//	}
//
//	return mode;
//}

static BCM_REG_T bcm_chip_imp_port_enable_read()
{
	BCM_REG_T mode;

	BCM_REG_ADDR_T imp_port_enable_reg_value;

	imp_port_enable_reg_value.spi_page = BCM_CHIP_REG_MANA;
	imp_port_enable_reg_value.spi_offset = BCM_CHIP_ADR_ENABLE_IMP;
	imp_port_enable_reg_value.bit_offset = BCM_CHIP_OFFSET_ENABLE_IMP;
	imp_port_enable_reg_value.bit_wide = BCM_CHIP_BIT_2;

	mode = chip_func_read( &imp_port_enable_reg_value );

	switch ( mode )
	{
		case NO_IMP_PORT:
			printf("NO IMP port\n");
			break;
		case ENABLE_IMP_PORT:
			printf("ENABLE IMP port\n");
			break;
		default:
			printf("error\n");
	}

	return mode;
}

static BCM_REG_T bcm_chip_receive_bpdu_enable_read()
{
	BCM_REG_T mode;

	BCM_REG_ADDR_T receive_unicast_reg_value;

	receive_unicast_reg_value.spi_page = BCM_CHIP_REG_CTRL;
	receive_unicast_reg_value.spi_offset = BCM_CHIP_ADR_ENABLE_BPDU;
	receive_unicast_reg_value.bit_offset = BCM_CHIP_OFFSET_ENABLE_BPDU;
	receive_unicast_reg_value.bit_wide = BCM_CHIP_BIT_1;

	mode = chip_func_read( &receive_unicast_reg_value );

	switch ( mode )
	{
		case ENABLE:
			printf("BPDU ENABLE\n");
			break;
		case DISABLE:
			printf("BPDU DISABLE\n");
			break;
		default:
			printf("BPDU error\n");
	}

	return mode;
}

static BCM_REG_T bcm_chip_brcm_header_port7_read()
{
	BCM_REG_T mode;

	BCM_REG_ADDR_T receive_unicast_reg_value;

	receive_unicast_reg_value.spi_page = BCM_CHIP_REG_MANA;
	receive_unicast_reg_value.spi_offset = BCM_CHIP_ADR_BRCM_HEADER;
	receive_unicast_reg_value.bit_offset = BCM_CHIP_OFFSET_BRCM_HEADER_PORT_7;
	receive_unicast_reg_value.bit_wide = BCM_CHIP_BIT_1;

	mode = chip_func_read( &receive_unicast_reg_value );

	switch ( mode )
	{
		case NO_HEADER:
			printf("Port 7 No header\n");
			break;
		case HEADER:
			printf("Port 7 have header\n");
			break;
		default:
			printf("Port 7 header error\n");
	}

	return mode;
}

static BCM_REG_T bcm_chip_brcm_header_port5_read()
{
	BCM_REG_T mode;

	BCM_REG_ADDR_T receive_unicast_reg_value;

	receive_unicast_reg_value.spi_page = BCM_CHIP_REG_MANA;
	receive_unicast_reg_value.spi_offset = BCM_CHIP_ADR_BRCM_HEADER;
	receive_unicast_reg_value.bit_offset = BCM_CHIP_OFFSET_BRCM_HEADER_PORT_5;
	receive_unicast_reg_value.bit_wide = BCM_CHIP_BIT_1;

	mode = chip_func_read( &receive_unicast_reg_value );

	switch ( mode )
	{
		case NO_HEADER:
			printf("Port 5 No header\n");
			break;
		case HEADER:
			printf("Port 5 have header\n");
			break;
		default:
			printf("Port 5 header error\n");
	}

	return mode;
}

static BCM_REG_T bcm_chip_brcm_header_port8_read()
{
	BCM_REG_T mode;

	BCM_REG_ADDR_T brcm_header_port_reg_value;

	brcm_header_port_reg_value.spi_page = BCM_CHIP_REG_MANA;
	brcm_header_port_reg_value.spi_offset = BCM_CHIP_ADR_BRCM_HEADER;
	brcm_header_port_reg_value.bit_offset = BCM_CHIP_OFFSET_BRCM_HEADER_PORT_8;
	brcm_header_port_reg_value.bit_wide = BCM_CHIP_BIT_1;

	mode = chip_func_read( &brcm_header_port_reg_value );

	switch ( mode )
	{
		case NO_HEADER:
			printf("Port 8 No header\n");
			break;
		case HEADER:
			printf("Port 8 have header\n");
			break;
		default:
			printf("Port 8 header error\n");
	}

	return mode;
}

static BCM_REG_T bcm_chip_dll_delay_read()
{
	BCM_REG_T mode;

	BCM_REG_ADDR_T dll_delay_reg_value;

	dll_delay_reg_value.spi_page = BCM_CHIP_REG_CTRL;
	dll_delay_reg_value.spi_offset = BCM_CHIP_ADR_DELAY;
	dll_delay_reg_value.bit_offset = BCM_CHIP_OFFSET_DELAY_DLL;
	dll_delay_reg_value.bit_wide = BCM_CHIP_BIT_1;

	mode = chip_func_read( &dll_delay_reg_value );

	switch ( mode )
	{
		case DELAY_MODE:
			printf("DLL Delay mode\n");
			break;
		case NORMAL_MODE:
			printf("DLL Normal mode\n");
			break;
		default:
			printf("error\n");
	}

	return mode;
}

static BCM_REG_T bcm_chip_rgmii_tx_clk_delay_read()
{
	BCM_REG_T mode;

	BCM_REG_ADDR_T rgmii_tx_clk_delay_reg_value;

	rgmii_tx_clk_delay_reg_value.spi_page = BCM_CHIP_REG_CTRL;
	rgmii_tx_clk_delay_reg_value.spi_offset = BCM_CHIP_ADR_DELAY;
	rgmii_tx_clk_delay_reg_value.bit_offset = BCM_CHIP_OFFSET_DELAY_RGMII;
	rgmii_tx_clk_delay_reg_value.bit_wide = BCM_CHIP_BIT_1;

	mode = chip_func_read( &rgmii_tx_clk_delay_reg_value );

	switch ( mode )
	{
		case DELAY_MODE:
			printf("rgmii Delay mode\n");
			break;
		case NORMAL_MODE:
			printf("rgmii Normal mode\n");
			break;
		default:
			printf("error\n");
	}
	return mode;
}


//void bcm_chip_status_init_value()
//{
//
//	g_bcm_chip_work_default_status.receive_unicast = ENABLE;
//
//	g_bcm_chip_work_default_status.receive_multicast = ENABLE;
//
//	g_bcm_chip_work_default_status.receive_broadcast = ENABLE;
//
//	g_bcm_chip_work_default_status.software_forwarding_mode = MANA_MODE;
//
//	g_bcm_chip_work_default_status.software_forwarding_enable = ENABLE;
//
//	g_bcm_chip_work_default_status.mii_software_override = CONTENTS;
//
//	g_bcm_chip_work_default_status.dll_clock_delay = NORMAL_MODE;
//
//	g_bcm_chip_work_default_status.rgmii_tx_clk_delayed = NORMAL_MODE;
//
//	g_bcm_chip_work_default_status.imp_port_enable = ENABLE_IMP_PORT;
//
//	g_bcm_chip_work_default_status.receive_bpdu_enable = ENABLE;
//
//	g_bcm_chip_work_default_status.brcm_header_port8 = NO_HEADER;
//
//	g_bcm_chip_work_default_status.brcm_header_port5 = NO_HEADER;
//
//	g_bcm_chip_work_default_status.brcm_header_port7 = NO_HEADER;
//
//	g_bcm_chip_work_default_status.rx_flow_control = NO_PAUSE;
//
//	g_bcm_chip_work_default_status.tx_flow_control = NO_PAUSE;
//
//}

BCM_CHIP_WORK_STATUS_T * bcm_chip_status_init_write( BCM_CHIP_WORK_STATUS_T * chip_current_status )
{


	bcm_chip_receive_unicast_write( chip_current_status->receive_unicast );

	bcm_chip_receive_multicast_write( chip_current_status->receive_multicast );

	bcm_chip_receive_broadcast_write( chip_current_status->receive_broadcast );

	bcm_chip_software_forward_mode_write( chip_current_status->software_forwarding_mode );

	bcm_chip_software_forward_enable_write( chip_current_status->software_forwarding_enable );

	bcm_chip_mii_software_override_write( chip_current_status->mii_software_override );

	bcm_chip_clock_dll_delay_write( chip_current_status->dll_clock_delay );

	bcm_chip_rgmii_tx_clk_delay_write( chip_current_status->rgmii_tx_clk_delayed );

	bcm_chip_imp_port_enable_write( chip_current_status->imp_port_enable );

	bcm_chip_receive_bpdu_enable_write( chip_current_status->receive_bpdu_enable );

	bcm_chip_brcm_header_port8_write( chip_current_status->brcm_header_port8 );

	bcm_chip_brcm_header_port5_write( chip_current_status->brcm_header_port5 );

	bcm_chip_brcm_header_port7_write( chip_current_status->brcm_header_port7 );

	bcm_chip_rx_flow_control_write( chip_current_status->rx_flow_control  );

	bcm_chip_tx_flow_control_write( chip_current_status->tx_flow_control);

	return chip_current_status;
}

static BCM_REG_T bcm_chip_receive_unicast_write( BCM_REG_T mode )
{
	BCM_REG_ADDR_T receive_unicast_reg_value;

	receive_unicast_reg_value.spi_page = BCM_CHIP_REG_CTRL;
	receive_unicast_reg_value.spi_offset = BCM_CHIP_ADR_CAST;
	receive_unicast_reg_value.bit_offset = BCM_CHIP_OFFSET_UNICAST;
	receive_unicast_reg_value.bit_wide = BCM_CHIP_BIT_1;

	return chip_func_write( &receive_unicast_reg_value, mode );
}

static BCM_REG_T bcm_chip_receive_multicast_write( BCM_REG_T mode )
{
	BCM_REG_ADDR_T receive_multicast_reg_value;

	receive_multicast_reg_value.spi_page = BCM_CHIP_REG_CTRL;
	receive_multicast_reg_value.spi_offset = BCM_CHIP_ADR_CAST;
	receive_multicast_reg_value.bit_offset = BCM_CHIP_OFFSET_MULTICAST;
	receive_multicast_reg_value.bit_wide = BCM_CHIP_BIT_1;

	return chip_func_write( &receive_multicast_reg_value, mode );
}

static BCM_REG_T bcm_chip_receive_broadcast_write( BCM_REG_T mode )
{
	BCM_REG_ADDR_T receive_broadcast_reg_value;

	receive_broadcast_reg_value.spi_page = BCM_CHIP_REG_CTRL;
	receive_broadcast_reg_value.spi_offset = BCM_CHIP_ADR_CAST;
	receive_broadcast_reg_value.bit_offset = BCM_CHIP_OFFSET_BROADCAST;
	receive_broadcast_reg_value.bit_wide = BCM_CHIP_BIT_1;

	return chip_func_write( &receive_broadcast_reg_value, mode );
}


static BCM_REG_T bcm_chip_software_forward_mode_write( BCM_REG_T mode )
{
	BCM_REG_ADDR_T software_forward_reg_value;

	software_forward_reg_value.spi_page = BCM_CHIP_REG_CTRL;
	software_forward_reg_value.spi_offset = BCM_CHIP_ADR_SOFTWARE_FORWARD;
	software_forward_reg_value.bit_offset = BCM_CHIP_OFFSET_SOFTWARE_FORWARD_MODE;
	software_forward_reg_value.bit_wide = BCM_CHIP_BIT_1;

	return chip_func_write( &software_forward_reg_value, mode );
}

static BCM_REG_T bcm_chip_software_forward_enable_write( BCM_REG_T mode )
{
	BCM_REG_ADDR_T software_forward_reg_value;

	software_forward_reg_value.spi_page = BCM_CHIP_REG_CTRL;
	software_forward_reg_value.spi_offset = BCM_CHIP_ADR_SOFTWARE_FORWARD;
	software_forward_reg_value.bit_offset = BCM_CHIP_OFFSET_SOFTWARE_FORWARD_ENABLE;
	software_forward_reg_value.bit_wide = BCM_CHIP_BIT_1;

	return chip_func_write( &software_forward_reg_value, mode );

}

static BCM_REG_T bcm_chip_tx_flow_control_write( BCM_REG_T mode )
{
	BCM_REG_ADDR_T flow_ctrl_tx_reg_value;

	flow_ctrl_tx_reg_value.spi_page = BCM_CHIP_REG_CTRL;
	flow_ctrl_tx_reg_value.spi_offset = BCM_CHIP_ADR_FLOW_CTRL;
	flow_ctrl_tx_reg_value.bit_offset = BCM_CHIP_OFFSET_FLOW_CTRL_TX;
	flow_ctrl_tx_reg_value.bit_wide = BCM_CHIP_BIT_1;

	return chip_func_write( &flow_ctrl_tx_reg_value, mode );
}

static BCM_REG_T bcm_chip_rx_flow_control_write( BCM_REG_T mode )
{
	BCM_REG_ADDR_T flow_ctrl_rx_reg_value;

	flow_ctrl_rx_reg_value.spi_page = BCM_CHIP_REG_CTRL;
	flow_ctrl_rx_reg_value.spi_offset = BCM_CHIP_ADR_FLOW_CTRL;
	flow_ctrl_rx_reg_value.bit_offset = BCM_CHIP_OFFSET_FLOW_CTRL_RX;
	flow_ctrl_rx_reg_value.bit_wide = BCM_CHIP_BIT_1;

	return chip_func_write( &flow_ctrl_rx_reg_value, mode );
}

static BCM_REG_T bcm_chip_mii_software_override_write( BCM_REG_T mode )
{
	BCM_REG_ADDR_T mii_software_override_reg_value;

	mii_software_override_reg_value.spi_page = BCM_CHIP_REG_CTRL;
	mii_software_override_reg_value.spi_offset = BCM_CHIP_ADR_MII_OVERRIDE;
	mii_software_override_reg_value.bit_offset = BCM_CHIP_OFFSET_MII_OVERRIDE;
	mii_software_override_reg_value.bit_wide = BCM_CHIP_BIT_1;

	return chip_func_write( &mii_software_override_reg_value, mode );
}

//static BCM_REG_T bcm_chip_reset_cnt_write();

static BCM_REG_T bcm_chip_imp_port_enable_write( BCM_REG_T mode )
{
	BCM_REG_ADDR_T imp_port_enable_reg_value;

	imp_port_enable_reg_value.spi_page = BCM_CHIP_REG_MANA;
	imp_port_enable_reg_value.spi_offset = BCM_CHIP_ADR_ENABLE_IMP;
	imp_port_enable_reg_value.bit_offset = BCM_CHIP_OFFSET_ENABLE_IMP;
	imp_port_enable_reg_value.bit_wide = BCM_CHIP_BIT_2;

	return chip_func_write( &imp_port_enable_reg_value, mode );
}

static BCM_REG_T bcm_chip_receive_bpdu_enable_write( BCM_REG_T mode )
{


	BCM_REG_ADDR_T receive_bpdu_enable_reg_value;

	receive_bpdu_enable_reg_value.spi_page = BCM_CHIP_REG_MANA;
	receive_bpdu_enable_reg_value.spi_offset = BCM_CHIP_ADR_ENABLE_BPDU;
	receive_bpdu_enable_reg_value.bit_offset = BCM_CHIP_OFFSET_ENABLE_BPDU;
	receive_bpdu_enable_reg_value.bit_wide = BCM_CHIP_BIT_1;

	return chip_func_write( &receive_bpdu_enable_reg_value,mode );
}

static BCM_REG_T bcm_chip_brcm_header_port8_write( BCM_REG_T mode )
{
	BCM_REG_ADDR_T brcm_header_port_reg_value;

	brcm_header_port_reg_value.spi_page = BCM_CHIP_REG_MANA;
	brcm_header_port_reg_value.spi_offset = BCM_CHIP_ADR_BRCM_HEADER;
	brcm_header_port_reg_value.bit_offset = BCM_CHIP_OFFSET_BRCM_HEADER_PORT_8;
	brcm_header_port_reg_value.bit_wide = BCM_CHIP_BIT_1;

	return chip_func_write( &brcm_header_port_reg_value, mode );
}

static BCM_REG_T bcm_chip_brcm_header_port5_write( BCM_REG_T mode )
{
	BCM_REG_ADDR_T brcm_header_port_reg_value;

	brcm_header_port_reg_value.spi_page = BCM_CHIP_REG_MANA;
	brcm_header_port_reg_value.spi_offset = BCM_CHIP_ADR_BRCM_HEADER;
	brcm_header_port_reg_value.bit_offset = BCM_CHIP_OFFSET_BRCM_HEADER_PORT_5;
	brcm_header_port_reg_value.bit_wide = BCM_CHIP_BIT_1;

	return chip_func_write( &brcm_header_port_reg_value, mode );
}

static BCM_REG_T bcm_chip_brcm_header_port7_write( BCM_REG_T mode )
{
	BCM_REG_ADDR_T brcm_header_port_reg_value;

	brcm_header_port_reg_value.spi_page = BCM_CHIP_REG_MANA;
	brcm_header_port_reg_value.spi_offset = BCM_CHIP_ADR_BRCM_HEADER;
	brcm_header_port_reg_value.bit_offset = BCM_CHIP_OFFSET_BRCM_HEADER_PORT_7;
	brcm_header_port_reg_value.bit_wide = BCM_CHIP_BIT_1;

	return chip_func_write( &brcm_header_port_reg_value, mode );
}

static BCM_REG_T bcm_chip_clock_dll_delay_write( BCM_REG_T mode )
{
	BCM_REG_ADDR_T clock_dll_delay_reg_value;

	clock_dll_delay_reg_value.spi_page = BCM_CHIP_REG_CTRL;
	clock_dll_delay_reg_value.spi_offset = BCM_CHIP_ADR_DELAY;
	clock_dll_delay_reg_value.bit_offset = BCM_CHIP_OFFSET_DELAY_DLL;
	clock_dll_delay_reg_value.bit_wide = BCM_CHIP_BIT_1;

	return chip_func_write( &clock_dll_delay_reg_value, mode );
}

static BCM_REG_T bcm_chip_rgmii_tx_clk_delay_write( BCM_REG_T mode )
{

	BCM_REG_ADDR_T rgmii_tx_clk_delay_reg_value;

	rgmii_tx_clk_delay_reg_value.spi_page = BCM_CHIP_REG_CTRL;
	rgmii_tx_clk_delay_reg_value.spi_offset = BCM_CHIP_ADR_DELAY ;
	rgmii_tx_clk_delay_reg_value.bit_offset = BCM_CHIP_OFFSET_DELAY_RGMII;
	rgmii_tx_clk_delay_reg_value.bit_wide = BCM_CHIP_BIT_1;

	return chip_func_write( &rgmii_tx_clk_delay_reg_value, mode );
}

