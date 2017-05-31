//
//  User.h
//  HeartCool
//
//  Created by wd on 2017/3/26.
//  Copyright © 2017年 uteamtec. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "WDSingleton.h"

@interface User : NSObject {
    @public
    NSString *infoId;
    NSString *username;
    NSString *password;
}

WDSingletonH(User)

+ (User*)shared;

- (NSString*)toString;
- (void)save;

- (BOOL)isRemeber;

@end
