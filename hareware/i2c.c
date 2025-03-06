#include "i2c.h"

void I2C_W_SCL(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_4, (BitAction)BitValue);		//根据BitValue，设置SCL引脚的电平
	delay_us(1);												//延时10us，防止时序频率超过要求
}

void I2C_W_SDA(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_3, (BitAction)BitValue);		//根据BitValue，设置SDA引脚的电平，BitValue要实现非0即1的特性
	delay_us(1);												//延时10us，防止时序频率超过要求
}


uint8_t I2C_R_SDA(void)
{
	uint8_t BitValue;
	BitValue = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3);		//读取SDA电平
	delay_us(1);												//延时10us，防止时序频率超过要求
	return BitValue;											//返回SDA电平
}


void I2C_GPIO_Init(void)
{
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//开启GPIOB的时钟
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);					
	/*设置默认电平*/
	GPIO_SetBits(GPIOB, GPIO_Pin_3 | GPIO_Pin_4);			
}

void I2C_Start(void)
{
	I2C_W_SDA(1);							//释放SDA，确保SDA为高电平
	I2C_W_SCL(1);							//释放SCL，确保SCL为高电平
	I2C_W_SDA(0);							//在SCL高电平期间，拉低SDA，产生起始信号
	I2C_W_SCL(0);							//起始后把SCL也拉低，即为了占用总线，也为了方便总线时序的拼接
}


void I2C_Stop(void)
{
	I2C_W_SCL(0);
	I2C_W_SDA(0);							//拉低SDA，确保SDA为低电平
	I2C_W_SCL(1);							//释放SCL，使SCL呈现高电平
	I2C_W_SDA(1);							//在SCL高电平期间，释放SDA，产生终止信号
}


void I2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	for (i = 0; i < 8; i ++)				//循环8次，主机依次发送数据的每一位
	{
		/*两个!可以对数据进行两次逻辑取反，作用是把非0值统一转换为1，即：!!(0) = 0，!!(非0) = 1*/
		I2C_W_SDA(!!(Byte & (0x80 >> i)));//使用掩码的方式取出Byte的指定一位数据并写入到SDA线
		I2C_W_SCL(1);						//释放SCL，从机在SCL高电平期间读取SDA
		I2C_W_SCL(0);						//拉低SCL，主机开始发送下一位数据
	}
}


uint8_t I2C_ReceiveByte(void)
{
	uint8_t i, Byte = 0x00;					//定义接收的数据，并赋初值0x00，此处必须赋初值0x00，后面会用到
	I2C_W_SDA(1);							//接收前，主机先确保释放SDA，避免干扰从机的数据发送
	for (i = 0; i < 8; i ++)				//循环8次，主机依次接收数据的每一位
	{
		I2C_W_SCL(0);
		I2C_W_SCL(1);						//拉低SCL，从机在SCL低电平期间写入SDA
		Byte<<=1;						
		if (I2C_R_SDA()){Byte++;}	//读取SDA数据，并存储到Byte变量
													//当SDA为1时，置变量指定位为1，当SDA为0时，不做处理，指定位为默认的初值0
	}
	return Byte;							//返回接收到的一个字节数据
}


void I2C_SendAck(uint8_t AckBit)
{
	I2C_W_SCL(0);
	I2C_W_SDA(AckBit);					//主机把应答位数据放到SDA线
	I2C_W_SCL(1);							//释放SCL，从机在SCL高电平期间，读取应答位
	I2C_W_SCL(0);							//拉低SCL，开始下一个时序模块
}


uint8_t I2C_ReceiveAck(void)
{
	uint8_t AckBit;							//定义应答位变量
	I2C_W_SDA(1);							//接收前，主机先确保释放SDA，避免干扰从机的数据发送
	I2C_W_SCL(1);							//释放SCL，主机机在SCL高电平期间读取SDA
	AckBit = I2C_R_SDA();					//将应答位存储到变量里
	I2C_W_SCL(0);							//拉低SCL，开始下一个时序模块
	return AckBit;							//返回定义应答位变量
}
