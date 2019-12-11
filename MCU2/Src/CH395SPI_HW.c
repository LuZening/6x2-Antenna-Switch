/********************************** (C) COPYRIGHT *******************************
* File Name          : CH395SPI_HW.C
* Author             : WCH
* Version            : V1.1
* Date               : 2014/8/1
* Description        : CH395芯片 硬件SPI串行连接的硬件抽象层 V1.0
*                     
*******************************************************************************/
#include <main.h>
#include <CH395SPI_HW.h>
extern SPI_HandleTypeDef hspi1;
//extern DMA_HandleTypeDef hdma_spi1_tx;
CH395_TypeDef ch395 = {.socket_connected = 0, .RX_received = 0, .TX_available = 0xff};
CH395_TypeDef *pch395 = &ch395;
PIN_typedef CH395_SCS = {.group = SPI1_CS_GPIO_Port, .pin=SPI1_CS_Pin};
/******************************************************************************
* Function Name  : CH395_PORT_INIT
* Description    : 硬件初始化部分
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CH395_PORT_INIT(void)
{
    xEndCH395Cmd();
   /* 如果是硬件SPI接口,那么可使用mode3(CPOL=1&CPHA=1)或mode0(CPOL=0&CPHA=0),
    CH395在时钟上升沿采样输入,下降沿输出,数据位是高位在前 */
    /* 对于双向I/O引脚模拟SPI接口,那么必须在此设置SPI_SCS,SPI_SCK,SPI_SDI为输出方向,
       SPI_SDO为输入方向 */
    /* 设置SPI模式3, DORD=0(MSB first), CPOL=1, CPHA=1, CH395也支持SPI模式0 */
}

/*******************************************************************************
* Function Name  : Spi395Exchange
* Description    : 硬件SPI输出且输入8个位数据
* Input          : d---将要送入到CH395的数据
* Output         : None
* Return         : None
*******************************************************************************/
//uint8_t SPI395Exchange( uint8_t d )
//{
//    /* 为了提高速度,可以将该子程序做成宏以减少子程序调用层次 */
////	xEndCH395Cmd();
//    HAL_SPI_Transmit(&hspi, &d, 1, HAL_MAX_DELAY);
//    Delay_us(2);
//    HAL_SPI_Receive(&hspi, ch395.buffer, 1, HAL_MAX_DELAY);
//	return(ch395.buffer[0]);                                              /* 先查询SPI状态寄存器以等待SPI字节传输完成,然后从SPI数据寄存器读出数据 */
//}

/******************************************************************************
* Function Name  : xWriteCH395Cmd
* Description    : 向CH395写命令
* Input          : cmd 8位的命令码
* Output         : None
* Return         : None
*******************************************************************************/
void xWriteCH395Cmd(uint8_t cmd)
{                                                                    
    xEndCH395Cmd();                                                  /* 防止CS原来为低，先将CD置高 */
    xCH395CmdStart( );                                               /* 命令开始，CS拉低 */
    HAL_SPI_Transmit(&hspi, &cmd, 1, 100);
    Delay_us(2);
}

/******************************************************************************
* Function Name  : xWriteCH395Data
* Description    : 向CH395写数据
* Input          : mdata 8位数据
* Output         : None
* Return         : None
*******************************************************************************/
void  xWriteCH395Data(uint8_t mdata)
{   
//    SPI395Exchange(mdata);                                           /* SPI发送数据 */
    HAL_SPI_Transmit(&hspi, &mdata, 1, 100);
}

//void xWriteCH395Bulk(uint8_t *psrc, int size)
//{
//
//	HAL_SPI_Transmit(&hspi, psrc, size, HAL_MAX_DELAY);
//}

//void xWriteDMACH395Data(uint8_t *psrc, int size)
//{
//	//ch395.tx_busy = true;
//	HAL_SPI_Transmit_DMA(&hspi, psrc, size);
//	//TODO: clear tx_busy in IT
//}
/*******************************************************************************
* Function Name  : xReadCH395Data
* Description    : 从CH395读数据
* Input          : None
* Output         : None
* Return         : 8位数据
*******************************************************************************/
uint8_t   xReadCH395Data( void )
{
    uint8_t i;
    HAL_SPI_Receive(&hspi, &i, 1, 100);
    return i;
}

//void xReadCH395Bulk(uint8_t* pdest, int size)
//{
//	HAL_StatusTypeDef status;
//	int retry = 0;
//	do
//	{
//		status = HAL_SPI_Receive(&hspi, pdest, size, HAL_MAX_DELAY);
//		retry++;
//	}while(status != HAL_OK && retry < 3);
//}
//void xReadDMACH395Data(uint8_t* pdest, int size)
//{
//	HAL_SPI_Receive_DMA(&hspi, pdest, size);
//}
/**************************** endfile *************************************/
