/************************************************************************************
 *  bcm 53134 vlan base on 802.1Q
 *
 *  Created on: 2017-11-16
 *
 *  Author: buggy
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <time.h>

#include "../chip/chip.h"
#include "vlan.h"

extern BCM_FUNC_OP_T chip_func_write( BCM_REG_ADDR_T * reg_address, BCM_REG_T func_mode );

extern BCM_REG_T     chip_func_read ( BCM_REG_ADDR_T *chip_reg_value );



/****************************************************************************************************/
/*description : vlan create function
/*param 0     : vlan number
 *param 1     : number port
 *param 2     : PORT_0,PORT_1...
/*return      : function status
*/
void bcm_vlan_create(bcm_vlan_creat_data_t * vlan);

/****************************************************************************************************/
/*description : read vlan status
/*param 0     : n/a
/*return      : n/a
*/

void bcm_vlan_status_read(BCM_VLAN_T num_vlan);

void bcm_vlan_status_single_read(BCM_VLAN_T);

void bcm_vlan_normal_status_read();

/****************************************************************************************************/
/*description : vlan enable status write
/*param 0     : enable
/*return      : function status
*/
static BCM_REG_T bcm_vlan_enable_write( BCM_REG_T mode );

/****************************************************************************************************/
/*description : vlan enable status read
/*param 0     : n/a
/*return      : function status
*/
static BCM_REG_T bcm_vlan_enable_read();

/****************************************************************************************************/
/*description: vlan table initialization control
/*param 0: no
*/
static BCM_FUNC_OP_T bcm_vlan_table_ctrl_init();

/****************************************************************************************************/
/*description: vlan table read or write control
/*param 0: function post
*/
static BCM_FUNC_OP_T bcm_vlan_table_ctrl_rw_write(BCM_REG_T rw_value);

/****************************************************************************************************/
/*description: vlan table judge control finish
/*param 0: n/a
*/
static BCM_FUNC_OP_T bcm_vlan_table_ctrl_init_finish();

/****************************************************************************************************/
/*description: vlan table write index
/*param 0: index nubmer 0-4095,0 and 4095 no effect
*/
static BCM_FUNC_OP_T bcm_vlan_table_index_write(BCM_VLAN_VTBL_ADDR_T index);

/****************************************************************************************************/
/*description: vlan table judge control finish
/*param 0: n/a
*/
static BCM_FUNC_OP_T bcm_vlan_table_entry_write(BCM_VLAN_VTBL_FUNC_T * entry);

/****************************************************************************************************/
/*description: vlan table entry mspt index write
/*param 0 : mstp index
 *return  : status
*/
static BCM_FUNC_OP_T bcm_vlan_table_entry_mspt_index_write(BCM_REG_T mspt_index_number);     //default

/****************************************************************************************************/
/*description: vlan table entry forward map write
/*param 0    : vlan table entry forward map
 *return     : operation status
*/
static BCM_FUNC_OP_T bcm_vlan_table_entry_fwd_map_write(BCM_REG_T entry_fwd_map);

/****************************************************************************************************/
/*description: write vlan table entry untag map
/*param 0 : vlan table entry untag map
/*return  : operation status
*/
static BCM_FUNC_OP_T bcm_vlan_table_entry_tag_map_write(BCM_REG_T entry_tag_map);

/****************************************************************************************************/
/*description: write vlan table entry forward mode
/*param 0 : forward mode
/*return  : operation status
*/
static BCM_REG_T bcm_vlan_table_entry_fwd_mode_write(BCM_REG_T forward_mode);

static BCM_REG_T bcm_vlan_table_single_read( BCM_PORT_T, BCM_REG_T, BCM_VLAN_VTBL_ADDR_T );

/****************************************************************************************************/
/*description: The VLAN-tagged Frame is allowed to be forwarded to the destination ports corresponding
/*			   bits set in the Map.
/*param 0    : port
/*param 1    : forward mode
/*return     : operation status
*/
static BCM_REG_T bcm_vlan_table_port_forward_write(BCM_PORT_T port_number, BCM_REG_T forward_mode);

/****************************************************************************************************/
/*description: write vlan table port tag
/*param 0    : port number
/*param 1    : work mode
/*return     : operation status
*/
static BCM_REG_T bcm_vlan_table_port_tag_write(BCM_PORT_T port_number, BCM_REG_T forward_mode);

/****************************************************************************************************/
/*description: read vlan table port forward status
/*param 0    : port number
/*return     : operation status
*/
static BCM_REG_T bcm_vlan_table_port_forward_read(BCM_PORT_T port_number );

/****************************************************************************************************/
/*description: read vlan table port tag
/*param 0    : port number
/*return     : operation status
*/
static BCM_REG_T bcm_vlan_table_port_tag_read( BCM_PORT_T port_number );

/****************************************************************************************************/
/*description: write port vid
/*param 0    : port number
/*param 1    : VID
/*return     : operation status
*/
BCM_FUNC_OP_T bcm_vlan_tag_info_pvid_write(BCM_PORT_T port_number, BCM_VLAN_PVID_T vid );

#ifdef DEBUG_VLAN
/****************************************************************************************************/
/*description: write vlan tag information
/*param 0    : port number
/*param 1    : tag information data
/*return     : operation status
*/
static BCM_FUNC_OP_T bcm_vlan_default_tag_info_write(BCM_PORT_T port_number, BCM_VLAN_TAG_INFO_T * tag_info);
#endif

/****************************************************************************************************/
/*description: read port vid
/*param 0    : port number
/*return     : operation status
*/
static BCM_REG_T bcm_vlan_pvid_read( BCM_PORT_T port_number );

/****************************************************************************************************/
/*description: write change 1Q vid
/*param 0    : work mode
/*return     : operation status
*/
static BCM_FUNC_OP_T bcm_vlan_ctrl_0_change_1Q_VID_write( BCM_REG_T mode );

/****************************************************************************************************/
/*description: write change 1Q vid out tag
/*param 0    : work mode
/*return     : operation status
*/
static BCM_FUNC_OP_T bcm_vlan_ctrl_0_change_1Q_VID_OUT_write( BCM_REG_T mode );

/****************************************************************************************************/
/*description: write change 1Q vid in tag
/*param 0    : work mode
/*return     : operation status
*/
static BCM_FUNC_OP_T bcm_vlan_ctrl_0_change_1Q_VID_IN_write( BCM_REG_T mode );

/****************************************************************************************************/
/*description: write IPMC frame tag
/*param 0    : work mode
/*return     : operation status
*/
static BCM_FUNC_OP_T bcm_vlan_ctrl_1_IPMC_frame_tag_write( BCM_REG_T mode );

/****************************************************************************************************/
/*description: write IPMC frame forward
/*param 0    : work mode
/*return     : operation status
*/
static BCM_FUNC_OP_T bcm_vlan_ctrl_1_IPMC_frame_fwd_write( BCM_REG_T mode );

/****************************************************************************************************/
/*description: write multicast frame tag map
/*param 0    : work mode
/*return     : operation status
*/
static BCM_FUNC_OP_T bcm_vlan_ctrl_1_mcast_frame_tag_write( BCM_REG_T mode );

/****************************************************************************************************/
/*description: write multicast frame check forward map
/*param 0    : work mode
/*return     : operation status
*/
static BCM_FUNC_OP_T bcm_vlan_ctrl_1_mcast_frame_fwd_write( BCM_REG_T mode );

/****************************************************************************************************/
/*description: write multicast frame tag map
/*param 0    : work mode
/*return     : operation status
*/
static BCM_FUNC_OP_T bcm_vlan_ctrl_2_GG_frame_tag_write( BCM_REG_T mode );

/****************************************************************************************************/
/*description: write multicast frame check forward map
/*param 0    : work mode
/*return     : operation status
*/
static BCM_FUNC_OP_T bcm_vlan_ctrl_2_GG_frame_check_fwdmap_write( BCM_REG_T mode );

/****************************************************************************************************/
/*description: write mii frame check forward map
/*param 0    : work mode
/*return     : operation status
*/
static BCM_FUNC_OP_T bcm_vlan_ctrl_2_mii_frame_check_fwdmap_write( BCM_REG_T mode );

/****************************************************************************************************/
/*description: write vlan multi port address control
/*param 0    : port number
/*param 1    : work mode
/*return     : operation status
*/
static BCM_FUNC_OP_T bcm_vlan_multi_port_addr_ctrl_write(BCM_PORT_T port_number, BCM_REG_T mode);

/****************************************************************************************************/
/*description: read vlan multi port address control
/*param 0    : port number
/*return     : operation status
*/
static BCM_FUNC_OP_T bcm_vlan_multi_port_addr_ctrl_read(BCM_PORT_T port_number);

/****************************************************************************************************/
/*description: write vlan egress vid remark table access
/*param 0    : table list
/*return     : operation status
*/
static BCM_FUNC_OP_T bcm_vlan_egress_VID_remark_table_access_write( BCM_VLAN_EGRESS_VID_RMK_TBL_ACS_T *table );

/****************************************************************************************************/
/*description : vlan table entry untag map write
/*param 0     : vlan table entry untag map
/*return      : operation status
*/
static BCM_FUNC_OP_T bcm_vlan_egress_VID_remark_table_data_write( BCM_VLAN_EGRESS_VID_RMK_TBL_DAT_T *data );

/****************************************************************************************************/
/*description: write vlan mst table
/*param 0    : port number
/*param 1    : work mode
/*return     : operation status
*/
static BCM_FUNC_OP_T bcm_vlan_mst_table_write(BCM_PORT_T port_number, BCM_REG_T mode);

/****************************************************************************************************/
/*description: read vlan mst table
/*param 0    : port number
/*return     : operation status
*/
static BCM_REG_T bcm_vlan_mst_table_read( BCM_PORT_T port_number );

/****************************************************************************************************/
/*description: multi write vlan table
/*param 0    : multi vlan table entry data
/*param 1    : vlan table entry index list
/*return     : function status
*/
static void bcm_vlan_table_multi_write( void *vtbl_entry_data,uint32_t count);

/****************************************************************************************************/
/*description: single write vlan table
/*param 0    : single vlan table entry data
/*param 1    : vlan table entry index
/*return     : function status
*/
void bcm_vlan_table_single_write( BCM_VLAN_VTBL_FUNC_T *vtbl_entry,BCM_VLAN_T vtbl_index);

/****************************************************************************************************/
/*description: read vlan table
/*param 0    : vlan table entry index
/*return     : function status
*/
static void bcm_vlan_table_read( BCM_VLAN_VTBL_ADDR_T vtbl_index_max );

/****************************************************************************************************/
/*description: clear vlan table
/*param 0    : vlan table entry index
/*return     : function status
*/
static void bcm_vlan_table_clear( BCM_VLAN_VTBL_ADDR_T vtbl_index );

/****************************************************************************************************/
/*description: write port join all vlan
/*param 0    : port number
/*param 1    : enable or disable
/*return     : function status
*/
static BCM_REG_T bcm_vlan_join_all_vlan_en(BCM_PORT_T port_number, BCM_REG_T mode );

/****************************************************************************************************/
/*description: read port join all vlan
/*param 0    : port number
/*return     : function status
*/
static BCM_REG_T bcm_vlan_join_all_vlan_en_read(BCM_PORT_T port_number);

/****************************************************************************************************/
/*description: wrtie double tag function
/*param 0    : enable or disable
/*return     : function status
*/
static BCM_REG_T bcm_vlan_double_tag_write(BCM_REG_T enable);

/****************************************************************************************************/
/*description: read double tag function
/*param 0    : N/A
/*return     : function status
*/
static BCM_REG_T bcm_vlan_double_tag_read();

/****************************************************************************************************/
/*description: write multi port type
/*param 0    : vlan table entry untag map
/*return     : status
*/
static BCM_REG_T bcm_vlan_ISP_port_write(BCM_PORT_T port_number, BCM_REG_T mode );

/****************************************************************************************************/
/*description: read mulit port is ISP or customer port
/*param 0    : Mport number
/*return     : status
*/
static BCM_REG_T bcm_vlan_ISP_port_read( BCM_PORT_T port_number );



void bcm_vlan()
{
	bcm_vlan_t * vlan;

	vlan = (bcm_vlan_t *)malloc(sizeof(bcm_vlan_t));

	/*vlan 1 profile*/
	BCM_PORT_T  port_group_default_1[NUM_PORT] = {PORT_UNUSE,
			 	 	 	 	 	 	 	    	  PORT_UNUSE,
												  PORT_UNUSE,
												  PORT_USE,
												  PORT_UNUSE,
												  PORT_UNUSE,
												  PORT_UNUSE,
												  PORT_UNUSE,
												  PORT_USE};

	/*vlan 2 profile*/
	BCM_PORT_T  port_group_default_0[NUM_PORT] = {PORT_USE,
			 	 	 	 	 	 	 	    	  PORT_UNUSE,
												  PORT_USE,
												  PORT_UNUSE,
												  PORT_UNUSE,
												  PORT_UNUSE,
												  PORT_UNUSE,
												  PORT_UNUSE,
												  PORT_UNUSE};

	/*create vlan 1 data eva*/
	vlan->data.creat.mst_mode = SPANNING_TREE_FORWARD;
	vlan->data.creat.pvid = PVID_0;
	vlan->data.creat.vlan_index = VLAN_0;
	vlan->data.creat.vlan_mode = VLAN_ENABLE;
	vlan->data.num_vlan = VLAN_1;
	memcpy( vlan->data.creat.port_group,port_group_default_0, NUM_PORT * sizeof(BCM_PORT_T));

	/*create vlan function pointer*/
	vlan->operate.status = bcm_vlan_status_read;

	/*vlan operation function pointer*/
	vlan->operate.create = bcm_vlan_create;
	vlan->operate.enable = bcm_vlan_enable_write;

	/*vlan 1 operate create function call*/
	vlan->operate.create(&vlan->data.creat);

	/*create vlan 2 data eva*/
	vlan->data.creat.mst_mode = SPANNING_TREE_FORWARD;
	vlan->data.creat.pvid = PVID_1;
	vlan->data.creat.vlan_index = VLAN_1;

	memcpy( vlan->data.creat.port_group,port_group_default_1, NUM_PORT * sizeof(BCM_PORT_T));

	/*vlan 1 operate create function call*/
	vlan->operate.create(&vlan->data.creat);

	/*vlan enable*/
	vlan->operate.enable(vlan->data.creat.vlan_mode);

	vlan->operate.status(vlan->data.num_vlan);

	free(vlan);
}

void bcm_vlan_create(bcm_vlan_creat_data_t * vlan)
{
	BCM_PORT_T i;
	BCM_VLAN_VTBL_FUNC_T vtbl_entry_value;

	bcm_vlan_creat_op_t * vlan_creat;

	vlan_creat = (bcm_vlan_creat_op_t *)malloc(sizeof(bcm_vlan_creat_op_t));

	vlan_creat->mst_table_write = bcm_vlan_mst_table_write;
	vlan_creat->pvid_write = bcm_vlan_tag_info_pvid_write;
	vlan_creat->table_single_write = bcm_vlan_table_single_write;


	memset(&vtbl_entry_value,0,sizeof(BCM_VLAN_VTBL_FUNC_T));

	for(i = 0; i < NUM_PORT;i++)
	{
		if(vlan->port_group[i] == PORT_USE)
		{
			vtbl_entry_value.fwd_map += 1 << i;
			vtbl_entry_value.untag_map += 1 << i;
			vlan_creat->pvid_write(i,vlan->pvid);
			vlan_creat->mst_table_write(i,vlan->mst_mode);
		}
	}

	vlan_creat->table_single_write(&vtbl_entry_value, vlan->vlan_index);
	free(vlan_creat);
}

//void bcm_vlan_status_read(BCM_VLAN_T num_vlan)
//{
//	BCM_PORT_T i;
//
//	for(i = 1; i <= num_vlan; i++)
//	{
//		bcm_vlan_status_single_read(i);
//	}
//
//	bcm_vlan_enable_read();
//
//	printf("*******************spinning tree set*******************\n");
//	bcm_vlan_mst_table_read(PORT_0);
//	bcm_vlan_mst_table_read(PORT_1);
//	bcm_vlan_mst_table_read(PORT_2);
//	bcm_vlan_mst_table_read(PORT_3);
//	bcm_vlan_mst_table_read(PORT_4);
//	bcm_vlan_mst_table_read(PORT_5);
//	bcm_vlan_mst_table_read(PORT_7);
//
//	printf("********************vlan join set**********************\n");
//	bcm_vlan_join_all_vlan_en_read(PORT_0);
//	bcm_vlan_join_all_vlan_en_read(PORT_1);
//	bcm_vlan_join_all_vlan_en_read(PORT_2);
//	bcm_vlan_join_all_vlan_en_read(PORT_3);
//	bcm_vlan_join_all_vlan_en_read(PORT_4);
//	bcm_vlan_join_all_vlan_en_read(PORT_5);
//	bcm_vlan_join_all_vlan_en_read(PORT_6);
//	bcm_vlan_join_all_vlan_en_read(PORT_7);
//	bcm_vlan_join_all_vlan_en_read(IMP);
//}

void bcm_vlan_status_read(BCM_VLAN_T num_vlan)
{
	bcm_vlan_status_t * vlan_status;

	vlan_status = (bcm_vlan_status_t * )malloc(sizeof(bcm_vlan_status_t));

	vlan_status->operate.enable_read = bcm_vlan_enable_read;
	vlan_status->operate.join_all_vlan_en_read = bcm_vlan_join_all_vlan_en_read;
	vlan_status->operate.mst_table_read = bcm_vlan_mst_table_read;
	vlan_status->operate.status_single_read = bcm_vlan_status_single_read;

	BCM_PORT_T i;
	printf("*********************vlan status set*******************\n");
	for(i = 1; i <= num_vlan; i++)
	{
		vlan_status->operate.status_single_read(i);
	}

	printf("*******************spinning tree set*******************\n");
	for(i = 0; i < NUM_PORT - 1; i++)
	{
		if( i != PORT_6 )
			bcm_vlan_mst_table_read(i);
	}
	printf("********************vlan join set**********************\n");
	for(i = 0; i < NUM_PORT; i++)
	{
		if( i != PORT_6 )
			bcm_vlan_join_all_vlan_en_read(i);
	}

	vlan_status->operate.enable_read();

	free(vlan_status);
}


void bcm_vlan_status_single_read(BCM_VLAN_T vlan_num)
{
	BCM_PORT_T i = 0;
	BCM_PORT_T pvid_port = 0;
	BCM_REG_T fwd_status[NUM_PORT];

	for(i = 0; i < NUM_PORT; i++)
	{
		if( i != PORT_6 )
		{
			fwd_status[i] = bcm_vlan_table_single_read(i,PORT_FWD,vlan_num);
		}
	}
	printf("VLAN %d contain: ",vlan_num);
	for(i = 0; i < NUM_PORT;i++)
	{
		if(fwd_status[i] == PORT_FWD)
		{
			printf("port %d,",i);
			pvid_port = i;
		}
	}
	printf("VID = %lld\n",bcm_vlan_pvid_read(pvid_port));
}

void bcm_vlan_normal_status_read()
{


	bcm_vlan_table_read( 2 );



}

/*description: read vlan table*/
static BCM_REG_T bcm_vlan_table_single_read( BCM_PORT_T port_num, BCM_REG_T type,BCM_VLAN_VTBL_ADDR_T vtbl_index )
{
	uint32_t retry = 0;
	BCM_REG_T port_status;
	bcm_vlan_table_index_write( vtbl_index );

	/*vlan table control*/
	bcm_vlan_table_ctrl_rw_write(VLAN_TBL_READ);

	bcm_vlan_table_ctrl_init();

	/* wait for complete */
	for(retry = 0; retry < TIME_OUT; retry++)
	{
		switch(bcm_vlan_table_ctrl_init_finish())
		{
			case VLAN_TBL_INIT_FINISH:
				retry = TIME_OUT;
				#ifdef DEBUG_VLAN
				printf("read finish\n");
				#endif
				break;
			case VLAN_TBL_INIT_START:
				if(retry == TIME_OUT - 1)
				{
					perror("read VTBL timeout");
					break;
				}
				printf("reading\n");
				break;
			default :
				perror("VTBL");
		}
	}


	if(type == PORT_FWD)
	{
		switch(port_num)
		{
			case PORT_0:
				port_status = bcm_vlan_table_port_forward_read(PORT_0);
				break;
			case PORT_1:
				port_status = bcm_vlan_table_port_forward_read(PORT_1);
				break;
			case PORT_2:
				port_status = bcm_vlan_table_port_forward_read(PORT_2);
				break;
			case PORT_3:
				port_status = bcm_vlan_table_port_forward_read(PORT_3);
				break;
			case PORT_4:
				port_status = bcm_vlan_table_port_forward_read(PORT_4);
				break;
			case PORT_5:
				port_status = bcm_vlan_table_port_forward_read(PORT_5);
				break;
			case PORT_7:
				port_status = bcm_vlan_table_port_forward_read(PORT_7);
				break;
			case IMP:
				port_status = bcm_vlan_table_port_forward_read(IMP);
				break;
			default:
				perror("port error");
		}
	}
	else
	{
		switch(port_num)
		{
			case PORT_0:
				port_status = bcm_vlan_table_port_tag_read(PORT_0);
				break;
			case PORT_1:
				port_status = bcm_vlan_table_port_tag_read(PORT_1);
				break;
			case PORT_2:
				port_status = bcm_vlan_table_port_tag_read(PORT_2);
				break;
			case PORT_3:
				port_status = bcm_vlan_table_port_tag_read(PORT_3);
				break;
			case PORT_4:
				port_status = bcm_vlan_table_port_tag_read(PORT_4);
				break;
			case PORT_5:
				port_status = bcm_vlan_table_port_tag_read(PORT_5);
				break;
			case PORT_7:
				port_status = bcm_vlan_table_port_tag_read(PORT_7);
				break;
			case IMP:
				port_status = bcm_vlan_table_port_tag_read(IMP);
				break;
			default:
				perror("port error");
		}
	}
	return port_status;
}

/****************************************************************************************************/
/*description: enable 802.1Q VLAN
/* para 0: enable or disable
*/
static BCM_REG_T bcm_vlan_enable_write( BCM_REG_T mode )
{
	/*register value*/
	BCM_REG_ADDR_T vlan_enable_reg_value;

	vlan_enable_reg_value.spi_page =   BCM_SPI_PAGE_VLAN;
	vlan_enable_reg_value.spi_offset = BCM_SPI_OFFSET_VLAN_CTRL_0;
	vlan_enable_reg_value.bit_offset = BCM_VLAN_BIT_OFFSET_VLAN_ENABLE;
	vlan_enable_reg_value.bit_wide =   BCM_CHIP_BIT_1;

	return chip_func_write( &vlan_enable_reg_value, mode );
}

//static BCM_REG_T bcm_vlan_enable_read()
//{
//	BCM_REG_T mode;
//
//	/*register value*/
//	BCM_REG_ADDR_T vlan_enable_reg_value;
//
//	vlan_enable_reg_value.spi_page =   BCM_SPI_PAGE_VLAN;
//	vlan_enable_reg_value.spi_offset = BCM_SPI_OFFSET_VLAN_CTRL_0;
//	vlan_enable_reg_value.bit_offset = BCM_VLAN_BIT_OFFSET_VLAN_ENABLE;
//	vlan_enable_reg_value.bit_wide =   BCM_CHIP_BIT_1;
//
//	mode = chip_func_read( &vlan_enable_reg_value );
//
//	switch ( mode )
//	{
//		case VLAN_ENABLE:
//			printf("VLAN Enable\n");
//			break;
//		case VLAN_DISABLE:
//			printf("VLAN Disable\n");
//			break;
//		default:
//			printf("error\n");
//	}
//
//	return mode;
//}

static BCM_REG_T bcm_vlan_enable_read()
{
	bcm_func_t vlan_enable;

	vlan_enable.function_address.spi_page = BCM_SPI_PAGE_VLAN;
	vlan_enable.function_address.spi_offset = BCM_SPI_OFFSET_VLAN_CTRL_0;
	vlan_enable.function_address.bit_offset = BCM_VLAN_BIT_OFFSET_VLAN_ENABLE;
	vlan_enable.function_address.bit_wide = BCM_CHIP_BIT_1;

	vlan_enable.function_read = chip_func_read;
	vlan_enable.function_write = chip_func_write;

	vlan_enable.function_address.func_value =
	vlan_enable.function_read(&vlan_enable.function_address);

	switch ( vlan_enable.function_address.func_value )
	{
		case VLAN_ENABLE:
			printf("VLAN Enable\n");
			break;
		case VLAN_DISABLE:
			printf("VLAN Disable\n");
			break;
		default:
			printf("error\n");
	}
	return 	vlan_enable.function_address.func_value;
}

/*normal mode*/
/*description: vlan table initialization control*/
static BCM_FUNC_OP_T bcm_vlan_table_ctrl_init()
{
	BCM_REG_ADDR_T vlan_table_ctrl_rw_reg_value;
	BCM_REG_T init_value = VLAN_TBL_INIT_START;

	vlan_table_ctrl_rw_reg_value.spi_page =   BCM_SPI_PAGE_VTBL;
	vlan_table_ctrl_rw_reg_value.spi_offset = BCM_SPI_OFFSET_VTBL_CTRL;
	vlan_table_ctrl_rw_reg_value.bit_offset = BCM_BIT_OFFSET_VTBL_CTRL_STDN;
	vlan_table_ctrl_rw_reg_value.bit_wide =   BCM_CHIP_BIT_1;

	return chip_func_write( &vlan_table_ctrl_rw_reg_value, init_value );
}

/*description: vlan table read or write control*/
static BCM_FUNC_OP_T bcm_vlan_table_ctrl_rw_write(BCM_REG_T rw_value)
{
	BCM_REG_ADDR_T vlan_table_ctrl_rw_reg_value;

	vlan_table_ctrl_rw_reg_value.spi_page =   BCM_SPI_PAGE_VTBL;
	vlan_table_ctrl_rw_reg_value.spi_offset = BCM_SPI_OFFSET_VTBL_CTRL;
	vlan_table_ctrl_rw_reg_value.bit_offset = BCM_BIT_OFFSET_VTBL_CTRL_RWCL;
	vlan_table_ctrl_rw_reg_value.bit_wide =   BCM_CHIP_BIT_2;

	return chip_func_write( &vlan_table_ctrl_rw_reg_value, rw_value );
}

/*description: vlan table judge control finish*/
static BCM_FUNC_OP_T bcm_vlan_table_ctrl_init_finish()
{
	BCM_REG_T mode;

	BCM_REG_ADDR_T ctrl_init_reg_value;

	ctrl_init_reg_value.spi_page =   BCM_SPI_PAGE_VTBL;
	ctrl_init_reg_value.spi_offset = BCM_SPI_OFFSET_VTBL_CTRL;
	ctrl_init_reg_value.bit_offset = BCM_BIT_OFFSET_VTBL_CTRL_STDN;
	ctrl_init_reg_value.bit_wide =   BCM_CHIP_BIT_1;

	mode = chip_func_read( &ctrl_init_reg_value );
	#ifdef VLAN_DEBUG
	switch ( mode )
	{
		case VLAN_TBL_INIT_START:
			printf("Initialization start\n");
			break;
		case VLAN_TBL_INIT_FINISH:

			printf("Initialization finish\n");

			break;
		default:
			printf("error\n");
	}
	#endif

	return mode;
}


/*structure mode*/
/*description: vlan table write index*/
static BCM_FUNC_OP_T bcm_vlan_table_index_write(BCM_VLAN_VTBL_ADDR_T index)
{
	return chip_register_write(BCM_SPI_PAGE_VTBL, BCM_SPI_OFFSET_VTBL_INDEX, (uint64_t)index);
}

/*structure mode*/
/*description: vlan table judge control finish*/
static BCM_FUNC_OP_T bcm_vlan_table_entry_write(BCM_VLAN_VTBL_FUNC_T * entry)
{
	BCM_SPI_PAGE_VLAN_T vtbl_entry_reg_value = 0;

	vtbl_entry_reg_value += entry->fwd_mode << BCM_BIT_OFFSET_VTBL_ENTRY_FWD_MOD;
	vtbl_entry_reg_value += entry->mstp_index << BCM_BIT_OFFSET_VTBL_ENTRY_MSTP_INDEX;
	vtbl_entry_reg_value += entry->untag_map << BCM_BIT_OFFSET_VTBL_ENTRY_UNTAG_MAP;
	vtbl_entry_reg_value += entry->fwd_map << BCM_BIT_OFFSET_VTBL_ENTRY_FWD_MAP;

	return chip_register_write(BCM_SPI_PAGE_VTBL, BCM_SPI_OFFSET_VTBL_ENTRY, vtbl_entry_reg_value);
}

/*description: vlan table entry mspt index write*/
static BCM_FUNC_OP_T bcm_vlan_table_entry_mspt_index_write(BCM_REG_T mspt_index_number)     //default
{
	BCM_REG_ADDR_T entry_mspt_index_reg_value;

	entry_mspt_index_reg_value.spi_page =   BCM_SPI_PAGE_VTBL;
	entry_mspt_index_reg_value.spi_offset = BCM_SPI_OFFSET_VTBL_ENTRY;
	entry_mspt_index_reg_value.bit_offset =	BCM_BIT_OFFSET_VTBL_ENTRY_MSTP_INDEX;
	entry_mspt_index_reg_value.bit_wide =   BCM_CHIP_BIT_1;

	return chip_func_write( &entry_mspt_index_reg_value, mspt_index_number );
}

/*description: vlan table entry forward map write*/
static BCM_FUNC_OP_T bcm_vlan_table_entry_fwd_map_write(BCM_REG_T entry_fwd_map)
{
	BCM_REG_ADDR_T entry_fwd_map_reg_value;

	entry_fwd_map_reg_value.spi_page =   BCM_SPI_PAGE_VTBL;
	entry_fwd_map_reg_value.spi_offset = BCM_SPI_OFFSET_VTBL_ENTRY;
	entry_fwd_map_reg_value.bit_offset = BCM_BIT_OFFSET_VTBL_ENTRY_FWD_MAP;
	entry_fwd_map_reg_value.bit_wide =   BCM_CHIP_BIT_8;

	return chip_func_write( &entry_fwd_map_reg_value, entry_fwd_map );
}

static BCM_FUNC_OP_T bcm_vlan_table_entry_fwd_map_port_write(BCM_PORT_T port_num, BCM_REG_T mode)
{
	BCM_REG_ADDR_T entry_fwd_map_reg_value;

	entry_fwd_map_reg_value.spi_page =   BCM_SPI_PAGE_VTBL;
	entry_fwd_map_reg_value.spi_offset = BCM_SPI_OFFSET_VTBL_ENTRY;

	entry_fwd_map_reg_value.bit_wide =   BCM_CHIP_BIT_1;

	switch(port_num)
	{
		case PORT_0:
			entry_fwd_map_reg_value.bit_offset = BCM_BIT_OFFSET_VTBL_ENTRY_FWD_MAP_PORT_0;
			break;
		case PORT_1:
			entry_fwd_map_reg_value.bit_offset = BCM_BIT_OFFSET_VTBL_ENTRY_FWD_MAP_PORT_1;
			break;
		case PORT_2:
			entry_fwd_map_reg_value.bit_offset = BCM_BIT_OFFSET_VTBL_ENTRY_FWD_MAP_PORT_2;
			break;
		case PORT_3:
			entry_fwd_map_reg_value.bit_offset = BCM_BIT_OFFSET_VTBL_ENTRY_FWD_MAP_PORT_3;
			break;
		case PORT_4:
			entry_fwd_map_reg_value.bit_offset = BCM_BIT_OFFSET_VTBL_ENTRY_FWD_MAP_PORT_4;
			break;
		case PORT_5:
			entry_fwd_map_reg_value.bit_offset = BCM_BIT_OFFSET_VTBL_ENTRY_FWD_MAP_PORT_5;
			break;
		case IMP:
			entry_fwd_map_reg_value.bit_offset = BCM_BIT_OFFSET_VTBL_ENTRY_FWD_MAP_PORT_8;
			break;
		default :
			perror("forward map port");
	}

	return chip_func_write( &entry_fwd_map_reg_value, mode );
}

static void bcm_vlan_fwd_port_write(uint32_t num_port,...)
{
	va_list var_arg;
	uint8_t i;
	BCM_PORT_T port_num_temp = 0;

	va_start(var_arg,num_port);

	for(i = 0; i < num_port; i++)
	{
		port_num_temp = va_arg(var_arg,BCM_PORT_T);
		bcm_vlan_table_entry_fwd_map_port_write(port_num_temp, PORT_FWD);
	}
	va_end(var_arg);
}

static BCM_FUNC_OP_T bcm_vlan_table_entry_tag_map_port_write(BCM_PORT_T port_num, BCM_REG_T mode)
{
	BCM_REG_ADDR_T entry_tag_map_reg_value;

	entry_tag_map_reg_value.spi_page =   BCM_SPI_PAGE_VTBL;
	entry_tag_map_reg_value.spi_offset = BCM_SPI_OFFSET_VTBL_ENTRY;

	entry_tag_map_reg_value.bit_wide =   BCM_CHIP_BIT_1;

	switch(port_num)
	{
		case PORT_0:
			entry_tag_map_reg_value.bit_offset = BCM_BIT_OFFSET_VTBL_ENTRY_FWD_MAP_PORT_0;
			break;
		case PORT_1:
			entry_tag_map_reg_value.bit_offset = BCM_BIT_OFFSET_VTBL_ENTRY_FWD_MAP_PORT_1;
			break;
		case PORT_2:
			entry_tag_map_reg_value.bit_offset = BCM_BIT_OFFSET_VTBL_ENTRY_FWD_MAP_PORT_2;
			break;
		case PORT_3:
			entry_tag_map_reg_value.bit_offset = BCM_BIT_OFFSET_VTBL_ENTRY_FWD_MAP_PORT_3;
			break;
		case PORT_4:
			entry_tag_map_reg_value.bit_offset = BCM_BIT_OFFSET_VTBL_ENTRY_FWD_MAP_PORT_4;
			break;
		case PORT_5:
			entry_tag_map_reg_value.bit_offset = BCM_BIT_OFFSET_VTBL_ENTRY_FWD_MAP_PORT_5;
			break;
		case IMP:
			entry_tag_map_reg_value.bit_offset = BCM_BIT_OFFSET_VTBL_ENTRY_FWD_MAP_PORT_8;
			break;
		default :
			perror("forward map port");
	}

	return chip_func_write( &entry_tag_map_reg_value, mode );
}

static void bcm_vlan_tag_port_write(uint32_t num_port,...)
{
	va_list var_arg;
	uint8_t i;
	BCM_PORT_T port_num_temp = 0;

	va_start(var_arg,num_port);

	for(i = 0; i < num_port; i++)
	{
		port_num_temp = va_arg(var_arg,BCM_PORT_T);
		bcm_vlan_table_entry_tag_map_port_write(port_num_temp, PORT_UNTAG);
	}
	va_end(var_arg);
}


/*description: write vlan table entry untag map*/
static BCM_FUNC_OP_T bcm_vlan_table_entry_tag_map_write(BCM_REG_T entry_tag_map)
{
	BCM_REG_ADDR_T entry_tag_map_reg_value;

	entry_tag_map_reg_value.spi_page =   BCM_SPI_PAGE_VTBL;
	entry_tag_map_reg_value.spi_offset = BCM_SPI_OFFSET_VTBL_ENTRY;
	entry_tag_map_reg_value.bit_offset = BCM_BIT_OFFSET_VTBL_ENTRY_UNTAG_MAP;
	entry_tag_map_reg_value.bit_wide =   BCM_CHIP_BIT_8;

	return chip_func_write( &entry_tag_map_reg_value, entry_tag_map );
}

/*description: write vlan table entry forward mode*/
static BCM_REG_T bcm_vlan_table_entry_fwd_mode_write(BCM_REG_T forward_mode)
{
	BCM_REG_ADDR_T vlan_table_ctrl_rw_reg_value;

	vlan_table_ctrl_rw_reg_value.spi_page =   BCM_SPI_PAGE_VTBL;
	vlan_table_ctrl_rw_reg_value.spi_offset = BCM_SPI_OFFSET_VTBL_ENTRY;
	vlan_table_ctrl_rw_reg_value.bit_offset = BCM_BIT_OFFSET_VTBL_CTRL_FWD_MODE;
	vlan_table_ctrl_rw_reg_value.bit_wide =   BCM_CHIP_BIT_1;

	return chip_func_write( &vlan_table_ctrl_rw_reg_value, forward_mode );
}



/****************************************************************************************************/
/*description: The VLAN-tagged Frame is allowed to be forwarded to the destination ports corresponding
/*			   bits set in the Map.
/* param 0: port
/* param 1: forward mode
*/
static BCM_REG_T bcm_vlan_table_port_forward_write(BCM_PORT_T port_number, BCM_REG_T forward_mode)
{
	BCM_REG_ADDR_T vlan_port_forward_reg_value;

	vlan_port_forward_reg_value.spi_page =   BCM_SPI_PAGE_VTBL;
	vlan_port_forward_reg_value.spi_offset = BCM_SPI_OFFSET_VTBL_ENTRY;

	vlan_port_forward_reg_value.bit_wide =   BCM_CHIP_BIT_1;

	switch ( port_number )
	{
		case PORT_0:
			vlan_port_forward_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_VLAN_BIT_OFFSET_VLAN_FORWARD_PORT_0;
			break;
		case PORT_1:
			vlan_port_forward_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_VLAN_BIT_OFFSET_VLAN_FORWARD_PORT_1;
			break;
		case PORT_2:
			vlan_port_forward_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_VLAN_BIT_OFFSET_VLAN_FORWARD_PORT_2;
			break;
		case PORT_3:
			vlan_port_forward_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_VLAN_BIT_OFFSET_VLAN_FORWARD_PORT_3;
			break;
		case IMP:
			vlan_port_forward_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_VLAN_BIT_OFFSET_VLAN_FORWARD_PORT_IMP;
			break;
	}

	return chip_func_write( &vlan_port_forward_reg_value, forward_mode );
}

/*description: write vlan table port tag*/
static BCM_REG_T bcm_vlan_table_port_tag_write(BCM_PORT_T port_number, BCM_REG_T forward_mode)
{
	BCM_REG_ADDR_T vlan_port_tag_reg_value;

	vlan_port_tag_reg_value.spi_page =   BCM_SPI_PAGE_VTBL;
	vlan_port_tag_reg_value.spi_offset = BCM_SPI_OFFSET_VTBL_ENTRY;

	vlan_port_tag_reg_value.bit_wide =   BCM_CHIP_BIT_1;

	switch ( port_number )
	{
		case PORT_0:
			vlan_port_tag_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_VLAN_BIT_OFFSET_VLAN_UNTAG_PORT_0;
			break;
		case PORT_1:
			vlan_port_tag_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_VLAN_BIT_OFFSET_VLAN_UNTAG_PORT_1;
			break;
		case PORT_2:
			vlan_port_tag_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_VLAN_BIT_OFFSET_VLAN_UNTAG_PORT_2;
			break;
		case PORT_3:
			vlan_port_tag_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_VLAN_BIT_OFFSET_VLAN_UNTAG_PORT_3;
			break;
		case IMP:
			vlan_port_tag_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_VLAN_BIT_OFFSET_VLAN_UNTAG_PORT_IMP;
			break;
	}

	return chip_func_write( &vlan_port_tag_reg_value, forward_mode );
}

/*description: read vlan table port forward status*/
static BCM_REG_T bcm_vlan_table_port_forward_read(BCM_PORT_T port_number )
{
	BCM_REG_ADDR_T vlan_port_forward_reg_value;

	vlan_port_forward_reg_value.spi_page =   BCM_SPI_PAGE_VTBL;
	vlan_port_forward_reg_value.spi_offset = BCM_SPI_OFFSET_VTBL_ENTRY;

	vlan_port_forward_reg_value.bit_wide =   BCM_CHIP_BIT_1;

	switch ( port_number )
	{
		case PORT_0:
			vlan_port_forward_reg_value.bit_offset = BCM_VLAN_BIT_OFFSET_VLAN_FORWARD_PORT_0;
			break;
		case PORT_1:
			vlan_port_forward_reg_value.bit_offset = BCM_VLAN_BIT_OFFSET_VLAN_FORWARD_PORT_1;
			break;
		case PORT_2:
			vlan_port_forward_reg_value.bit_offset = BCM_VLAN_BIT_OFFSET_VLAN_FORWARD_PORT_2;
			break;
		case PORT_3:
			vlan_port_forward_reg_value.bit_offset = BCM_VLAN_BIT_OFFSET_VLAN_FORWARD_PORT_3;
			break;
		case PORT_4:
			vlan_port_forward_reg_value.bit_offset = BCM_VLAN_BIT_OFFSET_VLAN_FORWARD_PORT_4;
			break;
		case PORT_5:
			vlan_port_forward_reg_value.bit_offset = BCM_VLAN_BIT_OFFSET_VLAN_FORWARD_PORT_5;
			break;
		case PORT_7:
			vlan_port_forward_reg_value.bit_offset = BCM_VLAN_BIT_OFFSET_VLAN_FORWARD_PORT_7;
			break;
		case IMP:
			vlan_port_forward_reg_value.bit_offset = BCM_VLAN_BIT_OFFSET_VLAN_FORWARD_PORT_IMP;
			break;
		default :
			perror("read vlan");
	}

	#ifdef VLAN_DEBUG
	switch(chip_func_read( &vlan_port_forward_reg_value ))
	{
		case PORT_UNFWD:
			printf("Port %d UNFWD\n",port_number);
			break;
		case PORT_FWD:
			printf("\033[1m\033[45;33mPort %d FWD\033[0m\n",port_number);
			break;
		default :
			perror("FWD");
	}
	#endif
	return chip_func_read( &vlan_port_forward_reg_value );
}

/*description: read vlan table port tag*/
static BCM_REG_T bcm_vlan_table_port_tag_read( BCM_PORT_T port_number )
{
	BCM_REG_ADDR_T vlan_port_tag_reg_value;

	vlan_port_tag_reg_value.spi_page =   BCM_SPI_PAGE_VTBL;
	vlan_port_tag_reg_value.spi_offset = BCM_SPI_OFFSET_VTBL_ENTRY;

	vlan_port_tag_reg_value.bit_wide =   BCM_CHIP_BIT_1;

	switch ( port_number )
	{
		case PORT_0:
			vlan_port_tag_reg_value.bit_offset = BCM_VLAN_BIT_OFFSET_VLAN_UNTAG_PORT_0;
			break;
		case PORT_1:
			vlan_port_tag_reg_value.bit_offset = BCM_VLAN_BIT_OFFSET_VLAN_UNTAG_PORT_1;
			break;
		case PORT_2:
			vlan_port_tag_reg_value.bit_offset = BCM_VLAN_BIT_OFFSET_VLAN_UNTAG_PORT_2;
			break;
		case PORT_3:
			vlan_port_tag_reg_value.bit_offset = BCM_VLAN_BIT_OFFSET_VLAN_UNTAG_PORT_3;
			break;
		case PORT_4:
			vlan_port_tag_reg_value.bit_offset = BCM_VLAN_BIT_OFFSET_VLAN_UNTAG_PORT_4;
			break;
		case PORT_5:
			vlan_port_tag_reg_value.bit_offset = BCM_VLAN_BIT_OFFSET_VLAN_UNTAG_PORT_5;
			break;
		case PORT_7:
			vlan_port_tag_reg_value.bit_offset = BCM_VLAN_BIT_OFFSET_VLAN_UNTAG_PORT_7;
			break;
		case IMP:
			vlan_port_tag_reg_value.bit_offset = BCM_VLAN_BIT_OFFSET_VLAN_UNTAG_PORT_IMP;
			break;
	}

	switch(chip_func_read( &vlan_port_tag_reg_value ))
	{
		case PORT_UNTAG:
			printf("\033[1m\033[45;33mPort %d unTag\033[0m\n",port_number);
			break;
		case PORT_TAG:
			printf("Port %d TAG\n",port_number);
			break;
		default :
			perror("TAG");
	}
	return chip_func_read( &vlan_port_tag_reg_value );
}

#ifdef DEBUG_VLAN
/*this function coding mode is different from other function which was code in front of vlan.c file.
 * because of VLAN ID bit wide has 11 bits,which has many 'mode'. */

/*description: write vlan tag information*/
BCM_FUNC_OP_T bcm_vlan_default_tag_info_write(BCM_PORT_T port_number, BCM_VLAN_TAG_INFO_T * tag_info)
{
	BCM_SPI_PAGE_VLAN_T info_tag_reg_value = 0;
	BCM_SPI_OFFSET_VLAN_T spi_offset = 0;

	if(port_number == IMP)
	{
		spi_offset = BCM_SPI_OFFSET_VLAN_TAG_INFO_IMP;
	}
	else
	{
		spi_offset = BCM_SPI_OFFSET_VLAN_TAG_INFO;
	}
	/*check*/
	if((tag_info->VLAN_ID > 0xFFE) || (tag_info->VLAN_ID == 0x0))
	{
		return 1;
	}

	if(tag_info->priority > 8)
	{
		return 1;
	}


	info_tag_reg_value += (tag_info->priority << BCM_VLAN_BIT_OFFSET_VLAN_TAG_INFO_PRI );
	info_tag_reg_value += (tag_info->VLAN_ID << BCM_VLAN_BIT_OFFSET_VLAN_TAG_INFO_VID );

	return chip_register_write(BCM_SPI_PAGE_VLAN, spi_offset, info_tag_reg_value);
}
#endif

BCM_FUNC_OP_T bcm_vlan_tag_info_pvid_write(BCM_PORT_T port_number, BCM_VLAN_PVID_T vid )
{
	BCM_REG_ADDR_T tag_info_pvid_reg_value;

	tag_info_pvid_reg_value.spi_page =   BCM_SPI_PAGE_VLAN;
	tag_info_pvid_reg_value.bit_offset = BCM_BIT_OFFSET_TAG_VID;

	/*fixed vlan id number range is 1 to 6,use bits are enough*/
	tag_info_pvid_reg_value.bit_wide =   BCM_CHIP_BIT_3;

	if(vid > VLAN_5)
	{
		perror("pvid");
	}

	switch ( port_number )
	{
		case PORT_0:
			tag_info_pvid_reg_value.spi_offset = ( BCM_PORT_REG_T )BCM_SPI_OFFSET_VID_PORT_0;
			break;
		case PORT_1:
			tag_info_pvid_reg_value.spi_offset = ( BCM_PORT_REG_T )BCM_SPI_OFFSET_VID_PORT_1;
			break;
		case PORT_2:
			tag_info_pvid_reg_value.spi_offset = ( BCM_PORT_REG_T )BCM_SPI_OFFSET_VID_PORT_2;
			break;
		case PORT_3:
			tag_info_pvid_reg_value.spi_offset = ( BCM_PORT_REG_T )BCM_SPI_OFFSET_VID_PORT_3;
			break;
		case PORT_4:
			tag_info_pvid_reg_value.spi_offset = ( BCM_PORT_REG_T )BCM_SPI_OFFSET_VID_PORT_4;
			break;
		case PORT_5:
			tag_info_pvid_reg_value.spi_offset = ( BCM_PORT_REG_T )BCM_SPI_OFFSET_VID_PORT_5;
			break;
		case IMP:
			tag_info_pvid_reg_value.spi_offset = ( BCM_PORT_REG_T )BCM_SPI_OFFSET_VID_PORT_IMP;
			break;
		default :
			perror("pvid");
	}
	return chip_func_write( &tag_info_pvid_reg_value,(BCM_REG_T)vid);
}

static BCM_FUNC_OP_T bcm_vlan_tag_info_pri_write(BCM_PORT_T port_number, BCM_VLAN_T vid )
{
	BCM_REG_ADDR_T tag_info_pri_reg_value;

	tag_info_pri_reg_value.spi_page =   BCM_SPI_PAGE_VLAN;
	tag_info_pri_reg_value.bit_offset = BCM_BIT_OFFSET_TAG_PRI;

	/*fixed vlan id number range is 1 to 6,use bits are enough*/
	tag_info_pri_reg_value.bit_wide =   BCM_CHIP_BIT_2;

	if(vid > VLAN_5)
	{
		perror("pvid");
	}

	switch ( port_number )
	{
		case PORT_0:
			tag_info_pri_reg_value.spi_offset = ( BCM_PORT_REG_T )BCM_SPI_OFFSET_VID_PORT_0;
			break;
		case PORT_1:
			tag_info_pri_reg_value.spi_offset = ( BCM_PORT_REG_T )BCM_SPI_OFFSET_VID_PORT_1;
			break;
		case PORT_2:
			tag_info_pri_reg_value.spi_offset = ( BCM_PORT_REG_T )BCM_SPI_OFFSET_VID_PORT_2;
			break;
		case PORT_3:
			tag_info_pri_reg_value.spi_offset = ( BCM_PORT_REG_T )BCM_SPI_OFFSET_VID_PORT_3;
			break;
		case PORT_4:
			tag_info_pri_reg_value.spi_offset = ( BCM_PORT_REG_T )BCM_SPI_OFFSET_VID_PORT_4;
			break;
		case PORT_5:
			tag_info_pri_reg_value.spi_offset = ( BCM_PORT_REG_T )BCM_SPI_OFFSET_VID_PORT_5;
			break;
		case IMP:
			tag_info_pri_reg_value.spi_offset = ( BCM_PORT_REG_T )BCM_SPI_OFFSET_VID_PORT_IMP;
			break;
		default :
			perror("pvid");
	}

	return chip_func_write( &tag_info_pri_reg_value,(BCM_REG_T)vid);
}

#ifdef DEBUG_VLAN
BCM_FUNC_OP_T bcm_vlan_tag_info_write(BCM_PORT_T port_number, BCM_VLAN_TAG_INFO_T * tag_info)
{
	BCM_REG_T info_tag_reg_value = 0;
	BCM_SPI_OFFSET_VLAN_T spi_offset = 0;

	switch ( port_number )
	{
		case PORT_0:
			spi_offset = ( BCM_PORT_REG_T )BCM_SPI_OFFSET_VID_PORT_0;
			break;
		case PORT_1:
			spi_offset = ( BCM_PORT_REG_T )BCM_SPI_OFFSET_VID_PORT_1;
			break;
		case PORT_2:
			spi_offset = ( BCM_PORT_REG_T )BCM_SPI_OFFSET_VID_PORT_2;
			break;
		case PORT_3:
			spi_offset = ( BCM_PORT_REG_T )BCM_SPI_OFFSET_VID_PORT_3;
			break;
		case PORT_4:
			spi_offset = ( BCM_PORT_REG_T )BCM_SPI_OFFSET_VID_PORT_4;
			break;
		case PORT_5:
			spi_offset = ( BCM_PORT_REG_T )BCM_SPI_OFFSET_VID_PORT_5;
			break;
		case IMP:
			spi_offset = ( BCM_PORT_REG_T )BCM_SPI_OFFSET_VID_PORT_IMP;
			break;
	}

	/*check*/
	if((tag_info->VLAN_ID > 0xFFE) || (tag_info->VLAN_ID == 0x0))
	{
		return 1;
	}

	if(tag_info->priority > 8)
	{
		return 1;
	}


	info_tag_reg_value += (tag_info->priority << BCM_VLAN_BIT_OFFSET_VLAN_TAG_INFO_PRI );
	info_tag_reg_value += (tag_info->VLAN_ID << BCM_VLAN_BIT_OFFSET_VLAN_TAG_INFO_VID );

	return chip_register_write(BCM_SPI_PAGE_VLAN, spi_offset, info_tag_reg_value);
}
#endif

static BCM_REG_T bcm_vlan_pvid_read(BCM_PORT_T port_number )
{
	bcm_func_t pvid;
	pvid.function_address.spi_page = BCM_SPI_PAGE_VLAN;
	pvid.function_address.bit_wide = BCM_CHIP_BIT_8;
	pvid.function_address.bit_offset = BCM_BIT_OFFSET_TAG_VID;

	pvid.function_read = chip_func_read;

	switch ( port_number )
	{
		case PORT_0:
			pvid.function_address.spi_offset = BCM_SPI_OFFSET_VID_PORT_0;
			break;
		case PORT_1:
			pvid.function_address.spi_offset = BCM_SPI_OFFSET_VID_PORT_1;
			break;
		case PORT_2:
			pvid.function_address.spi_offset = BCM_SPI_OFFSET_VID_PORT_2;
			break;
		case PORT_3:
			pvid.function_address.spi_offset = BCM_SPI_OFFSET_VID_PORT_3;
			break;
		case PORT_4:
			pvid.function_address.spi_offset = BCM_SPI_OFFSET_VID_PORT_4;
			break;
		case PORT_5:
			pvid.function_address.spi_offset = BCM_SPI_OFFSET_VID_PORT_5;
			break;
		case PORT_7:
			pvid.function_address.spi_offset = BCM_SPI_OFFSET_VID_PORT_7;
			break;
		case IMP:
			pvid.function_address.spi_offset = BCM_SPI_OFFSET_VID_PORT_IMP;
			break;
	}
	pvid.function_address.func_value = pvid.function_read(&pvid.function_address);

	return pvid.function_address.func_value;

}

/*enable or disable*/
/*description: write change 1Q vid*/
static BCM_FUNC_OP_T bcm_vlan_ctrl_0_change_1Q_VID_write( BCM_REG_T mode )
{
	BCM_REG_ADDR_T vlan_change_1Q_VID_reg_value;

	vlan_change_1Q_VID_reg_value.spi_page =   BCM_SPI_PAGE_VLAN;
	vlan_change_1Q_VID_reg_value.spi_offset = BCM_SPI_OFFSET_VLAN_CTRL_0;
	vlan_change_1Q_VID_reg_value.bit_offset = BCM_VLAN_BIT_OFFSET_VLAN_1Q_CHANGE;
	vlan_change_1Q_VID_reg_value.bit_wide =   BCM_CHIP_BIT_1;

	return chip_func_write( &vlan_change_1Q_VID_reg_value, mode );
}

/*description: write change 1Q vid out tag*/
static BCM_FUNC_OP_T bcm_vlan_ctrl_0_change_1Q_VID_OUT_write( BCM_REG_T mode )
{
	BCM_REG_ADDR_T vlan_change_1Q_VID_reg_value;

	vlan_change_1Q_VID_reg_value.spi_page =   BCM_SPI_PAGE_VLAN;
	vlan_change_1Q_VID_reg_value.spi_offset = BCM_SPI_OFFSET_VLAN_CTRL_0;
	vlan_change_1Q_VID_reg_value.bit_offset = BCM_VLAN_BIT_OFFSET_VLAN_1Q_CHANGE_OUT;
	vlan_change_1Q_VID_reg_value.bit_wide =   BCM_CHIP_BIT_1;

	return chip_func_write( &vlan_change_1Q_VID_reg_value, mode );
}

/*description: write change 1Q vid in tag*/
static BCM_FUNC_OP_T bcm_vlan_ctrl_0_change_1Q_VID_IN_write( BCM_REG_T mode )
{
	BCM_REG_ADDR_T vlan_change_1Q_VID_reg_value;

	vlan_change_1Q_VID_reg_value.spi_page =   BCM_SPI_PAGE_VLAN;
	vlan_change_1Q_VID_reg_value.spi_offset = BCM_SPI_OFFSET_VLAN_CTRL_0;
	vlan_change_1Q_VID_reg_value.bit_offset = BCM_VLAN_BIT_OFFSET_VLAN_1Q_CHANGE_IN;
	vlan_change_1Q_VID_reg_value.bit_wide =   BCM_CHIP_BIT_1;

	return chip_func_write( &vlan_change_1Q_VID_reg_value, mode );
}


/*description: write IPMC frame tag*/
static BCM_FUNC_OP_T bcm_vlan_ctrl_1_IPMC_frame_tag_write( BCM_REG_T mode )
{
	BCM_REG_ADDR_T vlan_ctrl_1_IPMC_frame_tag_reg_value;

	vlan_ctrl_1_IPMC_frame_tag_reg_value.spi_page =   BCM_SPI_PAGE_VLAN;
	vlan_ctrl_1_IPMC_frame_tag_reg_value.spi_offset = BCM_SPI_OFFSET_VLAN_CTRL_1;
	vlan_ctrl_1_IPMC_frame_tag_reg_value.bit_offset = BCM_VLAN_BIT_OFFSET_IPMC_TAG_CTRL;
	vlan_ctrl_1_IPMC_frame_tag_reg_value.bit_wide =   BCM_CHIP_BIT_1;

	return chip_func_write( &vlan_ctrl_1_IPMC_frame_tag_reg_value, mode );
}

/*description: write IPMC frame forward*/
static BCM_FUNC_OP_T bcm_vlan_ctrl_1_IPMC_frame_fwd_write( BCM_REG_T mode )
{
	BCM_REG_ADDR_T vlan_ctrl_1_IPMC_frame_fwd_reg_value;

	vlan_ctrl_1_IPMC_frame_fwd_reg_value.spi_page =   BCM_SPI_PAGE_VLAN;
	vlan_ctrl_1_IPMC_frame_fwd_reg_value.spi_offset = BCM_SPI_OFFSET_VLAN_CTRL_1;
	vlan_ctrl_1_IPMC_frame_fwd_reg_value.bit_offset = BCM_VLAN_BIT_OFFSET_IPMC_FWD_CTRL;
	vlan_ctrl_1_IPMC_frame_fwd_reg_value.bit_wide =   BCM_CHIP_BIT_1;

	return chip_func_write( &vlan_ctrl_1_IPMC_frame_fwd_reg_value, mode );
}

/*description: write multicast frame tag map*/
static BCM_FUNC_OP_T bcm_vlan_ctrl_1_mcast_frame_tag_write( BCM_REG_T mode )
{
	BCM_REG_ADDR_T vlan_ctrl_1_mcast_frame_tag_reg_value;

	vlan_ctrl_1_mcast_frame_tag_reg_value.spi_page =   BCM_SPI_PAGE_VLAN;
	vlan_ctrl_1_mcast_frame_tag_reg_value.spi_offset = BCM_SPI_OFFSET_VLAN_CTRL_1;
	vlan_ctrl_1_mcast_frame_tag_reg_value.bit_offset = BCM_VLAN_BIT_OFFSET_MULTICAST_TAG_CTRL;
	vlan_ctrl_1_mcast_frame_tag_reg_value.bit_wide =   BCM_CHIP_BIT_1;

	return chip_func_write( &vlan_ctrl_1_mcast_frame_tag_reg_value, mode );
}

/*description: write multicast frame check forward map*/
static BCM_FUNC_OP_T bcm_vlan_ctrl_1_mcast_frame_fwd_write( BCM_REG_T mode )
{
	BCM_REG_ADDR_T vlan_ctrl_1_mcast_frame_fwd_reg_value;

	vlan_ctrl_1_mcast_frame_fwd_reg_value.spi_page =   BCM_SPI_PAGE_VLAN;
	vlan_ctrl_1_mcast_frame_fwd_reg_value.spi_offset = BCM_SPI_OFFSET_VLAN_CTRL_1;
	vlan_ctrl_1_mcast_frame_fwd_reg_value.bit_offset = BCM_VLAN_BIT_OFFSET_MULTICAST_FWD_CTRL;
	vlan_ctrl_1_mcast_frame_fwd_reg_value.bit_wide =   BCM_CHIP_BIT_1;

	return chip_func_write( &vlan_ctrl_1_mcast_frame_fwd_reg_value, mode );
}

/*GG = GMRP,GVRP*/
/*description: write multicast frame tag map*/
static BCM_FUNC_OP_T bcm_vlan_ctrl_2_GG_frame_tag_write( BCM_REG_T mode )
{
	BCM_REG_ADDR_T vlan_ctrl_2_GG_frame_tag_reg_value;

	vlan_ctrl_2_GG_frame_tag_reg_value.spi_page =   BCM_SPI_PAGE_VLAN;
	vlan_ctrl_2_GG_frame_tag_reg_value.spi_offset = BCM_SPI_OFFSET_VLAN_CTRL_2;
	vlan_ctrl_2_GG_frame_tag_reg_value.bit_offset = BCM_VLAN_BIT_OFFSET_GG_TAG_CTRL;
	vlan_ctrl_2_GG_frame_tag_reg_value.bit_wide =   BCM_CHIP_BIT_1;

	return chip_func_write( &vlan_ctrl_2_GG_frame_tag_reg_value, mode );
}

/*description: write multicast frame check forward map*/
static BCM_FUNC_OP_T bcm_vlan_ctrl_2_GG_frame_check_fwdmap_write( BCM_REG_T mode )
{
	BCM_REG_ADDR_T vlan_ctrl_2_GG_frame_check_fwdmap_value;

	vlan_ctrl_2_GG_frame_check_fwdmap_value.spi_page =   BCM_SPI_PAGE_VLAN;
	vlan_ctrl_2_GG_frame_check_fwdmap_value.spi_offset = BCM_SPI_OFFSET_VLAN_CTRL_2;
	vlan_ctrl_2_GG_frame_check_fwdmap_value.bit_offset = BCM_VLAN_BIT_OFFSET_GG_CHK_FWD_MAP;
	vlan_ctrl_2_GG_frame_check_fwdmap_value.bit_wide =   BCM_CHIP_BIT_1;

	return chip_func_write( &vlan_ctrl_2_GG_frame_check_fwdmap_value, mode );
}

/*description: write mii frame check forward map*/
static BCM_FUNC_OP_T bcm_vlan_ctrl_2_mii_frame_check_fwdmap_write( BCM_REG_T mode )
{
	BCM_REG_ADDR_T vlan_ctrl_2_mii_frame_check_fwdmap_value;

	vlan_ctrl_2_mii_frame_check_fwdmap_value.spi_page =   BCM_SPI_PAGE_VLAN;
	vlan_ctrl_2_mii_frame_check_fwdmap_value.spi_offset = BCM_SPI_OFFSET_VLAN_CTRL_2;
	vlan_ctrl_2_mii_frame_check_fwdmap_value.bit_offset = BCM_VLAN_BIT_OFFSET_MII_CHK_FWD_MAP;
	vlan_ctrl_2_mii_frame_check_fwdmap_value.bit_wide =   BCM_CHIP_BIT_1;

	return chip_func_write( &vlan_ctrl_2_mii_frame_check_fwdmap_value, mode );
}

/*this is a abnormal write function style because of MPort reference switch in the register*/
/*description: write vlan multi port address control*/
static BCM_FUNC_OP_T bcm_vlan_multi_port_addr_ctrl_write(BCM_PORT_T port_number, BCM_REG_T mode)
{
	BCM_REG_ADDR_T multi_port_addr_reg_value;

	multi_port_addr_reg_value.spi_page =   BCM_SPI_PAGE_VLAN;
	multi_port_addr_reg_value.spi_offset = BCM_SPI_OFFSET_VLAN_MULTI_PORT_ADDR_CTL;

	multi_port_addr_reg_value.bit_wide = BCM_CHIP_BIT_1;

	if ((mode ==  MPORT_MAP_TAG) || (mode == MPORT_MAP_UNTAG))
	{	
        switch ( port_number )
		{
			case PORT_5:
				multi_port_addr_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_VLAN_MULITI_ADDR_PORT_5_TAG;
				break;
			case PORT_4:
				multi_port_addr_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_VLAN_MULITI_ADDR_PORT_4_TAG;
				break;
			case PORT_3:
				multi_port_addr_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_VLAN_MULITI_ADDR_PORT_3_TAG;
				break;
			case PORT_2:
				multi_port_addr_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_VLAN_MULITI_ADDR_PORT_2_TAG;
				break;
			case PORT_1:
				multi_port_addr_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_VLAN_MULITI_ADDR_PORT_1_TAG;
				break;
			case PORT_0:
				multi_port_addr_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_VLAN_MULITI_ADDR_PORT_0_TAG;
				break;
			default :
				perror("NO port");
		}

		if(MPORT_MAP_UNTAG)
		{
			mode = 1;
		}
		else
		{
			mode = 0;
		}
	}

	if((mode == MPORT_MAP_FWD) || (mode == MPORT_MAP_UNFWD))
	{
		switch ( port_number )

		{
			case PORT_5:
				multi_port_addr_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_VLAN_MULITI_ADDR_PORT_5_FWD;
				break;
			case PORT_4:
				multi_port_addr_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_VLAN_MULITI_ADDR_PORT_4_FWD;
				break;
			case PORT_3:
				multi_port_addr_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_VLAN_MULITI_ADDR_PORT_3_FWD;
				break;
			case PORT_2:
				multi_port_addr_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_VLAN_MULITI_ADDR_PORT_2_FWD;
				break;
			case PORT_1:
				multi_port_addr_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_VLAN_MULITI_ADDR_PORT_1_FWD;
				break;
			case PORT_0:
				multi_port_addr_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_VLAN_MULITI_ADDR_PORT_0_FWD;
				break;
			default :
				perror("NO port");
		}

		printf("fw:%d\n",multi_port_addr_reg_value.bit_offset);

		if(MPORT_MAP_FWD)
		{
			mode = 1;
		}
		else
		{
			mode = 0;
		}
	}

	return chip_func_write( &multi_port_addr_reg_value, mode );
}

/*this is a abnormal write function style because of MPort reference switch in the register*/
/*description: read vlan multi port address control*/
static BCM_FUNC_OP_T bcm_vlan_multi_port_addr_ctrl_read(BCM_PORT_T port_number)
{
	BCM_REG_ADDR_T multi_port_addr_reg_value;

	multi_port_addr_reg_value.spi_page =   BCM_SPI_PAGE_VLAN;
	multi_port_addr_reg_value.spi_offset = BCM_SPI_OFFSET_VLAN_MULTI_PORT_ADDR_CTL;

	multi_port_addr_reg_value.bit_wide = BCM_CHIP_BIT_1;

	switch ( port_number )
	{
		case PORT_5:
			multi_port_addr_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_VLAN_MULITI_ADDR_PORT_5_TAG;
			break;
		case PORT_4:
			multi_port_addr_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_VLAN_MULITI_ADDR_PORT_4_TAG;
			break;
		case PORT_3:
			multi_port_addr_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_VLAN_MULITI_ADDR_PORT_3_TAG;
			break;
		case PORT_2:
			multi_port_addr_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_VLAN_MULITI_ADDR_PORT_2_TAG;
			break;
		case PORT_1:
			multi_port_addr_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_VLAN_MULITI_ADDR_PORT_1_TAG;
			break;
		case PORT_0:
			multi_port_addr_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_VLAN_MULITI_ADDR_PORT_0_TAG;
			break;
		default :
			perror("NO port");
	}

	switch(chip_func_read( &multi_port_addr_reg_value ))
	{
		case 1:
			printf( "MPort %d check unTag map\n",port_number );
			break;

		case 0:
			printf( "MPort %d not check unTag map\n",port_number );
			break;
		default:
			perror("read");
	}

	switch ( port_number )
	{
		case PORT_5:
			multi_port_addr_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_VLAN_MULITI_ADDR_PORT_5_FWD;
			break;
		case PORT_4:
			multi_port_addr_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_VLAN_MULITI_ADDR_PORT_4_FWD;
			break;
		case PORT_3:
			multi_port_addr_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_VLAN_MULITI_ADDR_PORT_3_FWD;
			break;
		case PORT_2:
			multi_port_addr_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_VLAN_MULITI_ADDR_PORT_2_FWD;
			break;
		case PORT_1:
			multi_port_addr_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_VLAN_MULITI_ADDR_PORT_1_FWD;
			break;
		case PORT_0:
			multi_port_addr_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_VLAN_MULITI_ADDR_PORT_0_FWD;
			break;
		default :
			perror("NO port");
	}

	switch(chip_func_read( &multi_port_addr_reg_value ))
	{
		case 1:
			printf( "MPort %d map FWD\n",port_number );
			break;

		case 0:
			printf( "MPort %d map unFWD\n",port_number );
			break;
		default:
			perror("read");
	}

	return 0;
}

/*description: write vlan egress vid remark table access*/
static BCM_FUNC_OP_T bcm_vlan_egress_VID_remark_table_access_write( BCM_VLAN_EGRESS_VID_RMK_TBL_ACS_T *table )
{
	BCM_SPI_PAGE_VLAN_T reg_value = 0;

	reg_value += (table->table_address << BCM_BIT_OFFSET_VLAN_EGRESS_VID_RMK_TBL_ACS_ADR);
	reg_value += (table->egress_port << BCM_BIT_OFFSET_VLAN_EGRESS_VID_RMK_TBL_ACS_EPORT);
	reg_value += (table->operation << BCM_BIT_OFFSET_VLAN_EGRESS_VID_RMK_TBL_ACS_OPT);
	reg_value += (table->start_done << BCM_BIT_OFFSET_VLAN_EGRESS_VID_RMK_TBL_ACS_START);

	return chip_register_write(BCM_SPI_PAGE_VLAN,  BCM_SPI_OFFSET_VLAN_EGRESS_VID_RMK_TBL_ACS, reg_value);
}

/*description : vlan table entry untag map write*/
static BCM_FUNC_OP_T bcm_vlan_egress_VID_remark_table_data_write( BCM_VLAN_EGRESS_VID_RMK_TBL_DAT_T *data )
{
	BCM_SPI_PAGE_VLAN_T reg_value = 0;

	reg_value += (data->outer_tag_operation << BCM_BIT_OFFSET_VLAN_EGRESS_VID_RMK_TBL_DAT_OUT_TAG);
	reg_value += (data->outer_VID << BCM_BIT_OFFSET_VLAN_EGRESS_VID_RMK_TBL_DAT_OUT_VID);
	reg_value += (data->inner_tag_operation << BCM_BIT_OFFSET_VLAN_EGRESS_VID_RMK_TBL_DAT_IN_TAG);
	reg_value += (data->inner_VID << BCM_BIT_OFFSET_VLAN_EGRESS_VID_RMK_TBL_DAT_IN_VID);

	return chip_register_write(BCM_SPI_PAGE_VLAN,  BCM_SPI_OFFSET_VLAN_EGRESS_VID_RMK_TBL_DAT, reg_value);
}

/*description: write vlan mst table*/
static BCM_FUNC_OP_T bcm_vlan_mst_table_write(BCM_PORT_T port_number, BCM_REG_T mode)
{
	BCM_REG_ADDR_T mst_table_reg_value;

	mst_table_reg_value.spi_page =   BCM_MSTP_REG;
	mst_table_reg_value.spi_offset = BCM_SPI_OFFSET_MSPT;

	mst_table_reg_value.bit_wide = BCM_CHIP_BIT_3;

	switch ( port_number )
	{
		case PORT_0:
			mst_table_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_MST_TAB_PORT_0;
			break;
		case PORT_1:
			mst_table_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_MST_TAB_PORT_1;
			break;
		case PORT_2:
			mst_table_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_MST_TAB_PORT_2;
			break;
		case PORT_3:
			mst_table_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_MST_TAB_PORT_3;
			break;
		case PORT_4:
			mst_table_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_MST_TAB_PORT_4;
			break;
		case PORT_5:
			mst_table_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_MST_TAB_PORT_5;
			break;
		case PORT_7:
			mst_table_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_MST_TAB_PORT_7;
			break;
	}

	return chip_func_write( &mst_table_reg_value, mode );
}

/*description: read vlan mst table*/
static BCM_REG_T bcm_vlan_mst_table_read( BCM_PORT_T port_number )
{
	BCM_REG_ADDR_T mst_table_reg_value;

	mst_table_reg_value.spi_page =   BCM_MSTP_REG;
	mst_table_reg_value.spi_offset = BCM_SPI_OFFSET_MSPT;

	mst_table_reg_value.bit_wide = BCM_CHIP_BIT_3;

	switch ( port_number )
	{
		case PORT_0:
			mst_table_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_MST_TAB_PORT_0;
			break;
		case PORT_1:
			mst_table_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_MST_TAB_PORT_1;
			break;
		case PORT_2:
			mst_table_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_MST_TAB_PORT_2;
			break;
		case PORT_3:
			mst_table_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_MST_TAB_PORT_3;
			break;
		case PORT_4:
			mst_table_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_MST_TAB_PORT_4;
			break;
		case PORT_5:
			mst_table_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_MST_TAB_PORT_5;
			break;
		case PORT_7:
			mst_table_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_MST_TAB_PORT_7;
			break;
		default :
			perror("port");
	}

	switch(chip_func_read( &mst_table_reg_value ))
	{
		case NO_SPANNING_TREE:
			printf("Port %d no spanning tree\n",port_number);
			break;
		case SPANNING_TREE_DISABLE:
			printf("Port %d spanning tree disable\n",port_number);
			break;
		case SPANNING_TREE_BLOCKING:
			printf("Port %d spanning tree blocking\n",port_number);
			break;
		case SPANNING_TREE_LISTENING:
			printf("Port %d spanning tree listening\n",port_number);
			break;
		case SPANNING_TREE_LEARNING:
			printf("Port %d spanning tree learning\n",port_number);
			break;
		case SPANNING_TREE_FORWARD:
			printf("Port %d spanning tree forwarding\n",port_number);
			break;
		default :
			perror("spanning tree read\n");
	}
	return chip_func_read( &mst_table_reg_value );
}

#ifdef DEBUG_VLAN
/*description: multi write vlan table*/
void bcm_vlan_table_multi_write( void *vtbl_entry_data,uint32_t count)
{
	uint32_t retry = 0;
	uint32_t flag = 0;

	uint32_t i = 0;
	BCM_VLAN_VTBL_FUNC_T vtbl_entry;

	for(i=0;i < count; i++)
	{
		vtbl_entry = *((BCM_VLAN_VTBL_FUNC_T *)(vtbl_entry_data) + i);

		printf("index %d:1:0x%llx 2:0x%llx 3:0x%llx 4:0x%llx\n",i,vtbl_entry.mstp_index,vtbl_entry.fwd_map,vtbl_entry.untag_map,vtbl_entry.fwd_mode);
		//bcm_vlan_table_entry_write(vtbl_entry);

		bcm_vlan_table_entry_mspt_index_write(vtbl_entry.mstp_index);     //default
		bcm_vlan_table_entry_fwd_map_write(vtbl_entry.fwd_map);
		bcm_vlan_table_entry_tag_map_write(vtbl_entry.untag_map);
		bcm_vlan_table_entry_fwd_mode_write(vtbl_entry.fwd_mode);

		printf("========write index %d reg========\n",i+1);

		bcm_vlan_table_index_write(i + 1);

		/*vlan table control*/
		//bcm_vlan_table_ctrl_write(&vtbl_ctrl);

		bcm_vlan_table_ctrl_rw_write(VLAN_TBL_WRITE);

		bcm_vlan_table_ctrl_init();

		/* wait for complete */
		for( retry = 0; retry < 99; retry++)
		{
			switch(bcm_vlan_table_ctrl_init_finish())
			{
				case VLAN_TBL_INIT_FINISH:
					retry = 99;
					flag = 1;
					printf("Write finish\n");
					break;
				case VLAN_TBL_INIT_START:
					if(retry == 99)
					{
						perror("VTBL timeout\n");
						break;
					}
					printf("writing\n");
					break;
				default :
					perror("VTBL");
			}
		}
	}
}
#endif

/*description: single write vlan table*/
void bcm_vlan_table_single_write( BCM_VLAN_VTBL_FUNC_T *vtbl_entry,BCM_VLAN_T vtbl_index)
{
	uint32_t retry = 0;

	//bcm_vlan_table_entry_write(vtbl_entry);

	bcm_vlan_table_entry_mspt_index_write(vtbl_entry->mstp_index);     //default
	bcm_vlan_table_entry_fwd_map_write(vtbl_entry->fwd_map);
	bcm_vlan_table_entry_tag_map_write(vtbl_entry->untag_map);
	bcm_vlan_table_entry_fwd_mode_write(vtbl_entry->fwd_mode);


	bcm_vlan_table_index_write(vtbl_index);

	/*vlan table control*/
	//bcm_vlan_table_ctrl_write(&vtbl_ctrl);

	bcm_vlan_table_ctrl_rw_write(VLAN_TBL_WRITE);

	bcm_vlan_table_ctrl_init();

	/* wait for complete */
	for( retry = 0; retry < 99; retry++)
	{
		switch(bcm_vlan_table_ctrl_init_finish())
		{
			case VLAN_TBL_INIT_FINISH:
				retry = 99;
				#ifdef VLAN_DEBUG
				printf("Write finish\n");
				#endif
				break;
			case VLAN_TBL_INIT_START:
				if(retry == 99)
				{
					perror("VTBL timeout\n");
					break;
				}
				printf("writing\n");
				break;
			default :
				perror("VTBL");
		}
	}
}

/*description: read vlan table*/
static void bcm_vlan_table_read( BCM_VLAN_VTBL_ADDR_T vtbl_index_max )
{
	uint32_t retry = 0;
	uint32_t flag = 0;
	uint32_t i = 1;


	for(i = 0; i <= vtbl_index_max; i++)
	{
		printf("========read index %d reg========\n",i);

		bcm_vlan_table_index_write( i );

		/*vlan table control*/
		bcm_vlan_table_ctrl_rw_write(VLAN_TBL_READ);

		bcm_vlan_table_ctrl_init();

		/* wait for complete */
		for(retry = 0; retry < 99; retry++)
		{
			switch(bcm_vlan_table_ctrl_init_finish())
			{
				case VLAN_TBL_INIT_FINISH:
					retry = 99;
					flag = 1;
					#ifdef VLAN_DEBUG
					printf("read finish\n");
					#endif
					break;
				case VLAN_TBL_INIT_START:
					if(retry == 99)
					{
						perror("read VTBL timeout");
						break;
					}
					printf("reading\n");
					break;
				default :
					perror("VTBL");
			}
			if( flag == 1 )
			{
				break;
			}
		}

		bcm_vlan_table_port_forward_read(PORT_0);
		bcm_vlan_table_port_forward_read(PORT_1);
		bcm_vlan_table_port_forward_read(PORT_2);
		bcm_vlan_table_port_forward_read(PORT_3);
		bcm_vlan_table_port_forward_read(PORT_4);
		bcm_vlan_table_port_forward_read(PORT_5);
		bcm_vlan_table_port_forward_read(IMP);

		bcm_vlan_table_port_tag_read(PORT_0);
		bcm_vlan_table_port_tag_read(PORT_1);
		bcm_vlan_table_port_tag_read(PORT_2);
		bcm_vlan_table_port_tag_read(PORT_3);
		bcm_vlan_table_port_tag_read(PORT_4);
		bcm_vlan_table_port_tag_read(PORT_5);
		bcm_vlan_table_port_tag_read(IMP);
	}
}

/*description: clear vlan table*/
static void bcm_vlan_table_clear( BCM_VLAN_VTBL_ADDR_T vtbl_index )
{
	uint32_t retry = 0;
	uint32_t flag = 0;

	printf("========clear index %d reg========\n",vtbl_index);

	bcm_vlan_table_index_write( vtbl_index );

	/*vlan table control*/
	bcm_vlan_table_ctrl_rw_write(VLAN_TBL_CLEAR);

	bcm_vlan_table_ctrl_init();

	/* wait for complete */
	for(retry = 0; retry < 99; retry++)
	{
		switch(bcm_vlan_table_ctrl_init_finish())
		{
			case VLAN_TBL_INIT_FINISH:
				retry = 99;
				flag = 1;
				printf("clear finish\n");

				break;
			case VLAN_TBL_INIT_START:
				if(retry == 99)
				{
					perror("clear VTBL timeout");
					break;
				}
				printf("clearing\n");
				break;
			default :
				perror("VTBL");
		}
		if( flag == 1 )
		{
			break;
		}
	}

	bcm_vlan_table_port_forward_read(PORT_0);
	bcm_vlan_table_port_forward_read(PORT_1);
	bcm_vlan_table_port_forward_read(PORT_2);
	bcm_vlan_table_port_forward_read(PORT_3);
	bcm_vlan_table_port_forward_read(IMP);

	bcm_vlan_table_port_tag_read(PORT_0);
	bcm_vlan_table_port_tag_read(PORT_1);
	bcm_vlan_table_port_tag_read(PORT_2);
	bcm_vlan_table_port_tag_read(PORT_3);
	bcm_vlan_table_port_tag_read(IMP);

}

/*description: write port join all vlan*/
static BCM_REG_T bcm_vlan_join_all_vlan_en(BCM_PORT_T port_number, BCM_REG_T mode )
{
	BCM_REG_ADDR_T join_vlan_reg_value;

	join_vlan_reg_value.spi_page =   BCM_SPI_PAGE_VLAN;
	join_vlan_reg_value.spi_offset = BCM_SPI_OFFSET_JOIN_ALL_VLAN;
	join_vlan_reg_value.bit_wide =   BCM_CHIP_BIT_1;


	switch ( port_number )
		{
			case PORT_0:
				join_vlan_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_JOIN_ALL_VLAN_PORT_0;
				break;
			case PORT_1:
				join_vlan_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_JOIN_ALL_VLAN_PORT_1;
				break;
			case PORT_2:
				join_vlan_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_JOIN_ALL_VLAN_PORT_2;
				break;
			case PORT_3:
				join_vlan_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_JOIN_ALL_VLAN_PORT_3;
				break;
			case PORT_4:
				join_vlan_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_JOIN_ALL_VLAN_PORT_4;
				break;
			case PORT_5:
				join_vlan_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_JOIN_ALL_VLAN_PORT_5;
				break;
			case PORT_6:
				join_vlan_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_JOIN_ALL_VLAN_PORT_6;
				break;
			case PORT_7:
				join_vlan_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_JOIN_ALL_VLAN_PORT_7;
				break;
			case IMP:
				join_vlan_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_JOIN_ALL_VLAN_PORT_8;
				break;
		}

	return chip_func_write( &join_vlan_reg_value, mode );
}

/*description: read port join all vlan*/
static BCM_REG_T bcm_vlan_join_all_vlan_en_read(BCM_PORT_T port_number)
{
	BCM_REG_T mode;

	BCM_REG_ADDR_T join_vlan_reg_value;

	join_vlan_reg_value.spi_page =   BCM_SPI_PAGE_VLAN;
	join_vlan_reg_value.spi_offset = BCM_SPI_OFFSET_JOIN_ALL_VLAN;

	join_vlan_reg_value.bit_wide =   BCM_CHIP_BIT_1;

	switch ( port_number )
	{
		case PORT_0:
			join_vlan_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_JOIN_ALL_VLAN_PORT_0;
			break;
		case PORT_1:
			join_vlan_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_JOIN_ALL_VLAN_PORT_1;
			break;
		case PORT_2:
			join_vlan_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_JOIN_ALL_VLAN_PORT_2;
			break;
		case PORT_3:
			join_vlan_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_JOIN_ALL_VLAN_PORT_3;
			break;
		case PORT_4:
			join_vlan_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_JOIN_ALL_VLAN_PORT_4;
			break;
		case PORT_5:
			join_vlan_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_JOIN_ALL_VLAN_PORT_5;
			break;
		case PORT_6:
			join_vlan_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_JOIN_ALL_VLAN_PORT_6;
			break;
		case PORT_7:
			join_vlan_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_JOIN_ALL_VLAN_PORT_7;
			break;
		case IMP:
			join_vlan_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_JOIN_ALL_VLAN_PORT_8;
			break;
	}

	mode = chip_func_read( &join_vlan_reg_value );

	switch ( mode )
	{
		case VLAN_ALL_JOIN_ENABLE:
			printf("Port %d VLAN join Enable\n",port_number);
			break;
		case VLAN_ALL_JOIN_DISABLE:
			printf("Port %d VLAN join Disable\n",port_number);
			break;
		default:
			printf("error\n");
	}

	return mode;
}

/*description: wrtie double tag function*/
static BCM_REG_T bcm_vlan_double_tag_write(BCM_REG_T enable)
{
    
	BCM_REG_ADDR_T ctrl_4_reg_value;

	ctrl_4_reg_value.spi_page =   BCM_SPI_PAGE_VLAN;
	ctrl_4_reg_value.spi_offset = BCM_SPI_OFFSET_VLAN_CTRL_4;
	ctrl_4_reg_value.bit_offset = BCM_VLAN_BIT_OFFSET_DOUBLE_TAG;
	ctrl_4_reg_value.bit_wide =   BCM_CHIP_BIT_2;

	return chip_func_write( &ctrl_4_reg_value, enable );
}

/*description: read double tag function*/
static BCM_REG_T bcm_vlan_double_tag_read()
{
	BCM_REG_T mode;
	BCM_REG_ADDR_T ctrl_4_reg_value;

	ctrl_4_reg_value.spi_page =   BCM_SPI_PAGE_VLAN;
	ctrl_4_reg_value.spi_offset = BCM_SPI_OFFSET_VLAN_CTRL_4;
	ctrl_4_reg_value.bit_offset = BCM_VLAN_BIT_OFFSET_DOUBLE_TAG;
	ctrl_4_reg_value.bit_wide =   BCM_CHIP_BIT_2;

	mode = chip_func_read( &ctrl_4_reg_value );

	switch ( mode )
	{
		case DT_MODE_ENABLE:
			printf("DT mode Enable\n");
			break;
		case IDT_MODE_ENABLE:
			printf("IDT mode Enable\n");
			break;
		case DOUBLE_TAG_DISABLE:
			printf("Double tag Disable\n");
			break;
		default:
			printf("Double\n");
	}

	return mode;
}

/*description: write multi port type*/
static BCM_REG_T bcm_vlan_ISP_port_write(BCM_PORT_T port_number, BCM_REG_T mode )
{
	BCM_REG_ADDR_T isp_port_reg_value;

	isp_port_reg_value.spi_page =   BCM_SPI_PAGE_VLAN;
	isp_port_reg_value.spi_offset = BCM_SPI_OFFSET_VLAN_ISP_PORTMAP;
	isp_port_reg_value.bit_wide =   BCM_CHIP_BIT_1;


	switch ( port_number )
    {
        case PORT_0:
            isp_port_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_JOIN_ALL_VLAN_PORT_0;
            break;
        case PORT_1:
            isp_port_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_JOIN_ALL_VLAN_PORT_1;
            break;
        case PORT_2:
            isp_port_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_JOIN_ALL_VLAN_PORT_2;
            break;
        case PORT_3:
            isp_port_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_JOIN_ALL_VLAN_PORT_3;
            break;
        case PORT_4:
            isp_port_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_JOIN_ALL_VLAN_PORT_4;
            break;
        case PORT_5:
            isp_port_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_JOIN_ALL_VLAN_PORT_5;
            break;
        case PORT_6:
            isp_port_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_JOIN_ALL_VLAN_PORT_6;
            break;
        case PORT_7:
            isp_port_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_JOIN_ALL_VLAN_PORT_7;
            break;
        case IMP:
            isp_port_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_JOIN_ALL_VLAN_PORT_8;
            break;
    }

	return chip_func_write( &isp_port_reg_value, mode );
}

/*description: read mulit port is ISP or customer port*/
static BCM_REG_T bcm_vlan_ISP_port_read(BCM_PORT_T port_number )
{
	BCM_REG_T mode;
	BCM_REG_ADDR_T isp_port_reg_value;

	isp_port_reg_value.spi_page =   BCM_SPI_PAGE_VLAN;
	isp_port_reg_value.spi_offset = BCM_SPI_OFFSET_VLAN_ISP_PORTMAP;
	isp_port_reg_value.bit_wide =   BCM_CHIP_BIT_1;

	switch ( port_number )
	{
		case PORT_0:
			isp_port_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_JOIN_ALL_VLAN_PORT_0;
			break;
		case PORT_1:
			isp_port_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_JOIN_ALL_VLAN_PORT_1;
			break;
		case PORT_2:
			isp_port_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_JOIN_ALL_VLAN_PORT_2;
			break;
		case PORT_3:
			isp_port_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_JOIN_ALL_VLAN_PORT_3;
			break;
		case PORT_4:
			isp_port_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_JOIN_ALL_VLAN_PORT_4;
			break;
		case PORT_5:
			isp_port_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_JOIN_ALL_VLAN_PORT_5;
			break;
		case PORT_6:
			isp_port_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_JOIN_ALL_VLAN_PORT_6;
			break;
		case PORT_7:
			isp_port_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_JOIN_ALL_VLAN_PORT_7;
			break;
		case IMP:
			isp_port_reg_value.bit_offset = ( BCM_PORT_REG_T )BCM_BIT_OFFSET_JOIN_ALL_VLAN_PORT_8;
			break;
	}

	mode = chip_func_read( &isp_port_reg_value );

	switch ( mode )
	{
		case ISP_PORT:
			printf("Port %d is ISP port\n",port_number);
			break;
		case CUSTOMER_PORT:
			printf("Port %d VLAN customer port \n",port_number);
			break;
		default:
			printf("error\n");
	}

	return mode;
}

