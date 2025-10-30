# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gpatrici <gpatrici@student.42lisboa.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/30 23:20:00 by gpatrici          #+#    #+#              #
#    Updated: 2025/10/30 23:12:14 by gpatrici         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -O3
LDFLAGS = -lgmp

# Targets
TARGET_BASIC = fib
TARGET_BIGINT = fib_bigint

# Source files
SRC_BASIC = main.c
SRC_BIGINT = main_bigint.c

# Default target - build both versions
all: $(TARGET_BASIC) $(TARGET_BIGINT)

# Build basic version (limited to fib(92))
$(TARGET_BASIC): $(SRC_BASIC)
	$(CC) $(CFLAGS) -o $(TARGET_BASIC) $(SRC_BASIC)

# Build big integer version (arbitrary precision with GMP)
$(TARGET_BIGINT): $(SRC_BIGINT)
	$(CC) $(CFLAGS) -o $(TARGET_BIGINT) $(SRC_BIGINT) $(LDFLAGS)

# Clean up compiled binaries
clean:
	rm -f $(TARGET_BASIC) $(TARGET_BIGINT)

# Rebuild everything
re: clean all

# Run the basic version
run: $(TARGET_BASIC)
	./$(TARGET_BASIC)

# Run the big integer version
run_bigint: $(TARGET_BIGINT)
	./$(TARGET_BIGINT)

# Run both versions
run_all: run run_bigint

.PHONY: all clean re run run_bigint run_all
