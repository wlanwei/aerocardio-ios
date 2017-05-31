//
//  User.m
//  HeartCool
//
//  Created by Windom on 2017/3/26.
//  Copyright © 2017年 uteamtec. All rights reserved.
//

#import "User.h"

#import "KeychainStore.h"

@implementation User

WDSingletonM(User)

+ (User*)shared {
    User* user = [self sharedUser];
    if ([user->username length] > 0) {
        [user read];
    }
    return user;
}

- (NSString*)toString {
    return [NSString stringWithFormat:@"%@|%@|%@",
            infoId, username, password];
}

- (void)read {
    NSArray *array = [[KeychainStore read:@"User"] componentsSeparatedByString:@"|"];
    if ([array count] >= 3) {
        infoId = array[0];
        username = array[1];
        password = array[2];
    }
}

- (void)save {
    [KeychainStore save:@"User" value:[self toString]];
}

- (BOOL)isRemeber {
    return [self->password length] != 0;
}

@end
