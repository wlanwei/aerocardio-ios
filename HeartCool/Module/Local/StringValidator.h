//
//  StringValidator.h
//  HeartCool
//
//  Created by wd on 2017/3/25.
//  Copyright © 2017年 uteamtec. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface StringValidator : NSObject

+ (BOOL)isNumber:(NSString *)num;
+ (BOOL)isNumberOrLetter:(NSString *)num;
+ (BOOL)isMobileNumber:(NSString *)mobileNum;
+ (BOOL)isBlankString:(NSString *)string;

@end
