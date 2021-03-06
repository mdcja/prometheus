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
MATH=math.o math_test
ATTRIBUTES=attributes.o attributes_test
STACK=stack_array.o stack_linked_list.o stack_array_test stack_linked_list_test
QUEUE=queue_array.o queue_linked_list.o queue_circular_array.o queue_array_test queue_linked_list_test queue_circular_array_test
DEQUEUE=dequeue_array.o dequeue_circular_array.o dequeue_doubly_linked_list.o dequeue_array_test dequeue_circular_array_test dequeue_doubly_linked_list_test
HASH_TABLE=hash_table_open_addressing.o hash_table_seperate_chaining.o hash_table_open_addressing_test hash_table_seperate_chaining_test

# Make project
.PHONY: all
all: $(ERROR) $(MATH) $(ATTRIBUTES) $(STACK) $(QUEUE) $(DEQUEUE) $(HASH_TABLE)

###########################################################
# Build Additional Libraries
###########################################################

# Build error
error.o: error.c include/error.h
	$(CC) $(CFLAGS) -c $<

# Build math
math.o: math.c include/math.h include/error.h
	$(CC) $(CFLAGS) -c $<

# Build attributes
attributes.o: attributes.c include/error.h include/attributes.h
	$(CC) $(CFLAGS) -c $<

###########################################################
# Build ADT's
###########################################################

# Build stack
stack_array.o: stack_array.c include/stack.h include/attributes.h include/error.h
	$(CC) $(CFLAGS) -c $<

stack_linked_list.o: stack_linked_list.c include/stack.h include/attributes.h include/error.h
	$(CC) $(CFLAGS) -c $<

# Build queue
queue_array.o: queue_array.c include/queue.h include/attributes.h include/error.h
	$(CC) $(CFLAGS) -c $<

queue_linked_list.o: queue_linked_list.c include/queue.h include/attributes.h include/error.h
	$(CC) $(CFLAGS) -c $<

queue_circular_array.o: queue_circular_array.c include/queue.h include/attributes.h include/error.h
	$(CC) $(CFLAGS) -c $<

# Build dequeue
dequeue_array.o: dequeue_array.c include/dequeue.h include/attributes.h include/error.h
	$(CC) $(CFLAGS) -c $<

dequeue_circular_array.o: dequeue_circular_array.c include/dequeue.h include/attributes.h include/error.h
	$(CC) $(CFLAGS) -c $<

dequeue_doubly_linked_list.o: dequeue_doubly_linked_list.c include/dequeue.h include/attributes.h include/error.h
	$(CC) $(CFLAGS) -c $<

# Build hash table
hash_table_open_addressing.o: hash_table_open_addressing.c include/hash_table.h include/attributes.h include/error.h
	$(CC) $(CFLAGS) -c $<

hash_table_seperate_chaining.o: hash_table_seperate_chaining.c include/hash_table.h include/attributes.h include/error.h
	$(CC) $(CFLAGS) -c $<

###########################################################
# Build Tests
###########################################################

# Error tests
error_test.o: tests/error_test.c include/error.h
	$(CC) $(CFLAGS) -c $<

error_test: error_test.o error.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Math tests
math_test.o: tests/math_test.c include/math.h include/error.h
	$(CC) $(CFLAGS) -c $<

math_test: LDFLAGS+= -lm
math_test: math_test.o math.o error.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Attributes tests
attributes_test.o: tests/attributes_test.c include/attributes.h include/error.h
	$(CC) $(CFLAGS) -c $<
	
attributes_test: attributes_test.o attributes.o error.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Stack tests
stack_test.o: tests/stack_test.c include/stack.h include/attributes.h include/error.h
	$(CC) $(CFLAGS) -c $<

stack_array_test: stack_test.o stack_array.o attributes.o error.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

stack_linked_list_test: stack_test.o stack_linked_list.o attributes.o error.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Queue tests
queue_test.o: tests/queue_test.c include/queue.h include/attributes.h include/error.h
	$(CC) $(CFLAGS) -c $<

queue_array_test: queue_test.o queue_array.o attributes.o error.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

queue_linked_list_test: queue_test.o queue_linked_list.o attributes.o error.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

queue_circular_array_test: queue_test.o queue_circular_array.o attributes.o error.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Dequeue tests
dequeue_test.o: tests/dequeue_test.c include/dequeue.h include/attributes.h include/error.h
	$(CC) $(CFLAGS) -c $<

dequeue_array_test: dequeue_test.o dequeue_array.o attributes.o error.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

dequeue_circular_array_test: dequeue_test.o dequeue_circular_array.o attributes.o error.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

dequeue_doubly_linked_list_test: dequeue_test.o dequeue_doubly_linked_list.o attributes.o error.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Hash table tests
hash_table_test.o: tests/hash_table_test.c include/hash_table.h include/attributes.h include/error.h
	$(CC) $(CFLAGS) -c $<

hash_table_open_addressing_test: hash_table_test.o hash_table_open_addressing.o attributes.o error.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

hash_table_seperate_chaining_test: hash_table_test.o hash_table_seperate_chaining.o attributes.o error.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

###########################################################
# Run unit tests
###########################################################
.PHONY: check
check: $(ERROR) $(ATTRIBUTES) $(STACK)
	-./error_test
	-./math_test
	-./attributes_test
	-./stack_array_test
	-./stack_linked_list_test
	-./queue_array_test
	-./queue_linked_list_test
	-./queue_circular_array_test
	-./dequeue_array_test
	-./dequeue_circular_array_test
	-./dequeue_doubly_linked_list_test
	-./hash_table_open_addressing_test
	-./hash_table_seperate_chaining_test

###########################################################
# Clean project
###########################################################
.PHONY: clean
clean:
	-$(RM) *.o
	-$(RM) $(ERROR)
	-$(RM) $(MATH)
	-$(RM) $(ATTRIBUTES)
	-$(RM) $(STACK)
	-$(RM) $(QUEUE)
	-$(RM) $(DEQUEUE)
	-$(RM) $(HASH_TABLE)
