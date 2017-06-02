//
//  Device.h
//  HeartCool
//
//  Created by wd on 2017/3/26.
//  Copyright © 2017年 uteamtec. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "WDSingleton.h"

static const int MODEL_20_1 = 1;
static const int MODEL_20_3 = 2;
static const int MODEL_20_3_HI = 3;
static const int MODEL_20_2_HI = 4;
static const int STATE_ON = 1;
static const int STATE_OFF = 2;
static const int STATE_CONNECTED = 3;
static const int STATE_CONNECTING = 4;

@interface Device : NSObject {
    @public
    NSString *deviceId;
    NSString *mac;
    NSString *name;
    int model;
    int sps;
    int streamLen;
    NSString *key;
    NSString *keyPair;
}

WDSingletonH(Device)

+ (Device*)shared;

- (NSString*)toString;
- (void)save;

- (int)getModel;

@end
