//
//  HttpAPIClient.h
//  HeartCool
//
//  Created by wd on 2017/3/15.
//  Copyright © 2017年 uteamtec. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "AFNetworking.h"

@interface HttpAPIClient : AFHTTPSessionManager

+ (instancetype)sharedClient;

@end
