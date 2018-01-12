/************************************************************************************
 *  bcm 53134 mdio operation functions 
 *
 *  Created on: 2017-08-16
 *
 *  Author: buggy
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/mii.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/sockios.h>
#include <linux/types.h>
#include <netinet/in.h>

#include "phy.h"



/****************************************************************************************************/
/*description: read chip single function status
/*param 0: BCM_CHIP_FUNC_POST_T  single function register position
*/
BCM_REG_T chip_func_read( BCM_REG_ADDR_T *chip_reg_value );

/****************************************************************************************************/
/*description: read chip single function status
/*param 0:BCM_CHIP_FUNC_POST_T  single function register position
/*param 1:BCM_CHIP_FUNC_STATUS_T single function
*/
BCM_FUNC_OP_T chip_func_write( BCM_REG_ADDR_T * reg_address, BCM_REG_T func_mode );

/****************************************************************************************************/
/*description: bcm53134 chip register read
/*param 0: page number
/*param 1: spi register address
*/
BCM_REG_T chip_register_read(BCM_SPI_PAGE_T ,BCM_SPI_OFFSET_T );


/****************************************************************************************************/
/*description: bcm53134 chip register write
/*param 0: page number
/*param 1: spi register address
/*param 1: write value
*/
BCM_REG_T chip_register_write(BCM_SPI_PAGE_T,BCM_SPI_OFFSET_T,BCM_REG_T);


/****************************************************************************************************/
/*description: mdio register write
/*param 0: page number
/*param 1: spi register address
*/
static int8_t mdio_register_write(int16_t reg_adr,int16_t value);


/****************************************************************************************************/
/*description: mdio No 17 register write
/*param 0: page number
/*param 1: spi register address
*/
static int8_t mdio_register_r17_write(int16_t reg_adr,int16_t value);


/****************************************************************************************************/
/*description: mdio register read
/*param 0: page number
/*param 1: spi register address
*/
static uint16_t mdio_register_read(int16_t mii_register);

/*usage*/
void usage();

#ifdef DEBUG_PHY
int main(int argc, char *argv[])
{
	/*page num*/
	uint16_t page_num_value;

	/*spi register address*/
	uint16_t spi_reg_addr_value;

	/*spi register address value write*/
	uint16_t spi_reg_write_value;

	/*reture value*/
	int64_t write_value;
	int64_t read_value;

	if (argc < 6)
	{
		/* usage */
		usage();
	}

	/*operate device*/
	memcpy(eth,argv[1],sizeof(eth));

	/*check work mode*/
	if (memcmp(argv[2],"w",strlen(argv[2]))!= 0 && memcmp(argv[2],"r",strlen(argv[2])) != 0)
	{
		usage();
	}
	
	/*check argc number*/
	if (memcmp(argv[2],"w",strlen(argv[2])) == 0 && argc != 7)
	{
		usage();
	}

	/*check argc number*/
	if (memcmp(argv[2],"r",strlen(argv[2])) == 0 && argc != 6)
	{
		usage();
	}

	/**/
	g_pseudo_phy_addr = (uint16_t)strtoul(argv[3], NULL, 0);

	page_num_value = (uint16_t)strtoul(argv[4], NULL, 0);

	spi_reg_addr_value = (uint16_t)strtoul(argv[5], NULL, 0);

	if(argc == 7 )
	{
		spi_reg_write_value = (uint16_t)strtoul(argv[6], NULL, 0);
		printf("%d\n",spi_reg_write_value );
	}

	if(memcmp(argv[2],"r",strlen(argv[2])) == 0)
	{
		read_value = chip_register_read(page_num_value,spi_reg_addr_value);
		printf("read value:%lld\n",read_value);
		printf("read value:%llx\n",read_value);
	}
	
	if(memcmp(argv[2],"w",strlen(argv[2])) == 0)
	{
		chip_register_write(page_num_value,spi_reg_addr_value,spi_reg_write_value);	
	}

}
#endif

/*pseudo pht address*/
static uint16_t g_pseudo_phy_addr = PHY_ADR;

/*device name*/
static char eth[10] = "eth0";



BCM_REG_T chip_func_read(BCM_REG_ADDR_T *chip_reg_address)
{
	BCM_REG_T ret_value;
	BCM_REG_T reg_value;
	BCM_REG_T mask;

	reg_value = chip_register_read(chip_reg_address->spi_page, chip_reg_address->spi_offset);
	if ( reg_value == -1 )
	{
		/* code */
		printf( "read error\n" );
		return -1;
	}

	switch(chip_reg_address->bit_wide)
	{
		case BCM_CHIP_BIT_1:
			mask = MASK_SINGLE;
			break;
		case BCM_CHIP_BIT_2:
			mask = MASK_DOUBLE;
			break;
		case BCM_CHIP_BIT_3:
			mask = MASK_TRIPLE;
			break;
		case BCM_CHIP_BIT_8:
			mask = MASK_OCTUPLE;
			break;
		default:
			printf("mask error\n");
	}

	reg_value >>= chip_reg_address->bit_offset;
	ret_value = reg_value & mask;

	return ret_value;
}

BCM_FUNC_OP_T chip_func_write( BCM_REG_ADDR_T *reg_address, BCM_REG_T func_mode )
{
	/*64bits*/
	BCM_REG_T reg_value = 0;

	BCM_REG_T current_reg_value = 0;

	BCM_REG_T mask = 0;

	reg_value = chip_register_read(reg_address->spi_page, reg_address->spi_offset);
	if ( reg_value == FUNC_WRITE_ERR )
	{
		/* code */
		printf( "read error\n" );
		return FUNC_WRITE_ERR;
	}
	current_reg_value = reg_value;

	switch(reg_address->bit_wide)
	{
		case BCM_CHIP_BIT_1:
			mask = MASK_SINGLE;
			break;
		case BCM_CHIP_BIT_2:
			mask = MASK_DOUBLE;
			break;
		case BCM_CHIP_BIT_3:
			mask = MASK_TRIPLE;
			break;
		case BCM_CHIP_BIT_8:
			mask = MASK_OCTUPLE;
			break;
		default:
			return FUNC_WRITE_ERR;
	}

	/*clean the bits which i wanna write.save other bits*/
	reg_value &= (~( mask << reg_address->bit_offset ));

	/*write the bit*/
	reg_value += (func_mode << reg_address->bit_offset);

	/* write judgment */
	if( reg_value != current_reg_value )
	{
		chip_register_write(reg_address->spi_page, reg_address->spi_offset, reg_value);
	}

	return FUNC_WRITE_CORRECT;
}


BCM_REG_T chip_register_read(BCM_SPI_PAGE_T spi_page,BCM_SPI_OFFSET_T spi_offset)
{
	int32_t i;
	
	uint16_t result;

	uint64_t ret_value = 0;
	uint64_t back = 0;

	mdio_register_read(MII_REG_16);
	mdio_register_read(MII_REG_17);

	mdio_register_write(MII_REG_16,spi_page);
	mdio_register_write(MII_REG_17,spi_offset);
	
	for (i = 0; i < CHK_CNT; i++)
	{
		result = mdio_register_read(MII_REG_17);
		result = result & 0x3;
		if(result == 0)
		{
			#ifdef DEBUG
				printf("chip reg read successed.\n");
			#endif
			break;
		}
		if ( i == (CHK_CNT - 1) )
		{
			printf("chip reg read failed.\n");
			return 1;
		}
	}
	
	ret_value = mdio_register_read(MII_REG_24);
	
	/*operate register 25*/
	back = mdio_register_read(MII_REG_25);
	back <<= 16;
	ret_value += back;
	
	/*operate register 26*/
	back = 0;
	back = mdio_register_read(MII_REG_26);
	back <<= 32;
	ret_value += back;
	
	/*operate register 27*/
	back = 0;
	back = mdio_register_read(MII_REG_27);
	
	/*combine result*/
	ret_value += back;
	
	return ret_value;
}

BCM_REG_T chip_register_write(BCM_SPI_PAGE_T spi_page,BCM_SPI_OFFSET_T spi_offset,BCM_REG_T write_value)
{
	int32_t i;

	uint64_t back = 0;
	int16_t result;

	mdio_register_write(MII_REG_16,spi_page);

	mdio_register_write(MII_REG_24,(uint16_t)write_value);
	mdio_register_write(MII_REG_25,(uint16_t)(write_value>>16));
	mdio_register_write(MII_REG_26,(uint16_t)(write_value>>32));
	mdio_register_write(MII_REG_27,(uint16_t)(write_value>>48));

	mdio_register_r17_write(MII_REG_17,spi_offset);

	for (i = 0; i < CHK_CNT; i++)
	{
		result = mdio_register_read(MII_REG_17);
		result = result & 0x3;
		
		if(result == 0)
		{
			#ifdef DEBUG
				printf("chip reg write successed.\n");
				printf("CHK:%d\n", i);
			#endif
			break;
		}
		

		if ( i == (CHK_CNT - 1) )
		{
			#ifdef DEBUG
				printf("chip reg write failed.\n");
			#endif
			return -1;
		}
	}
	back = chip_register_read(spi_page,spi_offset);
	#ifdef DEBUG
		printf("write value:%llx\n",back);
	#endif
	return back;
}

int8_t mdio_register_write(int16_t reg_adr,int16_t value)
{
	int32_t sockfd;

	struct mii_ioctl_data *mii = NULL;
    struct ifreq ifr;
    int ret;

    if (reg_adr == MII_REG_16)
    {	    
    	value <<= 8;
    	value += 1;
	}
	else if (reg_adr == MII_REG_17)
	{
		/* code */
		value <<= 8;
		value += 2;
	}

    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, eth, IFNAMSIZ - 1);

    sockfd = socket(PF_LOCAL, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
    	/* code */
    	perror("write sock error\n");
    	return PHY_WRITE_ERR;
    }

    //get phy address in smi bus
    ret = ioctl(sockfd, SIOCGMIIPHY, &ifr);
    if (ret < 0)
    {
    	/* code */
    	perror("wirte ioctl error\n");
    	return PHY_WRITE_ERR;
    }

    mii = (struct mii_ioctl_data*)&ifr.ifr_data;

    mii->phy_id = g_pseudo_phy_addr;
    mii->reg_num = reg_adr;
    mii->val_in = value;

    ret = ioctl(sockfd, SIOCSMIIREG, &ifr);
    if (ret < 0)
    {
    	/* code */
    	perror("mii read error\n");
    	return PHY_WRITE_ERR;
    }

    #ifdef DEBUG
    	printf("write phy addr: 0x%x  reg: %d  value : 0x%x\n\n", mii->phy_id, mii->reg_num, mii->val_in);
    #endif
	close(sockfd);
	return PHY_WRITE_SUCCESS;
}

int8_t mdio_register_r17_write(int16_t reg_adr,int16_t value)
{
	int32_t sockfd;

	struct mii_ioctl_data *mii = NULL;
    struct ifreq ifr;
    int ret;

    if (reg_adr == MII_REG_16)
    {	    
    	value <<= 8;
    	value += 1;
	}
	else if (reg_adr == MII_REG_17)
	{
		/* code */
		value <<= 8;
		value += 1;
	}

    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, eth, IFNAMSIZ - 1);

    sockfd = socket(PF_LOCAL, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
    	/* code */
    	perror("write sock error\n");
    	return PHY_WRITE_ERR;
    }

    //get phy address in smi bus
    ret = ioctl(sockfd, SIOCGMIIPHY, &ifr);
    if (ret < 0)
    {
    	/* code */
    	perror("wirte ioctl error\n");
    	return PHY_WRITE_ERR;
    }

    mii = (struct mii_ioctl_data*)&ifr.ifr_data;

    mii->phy_id = g_pseudo_phy_addr;
    mii->reg_num = reg_adr;
    mii->val_in = value;

    ret = ioctl(sockfd, SIOCSMIIREG, &ifr);
    if (ret < 0)
    {
    	/* code */
    	perror("mii read error\n");
    	return PHY_WRITE_ERR;
    }

    #ifdef DEBUG
    	printf("write phy addr: 0x%x  reg: %d  value : 0x%x\n\n", mii->phy_id, mii->reg_num, mii->val_in);
    #endif
	
	close(sockfd);

	return PHY_WRITE_SUCCESS;

}


uint16_t mdio_register_read(int16_t mdio_register)
{
	int32_t sockfd;

	struct mii_ioctl_data *mii = NULL;
    struct ifreq ifr;
    int ret;

    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, eth, IFNAMSIZ - 1);

    sockfd = socket(PF_LOCAL, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
    	perror("read sock error\n");
    	exit(1);
    }

    //get phy address in smi bus
    ret = ioctl(sockfd, SIOCGMIIPHY, &ifr);
    if (ret < 0)
    {
    	perror("read ioctl error\n");
    	exit(1);
    }

    mii = (struct mii_ioctl_data*)&ifr.ifr_data;

    mii->reg_num = mdio_register;
    mii->phy_id = g_pseudo_phy_addr ;
        
    ret = ioctl(sockfd, SIOCGMIIREG, &ifr);
    if (ret < 0)
    {
    	perror("mii write error\n");
    	exit(1);
    }
	#ifdef DEBUG
    	printf("read phy addr: 0x%x  reg: %d   value : 0x%x\n\n", mii->phy_id, mii->reg_num, mii->val_out);
	#endif
    close(sockfd);
    return mii->val_out;
}

void usage()
{
    printf("For example:\n");            
    printf("./phy eth0 <w(write)/r(read)> <PHY address> <page_num> <spi_reg_addr> <write_value(if write)>\n");                  
    exit(0);
}

