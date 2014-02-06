###########################################################
# @brief A Makefile for the Prometheus Toolkit 
# @author Julian Martinez del Campo
#
###########################################################

CC=gcc
CFLAGS=-Wall -g -pedantic -std=c99 -I include
LDFLAGS= -lcheck
vpath %.c tests
vpath %.c src
vpath %.h include

ERROR=error.o error_test
ATTRIBUTES=attributes.o attributes_test

# Make project
.PHONY: all
all: $(ERROR) $(ATTRIBUTES)

###########################################################
# Build Additional Libraries
###########################################################

# Build error
error.o: error.c include/error.h
	$(CC) $(CFLAGS) -c $<

# Build attributes
attributes.o: attributes.c include/error.h include/attributes.h
	$(CC) $(CFLAGS) -c $<

###########################################################
# Build Tests
###########################################################

# Error tests
error_test.o: tests/error_test.c include/error.h
	$(CC) $(CFLAGS) -c $<

error_test: error_test.o error.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Attributes tests
attributes_test.o: tests/attributes_test.c include/attributes.h include/error.h
	$(CC) $(CFLAGS) -c $<
	
attributes_test: attributes_test.o attributes.o error.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

###########################################################
# Run unit tests
###########################################################
.PHONY: check
check:

###########################################################
# Clean project
###########################################################
.PHONY: clean
clean:
	-$(RM) *.o
	-$(RM) $(ERROR)
	-$(RM) $(ATTRIBUTES)
