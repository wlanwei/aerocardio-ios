//
//  AppTxQueue.m
//  HeartCool
//
//  Created by wd on 2017/4/5.
//  Copyright © 2017年 uteamtec. All rights reserved.
//

#import "AppTxQueue.h"

@implementation AppTxQueue

WDSingletonM(AppTxQueue)

static BlockingQueue *queue;

+ (AppTxQueue*)shared {
    AppTxQueue *q = [self sharedAppTxQueue];
    queue = [[BlockingQueue alloc] init];
    return q;
}

- (void)put:(AppMessage*)msg {
    if (msg != nil) {
        [self->queue queue:msg];
    }
}

- (AppMessage*)take {
    return (AppMessage*)[self->queue dequeue];
}

@end
