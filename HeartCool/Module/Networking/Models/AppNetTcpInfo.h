//
//  AppNetTcpInfo.h
//  HeartCool
//
//  Created by wd on 2017/3/19.
//  Copyright © 2017年 uteamtec. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface AppNetTcpInfo : NSObject

+ (void)bindDeviceByMacAddress:(NSString*)infoId mac:(NSString*)macAddress block:(void (^)(bool sucess, NSString *message, NSError *error))block;

+ (void)validateUserInfoAndDevice:(NSString*)infoId mac:(NSString*)macAddress block:(void (^)(bool sucess, NSArray *array, NSError *error))block;

+ (void)queryBindDeviceByInfoId:(NSString*)infoId block:(void (^)(bool sucess, NSArray *array, NSError *error))block;

@end
