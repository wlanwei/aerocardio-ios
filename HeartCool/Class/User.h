//
//  User.h
//  HeartCool
//
//  Created by wd on 2017/3/26.
//  Copyright © 2017年 uteamtec. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "WDSingleton.h"

static int const FESTATE_DISABLED = 1;
static int const FESTATE_DISCONNECTED = 2;
static int const FESTATE_DISCONNECTING = 3;
static int const FESTATE_CONNECTING = 4;
static int const FESTATE_CONNECTED = 5;
static int const FESTATE_REGISTERED = 6;
static int const APPSTATE_DISABLED = 1;
static int const APPSTATE_DISCONNECTED = 2;
static int const APPSTATE_DISCONNECTING = 3;
static int const APPSTATE_CONNECTING = 4;
static int const APPSTATE_CONNECTED = 5;
static int const APPSTATE_LOGIN = 6;

@interface User : NSObject {
    @public
    NSString *infoId;
    NSString *username;
    NSString *password;
    int feState;
    int appState;
}

WDSingletonH(User)

+ (User*)shared;

- (NSString*)toString;
- (void)save;

- (BOOL)isRemeber;

@end
