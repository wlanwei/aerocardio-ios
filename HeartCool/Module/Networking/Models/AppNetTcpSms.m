//
//  AppNetTcpSms.m
//  HeartCool
//
//  Created by wd on 2017/3/19.
//  Copyright © 2017年 uteamtec. All rights reserved.
//

#import "AppNetTcpSms.h"

#import "HttpAPIClient.h"

@implementation AppNetTcpSms

+ (void)sendMessage:(NSString*)telephone code:(NSString*)code block:(void (^)(bool sucess, NSString *message, NSError *error))block {
    [[HttpAPIClient sharedClient] POST:@"v1/sendMessage/sendMessage" parameters:@{@"tel": telephone, @"code":code} progress:nil success:^(NSURLSessionDataTask * __unused task, id JSON) {
        NSString *message = JSON;
        block([message hasPrefix:@"发送成功"], message, nil);
    } failure:^(NSURLSessionDataTask *__unused task, NSError *error) {
        block(NO, nil, error);
    }];
}

@end
