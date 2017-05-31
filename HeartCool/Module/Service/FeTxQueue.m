//
//  FeTxQueue.m
//  HeartCool
//
//  Created by wd on 2017/4/5.
//  Copyright © 2017年 uteamtec. All rights reserved.
//

#import "FeTxQueue.h"

@implementation FeTxQueue

WDSingletonM(FeTxQueue)

static BlockingQueue *queue;

+ (FeTxQueue*)shared {
    FeTxQueue *q = [self sharedFeTxQueue];
    queue = [[BlockingQueue alloc] init];
    return q;
}

- (void)put:(FeMessage*)msg {
    if (msg != nil) {
        [self->queue queue:msg];
    }
}

- (FeMessage*)take {
    return (FeMessage*)[self->queue dequeue];
}

@end

