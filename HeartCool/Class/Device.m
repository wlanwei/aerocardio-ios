//
//  Device.m
//  HeartCool
//
//  Created by wd on 2017/3/26.
//  Copyright © 2017年 uteamtec. All rights reserved.
//

#import "Device.h"

#import "KeychainStore.h"

@implementation Device

WDSingletonM(Device)

+ (Device*)shared {
    Device* device = [self sharedDevice];
    if ([device->mac length] > 0) {
        [device read];
    }
    return device;
}

- (NSString*)toString {
    return [NSString stringWithFormat:@"%@|%@|%@|%d|%d|%@|%@",
            deviceId, mac, name, sps, streamLen, key, keyPair];
}

- (void)read {
    NSArray *array = [[KeychainStore read:@"Device"] componentsSeparatedByString:@"|"];
    if ([array count] >= 7) {
        deviceId = array[0];
        mac = array[1];
        name = array[2];
        sps = [array[3] intValue];
        streamLen = [array[4] intValue];
        key = array[5];
        keyPair = array[6];
    }
}

- (void)save {
    [KeychainStore save:@"Device" value:[self toString]];
}

@end
