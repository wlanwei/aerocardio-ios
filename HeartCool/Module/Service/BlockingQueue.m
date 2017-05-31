//
//  BlockingQueue.m
//  HeartCool
//
//  Created by wd on 2017/4/5.
//  Copyright © 2017年 uteamtec. All rights reserved.
//

#import "BlockingQueue.h"

enum {
    kNoWorkQueued = 0,
    kWorkQueued = 1
};

@implementation BlockingQueue

- (id)init {
    if ((self = [super init])) {
        queueLock = [[NSConditionLock alloc] initWithCondition:kNoWorkQueued];
        queue = [[NSMutableArray alloc] init];
    }
    return self;
}

- (id)dequeueWaiting:(NSDate *)timeoutDate {
    id work = nil;
    if ([queueLock lockWhenCondition:kWorkQueued beforeDate:timeoutDate]) {
        work = [queue objectAtIndex:0];
        [queue removeObjectAtIndex:0];
        [queueLock unlockWithCondition:([queue count] ? kWorkQueued : kNoWorkQueued)];
    }
    return work;
}

- (id)dequeue {
    id work = nil;
    [queueLock lockWhenCondition:kWorkQueued];
    work = [queue objectAtIndex:0];
    [queue removeObjectAtIndex:0];
    [queueLock unlockWithCondition:([queue count] ? kWorkQueued : kNoWorkQueued)];
    return work;
}

- (void)queue:(id)work {
    [queueLock lock];
    [queue addObject:work];
    [queueLock unlockWithCondition:kWorkQueued];
}

@end
