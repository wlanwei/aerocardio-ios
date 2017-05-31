//
//  AppTxQueue.h
//  HeartCool
//
//  Created by wd on 2017/4/5.
//  Copyright © 2017年 uteamtec. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "BlockingQueue.h"
#import "AppMessage.h"
#import "WDSingleton.h"

@interface AppTxQueue : NSObject {
    @private
    BlockingQueue *queue;
}

WDSingletonH(AppTxQueue)

+ (AppTxQueue*)shared;

- (void)put:(AppMessage*)msg;
- (AppMessage*)take;

@end
