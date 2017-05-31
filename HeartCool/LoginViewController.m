//
//  LoginViewController.m
//  HeartCool
//
//  Created by wd on 2017/3/2.
//  Copyright © 2017年 uteamtec. All rights reserved.
//

#import "LoginViewController.h"

#import "AppNetTcpUser.h"
#import "SVProgressHUD.h"

#import "User.h"

#import "MainViewController.h"

@interface LoginViewController ()

@property (strong, nonatomic) IBOutlet UITextField *textUsername;
@property (strong, nonatomic) IBOutlet UITextField *textPassword;

@property (strong, nonatomic) IBOutlet UISwitch *switchRemeber;

@end

@implementation LoginViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    User *user = [User shared];
    if ([user->username length] > 0) {
        [self.textUsername setText:user->username];
        [self.textPassword setText:user->password];
    }
    [self.switchRemeber setOn:[user isRemeber]];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

#pragma mark - Action

- (IBAction)onDidEndOnExitUsername:(id)sender {
    [self.textPassword becomeFirstResponder];
}

- (IBAction)onDidEndOnExitPassword:(id)sender {
    [sender resignFirstResponder];
    [self.btn_login sendActionsForControlEvents:UIControlEventTouchUpInside];
}

- (IBAction)onTouchDownView:(id)sender {
    [[UIApplication sharedApplication] sendAction:@selector(resignFirstResponder) to:nil from:nil forEvent:nil];
}

- (IBAction)onClickLogin:(id)sender {
    if (true) {
//        // 蓝牙设备列表
//        [self performSegueWithIdentifier:@"push_devices" sender:self];
//        return;// TODO: test
        
        // 主界面
        MainViewController *next = [self.storyboard instantiateViewControllerWithIdentifier:@"MainViewController"];
        [self.navigationController pushViewController:next animated:YES];
        return;// TODO: test
    }
    [AppNetTcpUser validate:[self.textUsername text] password:[self.textPassword text] block:^(bool sucess, NSString *message, NSError *error) {
        if (sucess) {
            NSLog(@"success!");
            [SVProgressHUD showSuccessWithStatus:@"登录成功"];
            
            User *user = [User shared];
            user->username = [self.textUsername text];
            user->password = [self.switchRemeber isOn]?[self.textPassword text]:@"";
            [user save];

            [self performSegueWithIdentifier:@"push_devices" sender:self];
        }
        if (error) {
            NSLog(@"error: %@", error);
        } else {
            NSLog(@"message: %@", message);
        }
    }];
}

- (IBAction)onClickRegister:(id)sender {
    [self performSegueWithIdentifier:@"push_register" sender:self];
}

- (IBAction)onClickForgot:(id)sender {
    [self performSegueWithIdentifier:@"push_forgot" sender:self];
}

@end
