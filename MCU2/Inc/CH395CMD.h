/********************************** (C) COPYRIGHT *******************************
* File Name          : CH395CMD.H
* Author             : WCH
* Version            : V1.1
* Date               : 2014/8/1
* Description        : CH395芯片命令接口文件,常用子程序外部调用
*******************************************************************************/

#ifndef __CH395CMD_H__
#define __CH395CMD_H__

void CH395CMDReset(void);                                            /* 复位 */

void CH395CMDSleep(void);                                            /* 睡眠 */

uint8_t CH395CMDGetVer(void);                                          /* 获取芯片及固件版本号 */

uint8_t CH395CMDCheckExist(uint8_t testdata);                            /* 测试命令 */

void CH395CMDSetPHY(uint8_t phystat);                                  /* 设置PHY状态  */

uint8_t CH395CMDGetPHYStatus(void);                                    /* 获取PHY状态 */

uint8_t CH395CMDGetGlobIntStatus(void);                                /* 获取CH395全局中断状态 */

uint8_t CH395CMDInitCH395(void);                                       /* 初始化CH395 */

void CH395CMDSetUartBaudRate(uint32_t baudrate);                       /* 设置波特率 */

uint8_t CH395GetCmdStatus(void);                                       /* 获取命令执行状态 */

void CH395CMDSetIPAddr(uint8_t *ipaddr);                               /* 设置CH395的IP地址 */

void CH395CMDSetGWIPAddr(uint8_t *gwipaddr);                           /* 设置CH395的网关IP地址 */

void CH395CMDSetMASKAddr(uint8_t *maskaddr);                           /* 设置子网掩码 */

void CH395CMDSetMACAddr(uint8_t *amcaddr);                             /* 设置CH395的MAC地址 */

void CH395CMDGetMACAddr(uint8_t *amcaddr);                             /* 获取MAC地址 */

void CH395CMDSetMACFilt(uint8_t filtype,uint32_t table0,uint32_t table1);  /* 设置CH395的MAC过滤 */

void CH395CMDGetUnreachIPPT(uint8_t *list);                            /* 获取不可达地址以及端口 */

void CH395CMDGetRemoteIPP(uint8_t sockindex,uint8_t *list);              /* 获取远端IP和端口，一般在TCP Server下使用 */

void CH395SetSocketDesIP(uint8_t sockindex,uint8_t *ipaddr);             /* 设置scoket n的目的IP地址 */

void CH395SetSocketProtType(uint8_t sockindex,uint8_t prottype);         /* 设置socket n的协议类型 */

void CH395SetSocketDesPort(uint8_t sockindex,uint16_t desprot);          /* 设置socket n的目的端口 */

void CH395SetSocketSourPort(uint8_t sockindex,uint16_t surprot);         /* 设置socket n的源端口 */

void CH395SetSocketIPRAWProto(uint8_t sockindex,uint8_t prototype);      /* 在IPRAW模式下，设置socket n的IP包协议字段 */

void CH395SetRecvThresLen(uint8_t sockindex,uint16_t len);               /* 设置socket n的接收中断阀值 */



void CH395SendData(uint8_t sockindex, const uint8_t *databuf,uint16_t len);       /* 向socket n的发送缓冲区写数据 */

void CH395StartSendingData(uint8_t sockindex, uint16_t total_len);

void CH395ContinueSendingData(const uint8_t *databuf, uint16_t len);

void CH395Complete();

uint16_t CH395GetRecvLength(uint8_t sockindex);                          /* 获取socket n的接收长度 */

void CH395ClearRecvBuf(uint8_t sockindex);                             /* 清除socket n的接收缓冲区 */

void CH395GetRecvData(uint8_t sockindex,uint16_t len,uint8_t *pbuf);       /* 获取接收数据 */

void CH395CMDSetRetryCount(uint8_t count);                             /* 设置最大重试次数 */

void CH395CMDSetRetryPeriod(uint16_t period);                          /* 设置最大重试周期 单位 毫秒*/

void CH395CMDGetSocketStatus(uint8_t sockindex,uint8_t *status) ;        /* 获取socket n的状态 */

uint8_t  CH395OpenSocket(uint8_t sockindex);                             /* 打开socket n*/

uint8_t  CH395CloseSocket(uint8_t sockindex);                             /* 关闭socket n*/

uint8_t CH395TCPConnect(uint8_t sockindex);                               /* TCP连接 */

uint8_t CH395TCPListen(uint8_t sockindex);                                /* TCP监听 */

uint8_t CH395TCPDisconnect(uint8_t sockindex);                            /* TCP断开连接 */

uint8_t CH395GetSocketInt(uint8_t sockindex);                            /* 获取socket n的中断状态 */
                                                                     
uint8_t CH395CRCRet6Bit(uint8_t *mac_addr);                              /* 多播地址CRC32，用于HASH过滤 */
                                                                     /*Function Count = 36 */
void CH395GetIPInf(uint8_t *addr);

uint8_t  CH395DHCPEnable(uint8_t flag);

uint8_t CH395GetDHCPStatus(void);

void CH395WriteGPIOAddr(uint8_t regadd,uint8_t regval);

uint8_t CH395ReadGPIOAddr(uint8_t regadd);

uint8_t CH395EEPROMErase(void);

uint8_t CH395EEPROMWrite(uint16_t eepaddr,uint8_t *buf,uint8_t len);

void CH395EEPROMRead(uint16_t eepaddr,uint8_t *buf,uint8_t len);

void CH395SetTCPMss(uint16_t tcpmss);

void CH395SetSocketRecvBuf(uint8_t sockindex,uint8_t startblk,uint8_t blknum);

void CH395SetSocketSendBuf(uint8_t sockindex,uint8_t startblk,uint8_t blknum);

void CH395UDPSendTo(uint8_t *buf,uint32_t len,uint8_t *ip,uint16_t port,uint8_t sockindex);

void CH395SetStartPara(uint32_t mdata);

uint16_t CH395CMDGetGlobIntStatus_ALL(void);

void CH395SetKeepLive(uint8_t sockindex,uint8_t cmd);                                 /* keepalive 使能 */

void CH395KeepLiveCNT(uint8_t cnt);                                                /* keepalive 参数设置 */

void CH395KeepLiveIDLE(uint32_t idle);                                             /* keepalive 参数设置 */

void CH395KeepLiveINTVL(uint32_t intvl);                                           /* keepalive 参数设置 */

void CH395SetTTLNum(uint8_t sockindex,uint8_t TTLnum);                               /* TTL参数设置 */
 
void CH395EnablePing(uint8_t enable);                                              /* PING 使能 */


#endif
/**************************** endfile *************************************/
