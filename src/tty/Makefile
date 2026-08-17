include ../Makefile.inc

TARGET = ..\..\targets\objs

all		:	$(TARGET)\tty.o $(TARGET)\keyboard.o $(TARGET)\terminaldisplay.o
			
$(TARGET)\tty.o	:	TTY.cpp $(INCLUDE)\TTY.h
	$(CC) $(CFLAGS) -I"$(INCLUDE)" -c $< -o $@

$(TARGET)\keyboard.o	:	Keyboard.cpp $(INCLUDE)\Keyboard.h
	$(CC) $(CFLAGS) -I"$(INCLUDE)" -c $< -o $@	

$(TARGET)\terminaldisplay.o	:	TerminalDisplay.cpp $(INCLUDE)\TerminalDisplay.h
	$(CC) $(CFLAGS) -I"$(INCLUDE)" -c $< -o $@
