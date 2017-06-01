//
//  FeService.h
//  HeartCool
//
//  Created by wd on 2017/4/5.
//  Copyright © 2017年 uteamtec. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "WDSingleton.h"

@interface FeService : NSObject

WDSingletonH(FeService)

+ (FeService*)shared;

@end
