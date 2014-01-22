/*
 * Copyright (C): 2013-2014 Department of Software Engineering, University of Szeged
 *
 * Authors: David Tengeri <dtengeri@inf.u-szeged.hu>
 *
 * This file is part of SoDA.
 *
 *  SoDA is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  SoDA is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with SoDA.  If not, see <http://www.gnu.org/licenses/>.
 */


/**
  * @file This file contains the code that can be used to instrument programs.
  *       This code connects and sends the informations to the
  *       instrument server during the execution of the program.
  *       You have to compile this source into your project and you have to use
  *       -finstrument-functions option of gcc.
  *       The instrumentation only works if the INSTRUMENT environmental
  *       variable is set.
  *       You have to define the actual test inside the TEST_NAME environmental
  *       variable.
  */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <dlfcn.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define INSTRUMENT_TEST_MSG           1
#define INSTRUMENT_FUNCTION_ENTER_MSG 2
#define INSTRUMENT_FUNCTION_EXIT_MSG  3
#define INSTRUMENT_CLOSE_MSG          4

#ifdef __cplusplus
extern "C" {
#endif

void main_constructor(void) __attribute__ ((no_instrument_function, constructor));
void main_destructor(void) __attribute__ ((no_instrument_function, destructor));

/* Function prototypes with attributes */
void __cyg_profile_func_enter(void *, void *) __attribute__ ((no_instrument_function));
void __cyg_profile_func_exit(void *, void *) __attribute__ ((no_instrument_function));

void send_test_message(char *test) __attribute__ ((no_instrument_function));
void send_close_message() __attribute__ ((no_instrument_function));
void send_func_message(char type, const char * binaryPath, int *address) __attribute__ ((no_instrument_function));

#ifdef __cplusplus
}
#endif

pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;

const char* const socketName = "/tmp/instrument-server";
int socket_fd = -1;

/**
 * @brief Sends the actual test name to the instrument server.
 * @param test The name of the test.
 */
void send_test_message(char *test)
{
    if (socket_fd != -1) {
        char type = INSTRUMENT_TEST_MSG;
        size_t len  = strlen(test) + 1;
        write(socket_fd, &type, sizeof(type));
        write(socket_fd, &len, sizeof(len));
        write(socket_fd, test, len);
    }
}

/**
 * @brief Sends a message to close the instrument session and closes
 *        the connection.
 */
void send_close_message()
{
    if (socket_fd != -1) {
        char type = INSTRUMENT_CLOSE_MSG;
        write(socket_fd, &type, sizeof(type));
        close(socket_fd);
    }
}

/**
 * @brief Sends a message with the details of a function.
 * @param type the type of the message. It could be an entry or an exit message.
 * @param binaryPath The path to the binary that contains tha function.
 * @param address The address of the function.
 */
void send_func_message(char type, const char * binaryPath, int *address)
{
    if (socket_fd != -1) {
        size_t len = strlen(binaryPath) + 1;
        write(socket_fd, &type, sizeof(type));
        write(socket_fd, &len, sizeof(len));
        write(socket_fd, binaryPath, len);
        write(socket_fd, &address, sizeof(int));
    }
}

/**
 * @brief Runs when the program starts and ff the INSTRUMENT environmental
 *        variable is available.
 *        Gets the actual test name from the TEST_NAME environmental variable.
 *        Connects to the instrument server and sends a test message.
 */
void main_constructor(void)
{
    if (getenv("INSTRUMENT") != NULL) {
        /* Open instrument file */
        pthread_mutex_lock(&log_mutex);
        unsetenv("INSTRUMENT");

        char *test = getenv("TEST_NAME");
        if (test == NULL) {
            fprintf(stderr, "Test is NULL!!\n");
            return;
        }

        struct sockaddr_un name;
        /* Create the socket. */
        socket_fd = socket (PF_LOCAL, SOCK_STREAM, 0);
        if (socket < 0) {
            fprintf(stderr, "Can not create socket.\n");
            return;
        }
        /* Store the server’s name in the socket address. */
        name.sun_family = AF_LOCAL;
        strcpy (name.sun_path, socketName);
        /* Connect the socket. */
        int result = connect (socket_fd, (struct sockaddr*)&name, SUN_LEN(&name));
        if (result < 0) {
            fprintf(stderr, "Can not connect to server.\n");
            socket_fd = -1;
            return;
        }
        send_test_message(test);

        setenv("INSTRUMENT", "1", 1);
        pthread_mutex_unlock(&log_mutex);
    }
}

/**
 * @brief Runs at the end ot the program execution. Sends a close message to
 *        the instrument server.
 */
void main_destructor(void)
{
    if (getenv("INSTRUMENT") != NULL) {
        // Close file and clear test name.
        pthread_mutex_lock(&log_mutex);
        unsetenv("INSTRUMENT");

        send_close_message();

        setenv("INSTRUMENT", "1", 1);
        pthread_mutex_unlock(&log_mutex);
    }
}

/**
 * @brief Runs before every function call. Sends the details of the actual
 *        function to the instrument server.
 * @param func The address of the function.
 * @param callsite The address from the function was called.
 */
void __cyg_profile_func_enter(void *func, void *callsite)
{
    if (getenv("INSTRUMENT") != NULL) {
        pthread_mutex_lock(&log_mutex);
        unsetenv("INSTRUMENT");
        Dl_info info;
        if (dladdr(func, &info)) {
            if (strstr(info.dli_fname, ".so")) {
                send_func_message(INSTRUMENT_FUNCTION_ENTER_MSG, info.dli_fname, (int *)((uint64_t) func - (uint64_t) info.dli_fbase));
            } else {
                send_func_message(INSTRUMENT_FUNCTION_ENTER_MSG, info.dli_fname, (int *)func);
            }
        }
        setenv("INSTRUMENT", "1", 1);
        pthread_mutex_unlock(&log_mutex);
    }
	
}

/**
 * @brief Runs after every function call. Sends the details of the actual
 *        function to the instrument server.
 * @param func The address of the function.
 * @param callsite The address from the function was called.
 */
void __cyg_profile_func_exit(void *func, void *callsite)
{
    if (getenv("INSTRUMENT") != NULL) {
        pthread_mutex_lock(&log_mutex);
        unsetenv("INSTRUMENT");
        Dl_info info;
        if (dladdr(func, &info)) {
            if (strstr(info.dli_fname, ".so")) {
                send_func_message(INSTRUMENT_FUNCTION_EXIT_MSG, info.dli_fname, (int *)((uint64_t) func - (uint64_t) info.dli_fbase));
            } else {
                send_func_message(INSTRUMENT_FUNCTION_EXIT_MSG, info.dli_fname, (int *)func);
            }
        }
        setenv("INSTRUMENT", "1", 1);
        pthread_mutex_unlock(&log_mutex);
    }
}
