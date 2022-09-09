/*************************************************************************
 FM31256��һ�ֻ���I2C���ߡ����������弼���Ķ๦�ܴ洢оƬ��
 ���˷���ʧ�洢���⣬������������ʵʱʱ�ӡ��͵�ѹ��λ��
 ���Ź�������������ʧ���¼����������������Ĵ������ֱ�
 ʶ�ȶ��ֹ��ܡ�
 
 FM31256�Ķ�/д����
 FM31256��Ϊ�ӻ����������������ܲ�ͬ�Ĳ�����ÿ�����������Ա��������ʡ�
 һ���Ǵ洢��������ʱ�ӻ���ַ��λ7��4���뱻����Ϊ1010B��
 һ������Ҫ����ʵʱʱ��/���������£���ӻ���ַ��λ7��4���뱻����Ϊ1101B��
**************************************************************************/
#include "delay.h"
#include "config.h"
#include "FM31256.h"
#include "uart1.h"


uint8_t Updata_FG;////����ʱ����±�־λ
uint8_t FM_Datatemp[7]			=	{	0x32,	0x02,	0x0F,	0x06,		0x03,	0x09,	0x10};
		//ʱ���ʽ�����ֽ�--���ֽ�  �룬�֣�	ʱ��	���ڣ�	�գ�		�£��ꡣ




static void Start(void)//����IIC��ʼ�ź�
{
  IIC_Start();
}
static void Stop(void)//����IICֹͣ�ź�
{
  IIC_Stop() ;
}

static void Ack(void)
{
   IIC_Wait_Ack();
}
static void WriteByte(uint8_t txd)
{
	IIC_Send_Byte(txd) ;
}

//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
static uint8_t ReadByte(void)
{	uint8_t temp;
	uint8_t ack=0;
	temp=IIC_Read_Byte(ack);
	return temp;
}
static void WriteReg(uint8_t RegAdd,uint8_t Wdata)
{
	IIC_Start();  
	IIC_Send_Byte(REG_WRITE);	   //����д����
	IIC_Wait_Ack();
	IIC_Send_Byte(RegAdd& 0x1f);//���͸ߵ�ַ	    
	IIC_Wait_Ack(); 
	IIC_Send_Byte(Wdata);           //�������ģʽ			   
	IIC_Wait_Ack();	 	   
  IIC_Stop();//����һ��ֹͣ����	    
}

uint8_t FM_ReadReg(uint16_t ReadAddr)
{				  
	uint8_t temp=0;		  	    																 
  IIC_Start();  
	IIC_Send_Byte(REG_WRITE);	   //����д����
	IIC_Wait_Ack();
	IIC_Send_Byte(ReadAddr);//���͸ߵ�ַ	    
	IIC_Wait_Ack(); 
	IIC_Start();  	 	   
	IIC_Send_Byte(REG_READ);           //�������ģʽ			   
	IIC_Wait_Ack();	 
  temp=IIC_Read_Byte(0);		   
  IIC_Stop();//����һ��ֹͣ����	    
	return temp;
}

uint8_t ReadFRAM(uint16_t Address)
{
    uint8_t RetValue;
    Start();
    WriteByte(IIC_WRITE);
    Ack();
    WriteByte((uint8_t)(Address>>8));
    Ack();
    WriteByte((uint8_t)(Address));
    Ack();
    Start();
    WriteByte(IIC_READ);
    Ack();
    RetValue = ReadByte();
    IIC_SCL_Hight;delay_us(2);//delay_us(10);
		IIC_SDA_Hight;delay_us(2);//delay_us(10);
		IIC_SCL_Low;
    Stop();
    return RetValue;
}

void WriteFRAM(uint16_t Address,uint8_t Wdata)
{
    Start();
    WriteByte(IIC_WRITE);
    Ack();
    WriteByte((uint8_t)(Address>>8));
    Ack();
    WriteByte((uint8_t)(Address));
    Ack();
    WriteByte(Wdata);
    Ack();
    Stop();
}
uint8_t FM31256_ReadOneByte_Page(uint8_t Page,uint16_t ReadAddr)
{				  
	uint8_t temp=0;		  	    																 
  IIC_Start();  
//	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	   //����д����
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr>>8);//���͸ߵ�ַ	    
	}
//	else 
//		IIC_Send_Byte(Page+((ReadAddr/256)<<1));   //����������ַ0XA0,д���� 	   
	IIC_Wait_Ack(); 
  IIC_Send_Byte(ReadAddr%256);   //���͵͵�ַ
	IIC_Wait_Ack();	    
	IIC_Start();  	 	   
	IIC_Send_Byte(0XA0+1);           //�������ģʽ			   
	IIC_Wait_Ack();	 
  temp=IIC_Read_Byte(0);		   
  IIC_Stop();//����һ��ֹͣ����	    
	return temp;
}

void FM31256_WriteOneByte_Page(uint8_t Page,uint16_t WriteAddr,uint8_t DataToWrite)
{				   	  	    																 
  IIC_Start();  
//	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	    //����д����
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr>>8);//���͸ߵ�ַ	  
	}
//	else 
//		IIC_Send_Byte(0XA0+((WriteAddr/256)<<1));   //����������ַ0XA0,д���� 	 
	IIC_Wait_Ack();	   
  IIC_Send_Byte(WriteAddr%256);   //���͵͵�ַ
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(DataToWrite);     //�����ֽ�							   
	IIC_Wait_Ack();  		    	   
  IIC_Stop();//����һ��ֹͣ���� 
	delay_ms(10);
}



void Fm31256_init(void)
{
	WriteReg(0x01,0x80);//ֹͣʱ��
	WriteReg(CONTROL,0x00);	//����ʱ�� 
	//ʱ�Ӹ���	
	Updata_FG=FM31256_ReadOneByte_Page(0xA0,EE_Basic_Addarss+114);
	if(Updata_FG !=0X55)
	{
		Set_Realtime(FM_Datatemp);//FM_Datatemp����FM31256 �ڲ�ʱ��	
		FM31256_WriteOneByte_Page(0xA0,EE_Basic_Addarss+114,0X55);//����ʱ����±�־λ
	}	
//WatchDog_Enable(0x1e);//���Ź�ʹ�� 3000ms	
	WatchDog_Disable();
//--оƬ����--------------------------
//	FM31256_WriteOneByte_Page(0xa0,114,0x54);
//	for(i=0;i<10;i++)
//	{
//		temp=FM31256_ReadOneByte_Page(EE_Page1,FM31_Addarss+101+i);
//		UART1_send_byte(temp);
//	}	
//temp=	ReadFRAM(0x7d);
//temp=FM31256_ReadOneByte_Page(EE_Page1,114);
//UART1_send_byte(temp);
}


void Set_Realtime(uint8_t *realtime)//ʱ���ʽ�����ֽ�--���ֽ�  �룬�֣�ʱ�����ڣ��գ��£��ꡣ
{
    uint8_t i;
//		WriteReg(0x0B,0x04);	//���¿���
//		WriteReg(FLAGS,0x00);   //�����־λ
		WriteReg(FLAGS,0x02);   //����ʱ��д����
    for(i=0;i<7;i++)
		{
			WriteReg(0x02+i,hex_to_bcd(realtime[i]));//
			delay_us(10);
		}
    WriteReg(FLAGS,0x00);     //��оƬ����ʱ��
		//WriteReg(FLAGS,0x01);   //����ʱ�������
}

//void Uart1_Set_Realtime(void) //ͨ��������ʱ �ο����������ն�ͨ��Э��V0.4��
//{
//	FM_Datatemp[0]=Uart1_DataBuffer[10];	//��
//	FM_Datatemp[1]=Uart1_DataBuffer[9];		//����
//	FM_Datatemp[2]=Uart1_DataBuffer[8];		//��ʱ
//	FM_Datatemp[3]=Uart1_DataBuffer[11];	//������
//	FM_Datatemp[4]=Uart1_DataBuffer[7];		//����
//	FM_Datatemp[5]=Uart1_DataBuffer[6];		//����
//	FM_Datatemp[6]=Uart1_DataBuffer[5];		//����
//	Set_Realtime(FM_Datatemp);
//}


void Get_Realtime(uint8_t *realtime)//��ȡʱ�� ��ʽ�����ֽ�--���ֽ�  �룬�֣�ʱ�����ڣ��գ��£��ꡣ
{
    uint8_t i;
//    WriteReg(FLAGS,0x00);          //
    WriteReg(FLAGS,0x01); 	delay_us(50);         //����ʱ�������
    for(i=0;i<7;i++)
		{
			realtime[i]=bcd_to_hex(FM_ReadReg(0x02+i));//
			delay_us(5);
		}	
    WriteReg(FLAGS,0x00);          //оƬʱ��������û��Ĵ���
}
void WatchDog_Enable(uint8_t timer)//���Ź�ʹ��
{
	timer=timer | 0x80;           
  WriteReg(0x0A,timer);         //���ÿ��Ź�����������ֵ	
}
void WatchDog_Clear(void)//ι�� 
{
    WriteReg(0x09,0x0A);          
}
void WatchDog_Disable(void)
{
    WriteReg(0x0A,0x00);
}













//=========================================
/*;
* Name: hex_to_bcd;
* Description: ʮ��������ת����BCD���ʽ;
* Calls: None;
* Input: ��Ҫת����ʮ��������;
* Outputs: ת�����BCD��
*/
//=======================================
uint8_t hex_to_bcd(uint8_t condata)
{    
	uint8_t temp;    
	uint8_t temp0;    
	uint8_t temp10;    
	temp10=condata/10;    
	temp0=condata%10;    
	temp=(temp10<<4)|temp0;
	return(temp);
}
//=======================================
/*
* Name: bcd_to_hex;
* Description: BCD���ʽת����ʮ��������;
* Calls: None//���ã���;
* Input: ��Ҫת����BCD��;
* Outputs: ת�����ʮ��������
*/
//======================================
uint8_t bcd_to_hex(uint8_t condata)
{
	uint8_t temp;    
	uint8_t temp0;    
	uint8_t temp10;    
	temp10=(condata&0xF0)>>4;    
	temp0=condata&0x0F;    
	temp=temp10*10+temp0;    
	return(temp);
}



