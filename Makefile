CC=gcc
CFLAGS=-Wall -std=c99 -g -O2 -I./include
LDFLAGS=
OBJDIR=./obj
BINDIR=./bin
SRCDIR=./src
TESTDIR=./tests
INCDIR=./include

OBJS=$(OBJDIR)/aes_core.o $(OBJDIR)/key_schedule.o \
	$(OBJDIR)/key_schedule_test.o \
	$(OBJDIR)/main.o

TARGET=$(BINDIR)/a.out

.PHONY: all clean dir rebuild

all: dir $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJDIR)/main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o $@
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@
$(OBJDIR)/%.o: $(TESTDIR)/%.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

-include $(OBJS:.o=.d)



$(OBJDIR)/aes_core.o: $(SRCDIR)/aes_core.c $(INCDIR)/aes.h
$(OBJDIR)/key_schedule.o: $(SRCDIR)/key_schedule.c $(INCDIR)/aes.h
# $(OBJDIR)/aes_utils.o: $(SRCDIR)/aes_utils.c $(INCDIR)/aes.h

$(OBJDIR)/key_schedule_test.o: $(TESTDIR)/key_schedule_test.c $(INCDIR)/aes.h

clean:
	rm -f $(OBJS) $(TARGET) $(OBJDIR)/*.d 

dir:
	@mkdir -p $(OBJDIR) $(BINDIR)

rebuild: clean all

leak: 
	(cd bin && valgrind --leak-check=full --show-leak-kinds=all ./a.out)