//
//  FeMessage.h
//  HeartCool
//
//  Created by wd on 2017/4/5.
//  Copyright © 2017年 uteamtec. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "ACReuseQueue.h"
#import "User.h"
#import "Device.h"

static int const TYPE_STREAM_ECG_1 = 0; //16 bit ecg
static int const TYPE_STREAM_ECG_3 = 1; //16 bit ecg
static int const TYPE_STREAM_ECG_12 = 2; //16 bit ecg
static int const TYPE_STREAM_ECG_2 = 3; //16 bit ecg
static int const TYPE_STATUS = 100;
static int const TYPE_ANALYSIS_PHSIO = 150;
static int const TYPE_REGISTER = 101;
static int const TYPE_REGISTER_ACK = 230; // wd: 寄存器ACK
static int const TYPE_PULSE = 231; // wd: 脉冲
static int const TYPE_RESET = 240;
static int const TYPE_USERINPUT = 102;
static int const TYPE_NULL = 0xff;

@interface FeMessage : NSObject <ACReusableObject> {
    @private
    int type;
    NSData *body;
}

@property (nonatomic, copy) NSString *reuseIdentifier;

+ (int)getBodyLength:(int)type length:(int)streamLength resolution:(int)resolution;

- (int)getType;
- (NSData*)getBody;

- (id)extractInitEcg:(User*)user time:(long)timeInit;

- (id)extractEcg:(User*)user;

- (id)extractMark:(long)startTime;

- (Device*)extractDevice;

+ (FeMessage*)createRegAckMsg:(Device*)device;
+ (FeMessage*)createPulseMsg;
+ (FeMessage*)createResetMsg;
+ (FeMessage*)createMarkMsg;

@end
