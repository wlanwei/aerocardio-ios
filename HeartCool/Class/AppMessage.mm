//
//  AppMessage.m
//  HeartCool
//
//  Created by wd on 2017/4/5.
//  Copyright © 2017年 uteamtec. All rights reserved.
//

#import "AppMessage.h"

@implementation AppMessage

- (instancetype)init:(int)newType Body:(NSData*)newBody
{
    self = [super init];
    if (self) {
        self->type = newType;
        self->body = newBody;
    }
    return self;
}

- (id)extractMark {
    if (self->type == TYPE_MARK) {
    }
    return nil;
}

- (Device*)extractDevice {
    return nil;
}

+ (AppMessage*)createLoginMessage:(NSString*)id Key:(NSString*)key {
    return nil;
}

+ (AppMessage*)createRegMessage:(Device*)device {
    return nil;
}

@end
