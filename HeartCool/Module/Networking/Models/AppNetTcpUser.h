//
//  AppNetTcpUser.h
//  HeartCool
//
//  Created by wd on 2017/3/15.
//  Copyright © 2017年 uteamtec. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface AppNetTcpUser : NSObject

+ (void)isExistTelephone:(NSString*)telephone block:(void (^)(bool sucess, NSError *error))block;

+ (void)validate:(NSString*)name password:(NSString*)pwd block:(void (^)(bool sucess, NSString *message, NSError *error))block;

+ (void)createUserOrUpdateByApp:(NSString*)telephone password:(NSString*)password realName:(NSString*)realName sex:(NSString*)sex birthdate:(NSString*)birthdate age:(NSString*)age address:(NSString*)address block:(void (^)(bool sucess, NSString *userInfoId, NSError *error))block;

+ (void)queryAppUserInfoByInfoId:(NSString*)infoId block:(void (^)(bool sucess, id appUserInfo, NSError *error))block;

+ (void)queryInfoIdByAppUserCode:(NSString*)telephone block:(void (^)(bool sucess, NSString *infoId, NSString *message, NSError *error))block;

+ (void)updatePasswordByApp:(NSString*)telephone password:(NSString*)password block:(void (^)(bool sucess, NSString *message, NSError *error))block;

@end
