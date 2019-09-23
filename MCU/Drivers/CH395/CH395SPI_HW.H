#include "main.h"

#include "CH395.H"
#include "PIN.h"
#include "main.h"
#include "Delay.h"
/* SPI硬件相关宏定义 */

extern PIN_typedef CH395_SCS; // Chip selected pin

#define CH395_INT_WIRE              INT1                            /* CH395的INT#引脚 */

#define xCH395CmdStart( )         HAL_GPIO_WritePin(CH395_SCS.group, CH395_SCS.pin, GPIO_PIN_RESET)              /* 命令开始 */

#define xEndCH395Cmd()            HAL_GPIO_WritePin(CH395_SCS.group, CH395_SCS.pin, GPIO_PIN_SET)              /* 命令结束*/

#define hspi hspi1


// TODO: precise delay
void CH395_PORT_INIT(void);
UINT8 SPI395Exchange( UINT8 d );
void xWriteCH395Cmd(UINT8 cmd);
void  xWriteCH395Data(UINT8 mdata);
UINT8   xReadCH395Data( void );
void xWriteDMACH395Data(UINT8 *psrc, int size);
void xReadDMACH395Data(UINT8 *pdest, int size);
