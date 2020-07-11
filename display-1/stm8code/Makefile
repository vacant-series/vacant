CC=sdcc
SRCS=main.c
LIBS=-lstm8 -mstm8
IDIR=-I./
ODIR=bin
CFLAGS=$(IDIR)
FLASHER=stm8flash

all:
	mkdir -p $(ODIR)
	$(CC) $(SRCS) $(CFLAGS) $(LIBS) --out-fmt-ihx -o$(ODIR)/

.PHONY: clean	flash

clean:
	@rm -rf $(ODIR)

flash:
	@sudo $(FLASHER) -cstlinkv2 -pstm8s003k3 -w$(ODIR)/main.ihx
