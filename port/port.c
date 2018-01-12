/************************************************************************************
 *  bcm 53134 port operation
 *
 *  Created on: 2017-08-16
 *
 *  Author: buggy
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>

#include "port.h"
#include "../phy/phy.h"
#include "../chip/chip.h"

/****************************************************************************************************/
/*description: global variables
/****************************************************************************************************/

/*global variables*/
BCM_PORT_WORK_STATUS_T * g_p_bcm_port_0_work_status;
BCM_PORT_WORK_STATUS_T * g_p_bcm_port_1_work_status;
BCM_PORT_WORK_STATUS_T * g_p_bcm_port_2_work_status;
BCM_PORT_WORK_STATUS_T * g_p_bcm_port_3_work_status;
BCM_PORT_WORK_STATUS_T * g_p_bcm_port_imp_work_status;

/*port default status*/
const BCM_PORT_WORK_STATUS_T g_bcm_port_imp_work_default_status = { LINK_PASS, FULL_DUPLEX, HUNDRED_Mbps };
const BCM_PORT_WORK_STATUS_T g_bcm_port_gen_work_default_status = { LINK_PASS, FULL_DUPLEX, HUNDRED_Mbps };

/****************************************************************************************************/
/*description: ex function
/****************************************************************************************************/


/****************************************************************************************************/
/*description: initialization port default status
*/
void bcm_port_status_init_write();

/****************************************************************************************************/
/*description: read port all status
 * para 0: port number
 * para 1: port status pointer
*/
BCM_PORT_WORK_STATUS_T * bcm_port_status_read( BCM_PORT_T port_number, BCM_PORT_WORK_STATUS_T * port_status );
//bcm_port_reset( BCM_PORT_T port_number );

/****************************************************************************************************/
/*description: write port default status
 * para 0: port number
 * para 1: port status
*/
BCM_PORT_WORK_STATUS_T bcm_port_status_write( BCM_PORT_T port_number, BCM_PORT_WORK_STATUS_T * port_work_status );

/****************************************************************************************************/
/*description: check port status
 * para 0: port number
 * para 1: chip error status
*/
BCM_PORT_ERR_STATUS_T * bcm_port_status_check( BCM_PORT_T port_number, BCM_PORT_ERR_STATUS_T * port_err_status);

/****************************************************************************************************/
/*description: static function
/****************************************************************************************************/

void bcm_port_err_status_store( BCM_PORT_T port_number, BCM_PORT_ERR_STATUS_T * port_err_status);

/****************************************************************************************************/
/*description: read port link status
/*param 0: port number
*/
static BCM_PORT_STATUS_T bcm_port_link_status_read(BCM_PORT_T port_number);

/****************************************************************************************************/
/*description: read port duplex status
/*param 0: port number
*/
static BCM_PORT_STATUS_T bcm_port_duplex_status_read(BCM_PORT_T port_number);

/****************************************************************************************************/
/*description: read port speed status
/*param 0: port number
*/
static BCM_PORT_STATUS_T bcm_port_speed_status_read(BCM_PORT_T port_number);

/****************************************************************************************************/
/*description: write port link status
/*param 0: port number
/*param 1: link status
*/
static BCM_PORT_OP_STATUS_T bcm_port_link_status_write(BCM_PORT_T port_number, BCM_PORT_STATUS_T link_status);

/****************************************************************************************************/
/*description: write port duplex status
/*param 0: port number
/*param 1: duplex status
*/
static BCM_PORT_OP_STATUS_T bcm_port_duplex_status_write(BCM_PORT_T port_number, BCM_PORT_STATUS_T duplex_status);

/****************************************************************************************************/
/*description: write port speed status
/*param 0: port number
/*param 1: speed status
*/
static BCM_PORT_OP_STATUS_T bcm_port_speed_status_write( BCM_PORT_T port_number, BCM_PORT_STATUS_T speed_status );

void bcm_port_status_init_write()
{
	bcm_port_speed_status_write(IMP, HUNDRED_Mbps);
	bcm_port_duplex_status_write(IMP, FULL_DUPLEX);
	bcm_port_link_status_write(IMP, LINK_PASS);
}

BCM_PORT_WORK_STATUS_T * bcm_port_status_read( BCM_PORT_T port_number, BCM_PORT_WORK_STATUS_T * port_status )
{

	port_status->link_status = bcm_port_link_status_read( port_number );

	port_status->speed_status = bcm_port_speed_status_read( port_number );

	port_status->duplex_status = bcm_port_duplex_status_read( port_number );

	return port_status;
}

BCM_PORT_WORK_STATUS_T bcm_port_status_write( BCM_PORT_T port_number, BCM_PORT_WORK_STATUS_T * port_work_status )
{
	bcm_port_link_status_write( port_number, port_work_status->link_status );
	bcm_port_duplex_status_write( port_number, port_work_status->duplex_status );
	bcm_port_speed_status_write( port_number, port_work_status->speed_status );
}


BCM_PORT_ERR_STATUS_T * bcm_port_status_check( BCM_PORT_T port_number, BCM_PORT_ERR_STATUS_T * port_err_status)
{
	uint32_t i;

	memset(port_err_status,0,sizeof( BCM_PORT_ERR_STATUS_T ));

	BCM_PORT_WORK_STATUS_T * p_port_status;

	p_port_status = ( BCM_PORT_WORK_STATUS_T *)malloc( sizeof( BCM_PORT_WORK_STATUS_T ));

	p_port_status = bcm_port_status_read( port_number, p_port_status);

	for(i = 0; i < sizeof( BCM_PORT_WORK_STATUS_T )/sizeof( BCM_PORT_FUNC_STATUS_T ); i++ )
	{
		if( *(( BCM_PORT_FUNC_STATUS_T *)p_port_status + i ) != *(( BCM_PORT_FUNC_STATUS_T *)&g_bcm_port_imp_work_default_status + i ))
		{
			port_err_status->error_status[ port_err_status->error_cnt ] = *(( BCM_PORT_FUNC_STATUS_T *)&g_bcm_port_imp_work_default_status + i);
			port_err_status->error_pos[ port_err_status->error_cnt ] = i;
			port_err_status->error_cnt++;
		}
	}

	#ifdef DEBUG_PORT_ERR
	for(i = 0; i < 	port_err_status->error_cnt; i++)
	{
		printf("port %d error cnt:%d error pos:%d error sts:%d\n",port_number ,port_err_status->error_cnt, port_err_status->error_pos[ i ], port_err_status->error_status[ i ]);
	}
	#endif
	free( p_port_status );

	return port_err_status;
}


void bcm_port_err_status_store( BCM_PORT_T port_number, BCM_PORT_ERR_STATUS_T * err_status)
{
	uint32_t i;
	FILE * file;
	time_t timer;
	time( &timer );
	char time[50];
	char port[7];
	char fact[100];
	char *s_time;
	uint32_t s_time_len;

	file = fopen("/mnt/BCM_ERR.csv","a");
	if (file == NULL)
	{
		printf("open csv error");
	}

	s_time = ctime(&timer);

	s_time_len = strlen(s_time);
	sprintf(port,",Port %d",port_number);

	/*clear "\n"*/
	memcpy(time, s_time, s_time_len - 1);

	for(i = 0; i < err_status->error_cnt; i++)
	{
		memset(fact,0,sizeof(fact));

		switch(err_status->error_pos[i])
		{
			case BCM_PORT_STATUS_LINK_POS:
				strcpy(fact, ",Link failed\n");
				fputs(time,file);
				fputs(port,file);
				fputs(fact,file);
				break;
			case BCM_PORT_STATUS_DUPLEX_POS:
				strcpy(fact, ",Half duplex\n");
				fputs(time,file);
				fputs(port,file);
				fputs(fact,file);
				break;
			case BCM_PORT_STATUS_SPEED_POS:
				strcpy(fact, ",Speed low\n");
				fputs(time,file);
				fputs(port,file);
				fputs(fact,file);
				break;
		}
	}
	fclose(file);
}



static BCM_PORT_STATUS_T bcm_port_link_status_read(BCM_PORT_T port_number)
{
	BCM_PORT_STATUS_T ret_value;
	BCM_PORT_STATUS_T reg_value;
	BCM_PORT_REG_T mask = MASK_SINGLE;
	BCM_PORT_REG_T port_offset;

	reg_value = chip_register_read(BCM_CHIP_REG_STATUS,BCM_CHIP_ADR_LINK);
	
	if ( reg_value == -1 )
	{
		/* code */
		printf( "read error\n" );
		return -1;
	}

	switch (port_number)
	{
		case PORT_0:
			port_offset = ( BCM_PORT_REG_T )PORT_0_OFFSET_LINK;
			break;
		case PORT_1:
			port_offset = ( BCM_PORT_REG_T )PORT_1_OFFSET_LINK;
			break;
		case PORT_2:
			port_offset = ( BCM_PORT_REG_T )PORT_2_OFFSET_LINK;
			break;
		case PORT_3:
			port_offset = ( BCM_PORT_REG_T )PORT_3_OFFSET_LINK;
			break;
//		case PORT_4:
//			port_offset = ( BCM_PORT_REG_T )PORT_4_OFFSET_LINK;
//			break;
//		case PORT_5:
//			port_offset = ( BCM_PORT_REG_T )PORT_5_OFFSET_LINK;
//			break;
//		case PORT_7:
//			port_offset = ( BCM_PORT_REG_T )PORT_7_OFFSET_LINK;
//			break;
		case IMP:
			port_offset = ( BCM_PORT_REG_T )PORT_8_OFFSET_LINK;
			break;

	}

	reg_value >>= port_offset;

	switch (reg_value & mask)
	{
		case LINK_FAIL:
			ret_value = LINK_FAIL;
			printf("Port %d link fail\n",port_number);
			break;
		case LINK_PASS:
			ret_value = LINK_PASS;
			printf("Port %d link pass\n",port_number);
			break;
		default:
			return -1;
	}

	return ret_value;
}

static BCM_PORT_STATUS_T bcm_port_speed_status_read(BCM_PORT_T port_number)
{
	BCM_PORT_STATUS_T ret_value;
	BCM_PORT_STATUS_T reg_value;
	BCM_PORT_REG_T mask = MASK_DOUBLE;
	BCM_PORT_REG_T port_offset;

	reg_value = chip_register_read( BCM_CHIP_REG_STATUS, BCM_CHIP_ADR_SPD );
	
	if ( reg_value == -1 )
	{
		/* code */
		printf( "read error\n" );
		return -1;
	}

	switch (port_number)
	{
		case PORT_0:
			port_offset = ( BCM_PORT_REG_T )PORT_0_OFFSET_SPD;
			break;
		case PORT_1:
			port_offset = ( BCM_PORT_REG_T )PORT_1_OFFSET_SPD;
			break;
		case PORT_2:
			port_offset = ( BCM_PORT_REG_T )PORT_2_OFFSET_SPD;
			break;
		case PORT_3:
			port_offset = ( BCM_PORT_REG_T )PORT_3_OFFSET_SPD;
			break;
//		case PORT_4:
//			port_offset = ( BCM_PORT_REG_T )PORT_4_OFFSET_SPD;
//			break;
//		case PORT_5:
//			port_offset = ( BCM_PORT_REG_T )PORT_5_OFFSET_SPD;
//			break;
//		case PORT_7:
//			port_offset = ( BCM_PORT_REG_T )PORT_7_OFFSET_SPD;
//			break;
		case IMP:
			port_offset = ( BCM_PORT_REG_T )PORT_8_OFFSET_SPD;
			break;
	}


	reg_value >>= port_offset;

	switch (reg_value & mask)
	{
		case TEN_Mbps:
			ret_value = TEN_Mbps;
			#ifdef DEBUG_SPD
				printf("Port %d speed: 10Mbps\n",port_number);
			#endif
			break;
		case HUNDRED_Mbps:
			ret_value = HUNDRED_Mbps;
			#ifdef DEBUG_SPD
				printf("Port %d speed: 100Mbps\n",port_number);
			#endif
			break;
		case THOUSAND_Mbps:
			ret_value = THOUSAND_Mbps;
			#ifdef DEBUG_SPD
				printf("Port %d speed: 1000Mbps\n",port_number);
			#endif
			break;
		default:
			return -1;
	}

	return ret_value;
}

static BCM_PORT_STATUS_T bcm_port_duplex_status_read(BCM_PORT_T port_number)
{
	BCM_PORT_STATUS_T ret_value;
	BCM_PORT_STATUS_T reg_value;
	BCM_PORT_REG_T mask = MASK_SINGLE;
	BCM_PORT_REG_T port_offset;

	reg_value = chip_register_read( BCM_CHIP_REG_STATUS,BCM_CHIP_ADR_DUP );

	if ( reg_value == -1 )
	{
		/* code */
		printf( "read error\n" );
		return -1;
	}

	switch (port_number)
	{
		case PORT_0:
			port_offset = ( BCM_PORT_REG_T )PORT_0_OFFSET_DUP;
			break;
		case PORT_1:
			port_offset = ( BCM_PORT_REG_T )PORT_1_OFFSET_DUP;
			break;
		case PORT_2:
			port_offset = ( BCM_PORT_REG_T )PORT_2_OFFSET_DUP;
			break;
		case PORT_3:
			port_offset = ( BCM_PORT_REG_T )PORT_3_OFFSET_DUP;
			break;
//		case PORT_4:
//			port_offset = ( BCM_PORT_REG_T )PORT_4_OFFSET_DUP;
//			break;
//		case PORT_5:
//			port_offset = ( BCM_PORT_REG_T )PORT_5_OFFSET_DUP;
//			break;
//		case PORT_7:
//			port_offset = ( BCM_PORT_REG_T )PORT_7_OFFSET_DUP;
//			break;
		case IMP:
			port_offset = ( BCM_PORT_REG_T )PORT_8_OFFSET_DUP;
			break;
	}

	reg_value >>= port_offset;

	switch (reg_value & mask)
	{
		case HALF_DUPLEX:
			ret_value = HALF_DUPLEX;
			#ifdef DEBUG_DUP
				printf("Port %d work for Half Duplex\n",port_number);
			#endif
			break;
		case FULL_DUPLEX:
			ret_value = FULL_DUPLEX;
			#ifdef DEBUG_DUP
				printf("Port %d work for Full Duplex\n",port_number);
			#endif
			break;
		default:
			return -1;
	}

	return ret_value;
}









static BCM_PORT_OP_STATUS_T bcm_port_link_status_write(BCM_PORT_T port_number, BCM_PORT_STATUS_T link_status)
{
	BCM_PORT_STATUS_T current_reg_value;
	BCM_PORT_STATUS_T next_reg_value;
	BCM_PORT_REG_T mask = MASK_SINGLE;
	BCM_PORT_REG_T port_offset;

	current_reg_value = chip_register_read( BCM_CHIP_REG_IMP, BCM_CHIP_ADR_IMP );
	if ( current_reg_value == -1 )
	{
		/* code */
		printf( "read error\n" );
		return BCM_PORT_WRITE_ERR;
	}

	switch (port_number)
	{
		case PORT_0:
			port_offset = ( BCM_PORT_REG_T )PORT_0_OFFSET_LINK;
			break;
		case PORT_1:
			port_offset = ( BCM_PORT_REG_T )PORT_1_OFFSET_LINK;
			break;
		case PORT_2:
			port_offset = ( BCM_PORT_REG_T )PORT_2_OFFSET_LINK;
			break;
		case PORT_3:
			port_offset = ( BCM_PORT_REG_T )PORT_3_OFFSET_LINK;
			break;
//		case PORT_4:
//			port_offset = ( BCM_PORT_REG_T )PORT_4_OFFSET_LINK;
//			break;
//		case PORT_5:
//			port_offset = ( BCM_PORT_REG_T )PORT_5_OFFSET_LINK;
//			break;
//		case PORT_7:
//			port_offset = ( BCM_PORT_REG_T )PORT_7_OFFSET_LINK;
//			break;
		case IMP:
			port_offset = IMP_OFFSET_LINK;
			break;

	}

	/*clear the status bits*/
	current_reg_value &= ~(mask << port_offset);

	/*get the new status register value*/
	next_reg_value = current_reg_value + ((BCM_PORT_REG_T)link_status << port_offset);

	/*write register*/
	chip_register_write( BCM_CHIP_REG_IMP, BCM_CHIP_ADR_IMP, next_reg_value);

	return BCM_PORT_WRITE_SUCCESS;
}

static BCM_PORT_OP_STATUS_T  bcm_port_duplex_status_write(BCM_PORT_T port_number, BCM_PORT_STATUS_T duplex_status)
{
	BCM_PORT_STATUS_T current_reg_value;
	BCM_PORT_STATUS_T next_reg_value;

	BCM_PORT_REG_T mask = MASK_SINGLE;
	BCM_PORT_REG_T port_offset;

	current_reg_value = chip_register_read( BCM_CHIP_REG_IMP, BCM_CHIP_ADR_IMP );
	if ( current_reg_value == -1 )
	{
		/* code */
		printf( "read error\n" );
		return BCM_PORT_WRITE_ERR;
	}

	switch (port_number)
	{
//		case PORT_0:
//			port_offset = ( BCM_PORT_REG_T )PORT_0_OFFSET_DUP;
//			break;
//		case PORT_1:
//			port_offset = ( BCM_PORT_REG_T )PORT_1_OFFSET_DUP;
//			break;
//		case PORT_2:
//			port_offset = ( BCM_PORT_REG_T )PORT_2_OFFSET_DUP;
//			break;
//		case PORT_3:
//			port_offset = ( BCM_PORT_REG_T )PORT_3_OFFSET_DUP;
//			break;
//		case PORT_4:
//			port_offset = ( BCM_PORT_REG_T )PORT_4_OFFSET_DUP;
//			break;
//		case PORT_5:
//			port_offset = ( BCM_PORT_REG_T )PORT_5_OFFSET_DUP;
//			break;
//		case PORT_7:
//			port_offset = ( BCM_PORT_REG_T )PORT_7_OFFSET_DUP;
//			break;
		case IMP:
			port_offset = IMP_OFFSET_DUP;
			break;

	}

	/*clear the status bits*/
	current_reg_value &= (~( mask << port_offset ));

	/*get the new status register value*/
	next_reg_value = current_reg_value + ((BCM_PORT_REG_T)duplex_status << port_offset);

	/*write register*/
	chip_register_write( BCM_CHIP_REG_IMP, BCM_CHIP_ADR_IMP, next_reg_value);

	return BCM_PORT_WRITE_SUCCESS;
}

static BCM_PORT_OP_STATUS_T bcm_port_speed_status_write( BCM_PORT_T port_number, BCM_PORT_STATUS_T speed_status )
{
	BCM_PORT_STATUS_T current_reg_value;
	BCM_PORT_STATUS_T next_reg_value;
	BCM_PORT_REG_T mask = MASK_DOUBLE;
	BCM_PORT_REG_T port_offset;

	current_reg_value = chip_register_read( BCM_CHIP_REG_IMP, BCM_CHIP_ADR_IMP );
	if ( current_reg_value == -1 )
	{
		/* code */
		printf( "read error\n" );
		return BCM_PORT_WRITE_ERR;
	}

	switch ( port_number )
	{
		case PORT_0:
			port_offset = ( BCM_PORT_REG_T )PORT_0_OFFSET_SPD;
			break;
		case PORT_1:
			port_offset = ( BCM_PORT_REG_T )PORT_1_OFFSET_SPD;
			break;
		case PORT_2:
			port_offset = ( BCM_PORT_REG_T )PORT_2_OFFSET_SPD;
			break;
		case PORT_3:
			port_offset = ( BCM_PORT_REG_T )PORT_3_OFFSET_SPD;
			break;
//		case PORT_4:
//			port_offset = ( BCM_PORT_REG_T )PORT_4_OFFSET_SPD;
//			break;
//		case PORT_5:
//			port_offset = ( BCM_PORT_REG_T )PORT_5_OFFSET_SPD;
//			break;
//		case PORT_7:
//			port_offset = ( BCM_PORT_REG_T )PORT_7_OFFSET_SPD;
//			break;
		case IMP:
			port_offset = IMP_OFFSET_SPD;
			break;

	}

	/*clear the status bits*/
	current_reg_value &= (~( mask << port_offset ));

	/*get the new status register value*/
	next_reg_value = current_reg_value + (( BCM_PORT_REG_T )speed_status << port_offset);

	/*write register*/
	chip_register_write( BCM_CHIP_REG_IMP, BCM_CHIP_ADR_IMP, next_reg_value);
	return BCM_PORT_WRITE_SUCCESS;
}	
