#ifndef CHAR_DEVICE_H
#define CHAR_DEVICE_H

#include "TTY.h"

class CharDevice
{
public:
	CharDevice();
	virtual ~CharDevice();
	/* 
	 * 定义为虚函数，由派生类进行override实现设备
	 * 特定操作。正常情况下，基类中函数不应被调用到。
	 */
	virtual void Open(short dev, int mode) = 0;
	virtual void Close(short dev, int mode) = 0;
	virtual void Read(short dev) = 0;
	virtual void Write(short dev) = 0;
	virtual void SgTTY(short dev, TTY* pTTY) = 0;

public:
	TTY* m_TTY;		/* 指向字符设备TTY结构的指针 */
};


class ConsoleDevice : public CharDevice
{
public:
	ConsoleDevice();
	virtual ~ConsoleDevice();
	/* 
	 * Override基类CharDevice中的虚函数，实现
	 * 派生类ConsoleDevice特定的设备操作逻辑。
	 */
	void Open(short dev, int mode);
	void Close(short dev, int mode);
	void Read(short dev);
	void Write(short dev);
	void SgTTY(short dev, TTY* pTTY);
};

#endif
