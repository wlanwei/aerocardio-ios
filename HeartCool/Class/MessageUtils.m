//
//  MessageUtils.m
//  HeartCool
//
//  Created by wd on 2017/5/18.
//  Copyright © 2017年 uteamtec. All rights reserved.
//

#import "MessageUtils.h"

@implementation MessageUtils

+(NSData*)demiIntToBytes:(int)val {
    char bytes[sizeof(int)];
    memcpy(bytes,&val,sizeof(val));
    return [[NSData alloc] initWithBytes:bytes length:sizeof(int)];
}

+(int)bytesToDemiInt:(NSData*)data Offset:(int)offset {
    int val = 0;
    int mask = 0xff;
    char bytes[sizeof(int)];
    [data getBytes:bytes length:sizeof(int)];
    for (int m = offset; m < offset+sizeof(int)-1; m ++) {
        int num = bytes[m];
        num <<= (8*(m-offset));
        num &= mask;
        num <<= 8;
        num >>= 8;
        mask <<= 8;
        val += num;
    }
    return val;
}

+(NSData*)shortToBytes:(int)val {
    val <<= 16;
    val >>= 16;
    char bytes[sizeof(short)];
    for (int m = 0; m < sizeof(short); m ++) {
        bytes[m] = (char) ((val >> (8*m)) & 0x00ff);
    }
    return [[NSData alloc] initWithBytes:bytes length:sizeof(short)];
}

+(int)bytesToShort:(NSData*)data Offset:(int)offset {
    int val = 0;
    int mask = 0xff;
    char bytes[sizeof(short)];
    [data getBytes:bytes length:sizeof(short)];
    for (int m = offset; m < offset+sizeof(short)-1; m ++) {
        int num = bytes[m];
        num <<= (8*(m-offset));
        num &= mask;
        num <<= 8;
        num >>= 8;
        mask <<= 8;
        val += num;
    }
    return val;
}

+(int)bytesToUnsignedShort:(NSData*)data Offset:(int)offset {
    int val = 0;
    int mask = 0xff;
    unsigned char bytes[sizeof(short)];
    [data getBytes:bytes length:sizeof(short)];
    for (int m = offset; m < offset+sizeof(short)-1; m ++) {
        int num = bytes[m];
        num <<= (8*(m-offset));
        num &= mask;
        num <<= 8;
        num >>= 8;
        mask <<= 8;
        val += num;
    }
    return val;
}

+(int)bytesToInt:(NSData*)data Offset:(int)offset {
    int val = 0;
    int mask = 0xff;
    char bytes[sizeof(int)];
    [data getBytes:bytes length:sizeof(int)];
    for (int m = offset; m < offset+sizeof(int)-1; m ++) {
        int num = bytes[m];
        num <<= (8*(m-offset));
        num &= mask;
        num <<= 8;
        num >>= 8;
        mask <<= 8;
        val += num;
    }
    return val;
}

+(long)bytesToLong:(NSData*)data Offset:(int)offset {
    long val = 0;
    long mask = 0xff;
    char bytes[sizeof(long)];
    [data getBytes:bytes length:sizeof(long)];
    for (int m = offset; m < offset+sizeof(long)-1; m ++) {
        int num = bytes[m];
        num <<= (8*(m-offset));
        num &= mask;
        num <<= 8;
        num >>= 8;
        mask <<= 8;
        val += num;
    }
    return val;
}

+(NSData*)longToBytes:(long)val {
    char bytes[sizeof(long)];
    memcpy(bytes,&val,sizeof(val));
    return [[NSData alloc] initWithBytes:bytes length:sizeof(long)];
}

@end
