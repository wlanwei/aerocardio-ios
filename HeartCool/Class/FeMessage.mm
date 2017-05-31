//
//  FeMessage.m
//  HeartCool
//
//  Created by wd on 2017/4/5.
//  Copyright © 2017年 uteamtec. All rights reserved.
//

#import "FeMessage.h"

@implementation FeMessage

+ (int)getBodyLength:(int)type length:(int)streamLength resolution:(int)resolution {
    return 0;
}

- (int)getType {
    return 0;
}

- (NSData*)getBody {
    return nil;
}

- (id)extractInitEcg:(User*)user time:(long)timeInit {
    return nil;
}

- (id)extractEcg:(User*)user {
    return nil;
}

- (id)extractMark:(long)startTime {
    return nil;
}

- (Device*)extractDevice {
    return nil;
}

+ (FeMessage*)createRegAckMsg:(Device*)device {
    return nil;
}

+ (FeMessage*)createPulseMsg {
    return nil;
}

+ (FeMessage*)createResetMsg {
    return nil;
}

+ (FeMessage*)createMarkMsg {
    return nil;
}

@end
