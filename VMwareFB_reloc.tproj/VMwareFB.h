/*
 * Copyright (c) 2002 by Atomic Object LLC
 * All rights reserved.
 *
 * VMwareFB.h -- interface for vmware framebuffer display driver
 *
 * Created by Bill Bereza 2001/01/17
 * $Id: VMwareFB.h,v 1.1 2002/02/14 22:15:35 bereza Exp $
 */

#ifndef VMWAREFB_H
#define VMWAREFB_H

#import <driverkit/IOFrameBufferDisplay.h>

#include "vmware.h"

#ifndef VM_BUILD_DATE
#define VM_BUILD_DATE __DATE__
#endif

#define VMLog IOLog

/* Parameter to which setIntValues: can be called. */
#define VMWAREUPDATE_PARAM "VMwareUpdateScreen"

// first memory range in device description is frame buffer
#define FB_MEMRANGE 0

// second memory range in device description is FIFO
#define FIFO_MEMRANGE 1

// acceleration key for IOConfigTable
#define VMACCELERATION_KEY "VMware Acceleration"

// acceleration key values (mapped to enum values below)
#define VMACCELERATION_NONE "None"
#define VMACCELERATION_DEFAULT "Default"
#define VMACCELERATION_CURSOR "Cursor"

// acceleration modes
enum vm_acceleration {
    NO_ACCELERATION = 0,
    DEFAULT_ACCELERATION,
    CURSOR_ACCELERATION
};

@interface VMwareFB: IOFrameBufferDisplay
{
    /* Index register. */
    uint16 indexReg;

    /* Value register. */
    uint16 valueReg;

    /* Command FIFO array. */
    CARD32 *fifo;

    /** FIFO size. */
    unsigned long fifoLength;

    /* VMware device capability read from device. */
    int vmwareCapability;

    /* Mode selected by user as chosen by selectMode:count:value: */
    int selectedMode;

    /* acceleration flag from config table */
    enum vm_acceleration acceleration;
}

+ (BOOL) probe: deviceDescription;

- initFromDeviceDescription: deviceDescription;
- (void) enterLinearMode;
- (void) revertToVGAMode;
// 2002-02-13 removed
//- free;
@end

@interface VMwareFB (Accel)
// methods which may accelerate display, or allow windowed vmware
- (IOReturn) getIntValues: (unsigned int *)array
             forParameter: (IOParameterName)parameter
                    count: (unsigned int *)count;

- (IOReturn) setIntValues: (unsigned int *)array
             forParameter: (IOParameterName)parameter
                    count: (unsigned int)count;

- (IOReturn) getCharValues: (unsigned char *)array
              forParameter: (IOParameterName)parameter
                     count: (unsigned int *)count;

- (IOReturn) setCharValues: (unsigned char *)array
              forParameter: (IOParameterName)parameter
                     count: (unsigned int) count;

@end

@interface VMwareFB (Utility)
/* Utility methods. */
- (void) logInfo;
- (BOOL) setPixelEncoding: (IOPixelEncoding)pixelEncoding
             bitsPerPixel: (int)bitsPerPixel
                  redMask: (int)redMask
                greenMask: (int)greenMask
                 blueMask: (int)blueMask;

@end

@interface VMwareFB (Registers)
/* VMware specific methods. */

+ (uint32) getVMwareSVGAIDAtIndexRegister: (uint16)indexRegister
                            valueRegister: (uint16)valueRegister;

+ (IOReturn)getIndexRegister: (uint16 *)indexRegister
               valueRegister: (uint16 *)valueRegister
       withDeviceDescription: deviceDescription;

- (CARD32) readRegister: (int)index;
- (void) writeRegister: (int)index value: (CARD32)value;
- (void) writeWordToFIFO: (CARD32)value;
- (void) updateDisplayX1: (int)x1
                      Y1: (int)y1
                      X2: (int)x2
                      Y2: (int)y2;
- (void) updateFullScreen;

@end

#endif	// VMWAREFB_H
