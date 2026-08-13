#include "DeviceManager.h"

extern ATABlockDevice g_ATADevice;
extern ConsoleDevice g_ConsoleDevice;
extern Devtab g_Atab;

inline void* operator new(unsigned int, void* p)
{
	return p;
}

DeviceManager::DeviceManager()
{
}

DeviceManager::~DeviceManager()
{
}

void DeviceManager::Initialize()
{
	//for (int i = 0; i < DeviceManager::MAX_DEVICE_NUM; i++)
	//{
	//	this->bdevsw[i] = NULL;
	//	this->cdevsw[i] = NULL;
	//}

	new (&g_Atab) Devtab();
	new (&g_ATADevice) ATABlockDevice(&g_Atab);
	new (&g_ConsoleDevice) ConsoleDevice();

	this->bdevsw[0] = &g_ATADevice;
	this->nblkdev = 1;

	this->cdevsw[0] = &g_ConsoleDevice;
	this->nchrdev = 1;
}

int DeviceManager::GetNBlkDev()
{
	return this->nblkdev;
}

BlockDevice& DeviceManager::GetBlockDevice(short major)
{
	if(major >= this->nblkdev || major < 0)
	{
		Utility::Panic("Block Device Doesn't Exist!");
	}
	return *(this->bdevsw[major]);
}

int DeviceManager::GetNChrDev()
{
	return this->nchrdev;
}

CharDevice& DeviceManager::GetCharDevice(short major)
{
	if (major >= this->nchrdev || major < 0)
	{
		Utility::Panic("Char Device Doesn't Exist!");
	}
	return *(this->cdevsw[major]);
}
