CC = g++
CFLAGS = -Wall -g
CPPFLAGS = -c

SRCDIR = src
OBJDIR = obj
BLDDIR = bld

OBJS = $(OBJDIR)/ImageType.o $(OBJDIR)/ImageProcessing.o $(OBJDIR)/Histogram.o
IMAGE =  $(SRCDIR)/ImageType.h $(SRCDIR)/ImageType.cpp
PROCESSING = $(SRCDIR)/ImageProcessing.h $(SRCDIR)/ImageProcessing.cpp
HISTOGRAM = $(SRCDIR)/Histogram.h $(SRCDIR)/Histogram.cpp

default: $(BLDDIR)/Controller

$(BLDDIR)/Controller:  $(OBJS) $(SRCDIR)/Controller.cpp
	$(CC) -o $(BLDDIR)/Controller $(CFLAGS) $(OBJS) $(SRCDIR)/Controller.cpp

$(OBJDIR)/ImageType.o: $(IMAGE)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(SRCDIR)/ImageType.cpp -o $(OBJDIR)/ImageType.o

$(OBJDIR)/ImageProcessing.o: $(PROCESSING)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(SRCDIR)/ImageProcessing.cpp -o $(OBJDIR)/ImageProcessing.o

$(OBJDIR)/Histogram.o: $(HISTOGRAM)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(SRCDIR)/Histogram.cpp -o $(OBJDIR)/Histogram.o

clExe:
	rm -f $(BLDDIR)/Controller

clean:
	rm -f $(OBJS)