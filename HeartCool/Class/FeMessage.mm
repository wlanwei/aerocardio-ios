//
//  FeMessage.m
//  HeartCool
//
//  Created by wd on 2017/4/5.
//  Copyright © 2017年 uteamtec. All rights reserved.
//

#import "FeMessage.h"

#import "Device.h"
#import "MessageUtils.h"
#include "Ecg.h"

@implementation FeMessage

+ (int)getBodyLength:(int)type length:(int)streamLength resolution:(int)resolution {
    switch (type) {
        case TYPE_STREAM_ECG_1:
        case TYPE_STREAM_ECG_3:
        case TYPE_STREAM_ECG_12:
        case TYPE_STREAM_ECG_2:
            if (streamLength < 0 || resolution < 0) {
                //return invalide body length
                return -1;
            } else {
                if (type == TYPE_STREAM_ECG_1) {
                    return 2 + resolution * streamLength * 1;
                } else if (type == TYPE_STREAM_ECG_3) {
                    return 2 + resolution * streamLength * 3;
                } else if (type == TYPE_STREAM_ECG_12) {
                    return 2 + resolution * streamLength * 12;
                } else if (type == TYPE_STREAM_ECG_2) {
                    return 2 + resolution * streamLength * 2;
                } else {
                    return 2 + resolution * streamLength * 1;
                }
            }
        case TYPE_STATUS:
            return 2 + 4 + 1;
        case TYPE_ANALYSIS_PHSIO:
            return 2 + 8 + 1;
        case TYPE_REGISTER:
            return 8 + 2 + 6 + 16;
        case TYPE_REGISTER_ACK:
            return 8 + 2 + 6 + 16;
        case TYPE_USERINPUT:
            return 0;
        default:
            return -1;
    }
}

- (int)getType {
    return self->type;
}

- (NSData*)getBody {
    return self->body;
}

- (id)extractInitEcg:(User*)user time:(long)timeInit {
    int stamp;
    NSData *array;
    if (type == TYPE_STREAM_ECG_1 || type == TYPE_STREAM_ECG_3 || type == TYPE_STREAM_ECG_12 || type == TYPE_STREAM_ECG_2) {
        stamp = [MessageUtils bytesToUnsignedShort:self->body Offset:0];
        
        if ([[Device shared] getModel] == MODEL_20_3_HI) {
            //For 24 bit high resolution
            int data[[self->body length] - 2 / 3];
            for (int m = 0; m < sizeof(data); m++) {
                data[m] = [MessageUtils bytesToDemiInt:self->body Offset:2 + 3 * m];
            }
            array = [[NSData alloc] initWithBytes:&data length:sizeof(data)];
        } else if ([[Device shared] getModel] == MODEL_20_3 || [[Device shared] getModel] == MODEL_20_1) {
            //For 16 bit regular resolution
            int data[[self->body length] - 2 / 2];
            for (int m = 0; m < sizeof(data); m++) {
                data[m] = [MessageUtils bytesToShort:self->body Offset:2 + 2 * m];
            }
            array = [[NSData alloc] initWithBytes:&data length:sizeof(data)];
        } else if ([[Device shared] getModel] == MODEL_20_2_HI) {
            //For 24 bit regular resolution
            int data[[self->body length] - 2 / 2];
            for (int m = 0; m < sizeof(data) / 3; m++) {
                data[3 * m] = [MessageUtils bytesToDemiInt:self->body Offset:2 + 3 * (2 * m)];
                data[3 * m + 1] = [MessageUtils bytesToDemiInt:self->body Offset:2 + 3 * (2 * m + 1)];
                data[3 * m + 2] = data[3 * m + 1] - data[3 * m];
            }
            array = [[NSData alloc] initWithBytes:&data length:sizeof(data)];
        } else {
            return nil;
        }
        
        int ecgType;
        if (type == TYPE_STREAM_ECG_1) {
            ecgType = Ecg.TYPE_SINGLE;
        } else if (type == TYPE_STREAM_ECG_3) {
            ecgType = Ecg.TYPE_THREE;
        } else if (type == TYPE_STREAM_ECG_12) {
            ecgType = Ecg.TYPE_FULL;
        } else if (type == TYPE_STREAM_ECG_2) {
            ecgType = Ecg.TYPE_THREE;
        } else {
            ecgType = Ecg.TYPE_SINGLE;
        }
        Ecg *ecg = new Ecg(ecgType, -1, -1, -1, [array bytes]);
        
        Device *dev = [Device shared];
        int sps = dev.getSps();
        int streamLen = dev.getStreamLen();
        long timediff = streamLen * 1000 / sps;
        
        user.setTimeStreamPrev(timeInit);
        user.setStampStreamPrev(stamp);
        
        ecg.setSps(sps);
        ecg.setStartTime(timeInit);
        ecg.setStopTime(timeInit + timediff);
        
        return ecg;
    } else {
        return null;
    }
    
    return nil;
}

- (id)extractEcg:(User*)user {
    return nil;
}

- (id)extractMark:(long)startTime {
    return nil;
}

- (Device*)extractDevice {
    return nil;
}

+ (FeMessage*)createRegAckMsg:(Device*)device {
    return nil;
}

+ (FeMessage*)createPulseMsg {
    return nil;
}

+ (FeMessage*)createResetMsg {
    return nil;
}

+ (FeMessage*)createMarkMsg {
    return nil;
}

@end
