//
//  KeychainStore.h
//  HeartCool
//
//  Created by Windom on 2017/3/24.
//  Copyright © 2017年 uteamtec. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface KeychainStore : NSObject

+ (NSArray*)keys;
+ (NSString*)read:(NSString*)key;
+ (void)save:(NSString*)key value:(NSString*)value;

@end
