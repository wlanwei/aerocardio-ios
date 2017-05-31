//
//  MainViewController.h
//  HeartCool
//
//  Created by wd on 2017/3/11.
//  Copyright © 2017年 uteamtec. All rights reserved.
//

#import <UIKit/UIKit.h>

#import <CoreBluetooth/CoreBluetooth.h>
#import "BabyBluetooth.h"

@interface MainViewController : UIViewController {
    @public
    BabyBluetooth *baby;
}

@property(strong, nonatomic)CBPeripheral *currPeripheral;

@end
