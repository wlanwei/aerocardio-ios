//
//  HttpAPIClient.m
//  HeartCool
//
//  Created by wd on 2017/3/15.
//  Copyright © 2017年 uteamtec. All rights reserved.
//

#import "HttpAPIClient.h"

@implementation HttpAPIClient

+ (instancetype)sharedClient {
    static HttpAPIClient *_sharedClient = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        _sharedClient = [[HttpAPIClient alloc] initWithBaseURL:
                         [NSURL URLWithString:@"http://112.124.70.101:8097/webService"]];
        _sharedClient.responseSerializer.acceptableContentTypes = [NSSet setWithObject:@"text/plan"];
        _sharedClient.securityPolicy = [AFSecurityPolicy policyWithPinningMode:AFSSLPinningModeNone];
    });
    
    return _sharedClient;
}

@end
