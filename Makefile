CC     = cc
CFLAGS = -std=c99 -O2 -Wall -Wextra
TARGET = zim
SRC    = zim.c

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGET)

.PHONY: clean
