//
//  AppService.m
//  HeartCool
//
//  Created by wd on 2017/4/5.
//  Copyright © 2017年 uteamtec. All rights reserved.
//

#import "AppService.h"

@implementation AppService

WDSingletonM(AppService)

+ (AppService*)shared {
    AppService *q = [self sharedAppService];
    return q;
}

@end
