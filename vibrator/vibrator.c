/*
 * Copyright (C) 2008 The Android Open Source Project
 * Copyright (C) 2012 ALin Jerpelea <jerpelea@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <hardware_legacy/vibrator.h>
#include "qemu.h"

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include <time.h>

#include "vibrator.h"

#ifdef MSM_RPC_VIBRATOR
#define THE_DEVICE "/sys/devices/virtual/timed_output/vibrator/enable_vibrator"
#else
#define THE_DEVICE "/sys/class/vibrator/pmic-vib/control"
#endif

int vibrator_exists()
{
    int fd;

    fd = open(THE_DEVICE, O_RDWR);
    if(fd < 0)
        return 0;
    close(fd);
    return 1;
}

int sendit(int timeout_ms)
{
    int nwr, ret, fd;
    char value[20];


    fd = open(THE_DEVICE, O_RDWR);
    if(fd < 0)
        return errno;

#ifdef MSM_RPC_VIBRATOR
    nwr = sprintf(value, "%d\n", timeout_ms);
    ret = write(fd, value, nwr);

    close(fd);

    return (ret == nwr) ? 0 : -1;
#else 
    char buffer[20];


    if (timeout_ms > 0){

        nwr = snprintf(buffer, sizeof(buffer), "%s\n", "ON");
        ret = write (fd, buffer, nwr);
        usleep (timeout_ms*1000);
    }
    nwr = snprintf(buffer, sizeof(buffer), "%s\n", "0");
    ret = write (fd, buffer, nwr);

    close(fd);

    return (ret == nwr) ? 0 : -1;
#endif
}
