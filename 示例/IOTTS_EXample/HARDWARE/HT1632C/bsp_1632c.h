//ָ���
#define SYS_DIS 0x00                //�ر�ϵͳ��
#define SYS_EN  0x01                //��ϵͳ��
#define LED_OFF 0x02                //�ر�LED��ʾ
#define LED_ON  0x03                //��LED��ʾ
#define BLINK_OFF   0x08            //�ر���˸
#define BLINK_ON    0x09            //����˸
#define SLAVE_MODE  0x10            //��ģʽ
#define RC_MASTER_MODE  0x18        //�ڲ�RCʱ��
#define EXT_CLK_MASTER_MODE 0x1C    //�ⲿʱ��
#define COM_OPTION  0x24            //16COM��NMOSģʽ
//#define PWM_DUTY  0XA0            //PWM���ȿ���
#define PWM_DUTY    0xAf            //PWM���ȿ���

/****************************************
*������:HT1632C_Writer
*��    ��:HT1632C�������ݺ�������λ��ǰ
*��    ��:Data,cnt
*��    ��:
****************************************/
void HT1632C_Writer(unsigned char Data,unsigned char cnt);     

/*******************************
*������:HT1632C_Writer_CMD
*��    ��:���������
*��    ��:cmd
*��    ��:
********************************/
void HT1632C_Writer_CMD(unsigned char cmd);                   

/*******************************
*������:HT1632C_Writer_DATA
*��    ��:ָ����ַд������
*��    ��:Addr,Data
*��    ��:
********************************/
void HT1632C_Writer_DATA(unsigned char Addr,unsigned char Data);

/************************************************************
*������:HT1632C_Writer_AllDATA
*��    ��:ָ����ַд���������ݣ�cntΪд�����ݸ�������8λ��
*��    ��:Addr,*p,cnt
*��    ��:
*************************************************************/
void HT1632C_Writer_AllDATA(unsigned char Addr,unsigned char *p,unsigned char cnt);

/*******************************
*������:HT1632C_clr
*��    ��:��������
*��    ��:
*��    ��:
********************************/
void HT1632C_clr(void);  

/*******************************
*������:HT1632C_Init
*��    ��:HT1632C��ʼ������
*��    ��:
*��    ��:
********************************/
void HT1632C_Init(void);     

/*******************************
*������:display_num
*��    ��:�������ʾ����
*��    ��:
*��    ��:
********************************/
//��ʾ��ֵ����С����
void display_num(unsigned int dat);
