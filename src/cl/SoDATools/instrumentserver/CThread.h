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

#ifndef CTHREAD_H
#define CTHREAD_H

#include <pthread.h>

namespace soda {

/**
 * @brief Abstract wrapper class of pthread.
 */
class CThread
{
private:
    /**
     * @brief Starts the thread by calling the run method.
     * @param  object The actual instance of a CThread object.
     * @return
     */
    static void* startThread(void *object);

public:
    /**
     * @brief Initializes the members that required for pthread.
     */
    CThread();

    /**
     * @brief Destroy pthread attributes.
     */
    virtual ~CThread();

    /**
     * @brief The job of the thread. This method will be executed in this thread.
     */
    virtual void run() = 0;

    /**
     * @brief Waits until the job is done.
     */
    void join();

    /**
     * @brief Starts the execution of the thread.
     */
    void start();


private:
    pthread_t m_pthread;
    pthread_attr_t m_threadAttr;
};

} /* namespace soda */

#endif /* CTHREAD_H */
