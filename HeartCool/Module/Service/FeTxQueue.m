//
//  FeTxQueue.m
//  HeartCool
//
//  Created by wd on 2017/4/5.
//  Copyright © 2017年 uteamtec. All rights reserved.
//

#import "FeTxQueue.h"

#import "User.h"

@interface FeTxQueue ()

@property BOOL enable;
@property (nonatomic, strong) dispatch_queue_t mainQueue;
@property (nonatomic, strong) dispatch_queue_t txQueue;
@property (nonatomic, strong) dispatch_queue_t pulseQueue;

@end

#define LABEL "com.uteamtec.HeartCool"

@implementation FeTxQueue

WDSingletonM(FeTxQueue)

static BlockingQueue *queue;

+ (FeTxQueue*)shared {
    FeTxQueue *q = [self sharedFeTxQueue];
    queue = [[BlockingQueue alloc] init];
    return q;
}

- (instancetype)init
{
    self = [super init];
    if (self) {
        self->_mainQueue = dispatch_queue_create(LABEL, DISPATCH_QUEUE_CONCURRENT);
        self->_txQueue = dispatch_queue_create(LABEL, DISPATCH_QUEUE_CONCURRENT);
        self->_pulseQueue = dispatch_queue_create(LABEL, DISPATCH_QUEUE_CONCURRENT);
    }
    return self;
}

- (void)start {
    self->_enable = YES;
    dispatch_sync(self->_mainQueue, ^{
        while (self->_enable) {
            sleep(500);
            if (!self->_enable) {
                break;
            }
            if ([User shared]->feState == FESTATE_CONNECTED || [User shared]->feState == FESTATE_REGISTERED) {
                // TODO: ..........
            }
            // TODO: ..........
        }
    });
    dispatch_sync(self->_txQueue, ^{
        while (self->_enable) {
            FeMessage *msg = [self take];
            if (msg) {
                // TODO: ble write....
            }
            sleep(200);
        }
    });
    dispatch_sync(self->_pulseQueue, ^{
        while (self->_enable) {
            // TODO: ..........
            sleep(1000);
        }
    });
}

- (void)stop {
    self->_enable = NO;
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

