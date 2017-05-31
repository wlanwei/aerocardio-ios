//
//  RegisterNextViewController.m
//  HeartCool
//
//  Created by wd on 2017/3/11.
//  Copyright © 2017年 uteamtec. All rights reserved.
//

#import "RegisterNextViewController.h"

#import "RegisterPersonalViewController.h"

@interface RegisterNextViewController ()

@property (strong, nonatomic) IBOutlet UITextField *textPassword1;
@property (strong, nonatomic) IBOutlet UITextField *textPassword2;

@end

@implementation RegisterNextViewController

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
    if ([segue.identifier isEqualToString:@"push_register_personal"]) {
        RegisterPersonalViewController *next = [segue destinationViewController];
        next.username = self.username;
        next.password = [self.textPassword1 text];
    }
}

#pragma mark - Action

- (IBAction)onDidEndOnExitPassword1:(id)sender {
    [self.textPassword2 becomeFirstResponder];
}

- (IBAction)onDidEndOnExitPassword2:(id)sender {
    [sender resignFirstResponder];
}

- (IBAction)onTouchDownView:(id)sender {
    [[UIApplication sharedApplication] sendAction:@selector(resignFirstResponder) to:nil from:nil forEvent:nil];
}

- (IBAction)onClickNext:(id)sender {
    if ([[self.textPassword1 text] isEqualToString:[self.textPassword2 text]]) {
        [self performSegueWithIdentifier:@"push_register_personal" sender:self];
    }
}

@end
