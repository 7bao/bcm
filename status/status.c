#include "status.h"

#include "../port/port.h"
#include "../chip/chip.h"

void bcm_init()
{
	/* chip work status */
	g_p_bcm_chip_work_status = ( BCM_CHIP_WORK_STATUS_T *)malloc(sizeof( BCM_CHIP_WORK_STATUS_T ));

	/* port work status */
	g_p_bcm_port_0_work_status = ( BCM_PORT_WORK_STATUS_T *)malloc(sizeof( BCM_PORT_WORK_STATUS_T ));
	g_p_bcm_port_1_work_status = ( BCM_PORT_WORK_STATUS_T *)malloc(sizeof( BCM_PORT_WORK_STATUS_T ));
	g_p_bcm_port_2_work_status = ( BCM_PORT_WORK_STATUS_T *)malloc(sizeof( BCM_PORT_WORK_STATUS_T ));
	g_p_bcm_port_3_work_status = ( BCM_PORT_WORK_STATUS_T *)malloc(sizeof( BCM_PORT_WORK_STATUS_T ));
	g_p_bcm_port_4_work_status = ( BCM_PORT_WORK_STATUS_T *)malloc(sizeof( BCM_PORT_WORK_STATUS_T ));
	g_p_bcm_port_5_work_status = ( BCM_PORT_WORK_STATUS_T *)malloc(sizeof( BCM_PORT_WORK_STATUS_T ));
	g_p_bcm_port_imp_work_status = ( BCM_PORT_WORK_STATUS_T *)malloc(sizeof( BCM_PORT_WORK_STATUS_T ));

	/* initialization status */
	bcm_chip_status_init();
	bcm_port_status_init();
}

void bcm_status_read()
{
	g_p_bcm_chip_work_status = ( BCM_CHIP_WORK_STATUS_T *)bcm_chip_status_read( g_p_bcm_chip_work_status );
	g_p_bcm_port_0_work_status = ( BCM_PORT_WORK_STATUS_T *)bcm_port_status_read(PORT_0, g_p_bcm_port_0_work_status);
	g_p_bcm_port_1_work_status = ( BCM_PORT_WORK_STATUS_T *)bcm_port_status_read(PORT_1, g_p_bcm_port_1_work_status);
	g_p_bcm_port_2_work_status = ( BCM_PORT_WORK_STATUS_T *)bcm_port_status_read(PORT_2, g_p_bcm_port_2_work_status);
	g_p_bcm_port_3_work_status = ( BCM_PORT_WORK_STATUS_T *)bcm_port_status_read(PORT_3, g_p_bcm_port_3_work_status);
	g_p_bcm_port_4_work_status = ( BCM_PORT_WORK_STATUS_T *)bcm_port_status_read(PORT_4, g_p_bcm_port_4_work_status);
	g_p_bcm_port_imp_work_status = ( BCM_PORT_WORK_STATUS_T *)bcm_port_status_read(IMP, g_p_bcm_port_imp_work_status);

}

