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

#ifndef CTRACELOGGER_H
#define CTRACELOGGER_H

#include <map>
#include <set>

#include "data/CTraceData.h"

namespace soda {

/**
 * @brief A trace logger thread. Accepts the messages from the instrumented code
 *        through a socket, translates the code function address to its name and
 *        stores the information into the coverage matrix.
 */
class CTraceLogger
{
public:
    /**
     * @brief Creates a new CTraceLogger instance. The logger will listen on
     *        the socket and saves the informations in the data.
     * @param socket  The socket identifier.
     * @param data  The CTraceData instance.
     */
    CTraceLogger(int socket, CTraceData *data);
    CTraceLogger(const CTraceLogger &obj);
    ~CTraceLogger();

    /**
     * @brief Processes the messages coming through the socket.
     */
    void operator()();

    /**
     * @brief Returns the socket identifier.
     * @return
     */
    int getSocket();

private:
    /**
     * @brief Handles the test message.
     */
    void handleTestMessage();

    /**
     * @brief Handles the function enter message.
     */
    void handleFunctionEnterMessage();

    /**
     * @brief Handles the function exit message.
     */
    void handleFunctionExitMessage();

    /**
     * @brief Reads the information of a function message from the socket and
     *        returns the name of the function.
     * @return The name of the function.
     */
    String handleFunctionMessage();

    /**
     * @brief Translates the addres to a function name.
     * @param binaryPath  The path of the binary which contains the function
     *                   at address.
     * @param address  The address of the function.
     * @return The name of the function.
     */
    String translateAddressToFunction(const String &binaryPath, const int address);

private:
    /**
     * @brief Socket identifer.
     */
    int m_socket;

    /**
     * @brief Name of the test case. The trace information belong to this test.
     */
    String m_testcaseName;

    /**
     * @brief Stores the trace informations.
     */
    CTraceData *m_data;

    /**
     * @brief Types of the messages.
     */
    enum MessageType {
        INSTRUMENT_TEST_MSG = 1,
        INSTRUMENT_FUNCTION_ENTER_MSG,
        INSTRUMENT_FUNCTION_EXIT_MSG,
        INSTRUMENT_CLOSE_MSG
    };
};

} /* namespace soda */



#endif /* CTRACELOGGER_H */
