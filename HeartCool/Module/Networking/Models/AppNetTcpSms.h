//
//  AppNetTcpSms.h
//  HeartCool
//
//  Created by wd on 2017/3/19.
//  Copyright © 2017年 uteamtec. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface AppNetTcpSms : NSObject

+ (void)sendMessage:(NSString*)telephone code:(NSString*)code block:(void (^)(bool sucess, NSString *message, NSError *error))block;

@end
