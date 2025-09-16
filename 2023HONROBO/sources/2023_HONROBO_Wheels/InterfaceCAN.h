/*
 * Mbed-OS Microcontroller Library
 * Copyright (c) 2021 Embedded Planet
 * Copyright (c) 2021 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License
 */

#ifndef MBED_INTERFACE_CAN_H_
#define MBED_INTERFACE_CAN_H_

#include "hal/can_helper.h"

#include <cstring>

#include "platform/Callback.h"

namespace mbed {

#ifndef FEATURE_EXPERIMENTAL_API
// Forward declare CAN
class CAN;
#endif

/** \defgroup drivers-public-api-can CAN
 * \ingroup drivers-public-api
 */

/**
 * \defgroup drivers_CANMessage CANMessage class
 * \ingroup drivers-public-api-can
 * @{
 */

/** CANMessage class
 *
 * @note Synchronization level: Thread safe
 */


/** @}*/

namespace interface {

/* Having this as a struct allows interface::CAN and/or mbed::CAN to inherit the enums */
struct can {

    enum Mode {
        Reset = 0,
        Normal,
        Silent,
        LocalTest,
        GlobalTest,
        SilentTest
    };

    enum IrqType {
        RxIrq = 0,
        TxIrq,
        EwIrq,
        DoIrq,
        WuIrq,
        EpIrq,
        AlIrq,
        BeIrq,
        IdIrq,

        IrqCnt
    };

// Prevent slicing and user creation of base class.
protected:
    can() = default;
    ~can() = default;

public:

    /* Copy constructor and copy assignment operators will be deleted in subclasses as well */
    can(const can &) = delete;
    can &operator=(const can &) = delete;

};

#ifdef FEATURE_EXPERIMENTAL_API

// Pure virtual interface for CAN
struct CAN : public interface::can {

    virtual ~CAN() = default;
    virtual int frequency(int hz) = 0;
    virtual int write(CANMessage msg) = 0;
    virtual int read(CANMessage &msg, int handle = 0) = 0;
    virtual void reset() = 0;
    virtual void monitor(bool silent) = 0;
    virtual int mode(Mode mode) = 0;
    virtual int filter(unsigned int id, unsigned int mask, CANFormat format = CANAny, int handle = 0) = 0;
    virtual unsigned char rderror() = 0;
    virtual unsigned char tderror() = 0;
    virtual void attach(Callback<void()> func, IrqType type = IrqType::RxIrq) = 0;
};

#else
using CAN = ::mbed::CAN;
#endif

} // namespace interface

#if defined(FEATURE_EXPERIMENTAL_API) && !DEVICE_CAN
using CAN = interface::CAN;
#endif

} // namespace mbed

#endif /* MBED_INTERFACE_CAN_H_ */