//
//  BlockingQueue.h
//  HeartCool
//
//  Created by wd on 2017/4/5.
//  Copyright © 2017年 uteamtec. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface BlockingQueue : NSObject {
    NSMutableArray *queue;
    NSConditionLock *queueLock;
}

- (id)dequeueWaiting:(NSDate *)timeoutDate;
- (id)dequeue;
- (void)queue:(id)work;

@end
