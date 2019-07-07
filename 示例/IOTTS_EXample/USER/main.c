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
 *   ����SPI FLASH�ļ�ϵͳ
 */
  void Mount_SPI_Flash_File_System(void) 
	{
    spiffs_config SPIFlashCfg;
    SPIFlashCfg.phys_size        = SPIFLASH_CFG_PHYS_SZ;         // SPI Flash��������
    SPIFlashCfg.phys_addr        = SPIFLASH_CFG_PHYS_ADDR;       // ��ʼ��ַ
    SPIFlashCfg.phys_erase_block = SPIFLASH_CFG_PHYS_ERASE_SZ;   // FLASH���ɲ����Ŀ��С(W25X64���԰���������(4K)���߰������(64K))
    SPIFlashCfg.log_block_size   = SPIFLASH_CFG_LOG_BLOCK_SZ;    // ��Ĵ�С(W25X64ÿ�����16��������ÿ������4K�ֽڣ�ÿ���������Ϊ��16X4=64K=65535�ֽ�)
    SPIFlashCfg.log_page_size    = SPIFLASH_CFG_LOG_PAGE_SZ;     //  (W25X64ÿҳ����256���ֽڣ�16��ҳ����һ������)
    
    SPIFlashCfg.hal_read_f =  SPI_Flash_Read;    //��
    SPIFlashCfg.hal_write_f = SPI_Flash_Write;   //д
    SPIFlashCfg.hal_erase_f = SPI_Flash_Erase;   //����
    
		//����SPIFS	
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
 * ������ : bsp_Init
 * ����   ��none
 * ���   ��none 
 * ����   �������ʼ��
 **********************************************************/
void bsp_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(72);
	LED_Init();	        //LED�ӿڳ�ʼ��
	USART1_Init(9600);
	SPI_FLASH_Init();
}

/**********************************************************
 * ������ : main
 * ����   ��none
 * ���   ��none 
 * ����   ��
 **********************************************************/
int main(void)
{
	bsp_Init();//�����ʼ��
	
	Mount_SPI_Flash_File_System();//spi flash�ļ�ϵͳ��ʼ��
	
	//���ļ�������ļ������ڣ��Զ�����
	spiffs_file fc = SPIFFS_open(&SPIFlashFS, "my1_file.ini", SPIFFS_CREAT | SPIFFS_TRUNC | SPIFFS_RDWR, 0);

	//д�ļ�
	if (SPIFFS_write(&SPIFlashFS, fc, WriteBuf, sizeof(WriteBuf)) < 0)
	printf(" w_errno %i\n", SPIFFS_errno(&SPIFlashFS));
	SPIFFS_close(&SPIFlashFS, fc); 

	//���ļ�
	fc = SPIFFS_open(&SPIFlashFS, "my1_file.ini", SPIFFS_RDWR, 0);
	if (SPIFFS_read(&SPIFlashFS, fc, ReadBuf, sizeof(WriteBuf)) < 0)
	printf(" r_errno %i\n", SPIFFS_errno(&SPIFlashFS));
	SPIFFS_close(&SPIFlashFS, fc);

	//��ӡ����ļ�����
	printf("%s\n", ReadBuf);	
	
	while(1);
}

