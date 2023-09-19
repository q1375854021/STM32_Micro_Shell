#include "Usart1.h"
#include "stdio.h"


#pragma import(__use_no_semihosting)
//标准库需要的支持函数

//半主机模式 禁止  否则printf不会工作


struct __FILE
{
    int handle;
};
FILE __stdout;




struct USART_Data_Array_Struc *USART1_Data_Array;      //串口一缓冲区






//重定义fputc函数  //默认是USART1，然后编译的时候必须勾选micro lib
int fputc(int ch, FILE *f)
{
		while(LL_USART_IsActiveFlag_TC(USART1)==0);     //防止前面的数据还没有发完就被后面的覆盖
		LL_USART_TransmitData8(USART1,(uint8_t)ch);    //发送一个字符
		return ch;
}




void USART_Printf(USART_TypeDef *USART, char *str)        //发送字符串
{
	int temp_i = 0;         //用于取字符串的每一个字符
	while(*(str+temp_i) != '\0')
	{
		while(LL_USART_IsActiveFlag_TC(USART)==0);      //0表示数据还没有发完，要等待
		LL_USART_TransmitData8(USART,*(str+temp_i));    //发送
		temp_i++;
	}
}





// 发送一个字节
void USART_SendByte( USART_TypeDef *USART, uint8_t data)
{
	while(LL_USART_IsActiveFlag_TC(USART)==0);  //0表示数据还没有发完，要等待
	LL_USART_TransmitData8(USART,data);         //发送数据
}

// 发送多个字节，但是这里没有判别数组是否越界，使用时要注意
void USART_SendBytes(USART_TypeDef *USART,uint8_t *data, int Send_Byte_Num) 
{
	int temp_i = 0;       //用于取数组的每一个内容
	while(temp_i < Send_Byte_Num)
	{
		USART_SendByte(USART,*(data+temp_i));
		temp_i++;
	}
}




void USART_Send_Nbit_Data(USART_TypeDef *USART,void *data,int Nbit)  //int    long int     float 均为四位  double 8位
{
	uint8_t *temp_pointer = (uint8_t *)data;      //把float指针转为uint8_t指针，便于每8字节读取一个数据，否则则是32位  因为float就是32位的
	for(int i=0;i<Nbit;i++)   //float占用四个字节
	{
		USART_SendByte(USART,*(temp_pointer+Nbit-1-i));   //从高字节开始往低字节发，也就是说先发的是高位
	}  //完毕
}




// 功能 从循环队列中读一个数 
// 参数 缓冲区的指针
uint8_t USART_Readonebyte(struct USART_Data_Array_Struc *Struc)
{
	uint8_t *ch;
	if(USART_Data_Array_Struc_Pop(Struc, ch) != 0)
		printf("队列空\r\n");
	return *ch;
}

void USART_Array_Clear(struct USART_Data_Array_Struc *Struc)
{
	if(USART_Data_Array_Struc_Initialize( Struc, Struc->Data_Array_Length) != 0)   //重新初始化 
	{
		printf("清空串口缓冲区失败\r\n");
	}
}


// 向串口缓冲区写一个数据
void USART_Writeonebyte(struct USART_Data_Array_Struc *Struc, uint8_t ch)
{
	if( USART_Data_Array_Struc_Push(Struc,ch) != 0)
		printf("队列满\r\n");
}
	



 
// 功能  循环队列初始化
// 参数： 
// 缓冲区数组的指针
// 循环队列的长度 
uint8_t USART_Data_Array_Struc_Initialize(struct USART_Data_Array_Struc *Struc, uint16_t DATA_Array_Length)
{
	int i=0;
	Struc->front=0;                //队首 
	Struc->rear=0;                 //队尾 
	Struc->Data_Array_Length=DATA_Array_Length;    //队列长度初始化
	for(i=0;i<=DATA_Array_Length-1;i++)   //将数组清零 
	{
		Struc->Data_Array[i]=0;
	}
	// 循环队列的缓冲区不用初始化，自动为0 
	return 0;
}

// 功能 向循环队列写一个数
// 参数 缓冲区的指针
// 要写入的byte 
uint8_t USART_Data_Array_Struc_Push(struct USART_Data_Array_Struc *Struc, uint8_t ch)
{
	if( (Struc->rear + 1)%(Struc->Data_Array_Length) == Struc->front)
		return 1;       //队列满 
	else
	{
		Struc->Data_Array[Struc->rear] = ch;
		Struc->rear = (Struc->rear + 1)%Struc->Data_Array_Length;   //防止rear无线增大超过数据类型上限
		return 0;    //正确放入 
	} 
}

// 功能 从循环队列中读一个数 
// 参数 缓冲区的指针
// 读出的数据 
uint8_t USART_Data_Array_Struc_Pop(struct USART_Data_Array_Struc *Struc, uint8_t *ch)
{
	if(Struc->rear == Struc->front)
	{
		return 1;    //队空 
	}
	
	*ch = Struc->Data_Array[Struc->front];   //从front读取一个数
	Struc->front = (Struc->front + 1) % (Struc->Data_Array_Length);   //指针后移
	return 0;   //正确读取 
}

// 功能 计算循环队列的长度
// 参数  循环队列的指针 
int USART_Data_Array_Struc_GetDataArrayLength(struct USART_Data_Array_Struc *Struc)
{
	return (Struc->rear - Struc->front + Struc->Data_Array_Length) % Struc->Data_Array_Length;
}
