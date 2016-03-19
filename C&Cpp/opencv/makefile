SRCS=test.c
TARGET=test.exe
CC=gcc
CXX=g++
RM=rm -f
ifeq ($(OS),Windows_NT) 
    ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
        # CPPFLAGS += -D AMD64
        CPPFLAGS += -D MS_WIN64
    endif
    ifeq ($(PROCESSOR_ARCHITECTURE),x86)
        CPPFLAGS += -D IA32
    endif
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        CCFLAGS += -D LINUX
    endif
    ifeq ($(UNAME_S),Darwin)
        CCFLAGS += -D OSX
    endif
    UNAME_P := $(shell uname -p)
    ifeq ($(UNAME_P),x86_64)
        CCFLAGS += -D AMD64
    endif
    ifneq ($(filter %86,$(UNAME_P)),)
        CCFLAGS += -D IA32
    endif
    ifneq ($(filter arm%,$(UNAME_P)),)
        CCFLAGS += -D ARM
    endif
    CPPFLAGS=$(shell root-config --cflags)
    LDFLAGS=$(shell root-config --ldflags)
    LDLIBS=$(shell root-config --libs)
endif
CPPFLAGS+=-I "D:\OpenCv\build\include" -I "D:\opencv\build\include\opencv" -I "D:\opencv\build\include\opencv2"
LDLIBS+=-L "D:\opencv\build\x64\vc12\bin" -L "D:\opencv\build\x64\vc12" -L "D:\opencv\build\x64\vc12\lib" -lopencv_calib3d2410 -lopencv_contrib2410 -lopencv_core2410 -lopencv_features2d2410 -lopencv_flann2410 -lopencv_gpu2410 -lopencv_highgui2410 -lopencv_imgproc2410 -lopencv_legacy2410 -lopencv_ml2410 -lopencv_objdetect2410 -lopencv_video2410
# CPPFLAGS+=-I "C:\opencv\build\include" -I "C:\opencv\build\include\opencv" -I "C:\opencv\build\include\opencv2"
# LDLIBS+=-L "C:\opencv\build\x64\vc12\bin" -L "C:\opencv\build\x64\vc12" -L "C:\opencv\build\x64\vc12\lib" -lopencv_world310 -lopencv_ffmpeg310_64 -lopencv_world310d
# LDFLAGS+=-shared  

OBJS=$(subst .c,.o,$(SRCS))

all : $(TARGET)

show :
	@echo processor_architechture = $(PROCESSOR_ARCHITECTURE)
	@echo operating system = ${OS}
	@echo cppflags = ${CPPFLAGS}
	@echo ldflags  = ${LDFLAGS}

release : $(TARGET)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LDLIBS) -DNDEBUG -O3 -s

debug : $(TARGET)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LDLIBS) -DEBUG -O0

$(TARGET) : $(OBJS) 
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LDLIBS) 

depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend ;

clean :
	$(RM) $(OBJS) 

dist-clean : clean
	$(RM) $(TARGET) .depend

include .depend

# from http://stackoverflow.com/questions/2481269/how-to-make-simple-c-makefile