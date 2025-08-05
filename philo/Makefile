# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/27 15:56:11 by paalexan          #+#    #+#              #
#    Updated: 2025/08/05 15:25:18 by paalexan         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
#                                                                              #
#                                 Project Name                                 #
#                                                                              #
# **************************************************************************** #

NAME		= philo

# **************************************************************************** #
#                                                                              #
#                                 Directories                                  #
#                                                                              #
# **************************************************************************** #

SRC_DIR		= src
OBJ_DIR		= obj

# **************************************************************************** #
#                                                                              #
#                                 Source Files                                 #
#                                                                              #
# **************************************************************************** #

SRC			= $(SRC_DIR)/main.c \
			  $(SRC_DIR)/init.c \
			  $(SRC_DIR)/parser.c \
			  $(SRC_DIR)/threads.c \
			  $(SRC_DIR)/routines.c \
			  $(SRC_DIR)/monitor.c \
			  $(SRC_DIR)/utils.c \
			  $(SRC_DIR)/cleanup.c

# **************************************************************************** #
#                                                                              #
#                                 Object Files                                 #
#                                                                              #
# **************************************************************************** #

OBJS		= $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# **************************************************************************** #
#                                                                              #
#                               Compiler & Flags                               #
#                                                                              #
# **************************************************************************** #

CC			= cc
CFLAGS		= -Wall -Wextra -Werror -pthread
DFLAGS		= -g

^C==138472== 
==138472== Process terminating with default action of signal 2 (SIGINT)
==138472==    at 0x490E117: __futex_abstimed_wait_common64 (futex-internal.c:57)
==138472==    by 0x490E117: __futex_abstimed_wait_common (futex-internal.c:87)
==138472==    by 0x490E117: __futex_abstimed_wait_cancelable64 (futex-internal.c:139)
==138472==    by 0x4913623: __pthread_clockjoin_ex (pthread_join_common.c:105)
==138472==    by 0x4853A6C: pthread_join_WRK (hg_intercepts.c:578)
==138472==    by 0x4858D26: pthread_join (hg_intercepts.c:597)
==138472==    by 0x4013D0: run_simulation (main.c:38)
==138472==    by 0x40129C: main (main.c:66)
==138472== ---Thread-Announcement------------------------------------------
==138472== 
==138472== Thread #3 was created
==138472==    at 0x49A29F3: clone (clone.S:76)
==138472==    by 0x49A38EE: __clone_internal (clone-internal.c:83)
==138472==    by 0x49116D8: create_thread (pthread_create.c:295)
==138472==    by 0x49121FF: pthread_create@@GLIBC_2.34 (pthread_create.c:828)
==138472==    by 0x485741C: pthread_create_WRK (hg_intercepts.c:445)
==138472==    by 0x4858D17: pthread_create@* (hg_intercepts.c:478)
==138472==    by 0x401B94: create_philosopher_threads (threads.c:23)
==138472==    by 0x401364: run_simulation (main.c:28)
==138472==    by 0x40129C: main (main.c:66)
==138472== 
==138472== ----------------------------------------------------------------
==138472== 
==138472== Thread #3: Exiting thread still holds 2 locks
==138472==    at 0x49627F8: clock_nanosleep@@GLIBC_2.17 (clock_nanosleep.c:78)
==138472==    by 0x4967676: nanosleep (nanosleep.c:25)
==138472==    by 0x4998F2E: usleep (usleep.c:31)
==138472==    by 0x4024DB: precise_sleep (utils.c:69)
==138472==    by 0x401F9F: helper_eat (routines.c:23)
==138472==    by 0x401EC8: eat (routines.c:52)
==138472==    by 0x401D73: philosopher_routine (routines.c:84)
==138472==    by 0x4857614: mythread_wrapper (hg_intercepts.c:406)
==138472==    by 0x4911AC2: start_thread (pthread_create.c:442)
==138472==    by 0x49A2A03: clone (clone.S:100)
==138472== 
==138472== 
# **************************************************************************** #
#                                                                              #
#                                 Build Rules                                  #
#                                                                              #
# **************************************************************************** #

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $(DFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(DFLAGS) $(OBJS) -o $(NAME)
	@echo "$(PREFIX) $(B)Executable$(D) compiled $(SUCCESS)."

# **************************************************************************** #
#                                                                              #
#                                Cleaning Rules                                #
#                                                                              #
# **************************************************************************** #

clean:
	@if [ -d "$(OBJ_DIR)" ] || [ -f "$(NAME)" ]; then \
		rm -rf $(OBJ_DIR) $(NAME); \
		echo "$(PREFIX) Objects & Executable removed $(SUCCESS)."; \
	else \
		echo "$(PREFIX) Nothing to clean."; \
	fi

fclean: clean

re: fclean all

# **************************************************************************** #
#                                                                              #
#                              Colors & Messages                               #
#                                                                              #
# **************************************************************************** #

B  			= $(shell tput bold)
RED			= $(shell tput setaf 1)
GRN			= $(shell tput setaf 2)
YEL			= $(shell tput setaf 3)
BLU			= $(shell tput setaf 4)
MAG			= $(shell tput setaf 5)
D 			= $(shell tput sgr0)

PREFIX		= $(B)$(MAG)[Philosophers]$(D)
SUCCESS		= $(GRN)successfully$(D)
FAILURE		= $(GRN)failed$(D)
