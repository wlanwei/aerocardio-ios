//
//  EcgMarkConst.h
//  HeartCool
//
//  Created by wd on 2017/6/8.
//  Copyright © 2017年 uteamtec. All rights reserved.
//

#import <Foundation/Foundation.h>

static const int TYPE_GROUP_STATUS = 1;
static const int TYPE_GROUP_PHYSIO = 2;
static const int STATUS_LOWPOWER = 1;
static const int STATUS_LEADOFF = 2;
static const int STATUS_SHORT = 3;
static const int STATUS_CHARGING = 4;
static const int STATUS_UNPLUG = 5;
static const int STATUS_PLUG = 6;
static const int PHYSIO_HR = 1;
static const int PHYSIO_BR = 2;
static const int PHYSIO_NOISE = 3;
static const int PHYSIO_USERINPUT = 4;
static const int PHYSIO_ABNORMAL = 5;

@interface EcgMarkConst : NSObject

@end
