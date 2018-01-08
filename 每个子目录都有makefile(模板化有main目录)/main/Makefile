TOPDIR=./..

XX_TARGET=mainApp

LIBPATH=$(TOPDIR)/lib/
EXEPATH=$(TOPDIR)

CFLAGS=  -I$(TOPDIR)/include/ -I$(TOPDIR)/sender/ -I$(TOPDIR)/receiver/\
		-I$(TOPDIR)/responser/ -I$(TOPDIR)/logout/ 
LDFLAGS= -L$(LIBPATH) -lSender -lResponser -lReceiver -lLogout -lpthread -lrt

include $(TOPDIR)/include/Makefile.base