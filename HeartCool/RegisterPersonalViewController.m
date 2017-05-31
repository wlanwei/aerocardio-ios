//
//  RegisterPersonalViewController.m
//  HeartCool
//
//  Created by wd on 2017/3/21.
//  Copyright © 2017年 uteamtec. All rights reserved.
//

#import "RegisterPersonalViewController.h"

#import "AppNetTcpUser.h"

@interface RegisterPersonalViewController ()

@property (strong, nonatomic) IBOutlet UITextField *textName;
@property (strong, nonatomic) IBOutlet UISegmentedControl *segCtrlSex;
@property (strong, nonatomic) IBOutlet UITextField *textBirthday;
@property (strong, nonatomic) IBOutlet UITextField *textTel;
@property (strong, nonatomic) IBOutlet UITextField *textAddress;

@end

@implementation RegisterPersonalViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
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

- (IBAction)onDidEndOnExitName:(id)sender {
    [self.textBirthday becomeFirstResponder];
}

- (IBAction)onDidEndOnExitBirthday:(id)sender {
    [self.textAddress becomeFirstResponder];
}

- (IBAction)onDidEndOnExitAddress:(id)sender {
    [sender resignFirstResponder];
}

- (IBAction)onTouchDownView:(id)sender {
    [[UIApplication sharedApplication] sendAction:@selector(resignFirstResponder) to:nil from:nil forEvent:nil];
}

- (IBAction)onClickFinish:(id)sender {
    [self.textTel setText:self.username];
    
    int age = 0;
    NSString *birthdate = [self.textBirthday text];
    if ([birthdate length] >= 4) {
        age = [[birthdate substringToIndex:4] intValue];
    }
    [AppNetTcpUser createUserOrUpdateByApp:self.username password:self.password realName:[self.textName text] sex:self.segCtrlSex.numberOfSegments==0?@"men":@"female" birthdate:birthdate age:[NSString stringWithFormat:@"%d", age] address:[self.textAddress text] block:^(bool sucess, NSString *userInfoId, NSError *error) {
        if (sucess) {
            [[self navigationController] popToRootViewControllerAnimated:YES];
        }
    }];
}

@end
