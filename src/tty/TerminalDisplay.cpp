#include "TerminalDisplay.h"
#include "IOPort.h"

#include "vesa/console.h"

unsigned short* TerminalDisplay::m_VideoMemory = (unsigned short *)(0xB8000 + 0xC0000000);
unsigned int TerminalDisplay::m_CursorX = 0;
unsigned int TerminalDisplay::m_CursorY = 0;
char* TerminalDisplay::m_Position = 0;
char* TerminalDisplay::m_BeginChar = 0;

unsigned int TerminalDisplay::ROWS = 25;

void TerminalDisplay::Start(TTY* pTTY)
{
	char ch;
	if ( 0 == TerminalDisplay::m_BeginChar)
	{
		m_BeginChar = pTTY->t_outq.CurrentChar();
	}
	if ( 0 == m_Position )
	{
		m_Position = m_BeginChar;
	}

	while ( (ch = pTTY->t_outq.GetChar()) != TTY::GET_ERROR )
	{


#ifdef USE_VESA
		video::console::writeOutput(&ch, 1);
#else


		switch (ch)
		{
		case '\n':
			NextLine();
			TerminalDisplay::m_BeginChar = pTTY->t_outq.CurrentChar();
			m_Position = TerminalDisplay::m_BeginChar;
			break;

		case 0x15:
			//del_line();
			break;

		case '\b':
			if ( m_Position != TerminalDisplay::m_BeginChar )
			{
				BackSpace();
				m_Position--;
			}
			break;

		case '\t':
			Tab();
			m_Position++;
			break;

		default:	/* 在屏幕上回显普通字符 */
			WriteChar(ch);
			m_Position++;
			break;
		}
#endif
	}
}

void TerminalDisplay::MoveCursor(unsigned int col, unsigned int row)
{

#ifndef USE_VESA
	if ( (col < 0 || col >= TerminalDisplay::COLUMNS) || (row < 0 || row >= TerminalDisplay::ROWS) )
	{
		return;
	}

	/* 计算光标偏移量 */
	unsigned short cursorPosition = row * TerminalDisplay::COLUMNS + col;

	/* 选择 r14和r15寄存器，分别为光标位置的高8位和低8位 */
	IOPort::OutByte(TerminalDisplay::VIDEO_ADDR_PORT, 14);
	IOPort::OutByte(TerminalDisplay::VIDEO_DATA_PORT, cursorPosition >> 8);
	IOPort::OutByte(TerminalDisplay::VIDEO_ADDR_PORT, 15);
	IOPort::OutByte(TerminalDisplay::VIDEO_DATA_PORT, cursorPosition & 0xFF);
#endif
}

void TerminalDisplay::NextLine()
{

#ifndef USE_VESA
	m_CursorX = 0;
	m_CursorY += 1;

	/* 超出最大行数 */
	if ( m_CursorY >= TerminalDisplay::ROWS )
	{
		m_CursorY = 0;
		ClearScreen();
	}
	MoveCursor(m_CursorX, m_CursorY);
#endif
}

void TerminalDisplay::BackSpace()
{
	
#ifndef USE_VESA
	m_CursorX--;

	/* 移动光标，如果要回到上一行的话 */
	if ( m_CursorX < 0 )
	{
		m_CursorX = TerminalDisplay::COLUMNS - 1;
		m_CursorY--;
		if ( m_CursorY < 0 )
		{
			m_CursorY = 0;
		}
	}
	MoveCursor(m_CursorX, m_CursorY);

	/* 在光标所在位置填上空格 */
	m_VideoMemory[m_CursorY * COLUMNS + m_CursorX] = ' ' | TerminalDisplay::COLOR;
#endif
}

void TerminalDisplay::Tab()
{
	
#ifndef USE_VESA
	auto oldCursorX = m_CursorX;

	m_CursorX &= 0xFFFFFFF8;	/* 向左对齐到前一个Tab边界 */
	m_CursorX += 8;
	// const int TabWidth = 10;
	// m_CursorX -= m_CursorX % TabWidth;
	// m_CursorX += TabWidth;
	if ( m_CursorX >= TerminalDisplay::COLUMNS )
		NextLine();
	else {
		MoveCursor(m_CursorX, m_CursorY);
	}
#endif
}

void TerminalDisplay::WriteChar(char ch)
{
#ifndef USE_VESA
	m_VideoMemory[m_CursorY * TerminalDisplay::COLUMNS + m_CursorX] = (unsigned char) ch | TerminalDisplay::COLOR;
	m_CursorX++;
	
	if ( m_CursorX >= TerminalDisplay::COLUMNS )
	{
		NextLine();
	}
	MoveCursor(m_CursorX, m_CursorY);
#endif
}

void TerminalDisplay::ClearScreen()
{

#ifdef USE_VESA
	// todo
#else

	unsigned int i;

	for ( i = 0; i < COLUMNS * ROWS; i++ )
	{
		m_VideoMemory[i] = (unsigned short)' ' | TerminalDisplay::COLOR;
	}
#endif
}

