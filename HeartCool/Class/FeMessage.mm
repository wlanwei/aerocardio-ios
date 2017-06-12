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
#import "EcgConst.h"
#include "EcgMark.h"
#import "EcgMarkConst.h"

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
            ecgType = TYPE_SINGLE;
        } else if (type == TYPE_STREAM_ECG_3) {
            ecgType = TYPE_THREE;
        } else if (type == TYPE_STREAM_ECG_12) {
            ecgType = TYPE_FULL;
        } else if (type == TYPE_STREAM_ECG_2) {
            ecgType = TYPE_THREE;
        } else {
            ecgType = TYPE_SINGLE;
        }
        Ecg *ecg = new Ecg(ecgType, -1, -1, -1, (int*)[array bytes]);
        
        Device *dev = [Device shared];
        int sps = [dev getSps];
        int streamLen = [dev getStreamLen];
        long timediff = streamLen * 1000 / sps;
        
        
        [user setTimeStreamPrev:timeInit];
        [user setStampStreamPrev:stamp];

        ecg->setSps(sps);
        ecg->setTime_start(timeInit);
        ecg->setTime_stop(timeInit + timediff);
        
        return (__bridge id)ecg;
    }
    return nil;
}

- (id)extractEcg:(User*)user {
    int stamp;
    NSData *array;
    if (type == TYPE_STREAM_ECG_1 || type == TYPE_STREAM_ECG_3 || type == TYPE_STREAM_ECG_12 || type == TYPE_STREAM_ECG_2) {
        stamp = [MessageUtils bytesToUnsignedShort:body Offset:0];
        
        if ([[Device shared] getModel] == MODEL_20_3_HI) {
            //For 24 bit high resolution
            int data[[self->body length] - 2 / 3];
            for (int m = 0; m < sizeof(data); m++) {
                data[m] = [MessageUtils bytesToDemiInt:body Offset:2 + 3 * m];
            }
        } else if ([[Device shared] getModel] == MODEL_20_3 || [[Device shared] getModel] == MODEL_20_1) {
            //For 16 bit regular resolution
            int data[[self->body length] - 2 / 2];
            for (int m = 0; m < sizeof(data); m++) {
                data[m] = [MessageUtils bytesToShort:body Offset:2 + 2 * m ];
            }
        } else if ([[Device shared] getModel] == MODEL_20_2_HI) {
            //For 24 bit regular resolution
            int data[[self->body length] - 2 / 2];
            for (int m = 0; m < sizeof(data); m++) {
                data[3 * m] = [MessageUtils bytesToDemiInt:body Offset:2 + 3 * (2 * m)];
                data[3 * m + 1] = [MessageUtils bytesToDemiInt:body Offset:2 + 3 * (2 * m + 1)];
                data[3 * m + 2] = data[3 * m + 1] - data[3 * m];
            }
        } else {
            return nil;
        }
        
        int ecgType;
        if (type == TYPE_STREAM_ECG_1) {
            ecgType = TYPE_SINGLE;
        } else if (type == TYPE_STREAM_ECG_3) {
            ecgType = TYPE_THREE;
        } else if (type == TYPE_STREAM_ECG_12) {
            ecgType = TYPE_FULL;
        } else if (type == TYPE_STREAM_ECG_2) {
            ecgType = TYPE_THREE;
        } else {
            ecgType = TYPE_SINGLE;
        }
        //            L.i("<FECODER> ecg type = " + Integer.toString(ecgType));
        Ecg *ecg = new Ecg(ecgType, -1, -1, -1, (int*)[array bytes]);
        
        int stampPrev = [user getStampStreamPrev];
        long timePrev = [user getTimeStreamPrev];
        Device *dev = [Device shared];
        int sps = [dev getSps];
        int streamLen = [dev getStreamLen];
        long timediff = streamLen * 1000 / sps;
        
        int stampDiff = stampPrev < stamp ? stamp - stampPrev : 65535 - stampPrev + stamp;
        
        long timeNow = timePrev + stampDiff * timediff;
        [user setTimeStreamPrev:timeNow];
        [user setStampStreamPrev:stamp];
        
        ecg->setSps(sps);
        ecg->setTime_start(timeNow);
        ecg->setTime_stop(timeNow + timediff);
        
        return (__bridge id)ecg;
    }
    return nil;
}

- (id)extractMark:(long)startTime {
    if (type == TYPE_STATUS) {
        int markType = [MessageUtils bytesToShort:body Offset:0];
        int value = [MessageUtils bytesToInt:body Offset:2];
        
        EcgMark *ecgMark = new EcgMark(startTime, startTime, TYPE_GROUP_STATUS, markType, value);
        return (__bridge id)ecgMark;
    } else if (type == TYPE_USERINPUT) {
        EcgMark *ecgMark = new EcgMark(startTime, startTime, TYPE_GROUP_PHYSIO, PHYSIO_USERINPUT, 0);
        return (__bridge id)ecgMark;
    }
    return nil;
}

- (Device*)extractDevice {
    if (type == TYPE_REGISTER) {
        NSData *_id;
        int model = 0;
        int streamLen = 0;
        int sps = 0;
        NSData *key;
        
        _id = [body subdataWithRange:NSMakeRange(0, 8)];
        model = [MessageUtils bytesToShort:body Offset:8];

        char bytes[[body length]];
        [body getBytes:bytes length:[body length]];
        
        
        streamLen = bytes[10] & 0x00ff;
        
        sps = [MessageUtils bytesToShort: body Offset:11];
        
        key = [body subdataWithRange:NSMakeRange(16, 16)];
        
        Device *dev = [[Device alloc] init];
        
        dev->deviceId = [[NSString alloc] initWithData:_id encoding:NSUTF8StringEncoding];
        dev->sps = sps;
        dev->streamLen = streamLen;
        dev->key = [[NSString alloc] initWithData:key encoding:NSUTF8StringEncoding];
        
        return dev;
    }
    return nil;
}

+ (FeMessage*)createRegAckMsg:(Device*)device {
    char body[[self getBodyLength:TYPE_REGISTER_ACK length:0 resolution:0]];
    const char *bytes1 = [device->deviceId UTF8String];
    memcpy(&body, bytes1, 8);
    /*test code */
    body[0] = (char) 0x00ff;
    body[1] = (char) 0x00ff;
    const char *bytes2 = (char*)[[MessageUtils shortToBytes:[device getModel]] bytes];
    for (int i=8; i < 10; i++) {
        body[i] = bytes2[i-8];
    }
    body[10] = (char) ([device getStreamLen] & 0x00ff);
    const char *bytes3 = (char*)[[MessageUtils shortToBytes:[device getSps]] bytes];
    for (int i=10; i < 12; i++) {
        body[i] = bytes3[i-10];
    }
    device->keyPair = @"0000000000000000";
    const char *bytes4 = [device->keyPair UTF8String];
    for (int i=12; i < 28; i++) {
        body[i] = bytes4[i-12];
    }
    FeMessage *msg = [[FeMessage alloc] init];
    msg->type = TYPE_REGISTER_ACK;
    msg->body = [[NSData alloc] initWithBytes:body length:sizeof(body)];
    return msg;
}

+ (FeMessage*)createPulseMsg {
    FeMessage *msg = [[FeMessage alloc] init];
    msg->type = TYPE_PULSE;
    msg->body = nil;
    return msg;
}

+ (FeMessage*)createResetMsg {
    FeMessage *msg = [[FeMessage alloc] init];
    msg->type = TYPE_RESET;
    msg->body = nil;
    return msg;
}

+ (FeMessage*)createMarkMsg:(id)mark {
    EcgMark *_mark = (__bridge EcgMark*)mark;
    if (_mark->getTypeGroup() == TYPE_GROUP_STATUS) {
        char body[[self getBodyLength:TYPE_STATUS length:0 resolution:0]];
        body[0] = 0;
        body[1] = 0;
        
        const char *bytes1 = (char*)[[MessageUtils shortToBytes:_mark->getType()] bytes];
        for (int i=2; i < 2+2; i++) {
            body[i] = bytes1[i-2];
        }
        const char *bytes2 = (char*)[[MessageUtils shortToBytes:_mark->getValue()] bytes];
        for (int i=4; i < 4+2; i++) {
            body[i] = bytes2[i-4];
        }
        
        FeMessage *msg = [[FeMessage alloc] init];
        msg->type = TYPE_STATUS;
        msg->body = [[NSData alloc] initWithBytes:body length:sizeof(body)];
        return msg;
    } else if (_mark->getTypeGroup() == TYPE_GROUP_PHYSIO) {
        char body[[self getBodyLength:TYPE_ANALYSIS_PHSIO length:0 resolution:0]];
        if (_mark->getType() == PHYSIO_HR) {
            const char *bytes1 = (char*)[[MessageUtils shortToBytes:1] bytes];
            memcpy(&body, bytes1, 2);
        } else if (_mark->getType() == PHYSIO_BR) {
            const char *bytes1 = (char*)[[MessageUtils shortToBytes:2] bytes];
            memcpy(&body, bytes1, 2);
        } else {
            const char *bytes1 = (char*)[[MessageUtils shortToBytes:_mark->getType()] bytes];
            memcpy(&body, bytes1, 2);
        }
        const char *bytes2 = (char*)[[MessageUtils shortToBytes:_mark->getValue()] bytes];
        for (int i=2; i < 2+2; i++) {
            body[i] = bytes2[i-2];
        }
        
        FeMessage *msg = [[FeMessage alloc] init];
        msg->type = TYPE_ANALYSIS_PHSIO;
        msg->body = [[NSData alloc] initWithBytes:body length:sizeof(body)];
        return msg;
    }
    return nil;
}

@end
