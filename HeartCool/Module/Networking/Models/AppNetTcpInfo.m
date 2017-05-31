//
//  AppNetTcpInfo.m
//  HeartCool
//
//  Created by wd on 2017/3/19.
//  Copyright © 2017年 uteamtec. All rights reserved.
//

#import "AppNetTcpInfo.h"

#import "HttpAPIClient.h"

@implementation AppNetTcpInfo

+ (void)bindDeviceByMacAddress:(NSString*)infoId mac:(NSString*)macAddress block:(void (^)(bool sucess, NSString *message, NSError *error))block {
    [[HttpAPIClient sharedClient] GET:@"v1/info/bindDeviceByMacAddress" parameters:@{@"infoId": infoId, @"macAddress":macAddress} progress:nil success:^(NSURLSessionDataTask * __unused task, id JSON) {
        bool sucess = [JSON valueForKeyPath:@"sucess"];
        NSString *message = [JSON valueForKeyPath:@"message"];
        block(sucess, message, nil);
    } failure:^(NSURLSessionDataTask *__unused task, NSError *error) {
        block(NO, nil, error);
    }];
}

+ (void)validateUserInfoAndDevice:(NSString*)infoId mac:(NSString*)macAddress block:(void (^)(bool sucess, NSArray *array, NSError *error))block {
    [[HttpAPIClient sharedClient] GET:@"v1/info/validateUserInfoAndDevice" parameters:@{@"infoId": infoId, @"macAddress":macAddress} progress:nil success:^(NSURLSessionDataTask * __unused task, id JSON) {
        bool sucess = [JSON valueForKeyPath:@"sucess"];
        NSArray *array = [JSON valueForKeyPath:@"BoundedDevices"];
        block(sucess, array, nil);
    } failure:^(NSURLSessionDataTask *__unused task, NSError *error) {
        block(NO, nil, error);
    }];
}

+ (void)queryBindDeviceByInfoId:(NSString*)infoId block:(void (^)(bool sucess, NSArray *array, NSError *error))block {
    [[HttpAPIClient sharedClient] GET:@"v1/info/queryBindDeviceByInfoId" parameters:@{@"infoId": infoId} progress:nil success:^(NSURLSessionDataTask * __unused task, id JSON) {
        bool sucess = [JSON valueForKeyPath:@"sucess"];
        NSArray *array = [JSON valueForKeyPath:@"BoundedDevices"];
        block(sucess, array, nil);
    } failure:^(NSURLSessionDataTask *__unused task, NSError *error) {
        block(NO, nil, error);
    }];
}

@end
