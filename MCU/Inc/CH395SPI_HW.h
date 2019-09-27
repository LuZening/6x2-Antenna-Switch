#include "main.h"

#include "CH395.H"

#include "PIN.h"
#include "Delay.h"
/* SPI硬件相关宏定义 */

extern PIN_typedef CH395_SCS; // Chip selected pin

#define CH395_INT_WIRE              INT1                            /* CH395的INT#引脚 */

#define xCH395CmdStart( )         HAL_GPIO_WritePin(CH395_SCS.group, CH395_SCS.pin, GPIO_PIN_RESET)              /* 命令开始 */

#define xEndCH395Cmd()            HAL_GPIO_WritePin(CH395_SCS.group, CH395_SCS.pin, GPIO_PIN_SET)              /* 命令结束*/

#define hspi hspi1


// TODO: precise delay
void CH395_PORT_INIT(void);
uint8_t SPI395Exchange( uint8_t d );
void xWriteCH395Cmd(uint8_t cmd);
void  xWriteCH395Data(uint8_t mdata);
uint8_t   xReadCH395Data( void );
void xWriteDMACH395Data(uint8_t *psrc, int size);
void xWriteCH395Bulk(uint8_t *psrc, int size);
void xReadDMACH395Data(uint8_t *pdest, int size);
