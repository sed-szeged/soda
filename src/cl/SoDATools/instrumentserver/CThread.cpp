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

#include "CThread.h"

namespace soda {

CThread::CThread()
{
    pthread_attr_init(&m_threadAttr);
    pthread_attr_setdetachstate(&m_threadAttr, PTHREAD_CREATE_JOINABLE);
}

CThread::~CThread()
{
    pthread_attr_destroy(&m_threadAttr);
}

void* CThread::startThread(void *object)
{
    reinterpret_cast<CThread *>(object)->run();
    pthread_exit(NULL);
}

void CThread::start()
{
    pthread_create(&m_pthread, &m_threadAttr, &CThread::startThread, this);
}

void CThread::join()
{
    pthread_join(m_pthread, 0);
}

}
