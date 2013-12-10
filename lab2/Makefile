CC = gcc -std=c99
CFLAGS  = -g -Wall -pthread
CLOCKS  = -D LOCKS
RM = rm

# the build target executable:
TARGET = tester
OUTPUT = tester1lock
OUTPUT2 = tester2lock

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(OUTPUT) $(TARGET).c
	$(CC) $(CFLAGS) $(CLOCKS) -o $(OUTPUT2) $(TARGET).c

clean:
	rm -r -f $(OUTPUT)* $(OUTPUT2)*  

