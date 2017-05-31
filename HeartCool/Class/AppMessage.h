//
//  AppMessage.h
//  HeartCool
//
//  Created by wd on 2017/4/5.
//  Copyright © 2017年 uteamtec. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "ACReuseQueue.h"
#import "Device.h"

static int const TYPE_LOGIN = 1; //user login msg
static int const TYPE_LOGIN_ACK = 2; //user login msg ack
static int const TYPE_REG = 3; //register authorized device
static int const TYPE_REG_ACK = 4; //register authorized device ack
static int const TYPE_ACTIVATE = 5; //authorize and bind new device
static int const TYPE_ACTIVATE_ACK = 6; //authorize and bind new device ack
static int const TYPE_STREAM = 7; //send stream data
static int const TYPE_MARK = 8; //send to & back mark data
static int const TYPE_PULSE = 30;

static Byte const ACK_OK = 0x00;
static Byte const ACK_NO = 0x00ff;

@interface AppMessage : NSObject <ACReusableObject> {
    @public
    int type;
    NSData *body;
}

@property (nonatomic, copy) NSString *reuseIdentifier;

- (instancetype)init:(int)type Body:(NSData*)body;

- (id)extractMark;
- (Device*)extractDevice;

@end
