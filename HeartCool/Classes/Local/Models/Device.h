//
//  Device.h
//  HeartCool
//
//  Created by Windom on 2017/3/26.
//  Copyright © 2017年 uteamtec. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "WDSingleton.h"

@interface Device : NSObject {
    @public
    NSString *deviceId;
    NSString *mac;
    NSString *name;
    int sps;
    int streamLen;
    NSString *key;
    NSString *keyPair;
}

WDSingletonH(Device)

+ (Device*)shared;

- (NSString*)toString;
- (void)save;

@end
