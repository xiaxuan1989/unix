#include "CharDevice.h"
#include "Utility.h"
#include "Kernel.h"

/*==============================class CharDevice===============================*/
CharDevice::CharDevice()
{
	this->m_TTY = NULL;
}

CharDevice::~CharDevice()
{
	//nothing to do here
}

void CharDevice::Open(short dev, int mode)
{
	Utility::Panic("ERROR! Base Class: CharDevice::Open()!");
}

void CharDevice::Close(short dev, int mode)
{
	Utility::Panic("ERROR! Base Class: CharDevice::Close()!");
}

void CharDevice::Read(short dev)
{
	Utility::Panic("ERROR! Base Class: CharDevice::Read()!");
}

void CharDevice::Write(short dev)
{
	Utility::Panic("ERROR! Base Class: CharDevice::Write()!");
}

void CharDevice::SgTTY(short dev, TTY* pTTY)
{
	Utility::Panic("ERROR! Base Class: CharDevice::SgTTY()!");
}


/*==============================class ConsoleDevice===============================*/
/* 
 * 这里定义派生类ConsoleDevice的对象实例。
 * 该实例对象中override了字符设备基类中
 * Open(), Close(), Read(), Write()等虚函数。
 */
ConsoleDevice g_ConsoleDevice;
extern TTY g_TTY;

ConsoleDevice::ConsoleDevice()
{
	//nothing to do here
}

ConsoleDevice::~ConsoleDevice()
{
	//nothing to do here
}

void ConsoleDevice::Open(short dev, int mode)
{
	short minor = Utility::GetMinor(dev);
	User& u = Kernel::Instance().GetUser();

	if ( minor != 0 )	/* 选择的不是console */
	{
		return;
	}

	if ( NULL == this->m_TTY )
	{
		this->m_TTY = &g_TTY;
	}

	/* 该进程第一次打开这个设备 */
	if ( NULL == u.u_procp->p_ttyp )
	{
		u.u_procp->p_ttyp = this->m_TTY;	
	}

	/* 设置设备初始模式 */
	if ( (this->m_TTY->t_state & TTY::ISOPEN) == 0 )
	{
		this->m_TTY->t_state = TTY::ISOPEN | TTY::CARR_ON;
		this->m_TTY->t_flags = TTY::ECHO;
		this->m_TTY->t_erase = TTY::CERASE;
		this->m_TTY->t_kill = TTY::CKILL;
	}
}

void ConsoleDevice::Close(short dev, int mode)
{
	//nothing to do here
}

void ConsoleDevice::Read(short dev)
{
	short minor = Utility::GetMinor(dev);

	if ( 0 == minor )
	{
		this->m_TTY->TTRead();	/* 判断是否选择了console */
	}
}

void ConsoleDevice::Write(short dev)
{
	short minor = Utility::GetMinor(dev);

	if ( 0 == minor )
	{
		this->m_TTY->TTWrite();	/* 判断是否选择了console */
	}
}

void ConsoleDevice::SgTTY(short dev, TTY *pTTY)
{
}
