//
//  RegisterViewController.m
//  HeartCool
//
//  Created by wd on 2017/3/2.
//  Copyright © 2017年 uteamtec. All rights reserved.
//

#import "RegisterViewController.h"

#import "AppNetTcpSms.h"
#import "RegisterNextViewController.h"

@interface RegisterViewController ()

@property (strong, nonatomic) IBOutlet UITextField *textUsername;
@property (strong, nonatomic) IBOutlet UITextField *textCaptcha;

@property NSString *code;

@property (strong, nonatomic) IBOutlet UIButton *btnCapcha;

@end

@implementation RegisterViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
    if ([segue.identifier isEqualToString:@"push_register_next"]) {
        RegisterNextViewController *next = [segue destinationViewController];
        next.username = [self.textUsername text];
    }
}

#pragma mark - Action

- (IBAction)onDidEndOnExitUsername:(id)sender {
    [sender resignFirstResponder];
}

- (IBAction)onDidEndOnExitCaptcha:(id)sender {
    [sender resignFirstResponder];
}

- (IBAction)onTouchDownView:(id)sender {
    [[UIApplication sharedApplication] sendAction:@selector(resignFirstResponder) to:nil from:nil forEvent:nil];
}

- (IBAction)onClickAuthCode:(id)sender {
    self.code = [NSString stringWithFormat:@"%04d", arc4random() % 10000];
    [AppNetTcpSms sendMessage:[self.textUsername text] code:self.code block:^(bool sucess, NSString *message, NSError *error) {
        if (sucess) {
            [self.btnCapcha setTitle:@"验证码已经发送!" forState:UIControlStateNormal];
        }
        if (error) {
            NSLog(@"error: %@", error);
        } else {
            NSLog(@"message: %@", message);
        }
    }];
}

- (IBAction)onClickNext:(id)sender {
    if ([self.code isEqualToString:[self.textCaptcha text]]) {
        [self performSegueWithIdentifier:@"push_register_next" sender:self];
    }
}

@end
