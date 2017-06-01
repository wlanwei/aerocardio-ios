//
//  FeService.m
//  HeartCool
//
//  Created by wd on 2017/4/5.
//  Copyright © 2017年 uteamtec. All rights reserved.
//

#import "FeService.h"

@implementation FeService

WDSingletonM(FeService)

+ (FeService*)shared {
    FeService *q = [self sharedFeService];
    return q;
}

@end
