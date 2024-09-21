CC = g++
CFLAGS = -c -Wall -g

SRCDIR = src
OBJDIR = obj
BLDDIR = bld

OBJS = $(OBJDIR)/ImageType.o $(OBJDIR)/ImageProcessing.o
IMAGE =  $(SRCDIR)/ImageType.h $(SRCDIR)/ImageType.cpp
PROCESSING = $(SRCDIR)/ImageProcessing.h $(SRCDIR)/ImageProcessing.cpp

$(BLDDIR)/Controller =  $(OBJS) $(SRCDIR)/Controller.cpp
	$(CC) -o Controller $(CFLAGS) $(OBJS) $(SRCDIR)/Controller.cpp
$(OBJDIR)/ImageType.o: $(IMAGE)
	$(CC) -c $(CFLAGS) $(SRCDIR)/ImageType.cpp
$(OBJDIR)/ImageProcessing.o: $(PROCESSING)
	$(CC) -c $(CFLAGS) $(SRCDIR)/ImageProcessing.cpp
clean:
	rm -f $(OBJS)