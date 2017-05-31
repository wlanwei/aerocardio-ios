//
//  KeychainStore.m
//  HeartCool
//
//  Created by Windom on 2017/3/24.
//  Copyright © 2017年 uteamtec. All rights reserved.
//

#import "KeychainStore.h"

#import "SAMKeychain.h"

#define Service @"Service"

@implementation KeychainStore

+ (NSArray*)keys {
    return [SAMKeychain accountsForService:Service];
}

+ (NSString*)read:(NSString*)key {
    return [SAMKeychain passwordForService:Service account:key];
}

+ (void)save:(NSString*)key value:(NSString*)value {
    [SAMKeychain setPassword:value forService:Service account:key];
}

@end
