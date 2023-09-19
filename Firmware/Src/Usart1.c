#include "Usart1.h"
#include "stdio.h"


#pragma import(__use_no_semihosting)
//��׼����Ҫ��֧�ֺ���

//������ģʽ ��ֹ  ����printf���Ṥ��


struct __FILE
{
    int handle;
};
FILE __stdout;




struct USART_Data_Array_Struc *USART1_Data_Array;      //����һ������






//�ض���fputc����  //Ĭ����USART1��Ȼ������ʱ����빴ѡmicro lib
int fputc(int ch, FILE *f)
{
		while(LL_USART_IsActiveFlag_TC(USART1)==0);     //��ֹǰ������ݻ�û�з���ͱ�����ĸ���
		LL_USART_TransmitData8(USART1,(uint8_t)ch);    //����һ���ַ�
		return ch;
}




void USART_Printf(USART_TypeDef *USART, char *str)        //�����ַ���
{
	int temp_i = 0;         //����ȡ�ַ�����ÿһ���ַ�
	while(*(str+temp_i) != '\0')
	{
		while(LL_USART_IsActiveFlag_TC(USART)==0);      //0��ʾ���ݻ�û�з��꣬Ҫ�ȴ�
		LL_USART_TransmitData8(USART,*(str+temp_i));    //����
		temp_i++;
	}
}





// ����һ���ֽ�
void USART_SendByte( USART_TypeDef *USART, uint8_t data)
{
	while(LL_USART_IsActiveFlag_TC(USART)==0);  //0��ʾ���ݻ�û�з��꣬Ҫ�ȴ�
	LL_USART_TransmitData8(USART,data);         //��������
}

// ���Ͷ���ֽڣ���������û���б������Ƿ�Խ�磬ʹ��ʱҪע��
void USART_SendBytes(USART_TypeDef *USART,uint8_t *data, int Send_Byte_Num) 
{
	int temp_i = 0;       //����ȡ�����ÿһ������
	while(temp_i < Send_Byte_Num)
	{
		USART_SendByte(USART,*(data+temp_i));
		temp_i++;
	}
}




void USART_Send_Nbit_Data(USART_TypeDef *USART,void *data,int Nbit)  //int    long int     float ��Ϊ��λ  double 8λ
{
	uint8_t *temp_pointer = (uint8_t *)data;      //��floatָ��תΪuint8_tָ�룬����ÿ8�ֽڶ�ȡһ�����ݣ���������32λ  ��Ϊfloat����32λ��
	for(int i=0;i<Nbit;i++)   //floatռ���ĸ��ֽ�
	{
		USART_SendByte(USART,*(temp_pointer+Nbit-1-i));   //�Ӹ��ֽڿ�ʼ�����ֽڷ���Ҳ����˵�ȷ����Ǹ�λ
	}  //���
}




// ���� ��ѭ�������ж�һ���� 
// ���� ��������ָ��
uint8_t USART_Readonebyte(struct USART_Data_Array_Struc *Struc)
{
	uint8_t *ch;
	if(USART_Data_Array_Struc_Pop(Struc, ch) != 0)
		printf("���п�\r\n");
	return *ch;
}

void USART_Array_Clear(struct USART_Data_Array_Struc *Struc)
{
	if(USART_Data_Array_Struc_Initialize( Struc, Struc->Data_Array_Length) != 0)   //���³�ʼ�� 
	{
		printf("��մ��ڻ�����ʧ��\r\n");
	}
}


// �򴮿ڻ�����дһ������
void USART_Writeonebyte(struct USART_Data_Array_Struc *Struc, uint8_t ch)
{
	if( USART_Data_Array_Struc_Push(Struc,ch) != 0)
		printf("������\r\n");
}
	



 
// ����  ѭ�����г�ʼ��
// ������ 
// �����������ָ��
// ѭ�����еĳ��� 
uint8_t USART_Data_Array_Struc_Initialize(struct USART_Data_Array_Struc *Struc, uint16_t DATA_Array_Length)
{
	int i=0;
	Struc->front=0;                //���� 
	Struc->rear=0;                 //��β 
	Struc->Data_Array_Length=DATA_Array_Length;    //���г��ȳ�ʼ��
	for(i=0;i<=DATA_Array_Length-1;i++)   //���������� 
	{
		Struc->Data_Array[i]=0;
	}
	// ѭ�����еĻ��������ó�ʼ�����Զ�Ϊ0 
	return 0;
}

// ���� ��ѭ������дһ����
// ���� ��������ָ��
// Ҫд���byte 
uint8_t USART_Data_Array_Struc_Push(struct USART_Data_Array_Struc *Struc, uint8_t ch)
{
	if( (Struc->rear + 1)%(Struc->Data_Array_Length) == Struc->front)
		return 1;       //������ 
	else
	{
		Struc->Data_Array[Struc->rear] = ch;
		Struc->rear = (Struc->rear + 1)%Struc->Data_Array_Length;   //��ֹrear�������󳬹�������������
		return 0;    //��ȷ���� 
	} 
}

// ���� ��ѭ�������ж�һ���� 
// ���� ��������ָ��
// ���������� 
uint8_t USART_Data_Array_Struc_Pop(struct USART_Data_Array_Struc *Struc, uint8_t *ch)
{
	if(Struc->rear == Struc->front)
	{
		return 1;    //�ӿ� 
	}
	
	*ch = Struc->Data_Array[Struc->front];   //��front��ȡһ����
	Struc->front = (Struc->front + 1) % (Struc->Data_Array_Length);   //ָ�����
	return 0;   //��ȷ��ȡ 
}

// ���� ����ѭ�����еĳ���
// ����  ѭ�����е�ָ�� 
int USART_Data_Array_Struc_GetDataArrayLength(struct USART_Data_Array_Struc *Struc)
{
	return (Struc->rear - Struc->front + Struc->Data_Array_Length) % Struc->Data_Array_Length;
}
