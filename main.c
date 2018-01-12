/************************************************************************************
 * (c) Copyright 2017 CRRC.  All rights reserved.
 *
 *  bcm 53134 main
 *
 *  Created on: 2017-08-16
 *
 *  Author: buggy
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "port/port.h"
#include "chip/chip.h"
#include "vlan/vlan.h"

#define RUN_TIME 30

extern BCM_CHIP_WORK_STATUS_T * g_p_bcm_chip_work_status;
extern BCM_CHIP_WORK_STATUS_T g_bcm_chip_work_default_status;

/*global variables*/
extern BCM_PORT_WORK_STATUS_T * g_p_bcm_port_0_work_status;
extern BCM_PORT_WORK_STATUS_T * g_p_bcm_port_1_work_status;
extern BCM_PORT_WORK_STATUS_T * g_p_bcm_port_2_work_status;
extern BCM_PORT_WORK_STATUS_T * g_p_bcm_port_3_work_status;
extern BCM_PORT_WORK_STATUS_T * g_p_bcm_port_imp_work_status;


/****************************************************************************************************/
/*description: chip initialization
*/
void bcm_init();

/****************************************************************************************************/
/*description: chip status read
*/
void bcm_status_read();

/****************************************************************************************************/
/*description: chip status check
*/
void bcm_status_check();

void bcm_vlan();


/****************************************************************************************************/
/*description: chip function close
*/
void bcm_close();

int main(int argc, char * argv[])
{
	/*set ip*/
	system("ifconfig eth0 192.168.1.92");

	/*up network card*/
	system("ifconfig eth0 up");

	/*route multicast*/
	system("route add -net 224.0.0.0 netmask 224.0.0.0 dev eth0");

	/* initialization */
	bcm_init();
	bcm_vlan();

//	while( 1 )
	{
		/*status read*/
		bcm_status_read();

		/*status check*/
		//bcm_status_check();

		//sleep(RUN_TIME);
	}

	bcm_close();
}


void bcm_init()
{
	/* chip work status */
	g_p_bcm_chip_work_status = ( BCM_CHIP_WORK_STATUS_T *)malloc(sizeof( BCM_CHIP_WORK_STATUS_T ));
	if(g_p_bcm_chip_work_status == NULL)
	{
		exit(1);
	}

	/*port work status*/
	g_p_bcm_port_0_work_status = ( BCM_PORT_WORK_STATUS_T *)malloc(sizeof( BCM_PORT_WORK_STATUS_T ));
	if(g_p_bcm_chip_work_status == NULL)
	{
		exit(1);
	}

	g_p_bcm_port_1_work_status = ( BCM_PORT_WORK_STATUS_T *)malloc(sizeof( BCM_PORT_WORK_STATUS_T ));
	if(g_p_bcm_chip_work_status == NULL)
	{
		exit(1);
	}

	g_p_bcm_port_2_work_status = ( BCM_PORT_WORK_STATUS_T *)malloc(sizeof( BCM_PORT_WORK_STATUS_T ));
	if(g_p_bcm_chip_work_status == NULL)
	{
		exit(1);
	}

	g_p_bcm_port_3_work_status = ( BCM_PORT_WORK_STATUS_T *)malloc(sizeof( BCM_PORT_WORK_STATUS_T ));
	if(g_p_bcm_chip_work_status == NULL)
	{
		exit(1);
	}

	g_p_bcm_port_imp_work_status = ( BCM_PORT_WORK_STATUS_T *)malloc(sizeof( BCM_PORT_WORK_STATUS_T ));
	if(g_p_bcm_chip_work_status == NULL)
	{
		exit(1);
	}

	/* initialization default status */
	bcm_chip_status_init_write( &g_bcm_chip_work_default_status );

	bcm_port_status_init_write();
}

void bcm_status_read()
{
	/*chip work status*/
	g_p_bcm_chip_work_status = ( BCM_CHIP_WORK_STATUS_T *)bcm_chip_status_read( g_p_bcm_chip_work_status );

	/*port 0 status*/
	g_p_bcm_port_0_work_status = ( BCM_PORT_WORK_STATUS_T *)bcm_port_status_read(PORT_0, g_p_bcm_port_0_work_status);

	/*port 1 status*/
	g_p_bcm_port_1_work_status = ( BCM_PORT_WORK_STATUS_T *)bcm_port_status_read(PORT_1, g_p_bcm_port_1_work_status);

	/*port 2 status*/
	g_p_bcm_port_2_work_status = ( BCM_PORT_WORK_STATUS_T *)bcm_port_status_read(PORT_2, g_p_bcm_port_2_work_status);

	/*port 3 status*/
	g_p_bcm_port_3_work_status = ( BCM_PORT_WORK_STATUS_T *)bcm_port_status_read(PORT_3, g_p_bcm_port_3_work_status);

	/*port 4 status*/
	g_p_bcm_port_imp_work_status = ( BCM_PORT_WORK_STATUS_T *)bcm_port_status_read(IMP, g_p_bcm_port_imp_work_status);

}

void bcm_status_check()
{
	uint32_t i = 0;

	/*chip status*/
	BCM_CHIP_WORK_STATUS_T * chip_work_status;
	BCM_CHIP_ERR_STATUS_T * chip_error_status;
	BCM_CHIP_ERR_STATUS_T * chip_write_error_status;

	/*port status*/
	BCM_PORT_WORK_STATUS_T * port_work_status;
	BCM_PORT_ERR_STATUS_T * port_error_status;
	BCM_PORT_ERR_STATUS_T * port_write_error_status;


	/*malloc chip memory*/
	chip_error_status = ( BCM_CHIP_ERR_STATUS_T *)malloc(sizeof(BCM_CHIP_ERR_STATUS_T));
	chip_write_error_status = ( BCM_CHIP_ERR_STATUS_T *)malloc(sizeof(BCM_CHIP_ERR_STATUS_T));
	chip_work_status  = ( BCM_CHIP_WORK_STATUS_T * )malloc(sizeof( BCM_CHIP_WORK_STATUS_T ));

	/*malloc port memory*/
	port_error_status = ( BCM_PORT_ERR_STATUS_T *)malloc(sizeof(BCM_PORT_ERR_STATUS_T));
	port_write_error_status = ( BCM_PORT_ERR_STATUS_T *)malloc(sizeof(BCM_PORT_ERR_STATUS_T));
	port_work_status  = ( BCM_PORT_WORK_STATUS_T * )malloc(sizeof( BCM_PORT_WORK_STATUS_T ));


	/*check chip status*/
	chip_error_status = ( BCM_CHIP_ERR_STATUS_T *)bcm_chip_status_check( chip_error_status );

	/* fact write*/
	if( chip_error_status->error_cnt > 0 )
	{
		/*try to reset chip function*/
		bcm_chip_status_init_write( &g_bcm_chip_work_default_status );

		/*read chip status*/
		chip_work_status = ( BCM_CHIP_WORK_STATUS_T * )bcm_chip_status_read( chip_work_status );

		/*check again*/
		chip_write_error_status = ( BCM_CHIP_ERR_STATUS_T *)bcm_chip_status_check( chip_write_error_status );

		/*write error*/
		if( chip_error_status->error_cnt > 0 )
		{
			bcm_chip_err_status_store( chip_write_error_status );
		}
	}

	/* traverse port  */
	for( i = 0; i < PORT_COUNT; i++ )
	{
		port_error_status = ( BCM_PORT_ERR_STATUS_T *)bcm_port_status_check( i, port_error_status );
		if( port_error_status->error_cnt > 0 )
		{
			/*imp port*/
			if( i == IMP )
			{
				bcm_port_status_init_write();

				port_work_status = ( BCM_PORT_WORK_STATUS_T *)bcm_port_status_read(IMP,	port_work_status);

				port_error_status = ( BCM_PORT_ERR_STATUS_T *)bcm_port_status_check( IMP, port_error_status );
				if( port_error_status->error_cnt > 0 )
				{
					bcm_port_err_status_store( i, port_write_error_status );
				}

			}
			/*normal port*/
			else
			{
				bcm_port_err_status_store( i, port_write_error_status );
			}
		}
	}

	/*free memory*/
	free( chip_error_status );
	free( chip_work_status );
	free( chip_write_error_status );

	free( port_error_status );
	free( port_work_status );
	free( port_write_error_status );
}

void bcm_close()
{
	free(g_p_bcm_chip_work_status);
	free(g_p_bcm_port_0_work_status);
	free(g_p_bcm_port_1_work_status);
	free(g_p_bcm_port_2_work_status);
	free(g_p_bcm_port_3_work_status);

	free(g_p_bcm_port_imp_work_status);
}

