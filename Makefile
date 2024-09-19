CC=gcc
CFLAGS=-Wall -std=c99 -g -O2 -I./include
LDFLAGS=
OBJDIR=./obj
BINDIR=./bin
SRCDIR=./src
VSSRCDIR=./src/aesavs
TESTDIR=./test
INCDIR=./include

OBJS=$(OBJDIR)/aes_core.o $(OBJDIR)/key_schedule.o $(OBJDIR)/aes_utils.o $(OBJDIR)/aes32.o\
	$(OBJDIR)/aesavs.o $(OBJDIR)/aesavs_vartxt_kat.o \
	$(OBJDIR)/aes_test.o \
	$(OBJDIR)/main.o

TARGET=$(BINDIR)/a.out

.PHONY: all clean dir rebuild

all: dir $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

# $(OBJDIR)/main.o: main.c
# 	$(CC) $(CFLAGS) -c main.c -o $@
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@
$(OBJDIR)/%.o: $(VSSRCDIR)/%.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@
$(OBJDIR)/%.o: $(TESTDIR)/%.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

-include $(OBJS:.o=.d)

KATFILES_TO_DELETE = AES_KAT/VarTxt_KAT/AES128\(VARTXT\)KAT.req \
                  AES_KAT/VarTxt_KAT/AES128\(VARTXT\)KAT.fax \
                  AES_KAT/VarTxt_KAT/AES128\(VARTXT\)KAT.rsp \
				#   LEA128\(CBC\)MOVS/LEA128\(CBC\)MMT.req \
                #   LEA128\(CBC\)MOVS/LEA128\(CBC\)MMT.fax \
                #   LEA128\(CBC\)MOVS/LEA128\(CBC\)MMT.rsp \
				#   LEA128\(CBC\)MOVS/LEA128\(CBC\)MCT.req \
                #   LEA128\(CBC\)MOVS/LEA128\(CBC\)MCT.fax \
                #   LEA128\(CBC\)MOVS/LEA128\(CBC\)MCT.rsp

clean:
	rm -f $(OBJS) $(TARGET) $(OBJDIR)/*.d
	@echo "Removing AESAVS files ..."
	rm -rf $(KATFILES_TO_DELETE)
	@echo "AESAVS files removal completed  successfully."

dir:
	@mkdir -p $(OBJDIR) $(BINDIR)

rebuild: clean all

run:
	(cd bin && ./a.out)

leak: 
	(cd bin && valgrind --leak-check=full --show-leak-kinds=all ./a.out)

comp:
	(cd bin && ./a.out > speed.txt)
	(cd bin && mv speed.txt ../view/)
	(cd view && python3 compare_opt.py)