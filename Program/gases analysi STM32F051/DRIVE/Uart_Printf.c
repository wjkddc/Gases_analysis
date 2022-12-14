#include "stm32f0xx.h"
#include <stdio.h>

/*
*********************************************************************************************************
*	函 数 名: fputc
*	功能说明: 重定义putc函数，这样可以使用printf函数从串口1打印输出
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
int fputc(int ch, FILE *f)
{
	/* 写一个字节到USART1 */
	USART_SendData(USART1, (uint8_t) ch);

	/* 等待发送结束 */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
	{}

	return ch;
}

/*
*********************************************************************************************************
*	函 数 名: fgetc
*	功能说明: 重定义getc函数，这样可以使用scanff函数从串口1输入数据
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
int fgetc(FILE *f)
{
	/* 等待串口1输入数据 */
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

	return (int)USART_ReceiveData(USART1);
}







////在main文件中重定义<fputc>函数 如下：
//// 发送数据
//int fputc(int ch, FILE *f)
//{
//	USART_SendData(USART1, (unsigned char) ch);// USART1 可以换成 USART2 等
//	while (!(USART1->ISR & USART_FLAG_TXE));
//	return (ch);
//}
//// 接收数据
//int GetKey (void) {
//	while (!(USART1->ISR & USART_FLAG_RXNE));
//	return ((int)(USART1->TDR & 0x1FF));
//}
