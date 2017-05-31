//
//  FeTxQueue.h
//  HeartCool
//
//  Created by wd on 2017/4/5.
//  Copyright © 2017年 uteamtec. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "BlockingQueue.h"
#import "FeMessage.h"
#import "WDSingleton.h"

@interface FeTxQueue : NSObject {
@private
    BlockingQueue *queue;
}

WDSingletonH(FeTxQueue)

+ (FeTxQueue*)shared;

- (void)put:(FeMessage*)msg;
- (FeMessage*)take;

@end
