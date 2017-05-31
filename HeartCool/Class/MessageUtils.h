//
//  MessageUtils.h
//  HeartCool
//
//  Created by wd on 2017/5/18.
//  Copyright © 2017年 uteamtec. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface MessageUtils : NSObject

+(NSData*)demiIntToBytes:(int)val;
+(int)bytesToDemiInt:(NSData*)data Offset:(int)offset;
+(NSData*)shortToBytes:(int)val;
+(int)bytesToShort:(NSData*)data Offset:(int)offset;
+(int)bytesToUnsignedShort:(NSData*)data Offset:(int)offset;
+(int)bytesToInt:(NSData*)data Offset:(int)offset;
+(long)bytesToLong:(NSData*)data Offset:(int)offset;
+(NSData*)longToBytes:(long)val;

@end
