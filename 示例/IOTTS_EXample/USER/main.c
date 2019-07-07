/**********************************************************
* @ File name -> main.c
* @ Version   -> V1.0.1
* @ Date      -> 02-06-2017
* @ Brief     -> 
* @ Revise    -> 
***********************************************************/
#include "stm32_config.h"
#include "stdio.h"
#include "stdlib.h"
#include "bsp_led.h"
#include "delay.h"
#include "bsp_spi_flash.h"
#include "bsp_usart1.h" 

spiffs SPIFlashFS;
static u8_t FS_Work_Buf[SPIFLASH_CFG_LOG_PAGE_SZ*2];
static u8_t FS_FDS[32*4];
static u8_t FS_Cache_Buf[(SPIFLASH_CFG_LOG_PAGE_SZ+32)*4];
 
char WriteBuf[]={"L05 03 0 1 1 2 7 00010 02000 005 1\nL05 03 1 1 1 0 23 0 1\nP03 7 00030 20000 15 1\nM03 03 1\nL02 01 01\nP01 22 1 1\nM03 03 1\nL02 01 01\nP03 7 00010 20000 5 1\nP01 22 0 1\n"};

char ReadBuf[2000];	

 /***********************************************
 *   加载SPI FLASH文件系统
 */
  void Mount_SPI_Flash_File_System(void) 
	{
    spiffs_config SPIFlashCfg;
    SPIFlashCfg.phys_size        = SPIFLASH_CFG_PHYS_SZ;         // SPI Flash的总容量
    SPIFlashCfg.phys_addr        = SPIFLASH_CFG_PHYS_ADDR;       // 起始地址
    SPIFlashCfg.phys_erase_block = SPIFLASH_CFG_PHYS_ERASE_SZ;   // FLASH最大可擦除的块大小(W25X64可以按扇区擦除(4K)或者按块擦除(64K))
    SPIFlashCfg.log_block_size   = SPIFLASH_CFG_LOG_BLOCK_SZ;    // 块的大小(W25X64每块包含16个扇区，每个扇区4K字节，每块的总容量为：16X4=64K=65535字节)
    SPIFlashCfg.log_page_size    = SPIFLASH_CFG_LOG_PAGE_SZ;     //  (W25X64每页包含256个字节，16个页构成一个扇区)
    
    SPIFlashCfg.hal_read_f =  SPI_Flash_Read;    //读
    SPIFlashCfg.hal_write_f = SPI_Flash_Write;   //写
    SPIFlashCfg.hal_erase_f = SPI_Flash_Erase;   //擦除
    
		//挂载SPIFS	
    int res = SPIFFS_mount(&SPIFlashFS,
                           &SPIFlashCfg,
                           FS_Work_Buf,
                           FS_FDS,
                           sizeof(FS_FDS),
                           FS_Cache_Buf,
                           sizeof(FS_Cache_Buf),
                           0);
 }
		
/**********************************************************
 * 函数名 : bsp_Init
 * 输入   ：none
 * 输出   ：none 
 * 功能   ：外设初始化
 **********************************************************/
void bsp_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(72);
	LED_Init();	        //LED接口初始化
	USART1_Init(9600);
	SPI_FLASH_Init();
}

/**********************************************************
 * 函数名 : main
 * 输入   ：none
 * 输出   ：none 
 * 功能   ：
 **********************************************************/
int main(void)
{
	bsp_Init();//外设初始化
	
	Mount_SPI_Flash_File_System();//spi flash文件系统初始化
	
	//打开文件，如果文件不存在，自动创建
	spiffs_file fc = SPIFFS_open(&SPIFlashFS, "my1_file.ini", SPIFFS_CREAT | SPIFFS_TRUNC | SPIFFS_RDWR, 0);

	//写文件
	if (SPIFFS_write(&SPIFlashFS, fc, WriteBuf, sizeof(WriteBuf)) < 0)
	printf(" w_errno %i\n", SPIFFS_errno(&SPIFlashFS));
	SPIFFS_close(&SPIFlashFS, fc); 

	//读文件
	fc = SPIFFS_open(&SPIFlashFS, "my1_file.ini", SPIFFS_RDWR, 0);
	if (SPIFFS_read(&SPIFlashFS, fc, ReadBuf, sizeof(WriteBuf)) < 0)
	printf(" r_errno %i\n", SPIFFS_errno(&SPIFlashFS));
	SPIFFS_close(&SPIFlashFS, fc);

	//打印输出文件内容
	printf("%s\n", ReadBuf);	
	
	while(1);
}

