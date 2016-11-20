SRC=./
BUSI=$(SRC)/busi/
EVENT=$(SRC)/event/
EVENT_MODULE=$(EVENT)/epoll/
HANDLE=$(SRC)/handle/
MEM=$(SRC)/mem/
SERVER=$(SRC)/server/
COMMON=$(SRC)/common/
CONNECT=$(SRC)/connect/
HTTP=$(SRC)/http/
TOOLS=$(SRC)/tools/
OS=$(SRC)/os/
IO=$(SRC)/io/

BUSISRC:=$(wildcard $(BUSI)*.c)
EVENTSRC:=$(wildcard $(EVENT)*.c)
EVENT_MODULESRC:=$(wildcard $(EVENT_MODULE)*.c)
HANDLESRC:=$(wildcard $(HANDLE)*.c)
MEMSRC:=$(wildcard $(MEM)*.c)
SERVERSRC:=$(wildcard $(SERVER)*.c)
COMMONSRC:=$(wildcard $(COMMON)*.c)
CONNECTSRC:=$(wildcard $(CONNECT)*.c)
HTTPSRC:=$(wildcard $(HTTP)*.c)
TOOLSSRC:=$(wildcard $(TOOLS)*.c)
OSSRC:=$(wildcard $(OS)*.c)
IOSRC:=$(wildcard $(IO)*.c)

BUSIOBJ:=$(patsubst %.c,%.o,$(BUSISRC))
EVENTOBJ:=$(patsubst %.c,%.o,$(EVENTSRC))
EVENT_MODULEOBJ:=$(patsubst %.c,%.o,$(EVENT_MODULESRC))
HANDLEOBJ:=$(patsubst %.c,%.o,$(HANDLESRC))
MEMOBJ:=$(patsubst %.c,%.o,$(MEMSRC))
SERVEROBJ:=$(patsubst %.c,%.o,$(SERVERSRC))
COMMONOBJ:=$(patsubst %.c,%.o,$(COMMONSRC))
CONNECTOBJ:=$(patsubst %.c,%.o,$(CONNECTSRC))
HTTPOBJ:=$(patsubst %.c,%.o,$(HTTPSRC))
TOOLSOBJ:=$(patsubst %.c,%.o,$(TOOLSSRC))
OSOBJ:=$(patsubst %.c,%.o,$(OSSRC))
IOOBJ:=$(patsubst %.c,%.o,$(IOSRC))

BUSIDEP:=$(patsubst %.c,%.d,$(BUSISRC))
EVENTDEP:=$(patsubst %.c,%.d,$(EVENTSRC))
EVENT_MODULEDEP:=$(patsubst %.c,%.d,$(EVENT_MODULESRC))
HANDLEDEP:=$(patsubst %.c,%.d,$(HANDLESRC))
MEMDEP:=$(patsubst %.c,%.d,$(MEMSRC))
SERVERDEP:=$(patsubst %.c,%.d,$(SERVERSRC))
COMMONDEP:=$(patsubst %.c,%.d,$(COMMONSRC))
CONNNECTDEP:=$(patsubst %.c,%.d,$(CONNECTSRC))
HTTPDEP:=$(patsubst %.c,%.d,$(HTTPSRC))
TOOLSDEP:=$(patsubst %.c,%.d,$(TOOLSSRC))
OSDEP:=$(patsubst %.c,%.d,$(OSSRC))
IODEP:=$(patsubst %.c,%.d,$(IOSRC))

SRCC=$(BUSISRC) $(EVENTSRC) $(HANDLESRC) $(MEMSRC) $(SERVERSRC) $(COMMONSRC) $(CONNECTSRC) $(HTTPSRC) $(TOOLSSRC) $(OSSRC) $(IOSRC) $(EVENT_MODULESRC)
OBJS=$(BUSIOBJ) $(EVENTOBJ) $(HANDLEOBJ) $(MEMOBJ) $(SERVEROBJ) $(COMMONOBJ) $(CONNECTOBJ) $(HTTPOBJ) $(TOOLSOBJ) $(OSOBJ) $(IOOBJ) $(EVENT_MODULEOBJ)

INC=-I$(COMMON) -I$(BUSI) -I$(EVENT) -I$(HANDLE) -I$(MEM) -I$(CONNECT) -I$(HTTP) -I$(TOOLS) -I$(OS) -I$(IO) -I$(EVENT_MODULE)
LOG=-D_FILE_OFFSET_BITS=64 
LIB=-L/usr/lib64 -lpthread


CFLAGS:=-w -g $(INC) $(LOG)
CC=gcc

APPNAME=$(SRC)/build/yumeiz


all:$(APPNAME)

$(APPNAME):$(OBJS)
	$(CC) $^ $(CFLAGS) -o $@   $(LIB)



.PHONY:clean
clean:
	rm $(OBJS) $(APPNAME)



rebuild:clean all
