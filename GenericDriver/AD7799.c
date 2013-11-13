/***************************************************************************//**
 *   @file   AD7799.c
 *   @brief  Implementation of AD7799 Driver.
 *   @author Bancisor MIhai
********************************************************************************
 * Copyright 2012(c) Analog Devices, Inc.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  - Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  - Neither the name of Analog Devices, Inc. nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *  - The use of this software may or may not infringe the patent rights
 *    of one or more patent holders.  This license does not release you
 *    from the requirement that you obtain separate licenses from these
 *    patent holders to use this software.
 *  - Use of the software either in source or binary form, must be run
 *    on or directly connected to an Analog Devices Inc. component.
 *
 * THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL ANALOG DEVICES BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, INTELLECTUAL PROPERTY RIGHTS, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
********************************************************************************
 *   SVN Revision: 577
*******************************************************************************/

/******************************************************************************/
/* Include Files                                                              */
/******************************************************************************/
#include "AD7799.h"				// AD7799 definitions.
#include "Communication.h"		// Communication definitions.

/***************************************************************************//**
 * @brief Initializes the AD7799 and checks if the device is present.
 *
 * @param None.
 *
 * @return status - Result of the initialization procedure.
 *                  Example: 1 - if initialization was successful (ID is 0x0B).
 *                           0 - if initialization was unsuccessful.
*******************************************************************************/
unsigned char AD7799_Init(void)
{ 
	unsigned char status = 0x1;
	if((AD7799_GetRegisterValue(AD7799_REG_ID, 1) & 0x0F) != AD7799_ID)
	{
		status = 0x0;
	}
	
	return(status);
}

/***************************************************************************//**
 * @brief Sends 32 consecutive 1's on SPI in order to reset the part.
 *
 * @param None.
 *
 * @return  None.    
*******************************************************************************/
void AD7799_Reset(void)
{
	unsigned char dataToSend[5] = {0x03, 0xff, 0xff, 0xff, 0xff};
	AD7799_CS_LOW;	    
	SPI_Write(dataToSend,4);
	AD7799_CS_HIGH;	
}
/***************************************************************************//**
 * @brief Reads the value of the selected register
 *
 * @param regAddress - The address of the register to read.
 * @param size - The size of the register to read.
 *
 * @return data - The value of the selected register register.
*******************************************************************************/
unsigned long AD7799_GetRegisterValue(unsigned char regAddress, unsigned char size)
{
	unsigned char data[5] = {0x03, 0x00, 0x00, 0x00, 0x00};
	unsigned long receivedData = 0x00;	
	data[1] = AD7799_COMM_READ |  AD7799_COMM_ADDR(regAddress);
	AD7799_CS_LOW;  
	SPI_Write(data,1);
	SPI_Read(data,size);
	AD7799_CS_HIGH;
	if(size == 1)
	{
		receivedData += (data[0] << 0);
	}
	if(size == 2)
	{
		receivedData += (data[0] << 8);
		receivedData += (data[1] << 0);
	}
	if(size == 3)
	{
		receivedData += (data[0] << 16);
		receivedData += (data[1] << 8);
		receivedData += (data[2] << 0);
	}
    return receivedData;
}
/***************************************************************************//**
 * @brief Writes the value to the register
 *
 * @param -  regAddress - The address of the register to write to.
 * @param -  regValue - The value to write to the register.
 * @param -  size - The size of the register to write.
 *
 * @return  None.    
*******************************************************************************/
void AD7799_SetRegisterValue(unsigned char regAddress,
                             unsigned long regValue, 
                             unsigned char size)
{
	unsigned char data[5] = {0x03, 0x00, 0x00, 0x00, 0x00};	
	data[1] = AD7799_COMM_WRITE |  AD7799_COMM_ADDR(regAddress);
    if(size == 1)
    {
        data[2] = (unsigned char)regValue;
    }
    if(size == 2)
    {
		data[3] = (unsigned char)((regValue & 0x0000FF) >> 0);
        data[2] = (unsigned char)((regValue & 0x00FF00) >> 8);
    }
    if(size == 3)
    {
		data[4] = (unsigned char)((regValue & 0x0000FF) >> 0);
		data[3] = (unsigned char)((regValue & 0x00FF00) >> 8);
        data[2] = (unsigned char)((regValue & 0xFF0000) >> 16);
    }
	AD7799_CS_LOW;	    
	SPI_Write(data,(1 + size));
	AD7799_CS_HIGH;

}
/***************************************************************************//**
 * @brief Reads /RDY bit of status reg.
 *
 * @param None.
 *
 * @return rdy	- 0 if RDY is 1.
 *              - 1 if RDY is 0.
*******************************************************************************/
unsigned char AD7799_Ready(void)
{
    unsigned char rdy = 0;
    rdy = (AD7799_GetRegisterValue( AD7799_REG_STAT,1) & 0x80);   
	
	return(!rdy);
}

/***************************************************************************//**
 * @brief Sets the operating mode of AD7799.
 *
 * @param mode - Mode of operation.
 *
 * @return  None.    
*******************************************************************************/
void AD7799_SetMode(unsigned long mode)
{
    unsigned long command;
    command = AD7799_GetRegisterValue(AD7799_REG_MODE,2);
    command &= ~AD7799_MODE_SEL(0xFF);
    command |= AD7799_MODE_SEL(mode);
    AD7799_SetRegisterValue(
            AD7799_REG_MODE,
            command,
            2
    );
}
/***************************************************************************//**
 * @brief Selects the channel of AD7799.
 *
 * @param  channel - ADC channel selection.
 *
 * @return  None.    
*******************************************************************************/
void AD7799_SetChannel(unsigned long channel)
{
    unsigned long command;
    command = AD7799_GetRegisterValue(AD7799_REG_CONF,2);
    command &= ~AD7799_CONF_CHAN(0xFF);
    command |= AD7799_CONF_CHAN(channel);
    AD7799_SetRegisterValue(
            AD7799_REG_CONF,
            command,
            2
    );
}

/***************************************************************************//**
 * @brief  Sets the gain of the In-Amp.
 *
 * @param  gain - Gain.
 *
 * @return  None.    
*******************************************************************************/
void AD7799_SetGain(unsigned long gain)
{
    unsigned long command;
    command = AD7799_GetRegisterValue(AD7799_REG_CONF,2);
    command &= ~AD7799_CONF_GAIN(0xFF);
    command |= AD7799_CONF_GAIN(gain);
    AD7799_SetRegisterValue(
            AD7799_REG_CONF,
            command,
            2
    );
}
/***************************************************************************//**
 * @brief Enables or disables the reference detect function.
 *
 * @param state - State of the reference detect function.
 *               Example: 0	- Reference detect disabled.
 *                        1	- Reference detect enabled.
 *
 * @return None.    
*******************************************************************************/
void AD7799_SetReference(unsigned char state)
{
    unsigned long command = 0;
    command = AD7799_GetRegisterValue(AD7799_REG_CONF,2);
    command &= ~AD7799_CONF_REFDET(1);
    command |= AD7799_CONF_REFDET(state);
    AD7799_SetRegisterValue(AD7799_REG_CONF,
							command,
							2);
}